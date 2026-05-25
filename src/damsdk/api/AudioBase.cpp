#include <cmath>
#include <cstdio>
#include <cstring>
#include "AudioBase.h"
#include "EditorBase.h"

using namespace DamSDK;

namespace DamSDK {
namespace Api {

    // FUNCTION: DELAYLAMA 0x10001000
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

    // FUNCTION: DELAYLAMA 0x10001060
    static float _getParameter(DamPlugin* plugin, int32_t parameterId) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        return audioBase->getParameterValue(parameterId);
    }

    // FUNCTION: DELAYLAMA 0x10001080
    static void _setParameter(DamPlugin* plugin, int32_t parameterId, float value) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        audioBase->setParameterValue(parameterId, value);
    }

    // FUNCTION: DELAYLAMA 0x100010c0
    static void _processFloat(DamPlugin* plugin, float** inputBuffer, float** outputBuffer, int32_t bufferSize) {
        AudioBase* audioBase = (AudioBase*) plugin->object;
        audioBase->invokeAudioProcess(inputBuffer, outputBuffer, bufferSize);
    }

    // STUB: DELAYLAMA 0x100010a0
    void AudioBase::_process(DamPlugin* effect, float* * inputs, float* * outputs, int32_t sampleFrames) {
        // (*effect->object->vtable->invokeAudioProcess)(inputs,outputs,sampleFrames);
        // return;
    }

    // FUNCTION: DELAYLAMA 0x100010e0
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
        this->plugin.processingFunc       = _processFloat;
        this->plugin.settingParameterFunc = _setParameter;
        this->plugin.gettingParameterFunc = _getParameter;

        this->plugin.processAudioFloat    = _processFloat;
    }

    // FUNCTION: DELAYLAMA 0x100011a0
    AudioBase::~AudioBase() {
    }

    // STUB: DELAYLAMA 0x100011c0
    void AudioBase::destroy() {
        // DelayLamaEditor *editor;
        //
        // this->vtable = &AudioBaseVTable_1000b148;
        // editor = this->editor;
        // if (editor != (DelayLamaEditor *)0x0) {
        //   (*(editor->vtable->editorBase).destructor)(1);
        // }
        // return;
    }

    // FUNCTION: DELAYLAMA 0x100011e0
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
                return static_cast<int32_t>(this->getVolume() * 32767.0f);
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

    // FUNCTION: DELAYLAMA 0x100015b0 FOLDED
    void AudioBase::initializePlugin() {}

    // FUNCTION: DELAYLAMA 0x100015b0 FOLDED
    void AudioBase::shutdownPlugin() {}

    // LIBRARY: DELAYLAMA 0x1000b090
    void AudioBase::invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames) { this->processAudio(inputs, outputs, sampleFrames); }

    // FUNCTION: DELAYLAMA 0x10001590
    void AudioBase::processAudio(float* * inputs, float* * outputs, int32_t sampleFrames) {}

    // FUNCTION: DELAYLAMA 0x100015b0 FOLDED
    void AudioBase::disableAudioProcessing() {}

    // FUNCTION: DELAYLAMA 0x100015b0 FOLDED
    void AudioBase::enableAudioProcessing() {}

    // -- Parameters / Automation --
    // FUNCTION: DELAYLAMA 0x10001540 FOLDED
    void AudioBase::setParameterValue(int32_t parameterId, float value) {}

    // FUNCTION: DELAYLAMA 0x10001550
    float AudioBase::getParameterValue(int32_t parameterId) { return 0.f; }

    // FUNCTION: DELAYLAMA 0x10001a10
    int32_t AudioBase::automateHostParameter(int32_t parameterId, float value) {
        this->setParameterValue(parameterId, value);
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostAutomateParameter, parameterId, NULL, nullptr, value);
    }

    // FUNCTION: DELAYLAMA 0x100016b0 FOLDED
    void AudioBase::getParameterUnitLabel(int32_t parameterId, char* outText) { *outText = '\0'; }

    // FUNCTION: DELAYLAMA 0x100016b0 FOLDED
    void AudioBase::getParameterValueString(int32_t parameterId, char* outText) { *outText = '\0';}

    // FUNCTION: DELAYLAMA 0x100016b0 FOLDED
    void AudioBase::getParameterName(int32_t parameterId, char* outText) { *outText = '\0'; }

    // -- Presets --
    // FUNCTION: DELAYLAMA 0x10001630
    int32_t AudioBase::getActivePresetIndex() { return this->currentPreset; }
    
    // FUNCTION: DELAYLAMA 0x10001640
    void AudioBase::loadPresetByIndex(int32_t presetIndex) { this->currentPreset = presetIndex; }
    
    // FUNCTION: DELAYLAMA 0x10001650 FOLDED
    void AudioBase::setCurrentPresetName(char* newName) { *newName = '\0'; }
    
    // FUNCTION: DELAYLAMA 0x10001650 FOLDED
    void AudioBase::getCurrentPresetName(char* outText) { *outText = '\0'; }

    // FUNCTION: DELAYLAMA 0x10001900 FOLDED
    int32_t AudioBase::getPluginStateData(void* ptr, bool index) { return 0; }
    
    // FUNCTION: DELAYLAMA 0x10001910
    int32_t AudioBase::setPluginStateData(void* ptr, int32_t value, bool index) { return 0; }
    
    // -- IO --
    // FUNCTION: DELAYLAMA 0x10001970
    float AudioBase::getSampleRate() { return this->sampleRate; }
    
    // FUNCTION: DELAYLAMA 0x10001920
    void AudioBase::setSampleRate(float sampleRate) { this->sampleRate = sampleRate; }
    
    // FUNCTION: DELAYLAMA 0x10001980
    int32_t AudioBase::getMaxFramesPerProcess() { return this->blockSize; }
    
    // FUNCTION: DELAYLAMA 0x10001930
    void AudioBase::setMaxFramesPerProcess(int32_t blockSize) { this->blockSize = blockSize; }

    // FUNCTION: DELAYLAMA 0x10001950
    void AudioBase::setInputChannelCount(int32_t count) { this->plugin.inputChannelCount = count; }
    
    // FUNCTION: DELAYLAMA 0x10001960
    void AudioBase::setOutputChannelCount(int32_t count) { this->plugin.outputChannelCount = count;}
    
    // FUNCTION: DELAYLAMA 0x10001490
    bool AudioBase::isInputChannelConnected(int32_t channel) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t resultInt = this->hostCallback(&this->plugin, hostIsInputConnected, channel, 1, nullptr, 0.0);
        return resultInt == NULL;
    }
    
    // FUNCTION: DELAYLAMA 0x10001460
    bool AudioBase::isOutputChannelConnected(int32_t channel) { 
        if (this->hostCallback == nullptr)
            return false;
        int32_t resultInt = this->hostCallback(&this->plugin, hostIsInputConnected, channel, 0, nullptr, 0.0);
        return resultInt == NULL;
    }
    
    // -- Plugin Properties --
    // FUNCTION: DELAYLAMA 0x10001940
    void AudioBase::setPluginId(int32_t id) { this->plugin.id = id; }
    
    // FUNCTION: DELAYLAMA 0x100014c0
    void AudioBase::setReportsLoudnessToHost(bool reportsLoudness) {
        if (reportsLoudness) this->plugin.flags |= PluginFlags::ReportsLoudnessToHost;
        else                 this->plugin.flags &= ~PluginFlags::ReportsLoudnessToHost;
    }
    
    // FUNCTION: DELAYLAMA 0x100014e0
    void AudioBase::setHasClip(bool hasClip) {
        if (hasClip) this->plugin.flags |= PluginFlags::HasClip;
        else         this->plugin.flags &= ~PluginFlags::HasClip;
    }
    
    // FUNCTION: DELAYLAMA 0x10001520
    void AudioBase::setSupportsInPlaceProcessing(bool supportsInPlace) {
        if (supportsInPlace) this->plugin.flags |= PluginFlags::SupportsInPlaceProcessing;
        else                 this->plugin.flags &= ~PluginFlags::SupportsInPlaceProcessing;
    }
    
    // FUNCTION: DELAYLAMA 0x10001560
    void AudioBase::setHasEditor(bool hasEditor) {
        if (hasEditor) this->plugin.flags |= PluginFlags::HasEditor;
        else           this->plugin.flags &= ~PluginFlags::HasEditor;
    }
    
    // STUB: DELAYLAMA 0x10001500
    void AudioBase::setHasSoundOutput(bool hasOutput) {
        // uint uVar1;
        //
        // uVar1 = this->plugin.flags;
        // if (hasOutput) {
        //   this->plugin.flags = uVar1 | 8;
        //   return;
        // }
        // this->plugin.flags = uVar1 & 0xfffffff7;
        // return;
    }

    // FUNCTION: DELAYLAMA 0x100015a0
    void AudioBase::setAudioBase(AudioBase* base) { this->plugin.audioBase = base; }
    
    // FUNCTION: DELAYLAMA 0x100015c0
    void AudioBase::setPluginProcessingTime(int32_t processingTime) { this->plugin.pluginProcessingTime = processingTime; }

    // FUNCTION: DELAYLAMA 0x10001700
    float AudioBase::getVolume() { return 0.f; }
    
    // -- Host Communication --
    // FUNCTION: DELAYLAMA 0x100013f0
    int32_t AudioBase::getHostApiVersion() { 
        if (this->hostCallback == nullptr)
            return 1;
        int32_t hostVersionInt = this->hostCallback(&this->plugin, hostGetApiVersion, 0, NULL, nullptr, 0.0);
        if (hostVersionInt == NULL)
            return 1;
        return hostVersionInt;
    }
    
    // FUNCTION: DELAYLAMA 0x10001420
    int32_t AudioBase::getHostUniqueId() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetHostId, 0, NULL, nullptr, 0.0);
    }
    
    // FUNCTION: DELAYLAMA 0x10001440
    void AudioBase::sendIdleToHost() {
        if (this->hostCallback == nullptr)
            return;
        this->hostCallback(&this->plugin, hostProcessIdle, 0, NULL, nullptr, 0.0);
    }

    // -- String Formatting --
    // FUNCTION: DELAYLAMA 0x100015d0
    void AudioBase::formatFloatAsDecibelString(float linearValue, char* outText) {
        if (linearValue <= DECIBEL_THRESHOLD) {
            ::strcpy(outText, INF_STRING);
            return;
        }
        float dbValue = static_cast<float>(DECIBEL_FACTOR * ::log10(linearValue));
        this->formatFloatToString(dbValue, outText);
    }
    
    // FUNCTION: DELAYLAMA 0x10001660
    void AudioBase::formatSamplesAsHzString(float sampleCount, char* outText) {
        float sampleRate = this->getSampleRate();

        if (sampleCount == HZ_THRESHOLD) {
            this->formatFloatToString(0.0f, outText);
        } else {
            float hz = sampleRate / sampleCount;
            this->formatFloatToString(hz, outText);
        }
    }
    
    // FUNCTION: DELAYLAMA 0x100016c0
    void AudioBase::formatSamplesAsMsString(float sampleCount, char* outText) {
        float sampleRate = getSampleRate();               // virtual call vtable+0x88

        // milliseconds = (sampleCount * 1000) / sampleRate
        float ms = static_cast<float>((sampleCount * MS_FACTOR) / sampleRate);
        this->formatFloatToString(ms, outText);
    }
    
    // FUNCTION: DELAYLAMA 0x10001710
    void AudioBase::formatFloatToString(float value, char* outText) {
        double val = static_cast<double>(value);
        if (val >= HUGE_THRESHOLD) {
            ::strcpy(outText, HUGE_STRING);
            return;
        }

        bool negative = (val < 0.0);
        if (negative)
            val = -val;

        double intPart = ::floor(val);
        double fracPart = val - intPart;

        char intBuffer[32];
        char* p = intBuffer + sizeof(intBuffer) - 1;
        *p = '\0';

        if (intPart == 0.0) {
            *--p = '0';
        } else {
            while (intPart >= 1.0) {
                double digit = ::fmod(intPart, TEN);
                int d = static_cast<int>(digit);
                *--p = static_cast<char>('0' + d);
                intPart = ::floor(intPart * ONE_TENTH);
            }
        }

        char* out = outText;
        if (negative)
            *out++ = '-';
        ::strcpy(out, p);
        out += ::strlen(p);

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
    
    // FUNCTION: DELAYLAMA 0x10001990
    void AudioBase::formatIntToString(int32_t value, char* outSmall, int32_t unused1, int32_t unused2, char* outLarge) {
        if (value >= INT_HUGE_LIMIT) {
            ::strcpy(outSmall, HUGE_STRING);
        } else {
            ::sprintf(outSmall, "%d", value);
        }
    }

    // -- Unused --
    // FUNCTION: DELAYLAMA 0x10001580
    void AudioBase::setReservedValue(int32_t unusedValue) {
        this->plugin.zero = unusedValue;
    }
}
}