#pragma once
// Drives a VST 2.x plugin through a fixed scenario and records its output.
// Used both by the reference renderer (tools/render_reference.cpp, which loads the
// original DLL) and by the audio unit test (which uses this project's plugin), so
// both see exactly the same calls.
#include <cstdint>
#include <cstring>
#include <vector>

namespace RenderScenario {

    struct Plugin;
    typedef intptr_t (*HostCallback)(Plugin* plugin, int32_t opcode, int32_t index, intptr_t value, void* ptr, float opt);
    typedef intptr_t (*DispatcherFunc)(Plugin* plugin, int32_t opcode, int32_t index, intptr_t value, void* ptr, float opt);
    typedef void (*ProcessFunc)(Plugin* plugin, float** inputs, float** outputs, int32_t sampleFrames);
    typedef void (*SetParameterFunc)(Plugin* plugin, int32_t index, float value);
    typedef float (*GetParameterFunc)(Plugin* plugin, int32_t index);

    // VST 2.x AEffect layout (32-bit)
    struct Plugin {
        int32_t magic;
        DispatcherFunc dispatcher;
        ProcessFunc process;
        SetParameterFunc setParameter;
        GetParameterFunc getParameter;
        int32_t numPrograms;
        int32_t numParams;
        int32_t numInputs;
        int32_t numOutputs;
        int32_t flags;
        intptr_t reserved1;
        intptr_t reserved2;
        int32_t initialDelay;
        int32_t realQualities;
        int32_t offQualities;
        float ioRatio;
        void* object;
        void* user;
        int32_t uniqueID;
        int32_t version;
        ProcessFunc processReplacing;
    };

    const int kSampleRate = 44100;
    const int kBlockSize = 512;
    const int kBlocks = 100;  // ~1.16 seconds

    // Host callback: answers the few questions the plugin asks.
    inline intptr_t hostCallback(Plugin*, int32_t opcode, int32_t, intptr_t, void*, float) {
        switch (opcode) {
            case 1:  return 2300;          // audioMasterVersion
            case 16: return kSampleRate;   // audioMasterGetSampleRate
            case 17: return kBlockSize;    // audioMasterGetBlockSize
            default: return 0;
        }
    }

    struct MidiEvent {
        int32_t type;           // 1 = MIDI
        int32_t byteSize;
        int32_t deltaFrames;
        int32_t flags;
        int32_t noteLength;
        int32_t noteOffset;
        char midiData[4];
        char detune;
        char noteOffVelocity;
        char reserved1;
        char reserved2;
    };

    struct Events {
        int32_t numEvents;
        intptr_t reserved;
        void* events[8];
    };

    struct ScheduledMidi { int block; int offset; unsigned char status, data1, data2; };
    struct ScheduledParam { int block; int index; float value; };

    // The scenario: two notes with a legato glide, a pitch bend, the mod wheel
    // and some parameter changes, ending with a release.
    const ScheduledMidi kMidi[] = {
        {  2, 100, 0x90, 60, 100 },   // note on C4
        { 20,   0, 0xB0,  1,  90 },   // mod wheel (vibrato)
        { 30, 256, 0x90, 67, 100 },   // legato note on G4 -> portamento glide
        { 45,  17, 0x80, 67,   0 },   // release G4 -> back to C4
        { 55,   0, 0xE0,  0,  80 },   // pitch bend up
        { 65, 300, 0xE0,  0,  64 },   // pitch bend centre
        { 75,  50, 0x80, 60,   0 },   // note off
    };
    const ScheduledParam kParams[] = {
        {  0, 0, 0.3f },   // PortTime
        {  0, 2, 0.5f },   // Delay
        { 10, 1, 0.2f },   // Vowel
        { 25, 3, 0.8f },   // HeadSize
        { 40, 1, 0.9f },   // Vowel
        { 60, 2, 0.1f },   // Delay
    };

    // Called once the plugin is switched on, before the first block is processed.
    typedef void (*SetupHook)(Plugin* plugin);

    // Opens the plugin, plays the scenario and returns interleaved stereo samples.
    inline std::vector<float> render(Plugin* plugin, SetupHook afterSetup = 0) {
        std::vector<float> out;
        plugin->dispatcher(plugin, 0, 0, 0, 0, 0.0f);                     // effOpen
        plugin->dispatcher(plugin, 10, 0, 0, 0, (float)kSampleRate);      // effSetSampleRate
        plugin->dispatcher(plugin, 11, 0, kBlockSize, 0, 0.0f);           // effSetBlockSize
        plugin->dispatcher(plugin, 12, 0, 1, 0, 0.0f);                    // effMainsChanged (on)
        if (afterSetup)
            afterSetup(plugin);

        std::vector<float> left(kBlockSize), right(kBlockSize);
        std::vector<float> inL(kBlockSize, 0.0f), inR(kBlockSize, 0.0f);
        float* inputs[2] = { &inL[0], &inR[0] };
        float* outputs[2] = { &left[0], &right[0] };
        MidiEvent midi[8];

        for (int block = 0; block < kBlocks; block++) {
            for (size_t p = 0; p < sizeof(kParams) / sizeof(kParams[0]); p++)
                if (kParams[p].block == block)
                    plugin->setParameter(plugin, kParams[p].index, kParams[p].value);

            Events events;
            std::memset(&events, 0, sizeof(events));
            for (size_t m = 0; m < sizeof(kMidi) / sizeof(kMidi[0]); m++) {
                if (kMidi[m].block != block)
                    continue;
                MidiEvent& e = midi[events.numEvents];
                std::memset(&e, 0, sizeof(e));
                e.type = 1;
                e.byteSize = sizeof(MidiEvent);
                e.deltaFrames = kMidi[m].offset;
                e.midiData[0] = (char)kMidi[m].status;
                e.midiData[1] = (char)kMidi[m].data1;
                e.midiData[2] = (char)kMidi[m].data2;
                events.events[events.numEvents++] = &e;
            }
            if (events.numEvents)
                plugin->dispatcher(plugin, 25, 0, 0, &events, 0.0f);      // effProcessEvents

            std::fill(left.begin(), left.end(), 0.0f);
            std::fill(right.begin(), right.end(), 0.0f);
            plugin->processReplacing(plugin, inputs, outputs, kBlockSize);
            for (int i = 0; i < kBlockSize; i++) {
                out.push_back(left[i]);
                out.push_back(right[i]);
            }
        }

        plugin->dispatcher(plugin, 12, 0, 0, 0, 0.0f);                    // effMainsChanged (off)
        plugin->dispatcher(plugin, 1, 0, 0, 0, 0.0f);                     // effClose
        return out;
    }
}
