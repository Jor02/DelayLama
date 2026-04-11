#pragma once

namespace DamSDK {
namespace Api {
    /**
     * @brief Used for dispatching requests between the plugin and the host.
     */
    typedef intptr_t (*dispatchFunc)(struct DamPlugin* plugin, int32_t targetOperation, int32_t index, int32_t value, void * data, float optional);
    typedef void (*floatProcessingFunc) (struct DamPlugin* plugin, float** inputBuffer, float** outputBuffer, int32_t bufferSize);
    typedef void (*doubleProcessingFunc) (struct DamPlugin* plugin, double** inputBuffer, double** outputBuffer, int32_t bufferSize);
    typedef void (*setParameterFunc) (struct DamPlugin* plugin, int32_t parameterId, float value);
    typedef float (*getParameterFunc) (struct DamPlugin* plugin, int32_t parameterId);
    
    /**
     * @brief Generic function pointer type.
     */
    typedef void (*FuncPtr)();

    namespace PluginFlags {
        static constexpr uint32_t HasEditor                  = 1 << 0;  // 1
        static constexpr uint32_t SupportsInPlaceProcessing  = 1 << 1;  // 2
        static constexpr uint32_t HasClip                    = 1 << 2;  // 4
        static constexpr uint32_t ReportsLoudnessToHost      = 1 << 3;  // 8
        static constexpr uint32_t ProgramChunks              = 1 << 5;  // 32
        static constexpr uint32_t IsSynthesizer              = 1 << 8;  // 256
        static constexpr uint32_t IsNoRealTime               = 1 << 9;  // 512
        static constexpr uint32_t CanOverwrite               = 1 << 10;  // 1024
        static constexpr uint32_t SupportsOfflineProcessing  = 1 << 10;  // 2048
    }

    /**
     * @brief Main plugin state structure.
     *
     * This struct represents the plugin object exposed to the host.
     * The fields are intentionally low-level and closely mirror a classic
     * C-style plugin ABI.
     */
    struct DamPlugin {
        /// Magic identifier used to validate the structure.
        int magicNumber;

        /// Host dispatcher callback used by the host to send requests to the client.
        dispatchFunc dispatcherFunc;
        /// Audio processing entry point.
        floatProcessingFunc processingFunc;
        /// Callback used to set a parameter value.
        setParameterFunc settingParameterFunc;
        /// Callback used to query a parameter value.
        getParameterFunc gettingParameterFunc;

        /// Number of stored presets.
        int32_t presetCount;
        /// Number of automatable parameters.
        int32_t parameterCount;

        /// Number of input audio channels.
        int32_t inputChannelCount;
        /// Number of output audio channels.
        int32_t outputChannelCount;

        /// Bitmask of plugin capabilities and behavior flags.
        uint32_t flags;

        /// Reserved space for future use.
        unsigned char reserved[8];

        /// Internal processing-time value or host-reported timing state.
        int32_t pluginProcessingTime;

        /// Unused field.
        int32_t zero;

        /// Pointer to Audio base class.
        struct AudioBase *audioBase;

        /// Some kind of float value, I am unsure as to what it does. 
        float floatVal;

        /// Pointer to the plugin implementation object.
        void *object;
        /// User data pointer.
        void *user;

        /// Plugin identifier.
        int32_t id;
        /// Plugin version.
        int32_t version;

        /// Main audio processing function.
        floatProcessingFunc processAudioFloat;
        /// Alternate audio processing function.
        doubleProcessingFunc processAudioDouble;

        /// Unused padding
        char unused[56];
    };

    /**
     * @brief Unsigned 2D size in pixels.
     */
    struct Size {
        int32_t width;
        int32_t height;
    };

    /**
     * @brief Integer rectangle.
     */
    struct Rect {
        int16_t left;
        int16_t top;
        int16_t right;
        int16_t bottom;
    };

    struct KeyCode {
        int32_t asciiCharacter;
        unsigned char vkValue;
        unsigned char modifiers;
    };

