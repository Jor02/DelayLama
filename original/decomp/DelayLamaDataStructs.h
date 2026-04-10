typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;
typedef unsigned long long    GUID;

typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned int    uint3;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined3;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    wchar16;
typedef unsigned short    word;
typedef struct AudioBase AudioBase, *PAudioBase;

typedef struct AudioBaseVTable AudioBaseVTable, *PAudioBaseVTable;

typedef struct AudioBaseMembers AudioBaseMembers, *PAudioBaseMembers;

typedef struct DelayLamaEditor DelayLamaEditor, *PDelayLamaEditor;

typedef int int32_t;

typedef struct DamPlugin DamPlugin, *PDamPlugin;

typedef int32_t * (*HostCallback)(struct DamPlugin *, int32_t, int32_t, int32_t *, void *, float);

typedef struct DelayLamaEditorVTable DelayLamaEditorVTable, *PDelayLamaEditorVTable;

typedef struct EditorBaseMembers EditorBaseMembers, *PEditorBaseMembers;

typedef struct Monk Monk, *PMonk;

typedef struct TwoAxisSlider TwoAxisSlider, *PTwoAxisSlider;

typedef struct VerticalSlider VerticalSlider, *PVerticalSlider;

typedef struct HorizontalSlider HorizontalSlider, *PHorizontalSlider;

typedef struct Knob Knob, *PKnob;

typedef struct SplashScreen SplashScreen, *PSplashScreen;

typedef struct Bitmap Bitmap, *PBitmap;

typedef struct EditorBaseVTable EditorBaseVTable, *PEditorBaseVTable;

typedef struct DelayLamaPlugin DelayLamaPlugin, *PDelayLamaPlugin;

typedef struct HWND__ HWND__, *PHWND__;

typedef struct HWND__ *HWND;

typedef int BOOL;

typedef struct Rect Rect, *PRect;

typedef struct Window Window, *PWindow;

typedef ulong DWORD;

typedef struct MonkVTable MonkVTable, *PMonkVTable;

typedef struct TileGridMembers TileGridMembers, *PTileGridMembers;

typedef struct TwoAxisSliderVTable TwoAxisSliderVTable, *PTwoAxisSliderVTable;

typedef struct HorizontalSliderMembers HorizontalSliderMembers, *PHorizontalSliderMembers;

typedef struct VerticalSliderVTable VerticalSliderVTable, *PVerticalSliderVTable;

typedef struct ControlMembers ControlMembers, *PControlMembers;

typedef struct GDIDrawingContext GDIDrawingContext, *PGDIDrawingContext;

typedef struct Control Control, *PControl;

typedef struct tagPOINT tagPOINT, *PtagPOINT;

typedef struct tagPOINT POINT;

typedef struct HorizontalSliderVTable HorizontalSliderVTable, *PHorizontalSliderVTable;

typedef struct KnobVTable KnobVTable, *PKnobVTable;

typedef struct RotaryControlMembers RotaryControlMembers, *PRotaryControlMembers;

typedef struct SplashScreenVTable SplashScreenVTable, *PSplashScreenVTable;

typedef struct tagRECT tagRECT, *PtagRECT;

typedef struct tagRECT RECT;

typedef struct HBITMAP__ HBITMAP__, *PHBITMAP__;

typedef struct HBITMAP__ *HBITMAP;

typedef struct DelayLamaPluginVTable DelayLamaPluginVTable, *PDelayLamaPluginVTable;

typedef struct DelayLamaAudioMembers DelayLamaAudioMembers, *PDelayLamaAudioMembers;

typedef short int16_t;

typedef struct WindowVTable WindowVTable, *PWindowVTable;

typedef struct ViewMembers ViewMembers, *PViewMembers;

typedef struct View View, *PView;

typedef struct TileGridVTable TileGridVTable, *PTileGridVTable;

typedef struct ControlVTable ControlVTable, *PControlVTable;

typedef struct GDIDrawingContextVtable GDIDrawingContextVtable, *PGDIDrawingContextVtable;

typedef struct GDIDrawingContextMembers GDIDrawingContextMembers, *PGDIDrawingContextMembers;

