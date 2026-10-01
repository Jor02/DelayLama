#pragma once
#include "damsdk/api/AudioBaseExtended.h"

namespace DelayLama {
namespace Core {
    struct Preset {
        Preset();
        ~Preset();
        float portTime;
        float delay;
        float headSize;
        char name[24];
    };

    // VTABLE: DELAYLAMA 0x1000b460
    class DelayLamaAudio : public DamSDK::Api::AudioBaseExtended {
        public:
            float portamentoTime; // 0xb0
            float curVowelValue; // 0xb4
            float prevVowelValue; // 0xb8
            float delay; // 0xbc
            float monkSprite; // 0xc0
            float headSize; // 0xc4
            float padPitchSlider; // 0xc8
            float padPitch; // 0xcc
            float padVowel; // 0xd0
            bool padPitchDirty; // 0xd4
            bool padVowelDirty; // 0xd5
            char unusedBytes00[2]; // 0xd6
            Preset* presets;// 0xd8

            // MIDI: incoming events queued per sample, and the event sent to the host
            int32_t midiEventReadIndex; // 0xdc
            DamSDK::Api::MidiEvent midiQueue[1024]; // 0xe0
            DamSDK::Api::DamMidiEvent midiEvent; // 0x40e0
            DamSDK::Api::DamMidiEventList midiEventList; // 0x4100
            int32_t midiDataValue; // 0x4110
            int currentMidiEventData1; // 0x4114
            int currentMidiEventData2; // 0x4118
            int vowelBendLsb; //A midi note // 0x411c
            int vowelBendMsb; //A midi note // 0x4120
            float outputGain; // 0x4124
            int bendQueueLsb[1024]; // 0x4128
            int bendQueueMsb[1024]; // 0x5128
            int nextBendSample; // 0x6128
            int bendQueueCount; // 0x612c
            int bendQueueSpacing; // 0x6130
            int bendQueueIndex; // 0x6134
            int synthesisFrameCounter; // 0x6138 (written but never read)
            float vowelTargetValue; // 0x613c (written but never read)
            bool vowelBendDirty; // 0x6140
            bool isGlideActive; // 0x6141 (written but never read)
            bool vowelBufferNeedsUpdate; // 0x6142
            bool unusedBool; // 0x6143
            float notePitch; // 0x6144
            float glideStep; // 0x6148
            float glidePitch; // 0x614c
            bool isLegato; // 0x6150
            char unusedBytes04[3]; // 0x6151
            float voicePitch; // 0x6154
            float lfoPhaseAccumulator; // 0x6158
            float lfoRate; // 0x615c
            float vibratoDepth; // 0x6160
            float vibratoOffset; // 0x6164
            float lfoPhaseIncrement; // 0x6168
            int lfoReseedIntervalSamples; // 0x616c
            int sampleCounter; // 0x6170
            bool isSinging; // 0x6174
            char unusedBytes05[3]; // 0x6175
            int noteStack[128]; // 0x6178
            int pulseWriteIndex; // 0x6378
            int periodSamples; // 0x637c
            float unusedFloat; // 0x6380
            float voiceFrequency; // 0x6384
            int samplesSincePulse; // 0x6388
            int attackSamples; // 0x638c
            int sustainStart; // 0x6390
            int releaseSamples; // 0x6394
            float vowelLookupIndex; // 0x6398
            float formant1Bandwidth; // 0x639c
            float formant2Bandwidth; // 0x63a0
            float formant3Bandwidth; // 0x63a4
            char unusedBytes06[8]; // 0x63a8
            float headSizeScale; // 0x63b0
            int totalSmoothingFrames; // 0x63b4
            int smoothCounter; // 0x63b8
            int smoothingFrames; // 0x63bc
            int smoothStep; // 0x63c0
            int vowelCurrent; // 0x63c4
            int vowelTarget; // 0x63c8
            int vowelDelta; // 0x63cc
            int vowelSmoothingFramesRemaining; // 0x63d0
            float vowelStep; // 0x63d4
            float padPitchCurrent; // 0x63d8
            float padPitchTarget; // 0x63dc
            float padPitchDelta; // 0x63e0
            float padPitchStep; // 0x63e4
            int padPitchSmoothingFramesRemaining; // 0x63e8
            float pluginSampleRate; // 0x63ec
            float prevSampleRate; // 0x63f0
            int pluginBlockSize; // 0x63f4
            int rngState; // 0x63f8
            float rngScale; // 0x63fc
            bool needsMonkAnimationRefresh; // 0x6400
            char unusedBytes07[3]; // 0x6401
            float monkIdleFrameTable[24]; // 0x6404
            int currentIdleFrame; // 0x6464
            int idleAnimationSampleCounter; // 0x6468
            int globalAnimationSampleCounter; // 0x646c
            int idleSamplesPerFrame; // 0x6470
            int startBlink1; // 0x6474
            int stopBlink1; // 0x6478
            int startBlink2; // 0x647c
            int stopBlink2; // 0x6480
            int startIdleAnimation; // 0x6484
            int delayBufferSize; // 0x6488
            float* stereoDelayLBuffer; // 0x648c
            float* stereoDelayRBuffer; // 0x6490
            float delayFeedback; // 0x6494
            int delayWriteIndex; // 0x6498
            int delayReadIndexL; // 0x649c
            int delayReadIndexR; // 0x64a0
            float* grainBuffer; // 0x64a4
            int numSamples; // 0x64a8
            float* voiceBuffer; // 0x64ac
            int voiceBufferSize; // 0x64b0
            int voiceReadIndex; // 0x64b4
            float* sineTable; // 0x64b8
            int sineTableSize; // 0x64bc
            float* decayTable; // 0x64c0
            int decayTableSize; // 0x64c4
            float* grainWindow; // 0x64c8
            float* formantWave; // 0x64cc
            int formantWaveSize; // 0x64d0
            float formantWaveStepsPerHz; // 0x64d4
            float* fixedFormantBuffer; // 0x64d8
            float* frequencyTable; // 0x64dc
            int frequencyTableSize; // 0x64e0
            float* formantTable1; // 0x64e4
            float* formantTable2; // 0x64e8
            float* formantTable3; // 0x64ec
        public:
            DelayLamaAudio(DamSDK::Api::dispatchFunc hostCallback);
            ~DelayLamaAudio();

