#pragma once
#include "damsdk/DamPlugin.h"
#include "damsdk/AudioBase.h"
#include "damsdk/EditorBase.h"

namespace DamSDK {
    class AudioBaseExtended : public AudioBase {
    public:
        AudioBaseExtended(dispatchFunc hostCallback, int32_t presetCount, int32_t parameterCount);
        ~AudioBaseExtended();

        virtual int32_t dispatchPluginCallback(int32_t targetOperation, int32_t index, int32_t value, void * data, float optional) override;

        // --- Editor & UI ---
        virtual EditorBase* getEditor();
        virtual int32_t openWindow(HWND windowHandle);
        virtual bool closePluginEditorOnHost(HWND windowHandle);
        virtual bool requestHostEditorRefresh();
        virtual bool setViewPosition(int32_t x, int32_t y);
        virtual intptr_t getIcon();

        // --- MIDI & Events ---
        virtual void requestMidiSupport(intptr_t value);
        virtual int32_t processEvents(void* eventList);
        virtual int32_t getMidiProgram(int32_t index, void* out);
        virtual int32_t getMidiProgramName(int32_t index, char* outText);
        virtual int32_t getMidiProgramCategory(int32_t index, char* outText);
        virtual bool hasMidiProgramChanged(int32_t index);
        virtual bool getMidiKeyName(int32_t index, char* outText);
        virtual bool beginSetMidiProgram();
        virtual bool endSetMidiProgram();

        // --- Advanced Host Communication ---
        virtual int32_t getHostTransportTime(int32_t flags);
        virtual int32_t getHostTempoAtSample(float pos);
        virtual char* getHostWorkingDirectory();
        virtual bool openFileDialogOnHost(char* outText);
        virtual int32_t getHostLanguage();
        virtual bool getHostCompanyString(char* outText);
        virtual bool getHostProductString(char* outText);
        virtual int32_t getHostCompanyVersion();
        virtual bool hostSupports(char* target);

        // -- Advanced Parameters --
        virtual int32_t getHostAutomatableParameterCount();
        virtual int32_t getParameterStepSize();
        virtual bool canParameterBeAutomated(int32_t parameterId);
        virtual int32_t getParameter();
        virtual bool getParameterProperties(int32_t parameterId, void * data);
        virtual bool notifyHostClientBeginningParameterEdit(int32_t parameterId);
        virtual bool notifyHostClientEndingParameterEdit(int32_t parameterId);
        virtual int32_t getAutomationState();

        // -- Advanced Presets --
        virtual int32_t getPresetCategories();
        virtual bool getPresetNameByIndex(int32_t category, int32_t index, char *outText);
        virtual bool copyPreset(int32_t presetIndex);

        // -- Routing --
        virtual bool notifyHostIoConfigurationChanged();
        virtual int32_t getHostInputLatencySamples();
        virtual int32_t getHostOutputLatencySamples();
        virtual void connectInputBus(int32_t index, int32_t value);
        virtual void connectOutputBus(int32_t index, int32_t value);
        virtual bool getInputBusProperties(int32_t index,char *properties);
        virtual bool getOutputBusProperties(int32_t index,char *properties);
        virtual bool getOutputSpeakerArrangement(int32_t arrangement, void* param_2);
        virtual bool setOutputSpeakerArrangement(int32_t arrangement, void* param_2);
        virtual void setAudioSettings(int32_t hostBlockSize, int32_t sampleRate);
        virtual int32_t setHostOutputSampleRate(float sampleRate);
        virtual double* getHostSampleRate();

        // -- Offline Processing --
        virtual void setOfflineProcessingCapability(bool enable);
        virtual bool startOfflineProcessing(void* param_1, int32_t param_2, int32_t param_3);
        virtual bool offlineRead(float** audioBuffers, int32_t sampleFrames, bool readSource);
        virtual bool offlineWrite(float** audioBuffers, int32_t sampleFrames);
        virtual bool onOfflineNotify(void *data, int32_t value, bool isStarting);
        virtual bool prepareOffline(void *data, int32_t value);
        virtual bool runOffline(void *data, int32_t value);
        virtual bool getCurrentPass();
        virtual bool getCurrentMetaPass();

        // -- Plugin Metadata --
        virtual int32_t getPluginCategory();
        virtual bool getPluginName(char* outText);
        virtual bool getErrorText(char* outText);
        virtual bool getCompanyName(char* outText);
        virtual bool getProductName(char* outText);
        virtual int32_t getCompanyVersion();
        virtual int32_t getDamVersion();
        virtual bool pluginSupports(char* target);
        virtual void setIsSynthesizer(bool isSynthesizer);
        virtual void setCanProcessReplacing(bool canProcessReplacing);
        virtual void setOverwritingCapability(bool canOverwrite);
        virtual bool setBypass(bool bypass);
        virtual int32_t willReplaceOrAccumulate();
        virtual int32_t getCurrentProcessLevel();
        virtual int32_t getTailLengthSamples();

        // -- Workflow --
        virtual bool sizeWindow(int32_t width, int32_t height);
        virtual bool needIdle();
        virtual bool processIdle();
        virtual bool editorRequiresKeystroke();
        virtual int32_t getPreviousPlugin();
        virtual int32_t getNextPlugin();
        virtual int32_t getTransportInfo();
        virtual int32_t getOutputBuffer();
        virtual bool processVarIo(void *);
        virtual int32_t companySpecific(int32_t index, int32_t value, void *data, float optional);
        virtual intptr_t callCompanySpecific(int32_t index, int32_t valueHigh, int32_t valueLow, void* context);

        // To be categorized
        virtual int32_t getTrailSize();

        // -- Empty/Unknown Functions --
        virtual float returnZeroFloat();
    };
}