typedef long LONG;

typedef struct RotaryControlVTable RotaryControlVTable, *PRotaryControlVTable;

typedef union Color Color, *PColor;

typedef struct DelayLamaAudioVTable DelayLamaAudioVTable, *PDelayLamaAudioVTable;

typedef struct AudioBaseExtendedMembers AudioBaseExtendedMembers, *PAudioBaseExtendedMembers;

typedef struct Preset Preset, *PPreset;

typedef struct MidiEvent MidiEvent, *PMidiEvent;

typedef struct ViewVTable ViewVTable, *PViewVTable;

typedef struct HDC__ HDC__, *PHDC__;

typedef struct HDC__ *HDC;

typedef void *HGDIOBJ;

typedef uint UINT;

typedef struct ColorBytes ColorBytes, *PColorBytes;

typedef DWORD COLORREF;

typedef struct AudioBaseExtendedVTable AudioBaseExtendedVTable, *PAudioBaseExtendedVTable;

struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};

struct ColorBytes {
    byte r;
    byte g;
    byte b;
    byte a;
};

union Color {
    struct ColorBytes bytes;
    COLORREF rgba;
};

struct tagPOINT {
    LONG x;
    LONG y;
};

struct GDIDrawingContextMembers {
    POINT screenPos;
    POINT drawOffset;
    HDC hDC;
    HWND hWnd;
    void *parentFrame;
    undefined field5_0x1c;
    undefined field6_0x1d;
    undefined field7_0x1e;
    undefined field8_0x1f;
    undefined field9_0x20;
    undefined field10_0x21;
    undefined field11_0x22;
    undefined field12_0x23;
    union Color textColor;
    undefined4 field14_0x28;
    undefined4 field15_0x2c;
    LONG penWidth;
    union Color penColor;
    union Color backgroundColor;
    union Color penDashColor;
    undefined4 field20_0x40;
    RECT rect;
    HGDIOBJ obj1;
    HGDIOBJ obj2;
    HGDIOBJ obj3;
    HGDIOBJ originalPen;
    HGDIOBJ OriginalBrush;
    HGDIOBJ OriginalFont;
    UINT penStyle;
};

struct GDIDrawingContext { /* PlaceHolder Class Structure */
    struct GDIDrawingContextVtable *vtable;
    struct GDIDrawingContextMembers members;
};

struct ViewMembers {
    undefined4 referenceCount;
    RECT rect;
    RECT absRect;
    struct Window *parent;
    undefined4 field4_0x28;
    bool isDirty;
    bool isEnabled;
    bool useAlphaBlending;
};

struct ControlMembers {
    struct ViewMembers viewMembers;
    undefined field1_0x2f;
    void (*drawControl)(void *, struct GDIDrawingContext *, struct Control *);
    undefined4 parameterId;
    float prevValue;
    float defaultValue;
    float value;
    float min;
    float max;
    float wheelSensitivity;
    struct Bitmap *bitmap;
};

struct HorizontalSliderMembers {
    struct ControlMembers control;
    POINT backgroundOffset;
    POINT handlePos; /* Created by Rename Structure Field action */
    struct Bitmap *handleImage;
    int handleWidth;
    int handleHeight;
    int trackMinX;
    int trackMaxX;
    int flags;
    int trackLeftX;
    int handleMinPos;
    int handleMaxPos;
    int trackWidth;
    int trackHeight;
    float currentValue;
    bool isHandleTransparent;
    bool snapToMouse;
};

struct HorizontalSlider { /* PlaceHolder Class Structure */
    struct HorizontalSliderVTable *vtable;
    struct HorizontalSliderMembers members;
};

struct ViewVTable {
    void *destructor;
    void *onDraw;
    void *onMouseDown;
    void *update;
    void *routeMessage;
    void *onMouseWheel;
    void *draw1;
    void *draw2;
    void *isDirty;
    void *setDirty;
    void *setEnabled;
    void *getEnabled;
    void *setAbsRect;
    void *getRect;
    void *setUseAlphaBlending;
    void *getUseAlphaBlending;
    void *setRect;
    void *setParent;
    void *attached;
    void *returnTrue;
    void *release;
    void *useBitmap;
    void *getReferenceCount;
};