            virtual bool getPluginName(char* outText) override;
            virtual bool getCompanyName(char* outText) override;
            virtual void setParameterValue(int32_t parameterId, float value) override;
            void initialize();
            virtual void processAudio(float** inputs,float** outputs,int32_t sampleFrames) override;
            bool sendEventsToHost(DamSDK::Api::DamMidiEventList* eventsPtr);
            virtual void loadPresetByIndex(int32_t currentProgram) override;
            virtual void setCurrentPresetName(char* newName) override;
            virtual void getCurrentPresetName(char* outText) override;
            virtual void getParameterUnitLabel(int32_t parameterId, char* label) override;
            virtual void getParameterValueString(int32_t parameterId, char* outText) override;
            virtual void getParameterName(int32_t parameterId, char* outBuffer) override;
            virtual float getParameterValue(int32_t parameterId) override;
            virtual bool getOutputBusProperties(int32_t index, char* properties) override;
            virtual bool getPresetNameByIndex(int32_t category, int32_t index, char* outText) override;
            virtual bool copyPreset(int32_t destination) override;
            virtual bool getProductName(char* outText) override;
            virtual int32_t pluginSupports(char* target) override;
            virtual void setSampleRate(float sampleRate) override;
            virtual void setMaxFramesPerProcess(int32_t blocksize) override;
            virtual void disableAudioProcessing() override;
            virtual void enableAudioProcessing() override;
            void buildFormantCurveTable(int32_t* controlPoints, float* outSamples);
            virtual void invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames) override;
            void dispatchMidiEvents(int sampleIdx, int sampleFrame);
            void addGrainToVoice(int offsetIncrement);
            void synthesizeVowelBuffer(float vowelX);
            virtual int32_t processEvents(void* events) override;
            void handleNoteEvent(int midiData1, int midiData2);
            void handleControlChange(int midiData1, int midiData2);

            // New virtual functions, in the original vtable order (after AudioBaseExtended).
            virtual void initPresets();
            virtual float getRandomFloat();
            virtual void sendMidiToHost(uint8_t status, uint8_t data1, uint8_t data2);
    };
}
}
