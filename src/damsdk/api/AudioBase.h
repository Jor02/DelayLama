#pragma once
#include <cstdint>
#include "DamPlugin.h"
#include "AudioBase.h"

namespace DamSDK {
namespace Api {
    class AudioBase {
    public:
        float sampleRate;
        struct EditorBase *editor;
        Api::dispatchFunc hostCallback;
        int32_t presetCount;
        int32_t parameterCount;
        int32_t currentPreset;
        int32_t blockSize;
        struct Api::DamPlugin plugin;
    public:
        AudioBase(Api::dispatchFunc hostCallback, uint32_t presetCount, uint32_t parameterCount);
        ~AudioBase();

        // -- Core --
        virtual int32_t dispatchPluginCallback(int32_t targetOperation, int32_t index, int32_t value, void * data, float optional);
        virtual void initializePlugin();
        virtual void shutdownPlugin();

        virtual void invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames);
        virtual void processAudio(float* * inputs, float* * outputs, int32_t sampleFrames);

        virtual void disableAudioProcessing();
        virtual void enableAudioProcessing();

        // -- Parameters / Automation --
        virtual void setParameterValue(int32_t parameterId, float value);
        virtual float getParameterValue(int32_t parameterId);
        virtual int32_t automateHostParameter(int32_t parameterId, float value);

        virtual void getParameterUnitLabel(int32_t parameterId, char* outText);
        virtual void getParameterValueString(int32_t parameterId, char* outText);
        virtual void getParameterName(int32_t parameterId, char* outBuffer);

        // -- Presets --
        virtual int32_t getActivePresetIndex();
        virtual void loadPresetByIndex(int32_t presetIndex);
        virtual void setCurrentPresetName(char* newName);
        virtual void getCurrentPresetName(char* outText);

        virtual int32_t getPluginStateData(void* ptr, bool index);
        virtual int32_t setPluginStateData(void* ptr, int32_t value, bool index);
        
        // -- IO --
        virtual long double getSampleRate();
        virtual void setSampleRate(int32_t sampleRate);
        virtual int32_t getMaxFramesPerProcess();
        virtual void setMaxFramesPerProcess(int32_t blockSize);

        virtual void setInputChannelCount(int32_t count);
        virtual void setOutputChannelCount(int32_t count);
        virtual bool isInputChannelConnected(int32_t channel);
        virtual bool isOutputChannelConnected(int32_t channel);
        
        // -- Plugin Properties --
        virtual void setPluginId(int32_t id);
        virtual void setReportsLoudnessToHost(bool reportsLoudness);
        virtual void setHasClip(bool hasClip);
        virtual void setSupportsInPlaceProcessing(bool supportsInPlace);
        virtual void setHasEditor(bool hasEditor);
        virtual void setAudioBase(AudioBase* base);
        virtual void setPluginProcessingTime(int32_t processingTime);

        virtual long double getVolume();
        
        // -- Host Communication --
        virtual int32_t getHostApiVersion();
        virtual int32_t getHostUniqueId();
        virtual void sendIdleToHost();
    
        // -- String Formatting --
        virtual void formatFloatAsDecibelString(float linearValue, char* outText);
        virtual void formatSamplesAsHzString(float sampleCount, char* outText);
        virtual void formatSamplesAsMsString(float sampleCount, char* outText);
        virtual void formatFloatToString(float value, char* outText);
        virtual void formatIntToString(int32_t value, char* outSmall, int32_t unused1, int32_t unused2, char* outLarge);

        // -- Unused --
        virtual void setReservedValue(int32_t unusedValue); 
    };

    namespace {
        // formatFloatAsDecibelString
        const float  DECIBEL_THRESHOLD   = 0.0f;          // value <= 0 -> "-inf"
        const char   INF_STRING[]        = "-inf";        // original was " -oo   "
        const double DECIBEL_FACTOR      = 20.0;          // 20 * log10(linear)

        // formatSamplesAsHzString
        const float  HZ_THRESHOLD        = 0.0f;

        // formatSamplesAsMsString
        const double MS_FACTOR           = 1000.0;

        // formatFloatToString
        const double HUGE_THRESHOLD      = 1e9;           // value >= 1e9 -> "Huge!"
        const char   HUGE_STRING[]       = "Huge!";
        const double ONE_TENTH           = 0.1;
        const double TEN                 = 10.0;
        const double ONE                 = 1.0;
        const int    MAX_DIGITS          = 8;

        // formatIntToString
        const int    INT_HUGE_LIMIT      = 100000000;     // 0x5F5E100
    }
}
}