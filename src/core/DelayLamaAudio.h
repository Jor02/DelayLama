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
            int rngState;
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

            virtual bool getPluginName(char* outText) override;
            virtual bool getCompanyName(char* outText) override;
            virtual void setParameterValue(int parameterId, float value);
            virtual void initialize();
            virtual void processAudio(float** inputs,float** outputs,int32_t sampleFrames);
            virtual void initPresets();
            virtual bool sendEventsToHost(void* eventsPtr);
            virtual void destroy();
            virtual void loadPresetByIndex(int currentProgram);
            virtual void setCurrentPresetName(char* newName);
            virtual void getCurrentPresetName(char* outText);
            virtual void getParameterUnitLabel(char* label);
            virtual void getParameterValueString(int parameterId, char* outText);
            virtual void getParameterName(int parameterId, char* outBuffer);
            virtual float getParameterValue(int parameter);
            virtual uint32_t getOutputBusProperties(uint32_t index, char* properties);
            virtual bool getPresetNameByIndex(int category, int index, char* outText);
            virtual bool copyPreset(int param_1);
            virtual bool getProductName(char* outText);
            virtual void setSampleRate(uint32_t sampleRate);
            virtual void setMaxFramesPerProcess(uint32_t blocksize);
            virtual void disableAudioProcessing();
            virtual void enableAudioProcessing();
            virtual void buildPiecewiseCubicTable(int32_t* controlPoints, float* outSamples);
            virtual void invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames);
            virtual void dispatchMidiEvents(int sampleIdx, int sampleFrame);
            virtual void addSynthesisToExcitation(int offsetIncrement);
            virtual void updateVowelFilter(float vowelX);
            virtual void processEvents(DamSDK::Api::DamEventList* eventList);
            virtual void handleNoteEvent(int midiData1, int midiData2);
            virtual void handleControlChange(int midiData1, int midiData2);
            virtual float getRandomFloat();
            virtual void sendMidiToHost(uint8_t status, uint8_t data1, uint8_t data2);

            void iteratePresetBlocks(Preset* context, int stride, int iterationCount, void* callback, void* extra);
            void _conditionalRunCallback(void* context, int param2, int unknownLocal, void* extraParam);
            void __vector_destructor_iterator(int param_1, int param_2, int param_3, void* param_4);
            void _conditionalCleanup();
            void _forEachPresetBlockReverse(void* startPtr, int step, int count, void* callback);
    };
}
}