struct WindowVTable {
    struct ViewVTable view;
    void *registerChild;
    void *removeChild;
    void *destroyChildren;
    void *containsChild;
};

struct DamPlugin {
    int magicNumber;
    HostCallback dispatcherFunc; /* Created by Rename Structure Field action */
    HostCallback processFunc; /* Created by Rename Structure Field action */
    HostCallback setParameterFunc; /* Created by Rename Structure Field action */
    HostCallback getParameterFunc; /* Created by Rename Structure Field action */
    int32_t presetCount;
    int32_t parameterCount; /* Created by Rename Structure Field action */
    int32_t inputCount; /* Created by Rename Structure Field action */
    int32_t outputCount; /* Created by Rename Structure Field action */
    int32_t flags;
    byte reserved[8];
    int32_t pluginProcessingTime;
    int32_t zero;
    struct AudioBase *audioBase;
    float ratio;
    struct AudioBase *object;
    void *user;
    int32_t id;
    int32_t version;
    void *processAudioFloat;
    void *processAudioDouble;
    char unused[56];
};

struct AudioBaseMembers {
    float sampleRate;
    struct DelayLamaEditor *editor;
    HostCallback hostCallback;
    int presetCount;
    int parameterCount;
    int currentProgram;
    int blockSize;
    struct DamPlugin plugin;
};

struct AudioBase { /* PlaceHolder Class Structure */
    struct AudioBaseVTable *vtable;
    struct AudioBaseMembers members;
};

struct TileGridMembers {
    struct ControlMembers control;
    POINT srcOffset; /* Created by Rename Structure Field action */
    int tileWidth; /* Created by Rename Structure Field action */
    int tileHeight; /* Created by Rename Structure Field action */
};

struct Preset {
    float portTime;
    float delay;
    float headSize;
    char name[24];
};

struct ControlVTable {
    struct ViewVTable view;
    void *onIdle;
    void *setValue;
    void *getValue;
    void *setMin;
    void *getMin;
    void *setMax;
    void *getMax;
    void *setNumInputs;
    void *getPreviousValue;
    void *setNumOutputs;
    void *getDefaultValue;
    void *setParameterId;
    void *changeBitmap;
    void *getBitmap;
    void *setWheelSensitivity;
    void *getWheelSensitivity;
    void *clampValue;
};

struct RotaryControlVTable {
    struct ControlVTable control;
    void *drawIndicator;
    void *setStartAngle;
    void *getStartAngle;
    void *setTotalRange;
    void *getTotalRange;
    void *calculateXYFromValue;
    void *calculateAngleFromPoint;
    void *setKnobRadius;
    void *setIndicatorShadowColor;
    void *setIndicatorHighlightColor;
    void *setBitmap;
    void *setFineTuneDivider;
    void *getFineTuneDivider;
};

struct KnobVTable {
    struct RotaryControlVTable rotaryControl;
};

struct AudioBaseVTable {
    void *destructor;
    void *setParameterValue;
    void *getParameterValue;
    void *automateHostParameter;
    void *invokeAudioProcess;
    void *processAudio;
    void *dispatchPluginCallback;
    void *initializePlugin;
    void *shutdownPlugin;
    void *getActivePresetIndex;
    void *loadPresetByIndex;
    void *setCurrentPresetName;
    void *getCurrentPresetName;
    void *getParameterUnitLabel;
    void *getParameterValueString;
    void *getParameterName;
    void *getVolume;
    void *getPluginStateData;
    void *setPluginStateData;
    void *setSampleRate;
    void *setMaxFramesPerProcess;
    void *disableAudioProcessing;
    void *enableAudioProcessing;
    void *setPluginId;
    void *setInputChannelCount;
    void *setOutputChannelCount;
    void *setReportsLoudnessToHost;
    void *setHasClip;
    void *setHasSoundOutput;
    void *setSupportsInPlaceProcessing;
    void *setHasEditor;
    void *setReservedValue;
    void *setPluginProcessingTime;
    void *setAudioBase;
    void *getSampleRate;
    void *getMaxFramesPerProcess;
    void *getHostVersion;
    void *getHostUniqueId;
    void *sendIdleToHost;
    void *isOutputChannelConnected;
    void *isInputChannelConnected;
    void *formatFloatAsDecibelString;
    void *formatSamplesAsHzString;
    void *formatSamplesAsMsString;
    void *formatFloatToString;
    void *formatIntToString;
};

