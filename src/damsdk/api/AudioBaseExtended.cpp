#include "AudioBaseExtended.h"
#include "EditorBase.h"

namespace DamSDK {
namespace Api {
    AudioBaseExtended::AudioBaseExtended(dispatchFunc hostCallback, int32_t presetCount, int32_t parameterCount) : AudioBase(hostCallback, presetCount, parameterCount) {}
    AudioBaseExtended::~AudioBaseExtended() {}

    int32_t AudioBaseExtended::dispatchPluginCallback(int32_t targetOperation, int32_t index, int32_t value, void *data, float optional) {
        switch (targetOperation) {
            case pluginProcessEvents:
                this->processEvents(data);
                break;
            case pluginParameterSupportsAutomation:
                return this->canParameterBeAutomated(index);
            case pluginGetParameterNameByIndex:
                this->getParameterName(index, (char*)data);
                break;
            case pluginGetPresetCategoryCount:
                return this->getPresetCategories();
            case pluginGetPresetNameByIndex:
                return this->getPresetNameByIndex(value, index, (char*)data);
            case pluginCopyPreset:
                this->copyPreset(index);
                break;
            case pluginConnectInputChannel:
                this->connectInputBus(index, value != NULL);
                return 1;
            case pluginConnectOutputChannel:
                this->connectOutputBus(index, value != NULL);
                return 1;
            case pluginGetPluginInputSettings:
                return this->getInputBusProperties(index, (char*)data);
            case pluginGetPluginOutputSettings:
                return this->getOutputBusProperties(index, (char*)data);
            case pluginGetPluginCategory:
                return this->getPluginCategory();
            case pluginGetCurrentSamplePosition:
                return this->getTransportInfo();
            case pluginGetOutputBuffer:
                return this->getOutputBuffer();
            case pluginNotifyOffline:
                return this->onOfflineNotify(data, value, index != NULL);
            case pluginPrepareOfflineRun:
                return this->prepareOffline(data, value);
            case pluginRunOffline:
                return this->runOffline(data, value);
            case pluginProcessVarIo:
                return this->processVarIo(data);
            case pluginSetSpeakerArrangement:
                return this->setOutputSpeakerArrangement(value, data);
            case pluginSetAudioSettings:
                this->setAudioSettings(value, optional);
                return 1;
            case pluginSetBypassState:
                return this->setBypass(value != NULL);
            case pluginGetPluginName:
                return this->getPluginName((char*) data);
            case pluginGetErrorText:
                return this->getErrorText((char*) data);
            case pluginGetCompanyName:
                return this->getHostCompanyString((char*) data);
            case pluginGetProductName:
                return this->getHostProductString((char*) data);
            case pluginGetCompanyVersion:
                return this->getCompanyVersion();
            case plugingSpecific:
                return this->companySpecific(index, value, data, optional);
            case pluginSupportsFeature:
                return this->pluginSupports((char*) data);
            case pluginGetTrailSize:
                return this->getTrailSize();
            case pluginIdling:
                return this->processIdle();
            case pluginGetIcon:
                return this->getIcon();
            case pluginSetViewPosition:
                return this->setViewPosition(index, value);
            case pluginGetParameterSettings:
                return this->getParameterProperties(index, data);
            case pluginRequiresKeys:
                return !this->editorRequiresKeystroke();
            case pluginGetDamVersion:
                return this->getDamVersion();
            case pluginEditorKeyDownEvent:
                if (this->editor != nullptr) {
                    KeyCode keyCode;
                    keyCode.asciiCharacter = index;
                    keyCode.vkValue = (unsigned char)value;
                    keyCode.modifiers = (unsigned char)optional;
                    return editor->keyDown(&keyCode);
                }
                break;
            case pluginEditorKeyUpEvent:
                if (editor != nullptr) {
                    KeyCode keyCode;
                    keyCode.asciiCharacter = index;
                    keyCode.vkValue = (unsigned char)value;
                    keyCode.modifiers = (unsigned char)optional;
                    return editor->keyUp(&keyCode);
                }
                break;
            case pluginSetKnobMode:
                if (editor != nullptr) {
                    editor->setKnobMode(value);
                }
                break;
            case pluginGetMidiPresetName:
                return this->getMidiProgramName(index, (char *) data);
            case pluginGetMidiProgram:
                return this->getMidiProgram(index, data);
            case pluginGetCategoryOfMidiProgram:
                return this->getMidiProgramCategory(index, (char *) data);
            case pluginHasMidiProgramChanged:
                return this->hasMidiProgramChanged(index);
            case pluginNameOfMidiKey:
                return this->getMidiKeyName(index, (char *) data);
            case pluginStartSettigMidiProgram:
                return this->beginSetMidiProgram();
            case pluginStopSettingMidiProgram:
                return this->endSetMidiProgram();
            default:
                return AudioBase::dispatchPluginCallback(targetOperation, index, value, data, optional);
        }

        return 0;
    }

