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
            float portamentoTime; // 0xb0
            float curVowelValue; // 0xb4
            float prevVowelValue; // 0xb8
            float delay; // 0xbc
            float monkSprite; // 0xc0
            float headSize; // 0xc4
            float vibratoDepthCurrent; // 0xc8
            float vibratoAmount; // 0xcc
            float pitchValue; // 0xd0
            bool vibratoDirty; // 0xd4
            bool pitchValueDirty; // 0xd5
            char unusedBytes00[2]; // 0xd6
            Preset* presets;// 0xd8

             // Midi Section, I think I might have misidentified some MidiEvents wrong or something here. 
            int32_t midiEventReadIndex; // 0xdc
            DamSDK::Api::MidiEvent midiQueue[1024]; // 0xe0
            DamSDK::Api::DamMidiEvent midiEvent; // 0x40e0
            DamSDK::Api::DamMidiEventList midiEventList; // 0x40fc
            int unknownMidi; // 0x410c
            int32_t midiDataValue; // 0x4110
            int currentMidiEventData2; // 0x4114
            int currentMidiEventData1; // 0x4118
            int pitchBase; //A midi note // 0x411c
            int pitchTargetRaw; //A midi note // 0x4120
            float outputGain; // 0x4124
            int pitchInterpData1[1024]; // 0x4128
            int pitchInterpData2[1024]; // 0x5128
            int isInterpActive; // 0x6128
            int interpEventCount; // 0x612c
            int interpSampleStep; // 0x6130
            int interpCurrentIdx; // 0x6134
            int synthesisFrameCounter; // 0x6138
            float vowelTargetValue; // 0x613c
            bool pitchTargetDirty; // 0x6140
            bool isGlideActive; // 0x6141
            bool formantTableNeedsUpdate; // 0x6142
            bool unusedBool; // 0x6143
            int pitchTargetValue; // 0x6144
            int formantMorphStep; // 0x6148
            float formantMorphValue; // 0x614c
            bool isGateActive; // 0x6150
            char unusedBytes04[3]; // 0x6151
            float currentFormantMorphValue; // 0x6154
            float lfoPhaseAccumulator; // 0x6158
            float lfoPhaseWrapValue; // 0x615c
            float lfoDepth; // 0x6160
            float lfoSampleValue; // 0x6164
            float lfoPhaseIncrement; // 0x6168
            int lfoReseedIntervalSamples; // 0x616c
            int sampleCounter; // 0x6170
            bool isSinging; // 0x6174
            char unusedBytes05[3]; // 0x6175
            int noteStack[128]; // 0x6178
            int writeIndex; // 0x6378
            int frequencyIndex; // 0x637c
            float unusedFloat; // 0x6380
            float frequencyValue; // 0x6384
            int excitationWriteIndex; // 0x6388
            int attackSamples; // 0x638c
            int sustainStart; // 0x6390
            int releaseSamples; // 0x6394
            float vowelLookupIndex; // 0x6398
            float formant1Bandwidth; // 0x639c
            float formant2Bandwidth; // 0x63a0
            float formant3Bandwidth; // 0x63a4
            char unusedBytes06[8]; // 0x63a8
            float vowelBlendFactor; // 0x63b0
            int totalSmoothingFrames; // 0x63b4
            int smoothCounter; // 0x63b8
            int smoothingFrames; // 0x63bc
            int smoothStep; // 0x63c0
            int pitchCurrent; // 0x63c4
            int pitchTarget; // 0x63c8
            int pitchDelta; // 0x63cc
            int pitchSmoothingFramesRemaining; // 0x63d0
            int pitchStep; // 0x63d4
            float vibratoCurrent; // 0x63d8
            float vibratoTarget; // 0x63dc
            float vibratoDelta; // 0x63e0
            float vibratoStep; // 0x63e4
            int vibratoSmoothingFramesRemaining; // 0x63e8
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
            float* synthesisBuffer; // 0x64a4
            int numSamples; // 0x64a8
            float* excitationBuffer; // 0x64ac
            int excitationBufferSize; // 0x64b0
            int excitationReadIndex; // 0x64b4
            float* sineTable; // 0x64b8
            int sineTableSize; // 0x64bc
            float* formantTable; // 0x64c0
            int formantTableSize; // 0x64c4
            float* vocalEnvelope; // 0x64c8
            float* glottalSource; // 0x64cc
            int glottalTableSize; // 0x64d0
            float glottalPhaseInc; // 0x64d4
            float* harmonicBuffer; // 0x64d8
            float* frequencyTable; // 0x64dc
            int frequencyTableSize; // 0x64e0
            float* formantTable1; // 0x64e4
            float* formantTable2; // 0x64e8
            float* formantTable3; // 0x64ec
        public:
            DelayLamaAudio(DamSDK::Api::dispatchFunc hostCallback);
            ~DelayLamaAudio();

            static void constructPresetArray(Preset* context, int stride, int iterationCount, void* callback, void* extra);
            static void handlePresetArrayConstructionException(void* context, int stride, int processedCount, void* extra, bool successFlag);
            static void destructPresetArrayElements(void* startPtr, int step, int count, void* callback);
            static void presetElementConstructor();

            virtual bool getPluginName(char* outText) override;
            virtual bool getCompanyName(char* outText) override;
            virtual void setParameterValue(int32_t parameterId, float value) override;
            virtual void initialize();
            virtual void processAudio(float** inputs,float** outputs,int32_t sampleFrames) override;
            virtual void initPresets();
            virtual bool sendEventsToHost(DamSDK::Api::DamMidiEventList* eventsPtr);
            virtual void destroy();
            virtual void loadPresetByIndex(int32_t currentProgram) override;
            virtual void setCurrentPresetName(char* newName) override;
            virtual void getCurrentPresetName(char* outText) override;
            virtual void getParameterUnitLabel(int32_t parameterId, char* label) override;
            virtual void getParameterValueString(int32_t parameterId, char* outText) override;
            virtual void getParameterName(int32_t parameterId, char* outBuffer) override;
            virtual float getParameterValue(int32_t parameterId) override;
            virtual bool getOutputBusProperties(int32_t index, char* properties) override;
            virtual bool getPresetNameByIndex(int32_t category, int32_t index, char* outText) override;
            virtual bool copyPreset(int32_t param_1) override;
            virtual bool getProductName(char* outText) override;
            virtual void setSampleRate(float sampleRate) override;
            virtual void setMaxFramesPerProcess(int32_t blocksize) override;
            virtual void disableAudioProcessing() override;
            virtual void enableAudioProcessing() override;
            virtual void buildFormantCurveTable(int32_t* controlPoints, float* outSamples);
            virtual void invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames) override;
            virtual void dispatchMidiEvents(int sampleIdx, int sampleFrame);
            virtual void addSynthesisToExcitation(int offsetIncrement);
            virtual void synthesizeVowelBuffer(float vowelX);
            virtual void processEvents(DamSDK::Api::DamEventList* eventList);
            virtual void handleNoteEvent(int midiData1, int midiData2);
            virtual void handleControlChange(int midiData1, int midiData2);
            virtual float getRandomFloat();
            virtual void sendMidiToHost(uint8_t status, uint8_t data1, uint8_t data2);
    };
}
}