struct AudioBaseExtendedVTable {
    struct AudioBaseVTable audioEffect;
    void *getEditor;
    void *requestMidiSupport;
    void *getHostTransportTime;
    void *getHostTempoAtSample;
    void *processEvents;
    void *getHostAutomatableParameterCount;
    void *getParameterStepSize;
    void *canParameterBeAutomated;
    void *getParameter;
    void *returnZeroFloat;
    void *getPresetCategories;
    void *getPresetNameByIndex;
    void *copyPreset;
    void *notifyHostIoConfigurationChanged;
    void *needIdle;
    void *sizeWindow;
    void *getHostSampleRate;
    void *getBlockSize;
    void *getInputLatencySamples;
    void *getOutputLatencySamples;
    void *getPreviousPlugin;
    void *getNextPlugin;
    void *connectInputBus;
    void *connectOutputBus;
    void *getInputBusProperties;
    void *getOutputBusProperties;
    void *getPluginCategory;
    void *willOverwriteOutput;
    void *getCurrentProcessLevel;
    void *getAutomationState;
    void *setOverwritingCapability;
    void *setOfflineProcessingCapability;
    void *getTransportInfo;
    void *getOutputBuffer;
    void *offlineRead;
    void *offlineWrite;
    void *startOfflineProcessing;
    void *getCurrentPass;
    void *getCurrentMetaPass;
    void *onOfflineNotify;
    void *prepareOffline;
    void *runOffline;
    void *returnZero1;
    void *returnZero2;
    void *setHostOutputSampleRate;
    void *getOutputSpeakerArrangement;
    void *getHostCompanyString;
    void *getHostProductString;
    void *getHostVersion;
    void *callCompanySpecific;
    void *hostSupports;
    void *setIsSynth;
    void *setSupportsOutputOverwrite;
    void *getHostLanguage;
    void *openWindow;
    void *closePluginEditorOnHost;
    void *getHostWorkingDirectory;
    void *requestHostEditorRefresh;
    void *processVarIo;
    void *setOutputSpeakerArrangement;
    void *setAudioSettings;
    void *setBypass;
    void *getEffectName;
    void *getErrorText;
    void *getCompanyName;
    void *getProductName;
    void *getCompanyVersion;
    void *companySpecific;
    void *pluginSupports;
    void *getIcon;
    void *setViewPosition;
    void *getTailLengthSamples;
    void *processIdle;
    void *getParameterProperties;
    void *editorRequiresKeystroke;
    void *getDamVersion;
    void *getMidiProgramName;
    void *getMidiProgram;
    void *getMidiProgramCategory;
    void *hasMidiProgramChanged;
    void *getMidiKeyName;
    void *beginSetMidiProgram;
    void *endSetMidiProgram;
    void *notifyHostClientBeginningParameterEdit;
    void *notifyHostClientEndingParameterEdit;
    void *openFileDialogOnHost;
};

struct DelayLamaAudioVTable {
    struct AudioBaseExtendedVTable audioEffectX;
    void *initPresets;
    void *getRandomFloat;
    void *sendMidiToHost;
};

struct DelayLamaPluginVTable {
    struct DelayLamaAudioVTable audioVtable;
};

struct Rect {
    int16_t left;
    int16_t top;
    int16_t right;
    int16_t bottom;
};

struct EditorBaseMembers {
    struct DelayLamaPlugin *mainPlugin;
    HWND hParent;
    BOOL needsRedraw;
    struct Rect rect;
    struct Window *frame;
    DWORD lastIdleTick;
    bool isInIdleUpdate;
};

