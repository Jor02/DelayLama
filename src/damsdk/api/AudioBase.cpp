#include <cmath>
#include <cstdio>
#include <cstring>
#include "AudioBase.h"
#include "EditorBase.h"

using namespace DamSDK;

namespace DamSDK {
namespace Api {
    static intptr_t _dispatcher(DamPlugin * plugin, int32_t targetOperation, int32_t index, int32_t value, void * data, float optional) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        if (targetOperation == 1) {
            audioBase->dispatchPluginCallback(1,index,value,data,optional);
            if (audioBase != nullptr) {
                delete audioBase;
            }
            return 1;
        }
        return audioBase->dispatchPluginCallback(targetOperation,index,value,data,optional);
    }

    static float _getParameter(DamPlugin* plugin, int32_t parameterId) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        return audioBase->getParameterValue(parameterId);
    }

    static void _setParameter(DamPlugin* plugin, int32_t parameterId, float value) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        audioBase->setParameterValue(parameterId, value);
    }

    static void _processFloat(DamPlugin* plugin, float** inputBuffer, float** outputBuffer, int32_t bufferSize) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        audioBase->invokeAudioProcess(inputBuffer, outputBuffer, bufferSize);
    }

    static void _processDouble(DamPlugin* plugin, double** inputBuffer, double** outputBuffer, int32_t bufferSize) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        audioBase->invokeAudioProcess((float**)inputBuffer, (float**)outputBuffer, bufferSize);
    }

    AudioBase::AudioBase(dispatchFunc hostCallback, uint32_t presetCount, uint32_t parameterCount) {
        // Zero out the DamPlugin structure to ensure all reserved fields are 0
        memset(&this->plugin, 0, sizeof(DamPlugin));

        // Initialize Class Members
        this->hostCallback = hostCallback;
        this->parameterCount = parameterCount;
        this->presetCount = presetCount;
        this->currentPreset = 0;
        this->editor = nullptr;
        this->sampleRate = 44100.0f;
        this->blockSize = 1024;

        // Initialize the DamPlugin Interface
        this->plugin.magicNumber = 'VstP';  // "VstP" Magic Number
        this->plugin.id          = 'NoEf';  // "NoEf" Default ID
        this->plugin.version     = 1;
        this->plugin.object      = this;    // Link back to this class instance
        this->plugin.user        = nullptr;
        
        // IO and Parameters
        this->plugin.inputChannelCount  = 1;
        this->plugin.outputChannelCount = 2;
        this->plugin.parameterCount     = parameterCount;
        this->plugin.presetCount        = presetCount;
        this->plugin.floatVal           = 1.0f;

        // Assign Global Dispatcher/Process Function Pointers
        this->plugin.dispatcherFunc       = _dispatcher;
        this->plugin.processAudioFloat    = _processFloat;
        this->plugin.settingParameterFunc = _setParameter;
        this->plugin.gettingParameterFunc = _getParameter;
        this->plugin.processAudioDouble   = _processDouble;
    }

    AudioBase::~AudioBase() {
    }

    int32_t AudioBase::dispatchPluginCallback(int32_t targetOperation, int32_t index, int32_t value, void * data, float optional)
    {
        switch(targetOperation) {
            case pluginInitialize:
                this->initializePlugin();
                break;
            case pluginShutdown:
                this->shutdownPlugin();
                break;
            case pluginSetPresetByIndex:
                if ((int)value < presetCount) {
                    this->loadPresetByIndex(value);
                }
                break;
            case pluginGetCurrentPreset:
                return this->getActivePresetIndex();
            case pluginSetCurrentPresetName:
                this->setCurrentPresetName((char*)data);
                break;
            case pluginGetCurrentPresetName:
                this->getCurrentPresetName((char*)data);
                break;
            case pluginGetParameterUnitLabel:
                this->getParameterUnitLabel(index, (char*)data);
                break;
            case pluginGetParameterValueText:
                this->getParameterValueString(index, (char*)data);
                break;
            case pluginGetParameterName:
                this->getParameterName(index, (char*)data);
                break;
            case pluginGetVolume:
                return this->getVolume();
            case pluginSetSampleRate:
                this->setSampleRate(optional);
                break;
            case pluginSetMaxFramesPerProcess:
                this->setMaxFramesPerProcess(value);
                break;
            case pluginSetProcessingState:
                if (value == 0) {
                    this->disableAudioProcessing();
                }
                else
                {
                    this->enableAudioProcessing();
                }
                break;
            case pluginGetEditorRect:
                if (editor != nullptr) {
                    editor->getRect((Rect**)data);
                }
                break;
            case pluginOpenEditor:
                if (editor != nullptr) {
                    editor->open((HWND)data);
                }
                break;
            case pluginCloseEditor:
                if (editor != nullptr) {
                    editor->close();
                }
                break;
            case pluginEditorIdling:
                if (editor != nullptr) {
                    editor->onIdle();
                }
                break;
            case pluginGetIdentifier:
                return 'NvEf'; //0x4e764566;
            case pluginGetPresetData:
                return this->getPluginStateData(data, index != NULL);
            case pluginSetPresetData:
                return this->setPluginStateData(data, value, index != NULL);
        }
        return 0;
    }

    void AudioBase::initializePlugin() {}
    void AudioBase::shutdownPlugin() {}

    void AudioBase::invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames) { this->processAudio(inputs, outputs, sampleFrames); }
    void AudioBase::processAudio(float* * inputs, float* * outputs, int32_t sampleFrames) {}

    void AudioBase::disableAudioProcessing() {}
    void AudioBase::enableAudioProcessing() {}

    // -- Parameters / Automation --
    void AudioBase::setParameterValue(int32_t parameterId, float value) {}
    float AudioBase::getParameterValue(int32_t parameterId) { return 0.f; }
    int32_t AudioBase::automateHostParameter(int32_t parameterId, float value) {
        this->setParameterValue(parameterId, value);
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostAutomateParameter, parameterId, NULL, nullptr, value);
    }

    void AudioBase::getParameterUnitLabel(int32_t parameterId, char* outText) { *outText = '\0'; }
    void AudioBase::getParameterValueString(int32_t parameterId, char* outText) { *outText = '\0';}
    void AudioBase::getParameterName(int32_t parameterId, char* outText) { *outText = '\0'; }

    // -- Presets --
    int32_t AudioBase::getActivePresetIndex() { return this->currentPreset; }
    void AudioBase::loadPresetByIndex(int32_t presetIndex) { this->currentPreset = presetIndex; }
    void AudioBase::setCurrentPresetName(char* newName) { *newName = '\0'; }
    void AudioBase::getCurrentPresetName(char* outText) { *outText = '\0'; }

    int32_t AudioBase::getPluginStateData(void* ptr, bool index) { return 0; }
    int32_t AudioBase::setPluginStateData(void* ptr, int32_t value, bool index) { return 0; }
    
    // -- IO --
    double AudioBase::getSampleRate() { return this->sampleRate; }
    void AudioBase::setSampleRate(int32_t sampleRate) { this->sampleRate = sampleRate; }
    int32_t AudioBase::getMaxFramesPerProcess() { return this->blockSize; }
    void AudioBase::setMaxFramesPerProcess(int32_t blockSize) { this->blockSize = blockSize; }

    void AudioBase::setInputChannelCount(int32_t count) { this->plugin.inputChannelCount = count; }
    void AudioBase::setOutputChannelCount(int32_t count) { this->plugin.outputChannelCount = count;}
    bool AudioBase::isInputChannelConnected(int32_t channel) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t resultInt = this->hostCallback(&this->plugin, hostIsInputConnected, channel, 1, nullptr, 0.0);
        return resultInt == NULL;
    }
    bool AudioBase::isOutputChannelConnected(int32_t channel) { 
        if (this->hostCallback == nullptr)
            return false;
        int32_t resultInt = this->hostCallback(&this->plugin, hostIsInputConnected, channel, 0, nullptr, 0.0);
        return resultInt == NULL;
    }
    
    // -- Plugin Properties --
    void AudioBase::setPluginId(int32_t id) { this->plugin.id = id; }
    void AudioBase::setReportsLoudnessToHost(bool reportsLoudness) {
        if (reportsLoudness) this->plugin.flags |= PluginFlags::ReportsLoudnessToHost;
        else                 this->plugin.flags &= ~PluginFlags::ReportsLoudnessToHost;
    }
    void AudioBase::setHasClip(bool hasClip) {
        if (hasClip) this->plugin.flags |= PluginFlags::HasClip;
        else         this->plugin.flags &= ~PluginFlags::HasClip;
    }
    void AudioBase::setSupportsInPlaceProcessing(bool supportsInPlace) {
        if (supportsInPlace) this->plugin.flags |= PluginFlags::SupportsInPlaceProcessing;
        else                 this->plugin.flags &= ~PluginFlags::SupportsInPlaceProcessing;
    }
    void AudioBase::setHasEditor(bool hasEditor) {
        if (hasEditor) this->plugin.flags |= PluginFlags::HasEditor;
        else           this->plugin.flags &= ~PluginFlags::HasEditor;
    }
    void AudioBase::setAudioBase(AudioBase* base) { this->plugin.audioBase = base; }
    void AudioBase::setPluginProcessingTime(int32_t processingTime) { this->plugin.pluginProcessingTime = processingTime; }

    long double AudioBase::getVolume() { return 0.; }
    
    // -- Host Communication --
    int32_t AudioBase::getHostApiVersion() { 
        if (this->hostCallback == nullptr)
            return 1;
        int32_t hostVersionInt = this->hostCallback(&this->plugin, hostGetApiVersion, 0, NULL, nullptr, 0.0);
        if (hostVersionInt == NULL)
            return 1;
        return hostVersionInt;
    }
    int32_t AudioBase::getHostUniqueId() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetHostId, 0, NULL, nullptr, 0.0);
    }
    void AudioBase::sendIdleToHost() {
        if (this->hostCallback == nullptr)
            return;
        this->hostCallback(&this->plugin, hostProcessIdle, 0, NULL, nullptr, 0.0);
    }

    // -- String Formatting --
    void AudioBase::formatFloatAsDecibelString(float linearValue, char* outText) {
        if (linearValue <= DECIBEL_THRESHOLD) {
            std::strcpy(outText, INF_STRING);
            return;
        }
        float dbValue = static_cast<float>(DECIBEL_FACTOR * std::log10(linearValue));
        this->formatFloatToString(dbValue, outText);
    }
    void AudioBase::formatSamplesAsHzString(float sampleCount, char* outText) {
        float sampleRate = this->getSampleRate();

        if (sampleCount == HZ_THRESHOLD) {
            this->formatFloatToString(0.0f, outText);
        } else {
            float hz = sampleRate / sampleCount;
            this->formatFloatToString(hz, outText);
        }
    }
    void AudioBase::formatSamplesAsMsString(float sampleCount, char* outText) {
        float sampleRate = getSampleRate();               // virtual call vtable+0x88

        // milliseconds = (sampleCount * 1000) / sampleRate
        float ms = static_cast<float>((sampleCount * MS_FACTOR) / sampleRate);
        this->formatFloatToString(ms, outText);
    }
    void AudioBase::formatFloatToString(float value, char* outText) {
        double val = static_cast<double>(value);
        if (val >= HUGE_THRESHOLD) {
            std::strcpy(outText, HUGE_STRING);
            return;
        }

        bool negative = (val < 0.0);
        if (negative)
            val = -val;

        double intPart = std::floor(val);
        double fracPart = val - intPart;

        char intBuffer[32];
        char* p = intBuffer + sizeof(intBuffer) - 1;
        *p = '\0';

        if (intPart == 0.0) {
            *--p = '0';
        } else {
            while (intPart >= 1.0) {
                double digit = std::fmod(intPart, TEN);
                int d = static_cast<int>(digit);
                *--p = static_cast<char>('0' + d);
                intPart = std::floor(intPart * ONE_TENTH);
            }
        }

        char* out = outText;
        if (negative)
            *out++ = '-';
        std::strcpy(out, p);
        out += std::strlen(p);

        *out++ = '.';

        fracPart *= TEN;
        int digit = static_cast<int>(fracPart);
        *out++ = static_cast<char>('0' + digit);
        fracPart -= digit;
        int digitCount = 1;

        while (fracPart > 0.0 && digitCount < MAX_DIGITS) {
            fracPart *= TEN;
            digit = static_cast<int>(fracPart);
            *out++ = static_cast<char>('0' + digit);
            fracPart -= digit;
            ++digitCount;
        }

        *out = '\0';
    }
    void AudioBase::formatIntToString(int32_t value, char* outSmall, int32_t unused1, int32_t unused2, char* outLarge) {
        if (value >= INT_HUGE_LIMIT) {
            std::strcpy(outSmall, HUGE_STRING);
        } else {
            std::sprintf(outSmall, "%d", value);
        }
    }

    // -- Unused --
    void AudioBase::setReservedValue(int32_t unusedValue) {
        this->plugin.zero = unusedValue;
    }
}
}