    /**
     * @brief Requests that a plugin can send to the host.
     * 
     */
    typedef enum HostRequestType {
        hostAutomateParameter = 0,
        hostGetApiVersion = 1,
        hostGetHostId = 2,
        hostProcessIdle = 3,
        hostIsInputConnected = 4,
        // hostUnusedCallback = 5,
        hostSupportsMidiInput = 6,
        hostGetTransportTimeInfo = 7,
        hostProcessDeferredEvents = 8,
        hostSetTransportTime = 9,
        hostGetTempoAtSamplePosition = 10,
        hostGetAutomatableParameterCount = 11,
        hostGetParameterStepSize = 12,
        hostNotifyIOConfigurationChanged = 13,
        hostRequestIdleProcessing = 14,
        hostResizeEditorWindow = 15,
        hostGetSampleRate = 16,
        hostGetMaxFramesPerProcess = 17,
        hostGetInputLatencySamples = 18,
        hostGetOutputLatencySamples = 19,
        hostGetPreviousPluginInstance = 20,
        hostGetNextPluginInstance = 21,
        hostWillProcessInPlace = 22,
        hostGetProcessingContextLevel = 23,
        hostGetAutomationPlaybackState = 24,
        hostBeginOfflineProcessing = 25,
        hostOfflineReadAudio = 26,
        hostOfflineWriteAudio = 27,
        hostGetOfflineProcessingPassIndex = 28,
        hostGetOfflineProcessingSubPassIndex = 29,
        hostSetOutputSampleRate = 30,
        hostGetOutputChannelLayout = 31,
        hostGetHostCompanyName = 32,
        hostGetHostProductName = 33,
        hostGetHostVersion = 34,
        hostHandleCompanySpecific = 35,
        hostSetPluginIcon = 36,
        hostSupportsFeature = 37,
        hostGetHostLanguage = 38,
        hostOpenPluginEditor = 39,
        hostClosePluginEditor = 40,
        hostGetHostWorkingDirectory = 41,
        hostRequestEditorRefresh = 42,
        hostBeginParameterEdit = 43,
        hostEndParameterEdit = 44,
        hostOpenFileDialog = 45,
        hostCloseFileDialog = 46,
        hostOpenFileInEditor = 47,
        hostGetStateFileHandle = 48,
        hostGetInputChannelLayout = 49
    } HostRequestType;

    /**
     * @brief Requests that the host can send to the plugin.
     * 
     */
    typedef enum PluginCallbackOpcodes {
        pluginInitialize = 0,
        pluginShutdown = 1,
        pluginSetPresetByIndex = 2,
        pluginGetCurrentPreset = 3,
        pluginSetCurrentPresetName = 4,
        pluginGetCurrentPresetName = 5,
        pluginGetParameterUnitLabel = 6,
        pluginGetParameterValueText = 7,
        pluginGetParameterName = 8,
        pluginGetVolume = 9,
        pluginSetSampleRate = 10,
        pluginSetMaxFramesPerProcess = 11,
        pluginSetProcessingState = 12,
        pluginGetEditorRect = 13,
        pluginOpenEditor = 14,
        pluginCloseEditor = 15,
        pluginDrawEditor = 16,
        pluginEditorMouseEvent = 17,
        pluginEditorKeyEvent = 18,
        pluginEditorIdling = 19,
        pluginEditorTop = 20,
        pluginEditorSleep = 21,
        pluginGetIdentifier = 22,
        pluginGetPresetData = 23,
        pluginSetPresetData = 24,
        pluginProcessEvents = 25,
        pluginParameterSupportsAutomation = 26,
        pluginGetParameterNameByIndex = 27,
        pluginGetPresetCategoryCount = 28,
        pluginGetPresetNameByIndex = 29,
        pluginCopyPreset = 30,
        pluginConnectInputChannel = 31,
        pluginConnectOutputChannel = 32,
        pluginGetPluginInputSettings = 33,
        pluginGetPluginOutputSettings = 34,
        pluginGetPluginCategory = 35,
        pluginGetCurrentSamplePosition = 36,
        pluginGetOutputBuffer = 37,
        pluginNotifyOffline = 38,
        pluginPrepareOfflineRun = 39,
        pluginRunOffline = 40,
        pluginProcessVarIo = 41,
        pluginSetSpeakerArrangement = 42,
        pluginSetAudioSettings = 43,
        pluginSetBypassState = 44,
        pluginGetPluginName = 45,
        pluginGetErrorText = 46,
        pluginGetCompanyName = 47,
        pluginGetProductName = 48,
        pluginGetCompanyVersion = 49,
        plugingSpecific = 50,
        pluginSupportsFeature = 51,
        pluginGetTrailSize = 52,
        pluginIdling = 53,
        pluginGetIcon = 54,
        pluginSetViewPosition = 55,
        pluginGetParameterSettings = 56,
        pluginRequiresKeys = 57,
        pluginGetDamVersion = 58,
        pluginEditorKeyDownEvent = 59,
        pluginEditorKeyUpEvent = 60,
        pluginSetKnobMode = 61,
        pluginGetMidiPresetName = 62,
        pluginGetMidiProgram = 63,
        pluginGetCategoryOfMidiProgram = 64,
        pluginHasMidiProgramChanged = 65,
        pluginNameOfMidiKey = 66,
        pluginStartSettigMidiProgram = 67,
        pluginStopSettingMidiProgram = 68,
        pluginGetSpeakerArrangement = 69,
        pluginUnknown3 = 70,
        pluginStartProcessing = 71,
        pluginStopProcessing = 72,
        pluginSetTotalLatencySamples = 73,
        pluginSetPanLaw = 74,
        pluginLoadBank = 75,
        pluginLoadPreset = 76,
        pluginUnknown4 = 77,
        pluginGetMidiInputChannelCount = 78,
        pluginGetMidiOutputChannelCount = 79,
        pluginGetNumberOfOpcodes = 80,
    } PluginCallbackOpcodes;
}
}