struct DelayLamaEditor {
    struct DelayLamaEditorVTable *vtable;
    struct EditorBaseMembers guiEditor;
    char unused[3];
    void *drawControl;
    struct Monk *monk;
    struct TwoAxisSlider *autoClass10;
    struct VerticalSlider *singingVerticalSlider;
    struct HorizontalSlider *singingHorizontalSlider;
    struct HorizontalSlider *reverbSlider;
    struct Knob *leftKnob;
    struct Knob *knob;
    struct SplashScreen *splashScreen;
    struct Bitmap *backgroundBitmap;
    struct Bitmap *monkSpriteSheetBitmap;
    struct Bitmap *singingYHandleBitmap;
    struct Bitmap *singingXHandleBitmap;
    struct Bitmap *reverbHandleBitmap;
    struct Bitmap *leftKnobBitmap;
    struct Bitmap *rightKnobBitmap;
    struct Bitmap *aboutScreenBitmap;
};

struct TileGridVTable {
    struct ControlVTable control;
};

struct MonkVTable {
    struct TileGridVTable tileGrid;
};

struct SplashScreenVTable {
    struct ControlVTable control;
};

struct VerticalSlider { /* PlaceHolder Class Structure */
    struct VerticalSliderVTable *vtable;
    struct ControlMembers control;
    POINT backgroundOffset;
    POINT handlePos;
    struct Bitmap *handleImage;
    undefined4 handleWidth;
    int handleHeight;
    int trackMin;
    int trackMaxY;
    undefined4 flags;
    int trackTopY;
    int handleMinPos;
    int handleMaxPos;
    undefined4 trackWidth;
    int trackHeight;
    undefined4 fineTuneDivider;
    bool isHandleTransparent;
    bool snapToMouse;
};

struct TwoAxisSlider { /* PlaceHolder Class Structure */
    struct TwoAxisSliderVTable *vtable;
    struct HorizontalSliderMembers slider;
    undefined field2_0x9a;
    undefined field3_0x9b;
    int yTop;
    int yBottom;
    int field6_0xa4;
    int xMinOffset;
    int xMaxOffset;
    int yTrackBottom;
    float xRange;
    float yRange;
    int halfHandleHeight1;
    float xValueRange;
    float yValueRange;
    int halfHandleHeight2;
};

struct VerticalSliderVTable {
    struct ControlVTable control;
    void *setIsHandleTransparent;
    void *setSnapToMouse;
    void *getSnapToMouse;
    void *setHandlePos;
    void *changeHandle;
    void *getTrackLength;
    void *setCurrentValue;
    void *getCurrentValue;
};

struct HBITMAP__ {
    int unused;
};

struct GDIDrawingContextVtable {
    void *Destructor_0x0;
};

struct EditorBaseVTable {
    void *destructor;
    void *getRect;
    void *open;
    void *close;
    void *onIdle;
    void *unimplemented;
    void *invalidate;
    void *keyDown;
    void *keyUp;
    void *setKnobMode;
    void *unkownFunction;
    void *dispatcher;
    void *draw;
    void *idleHandler;
};

struct DelayLamaEditorVTable {
    struct EditorBaseVTable editorBase;
};

struct RotaryControlMembers {
    struct ControlMembers control; /* Type 'AutoClass4Members.conflict' was deleted */
    undefined4 srcPoint;
    undefined4 y;
    union Color indicatorHighlightColor;
    union Color indicatorShadowColor;
    struct Bitmap *bmp;
    float knobRadius;
    float startAngle;
    float totalRange;
    undefined4 deadZoneSize;
    undefined4 angleRange;
    undefined4 angleOffset;
    float center;
    float fineTuneDivider;
};

struct Knob { /* PlaceHolder Class Structure */
    struct KnobVTable *vtable;
    struct RotaryControlMembers rotaryControl;
    int totalFrames;
    int frameHeight;
};

struct HorizontalSliderVTable {
    struct ControlVTable control;
    void *setIsHandleTransparent;
    void *setSnapToMouse;
    void *getSnapToMouse;
    void *setHandlePos;
    void *changeHandle;
    void *getTrackLength;
    void *setCurrentValue;
    void *getCurrentValue;
};

struct TwoAxisSliderVTable {
    struct HorizontalSliderVTable slider;
};