    // --- Editor & UI ---
    EditorBase* AudioBaseExtended::getEditor() { return this->editor; }
    int32_t AudioBaseExtended::openWindow(HWND windowHandle) {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostOpenPluginEditor, 0, NULL, windowHandle, 0.0f);
    }
    bool AudioBaseExtended::closePluginEditorOnHost(HWND windowHandle) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t output = this->hostCallback(&this->plugin, hostClosePluginEditor, 0, NULL, windowHandle, 0.0f); 
        return output != NULL;
    }
    bool AudioBaseExtended::requestHostEditorRefresh() {
        if (this->hostCallback == nullptr)
            return false;
        int32_t output = this->hostCallback(&this->plugin, hostRequestEditorRefresh, 0, NULL, nullptr, 0.0f); 
        return output != NULL;
    }
    bool AudioBaseExtended::setViewPosition(int32_t x, int32_t y) { return false; }
    intptr_t AudioBaseExtended::getIcon() { return 0; }

    // --- MIDI & Events ---
    void AudioBaseExtended::requestMidiSupport(int32_t value) {
        if (this->hostCallback == nullptr)
            return;
        this->hostCallback(&this->plugin, hostSupportsMidiInput, 0, value, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::processEvents(void * data) { return 0; }
    int32_t AudioBaseExtended::getMidiProgram(int32_t index, void* out) { return -1; }
    int32_t AudioBaseExtended::getMidiProgramName(int32_t index,char* outText) { return 0; }
    int32_t AudioBaseExtended::getMidiProgramCategory(int32_t index, char* outText) { return 0; }
    bool AudioBaseExtended::hasMidiProgramChanged(int32_t index) { return false; }
    bool AudioBaseExtended::getMidiKeyName(int32_t index, char* outText) { return false; }
    bool AudioBaseExtended::beginSetMidiProgram() { return false; }
    bool AudioBaseExtended::endSetMidiProgram() { return false; }

    // --- Advanced Host Communication ---
    int32_t AudioBaseExtended::getHostTransportTime(int32_t flags) {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetTransportTimeInfo, 0, flags, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::getHostTempoAtSample(float pos) {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetTempoAtSamplePosition, 0, pos, nullptr, 0.0f);
    }
    char* AudioBaseExtended::getHostWorkingDirectory() {
        if (this->hostCallback == nullptr)
            return nullptr;
        return (char *)this->hostCallback(&this->plugin, hostGetHostWorkingDirectory, 0, NULL, nullptr, 0.0f);
    }
    bool AudioBaseExtended::openFileDialogOnHost(char* outText) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostGetHostWorkingDirectory, 0, NULL, outText, 0.0f);
        return result != NULL;
    }
    int32_t AudioBaseExtended::getHostLanguage() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetHostLanguage, 0, NULL, nullptr, 0.0f);
    }
    bool AudioBaseExtended::getHostCompanyString(char* outText) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostGetHostCompanyName, 0, NULL, outText, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::getHostProductString(char* outText) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostGetHostProductName, 0, NULL, outText, 0.0f);
        return result != NULL;
    }
    int32_t AudioBaseExtended::getHostCompanyVersion() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetHostVersion, 0, NULL, nullptr, 0.0f);
    }
    bool AudioBaseExtended::hostSupports(char* target) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostSupportsFeature, 0, NULL, target, 0.0f);
        return result != NULL;
    }

    // -- Advanced Parameters --
    int32_t AudioBaseExtended::getHostAutomatableParameterCount() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetAutomatableParameterCount, 0, NULL, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::getParameterStepSize() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetParameterStepSize, 0, NULL, nullptr, 0.0f);
    }
    bool AudioBaseExtended::canParameterBeAutomated(int32_t parameterId) { return true; }
    int32_t AudioBaseExtended::getParameter() { return 0; }
    bool AudioBaseExtended::getParameterProperties(int32_t parameterId, void * data) { return false; }
    bool AudioBaseExtended::notifyHostClientBeginningParameterEdit(int32_t parameterId) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostBeginParameterEdit, parameterId, NULL, nullptr, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::notifyHostClientEndingParameterEdit(int32_t parameterId) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostEndParameterEdit, parameterId, NULL, nullptr, 0.0f);
        return result != NULL;
    }
    int32_t AudioBaseExtended::getAutomationState() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetAutomationPlaybackState, 0, NULL, nullptr, 0.0f);
    }

    // -- Advanced Presets --
    int32_t AudioBaseExtended::getPresetCategories() { return 1; }
    bool AudioBaseExtended::getPresetNameByIndex(int32_t category, int32_t index, char *outText) { return false; }
    bool AudioBaseExtended::copyPreset(int32_t presetIndex) { return false; }

    // -- Routing --
    bool AudioBaseExtended::notifyHostIoConfigurationChanged() {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostNotifyIOConfigurationChanged, 0, NULL, nullptr, 0.0f);
        return result != NULL;
    }
    int32_t AudioBaseExtended::getHostInputLatencySamples() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetInputLatencySamples, 0, NULL, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::getHostOutputLatencySamples() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetOutputLatencySamples, 0, NULL, nullptr, 0.0f);
    }
    void AudioBaseExtended::connectInputBus(int32_t index, int32_t value) {}
    void AudioBaseExtended::connectOutputBus(int32_t index, int32_t value) {}
    bool AudioBaseExtended::getInputBusProperties(int32_t index,char *properties) { return false; }
    bool AudioBaseExtended::getOutputBusProperties(int32_t index,char *properties) { return false; }
    bool AudioBaseExtended::getOutputSpeakerArrangement(int32_t arrangement, void* param_2) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostGetOutputChannelLayout, 0, arrangement, param_2, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::setOutputSpeakerArrangement(int32_t arrangement, void* param_2) { return false; }
    void AudioBaseExtended::setAudioSettings(int32_t hostBlockSize, int32_t sampleRate) {
        this->blockSize = hostBlockSize;
        this->sampleRate = sampleRate;
    }
    int32_t AudioBaseExtended::setHostOutputSampleRate(float sampleRate) {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostSetOutputSampleRate, 0, NULL, nullptr, sampleRate);
    }
    double* AudioBaseExtended::getHostSampleRate() {
        if (this->hostCallback == nullptr)
            return 0;
        return (double *) this->hostCallback(&this->plugin, hostGetSampleRate, 0, NULL, nullptr, 0.0f);
    }

    // -- Offline Processing --
    void AudioBaseExtended::setOfflineProcessingCapability(bool enable) {
        if (enable) this->plugin.flags |= PluginFlags::SupportsOfflineProcessing;
        else        this->plugin.flags &= ~PluginFlags::SupportsOfflineProcessing;
    }
    bool AudioBaseExtended::startOfflineProcessing(void* param_1, int32_t param_2, int32_t param_3) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostBeginOfflineProcessing, param_3, param_2, param_1, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::offlineRead(float** audioBuffers, int32_t sampleFrames, bool readSource) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostOfflineReadAudio, readSource, sampleFrames, audioBuffers, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::offlineWrite(float** audioBuffers, int32_t sampleFrames) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostOfflineWriteAudio, 0, 0, audioBuffers, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::onOfflineNotify(void *data, int32_t value, bool index) { return false; }
    bool AudioBaseExtended::prepareOffline(void *data, int32_t value) { return false; }
    bool AudioBaseExtended::runOffline(void *data, int32_t value) { return false; }
    bool AudioBaseExtended::getCurrentPass() {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostGetOfflineProcessingPassIndex, 0, 0, nullptr, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::getCurrentMetaPass() {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostGetOfflineProcessingSubPassIndex, 0, 0, nullptr, 0.0f);
        return result != NULL;
    }

    // -- Plugin Metadata --
    int32_t AudioBaseExtended::getPluginCategory() {
        // Return 2 for synthesizer, 0 for audio effect
        return (this->plugin.flags & PluginFlags::IsSynthesizer) ? 2 : 0;
    }
    bool AudioBaseExtended::getPluginName(char* outText) { return false; }
    bool AudioBaseExtended::getErrorText(char* outText) { return false; }
    bool AudioBaseExtended::getCompanyName(char* outText) { return false; }
    bool AudioBaseExtended::getProductName(char* outText) { return false; }
    int32_t AudioBaseExtended::getCompanyVersion() { return 0; }
    int32_t AudioBaseExtended::getDamVersion() { return 2200; }
    bool AudioBaseExtended::pluginSupports(char* target) {
        if (strcmp(target, "receiveDamEvents") == 0 || strcmp(target, "receiveVstEvents") == 0) return 1;
        if (strcmp(target, "receiveDamMidiEvent") == 0 || strcmp(target, "receiveVstMidiEvent") == 0) return 1;
        if (strcmp(target, "sendDamMidiEvent") == 0 || strcmp(target, "sendVstMidiEvent") == 0) return 1;
        if (strcmp(target, "sendDamEvents") == 0 || strcmp(target, "sendVstEvents") == 0) return 1;
        return 0;
    }
    void AudioBaseExtended::setIsSynthesizer(bool isSynthesizer) {
        if (isSynthesizer) this->plugin.flags |= PluginFlags::IsSynthesizer;
        else               this->plugin.flags &= ~PluginFlags::IsSynthesizer;
    }
    void AudioBaseExtended::setCanProcessReplacing(bool canProcessReplacing) {
        if (canProcessReplacing) this->plugin.flags |= PluginFlags::IsNoRealTime;
        else                     this->plugin.flags &= ~PluginFlags::IsNoRealTime;
    }
    void AudioBaseExtended::setOverwritingCapability(bool canOverwrite) {
        if (canOverwrite) this->plugin.flags |= PluginFlags::CanOverwrite;
        else              this->plugin.flags &= ~PluginFlags::CanOverwrite;
    }
    bool AudioBaseExtended::setBypass(bool bypaWss) { return false; }
    int32_t AudioBaseExtended::willReplaceOrAccumulate() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostWillProcessInPlace, 0, NULL, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::getCurrentProcessLevel() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetProcessingContextLevel, 0, NULL, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::getTailLengthSamples() { return 0; }

    // -- Workflow --
    bool AudioBaseExtended::sizeWindow(int32_t width, int32_t height) {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin,hostResizeEditorWindow, 0, 0, nullptr, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::needIdle() {
        if (this->hostCallback == nullptr)
            return false;
        int32_t result = this->hostCallback(&this->plugin, hostRequestIdleProcessing, 0, 0, nullptr, 0.0f);
        return result != NULL;
    }
    bool AudioBaseExtended::processIdle() { return false; }
    bool AudioBaseExtended::editorRequiresKeystroke() { return false; }
    int32_t AudioBaseExtended::getPreviousPlugin() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetPreviousPluginInstance, 0, NULL, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::getNextPlugin() {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostGetNextPluginInstance, 0, NULL, nullptr, 0.0f);
    }
    int32_t AudioBaseExtended::getTransportInfo() { return 0; }
    int32_t AudioBaseExtended::getOutputBuffer() { return 0; }
    bool AudioBaseExtended::processVarIo(void *) { return false; }
    int32_t AudioBaseExtended::companySpecific(int32_t index, int32_t value, void *data, float optional) { return 0; }
    intptr_t AudioBaseExtended::callCompanySpecific(int32_t index, int32_t valueHigh, int32_t valueLow, void* context) {
        if (this->hostCallback == nullptr)
            return 0;
        return this->hostCallback(&this->plugin, hostHandleCompanySpecific, index, valueHigh, context, valueLow);
    }

    // To be categorized
    int32_t AudioBaseExtended::getTrailSize() { return 0; }

    // -- Empty/Unknown Functions --
    float AudioBaseExtended::returnZeroFloat() { return 0.f; }
}
}