#pragma once
#include "damsdk/api/AudioBaseExtended.h"

namespace DelayLama {
namespace Core {
    struct Preset {
        float portTime;
        float delay;
        float headSize;
        char name[24];
    };

    // VTABLE: DELAYLAMA 0x1000b460
    class DelayLamaAudio : public DamSDK::Api::AudioBaseExtended {
        public:
            float portamentoTime;
            float curVowelValue;
            float prevVowelValue;
            float delay;
            float monkSprite;
            float headSize;
            float vibratoDepthCurrent;
            float vibratoAmount;
            float pitchValue;
            bool vibratoDirty;
            bool pitchValueDirty;
            char unusedBytes00[2];
            Preset* presets;

            // Midi Section, I think I might have misidentified some MidiEvents wrong or something here.
            int32_t midiEventReadIndex;
            // MidiEvent midiQueue[1024];
            // MidiEvent* vstMidiEvent;
            int vstMidiEventSize;
            int unknownMidi2;
            char unusedBytes01[12];
            byte midiStatus;
            byte midiData1;
            byte midiData2;
            char unusedBytes02[5];
            void* eventContainer;
            int unknown;
            // MidiEvent* midiEvent;
            int unknownMidi;
            int32_t midiDataValue;

            float vowelFilterState1;
            float vowelFilterState2;
            int pitchBase; //A midi note
            int pitchTargetRaw; //A midi note
            float outputGain;
            int pitchInterpData1[1024];
            int pitchInterpData2[1024];
            int isInterpActive;
            int interpEventCount;
            int interpSampleStep;
            int interpCurrentIdx;
            int synthesisFrameCounter;
            float vowelTargetValue;
            bool pitchTargetDirty;
            bool isGlideActive;
            bool vowelFilterDirty;
            bool unusedBool;
            int pitchTargetValue;
            int vowelMorphDelta;
            float vowelMorph;
            bool isGateActive;
            char unusedBytes04[3];
            float vowelMorphCurrent;
            float lfoPhaseAccumulator;
            float lfoPhaseWrapValue;
            float lfoDepth;
            float lfoSampleValue;
            float lfoPhaseIncrement;
            int vowelFrameCounter;
            int sampleCounter;
            bool isSinging;
            char unusedBytes05[3];
            int noteStack[128];
            int writeIndex;
            int frequencyIndex;
            float unusedFloat;
            float frequencyValue;
            int excitationWriteIndex;
            int attackSamples;
            int sustainStart;
            int releaseSamples;
            float vowelLookupIndex;
            float formant1Bandwidth;
            float formant2Bandwidth;
            float formant3Bandwidth;
            char unusedBytes06[8];
            float resonanceGain;
            int totalSmoothingFrames;
            int smoothCounter;
            int smoothingFrames;
            int smoothStep;
            int pitchCurrent;
            int pitchTarget;
            int pitchDelta;
            int pitchSmoothingFramesRemaining;
            int pitchStep;
            float vibratoCurrent;
            int vibratoTarget;
            int vibratoDelta;
            int vibratoStep;
            int vibratoSmoothingFramesRemaining;
            float pluginSampleRate;
            float prevSampleRate;
            int blockSize;
            float rngState;
            float delayTimeScaler;
            bool initialVowelNeedsUpdate;
            char unusedBytes07[3];
            float vowelPresetTable[24];
            int vowelPresetIndex;
            int vowelStepIndex;
            int anotherVowelIndex;
            int vowelTriggerBase;
            int vowelTriggerA;
            int vowelTriggerB;
            int vowelTriggerC;
            int vowelTriggerD;
            int vowelTriggerE;
            int delayBufferSize;
            int* stereoDelayLBuffer;
            int* stereoDelayRBuffer;
            float delayFeedback;
            int delayWriteIndex;
            int delayReadIndexL;
            int delayReadIndexR;
            float* synthesisBuffer;
            int numSamples;
            float* excitationBuffer;
            int excitationBufferSize;
            int excitationReadIndex;
            float* sineTable;
            int sineTableSize;
            float* formantTable;
            int formantTableSize;
            float* vocalEnvelope;
            float* glottalSource;
            int glottalTableSize;
            float glottalPhaseInc;
            float* harmonicBuffer;
            float* frequencyTable;
            int frequencyTableSize; // 4096 size
            float* formantTable1;
            float* formantTable2;
            float* formantTable3;
        public:
            DelayLamaAudio(DamSDK::Api::dispatchFunc hostCallback);
            ~DelayLamaAudio();

            bool getPluginName(char* outText) override;
            bool getCompanyName(char* outText) override;
            void setParameterValue(int parameterId,float value);
            void initialize();
            void processAudio(float** inputs,float** outputs,int32_t sampleFrames);
            void initPresets();
    };
}
}