struct MidiEvent {
    int timestamp;
    int status;
    int data1;
    int data2;
};

struct Bitmap { /* image */
    uint resourceId;
    undefined4 refCount;
    undefined4 width;
    undefined4 height;
    HBITMAP bitmap;
    undefined4 maskBitmap;
};

struct Control { /* PlaceHolder Class Structure */
    struct ControlVTable *vtable;
    struct ControlMembers members;
};

struct SplashScreen { /* PlaceHolder Class Structure */
    struct SplashScreenVTable *vtable;
    struct ControlMembers control;
    RECT destRect;
    RECT absRect;
    POINT srcPoint;
};

struct Monk { /* PlaceHolder Class Structure */
    struct MonkVTable *vtable;
    struct TileGridMembers tileGrid;
};

struct Window { /* PlaceHolder Class Structure */
    struct WindowVTable *vtable;
    struct ViewMembers view;
    undefined field2_0x33;
    struct DelayLamaEditor *editor;
    HWND handle;
    struct Bitmap *backgroundBitmap;
    undefined4 numChildren;
    undefined4 maxChildren;
    struct Control **children;
    struct View *modalView;
    undefined4 *field10_0x50;
    bool redrawPending;
    bool visible;
    undefined1 field13_0x56;
    undefined field14_0x57;
    HWND hWnd;
    bool isActive;
    undefined field17_0x5d;
    undefined field18_0x5e;
    undefined field19_0x5f;
    void *closeParameter;
};

struct AudioBaseExtendedMembers {
    struct AudioBaseMembers audioEffectMembers;
};

struct DelayLamaAudioMembers {
    struct AudioBaseExtendedMembers audioEffectXMembers;
    float portamentoTime;
    float curVowelValue;
    float prevVowelValue;
    float delay;
    float scaledVowel; /* Created by Rename Structure Field action */
    float headSize; /* Created by Rename Structure Field action */
    float vibratoDepthCurrent; /* Created by Rename Structure Field action */
    float vibratoAmount; /* Created by Rename Structure Field action */
    float pitchValue; /* Created by Rename Structure Field action */
    bool vibratoDirty; /* Created by Rename Structure Field action */
    bool pitchValueDirty;
    char unusedBytes00[2];
    struct Preset (*presets)[5];
    int32_t midiEventReadIndex;
    struct MidiEvent midiQueue[1024];
    struct MidiEvent *damMidiEvent;
    int damMidiEventSize; /* Created by Rename Structure Field action */
    int unknownMidi2;
    char unusedBytes01[12];
    byte midiStatus; /* Created by Rename Structure Field action */
    byte midiData1; /* Created by Rename Structure Field action */
    byte midiData2; /* Created by Rename Structure Field action */
    char unusedBytes02[5];
    void *eventContainer;
    int field25_0x4100;
    struct MidiEvent *midiEvent;
    int unknownMidi;
    int32_t midiDataValue;
    float vowelFilterState1;
    float vowelFilterState2;
    int pitchBase; /* Midi Note */
    int pitchTargetRaw; /* Midi Note */
    float outputGain;
    int pitchInterpData1[1024]; /* Created by Rename Structure Field action */
    int pitchInterpData2[1024]; /* Created by Rename Structure Field action */
    int isInterpActive; /* Created by Rename Structure Field action */
    int interpEventCount; /* Created by Rename Structure Field action */
    int interpSampleStep; /* Created by Rename Structure Field action */
    int interpCurrentIdx; /* Created by Rename Structure Field action */
    int synthesisFrameCounter;
    float vowelTargetValue;
    bool pitchTargetDirty;
    bool isGlideActive;
    bool vowelFilterDirty;
    bool unusedBool;
    int pitchTargetValue; /* Created by Rename Structure Field action */
    int vowelMorphDelta;
    float vowelMorph; /* Created by Rename Structure Field action */
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
    int noteStack[128]; /* Created by Rename Structure Field action */
    int writeIndex; /* Created by Rename Structure Field action */
    int frequencyIndex;
    float unusedFloat;
    float frequencyValue;
    int excitationWriteIndex; /* Created by Rename Structure Field action */
    int attackSamples;
    int sustainStart;
    int releaseSamples;
    float vowelLookupIndex; /* Created by Rename Structure Field action */
    float formant1Bandwidth;
    float formant2Bandwidth;
    float formant3Bandwidth;
    char unusedBytes06[8];
    float resonanceGain; /* Created by Rename Structure Field action */
    int totalSmoothingFrames;
    int smoothCounter;
    int smoothingFrames;
    int smoothStep;
    int pitchCurrent;
    int pitchTarget; /* Created by Rename Structure Field action */
    int pitchDelta; /* Created by Rename Structure Field action */
    int pitchSmoothingFramesRemaining;
    int pitchStep; /* Created by Rename Structure Field action */
    float vibratoCurrent;
    int vibratoTarget; /* Created by Rename Structure Field action */
    int vibratoDelta; /* Created by Rename Structure Field action */
    int vibratoStep; /* Created by Rename Structure Field action */
    int vibratoSmoothingFramesRemaining;
    float sampleRate;
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
    int **stereoDelayLBuffer;
    int **stereoDelayRBuffer;
    float delayFeedback;
    int delayWriteIndex;
    int delayReadIndexL;
    int delayReadIndexR;
    float **synthesisBuffer; /* Created by Rename Structure Field action */
    int numSamples; /* Created by Rename Structure Field action */
    int **excitationBuffer;
    int excitationBufferSize;
    int excitationReadIndex; /* Created by Rename Structure Field action */
    float **sineTable;
    int sineTableSize;
    float **formantTable; /* Created by Rename Structure Field action */
    int formantTableSize;
    float **vocalEnvelope; /* Created by Rename Structure Field action */
    float **glottalSource; /* Created by Rename Structure Field action */
    int glottalTableSize;
    float glottalPhaseInc;
    float **harmonicBuffer; /* Created by Rename Structure Field action */
    float *frequencyTable;
    int frequencyTableSize; /* 4096 */
    float *formantTable1;
    float *formantTable2;
    float *formantTable3;
};

struct DelayLamaPlugin {
    struct DelayLamaPluginVTable *vtable;
    struct DelayLamaAudioMembers members;
};

struct View { /* PlaceHolder Class Structure */
    struct ViewVTable *vtable;
    struct ViewMembers members;
};

struct HWND__ {
    int unused;
};

struct HDC__ {
    int unused;
};

typedef struct AudioBaseExtended AudioBaseExtended, *PAudioBaseExtended;

struct AudioBaseExtended { /* PlaceHolder Class Structure */
    struct AudioBaseExtendedVTable *vtable;
    struct AudioBaseExtendedMembers members;
};

typedef struct DamEvent DamEvent, *PDamEvent;

typedef char int8_t;

struct DamEvent {
    int type;
    char unknown[4];
    int value;
    char unknown2[12];
    int8_t a;
    int8_t b;
    int8_t c;
};

typedef struct DamEventList DamEventList, *PDamEventList;

struct DamEventList {
    int count;
    int unknown;
    struct DamEvent *events;
};

typedef enum HostRequestType {
    hostAutomateParameter=0,
    hostGetApiVersion=1 /* host version of DAM */,
    hostGetCurrentPluginId=2,
    hostProcessIdle=3 /* idle call */,
    hostIsInputConnected=4,
    hostUnusedCallback = 5,
    hostSupportsMidiInput=6,
    hostGetTransportTimeInfo=7 /* request time info */,
    hostProcessDeferredEvents=8,
    hostSetTransportTime=9,
    hostGetTempoAtSamplePosition=10,
    hostGetAutomatableParameterCount=11,
    hostGetParameterStepSize=12,
    hostNotifyIOConfigurationChanged=13,
    hostRequestIdleProcessing=14,
    hostResizeEditorWindow=15,
    hostGetSampleRate=16,
    hostGetMaxFramesPerProcess=17,
    hostGetInputLatencySamples=18,
    hostGetOutputLatencySamples=19,
    hostGetPreviousPluginInstance=20,
    hostGetNextPluginInstance=21,
    hostWillProcessInPlace=22,
    hostGetProcessingContextLevel=23,
    hostGetAutomationPlaybackState=24,
    hostBeginOfflineProcessing=25,
    hostOfflineReadAudio=26,
    hostOfflineWriteAudio=27,
    hostGetOfflineProcessingPassIndex=28,
    hostGetOfflineProcessingSubPassIndex=29,
    hostSetOutputSampleRate=30,
    hostGetOutputChannelLayout=31,
    hostGetHostCompanyName=32,
    hostGetHostProductName=33,
    hostGetHostVersion=34,
    hostHandleCompanySpecific=35,
    hostSetPluginIcon=36,
    hostSupportsFeature=37,
    hostGetHostLanguage=38,
    hostOpenPluginEditor=39,
    hostClosePluginEditor=40,
    hostGetHostWorkingDirectory=41,
    hostRequestEditorRefresh=42,
    hostBeginParameterEdit=43,
    hostEndParameterEdit=44,
    hostOpenFileDialog=45,
    hostCloseFileDialog=46,
    hostOpenFileInEditor=47,
    hostGetStateFileHandle=48,
    hostGetInputChannelLayout=49
} HostRequestType;

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

typedef struct DropTarget DropTarget, *PDropTarget;

typedef struct DropTargetVTable DropTargetVTable, *PDropTargetVTable;

struct DropTarget { /* PlaceHolder Class Structure */
    struct DropTargetVTable *vtable;
    int refCount;
    bool canAcceptDrop;
    undefined field3_0x9;
    undefined field4_0xa;
    undefined field5_0xb;
    struct Window *parentFrame;
};

struct DropTargetVTable {
    void *queryInterface;
    void *addRef;
    void *release;
    void *dragEnter;
    void *dragOver;
    void *returnZero;
    void *drop;
    void *destructor;
};

typedef struct EditorBase EditorBase, *PEditorBase;

struct EditorBase { /* PlaceHolder Class Structure */
    struct EditorBaseVTable *vtable;
    struct EditorBaseMembers members;
};

typedef struct RotaryControl RotaryControl, *PRotaryControl;

struct RotaryControl { /* PlaceHolder Class Structure */
    struct RotaryControlVTable *vtable;
    struct RotaryControlMembers members;
};

typedef struct TileGrid TileGrid, *PTileGrid;

struct TileGrid { /* PlaceHolder Class Structure */
    struct TileGridVTable *vtable;
    struct TileGridMembers members;
};

typedef struct OffscreenGDIDrawingContextVTable OffscreenGDIDrawingContextVTable, *POffscreenGDIDrawingContextVTable;

struct OffscreenGDIDrawingContextVTable {
    void *destructor;
};

typedef struct UnusedClassVTable UnusedClassVTable, *PUnusedClassVTable;

struct UnusedClassVTable {
    void *destructor;
    void *zeroParam;
    void *setParam;
    void *unimplemented1;
    void *checkValue;
    void *unimplemented2;
    void *invalidate;
    void *getNotImplementedResult1;
    void *getNotImplementedResult2;
    void *returnZero4;
    void *returnZero6;
};

typedef struct DelayLamaAudio DelayLamaAudio, *PDelayLamaAudio;

struct DelayLamaAudio { /* PlaceHolder Class Structure */
    struct DelayLamaAudioVTable *vtable;
    struct DelayLamaAudioMembers members;
};

typedef struct EditorRect EditorRect, *PEditorRect;

struct EditorRect {
    undefined4 field0_0x0;
    undefined4 field1_0x4;
    undefined4 field2_0x8;
    undefined4 field3_0xc;
    undefined4 field4_0x10;
    undefined field5_0x14;
    undefined field6_0x15;
};

typedef struct Range Range, *PRange;

typedef int __int32_t;

struct Range {
    __int32_t min;
    __int32_t max;
};

typedef struct UnusedClass UnusedClass, *PUnusedClass;

struct UnusedClass { /* PlaceHolder Class Structure */
    struct UnusedClassVTable *vtable;
    int field1_0x4;
    int field2_0x8;
    int field3_0xc;
};

