#pragma once
#include <cstdint>
#include <windows.h>

// Forward Decls
class AudioBase;
class AudioBaseExtended;
class Bitmap;
class Control;
class DelayLamaAudio;
class DelayLamaEditor;
class DelayLamaPlugin;
class DropTarget;
class EditorBase;
class GDIDrawingContext;
class HorizontalSlider;
class Knob;
class Monk;
class OffscreenGDIDrawingContext;
class RotaryControl;
class SplashScreen;
class TileGrid;
class TwoAxisSlider;
class UnusedClass;
class VerticalSlider;
class View;
class Window;
class DrawControl;
struct Preset;
struct Range;

// DamSDK
struct DamPlugin;
struct DamEventList;
struct Color;
struct Rect;
typedef void * HostCallback;

// --- Class Definitions ---
class AudioBase {
public:
    uint32_t _dispatcher(DamPlugin* effect, int32_t opcode, int32_t index, int32_t value, void* ptr, float opt);
    float _getParameter(DamPlugin* effect, int32_t index);
    void _setParameter(DamPlugin* effect, int32_t index, float parameter);
    void _process(DamPlugin* effect, float* * inputs, float* * outputs, int32_t sampleFrames);
    AudioBase(AudioBase* pThis, HostCallback hostCallback, int presetCount, int parameterCount);
    void destructAudioBase(bool destroyObject);
    void Destroy();
    int32_t dispatchPluginCallback(int32_t opcode, int32_t* index, int32_t* value, void* ptr, float opt);
    uint32_t getHostVersion();
    void getHostUniqueId();
    void sendIdleToHost();
    bool isOutputChannelConnected(uint32_t pinConnected);
    bool isInputChannelConnected(uint32_t pinConnected);
    void setReportsLoudnessToHost(bool reportsLoudness);
    void setHasClip(bool hasClip);
    void setHasSoundOutput(bool hasOutput);
    void setSupportsInPlaceProcessing(bool canProcess);
    void generic14();
    long double getParameterValue();
    void setHasEditor(bool hasEditor);
    void setReservedValue(uint32_t unusedValue);
    void processAudio();
    void setPluginProcessingTime(AudioBase* base);
    void setAudioBase(uint32_t processingTime);
    void formatFloatAsDecibelString(float linearValue, char* outText);
    uint32_t getActivePresetIndex();
    void loadPresetByIndex(uint32_t presetIndex);
    void generic2(char* newName);
    void formatSamplesAsHzString(float sampleCount, char* outText);
    void generic7(char* param_2);
    void formatSamplesAsMsString(float sampleCount, char* outText);
    long double getVolume();
    void formatFloatToString(float value, char* outText);
    uint32_t generic10();
    uint32_t setPluginStateData();
    void setSampleRate(uint32_t sampleRate);
    void setMaxFramesPerProcess(uint32_t blockSize);
    void setPluginId(int32_t id);
    void generic11(uint32_t count);
    void generic17(int count);
    long double getSampleRate();
    uint32_t getMaxFramesPerProcess();
    void formatIntToString(int value, char* outSmall, uint32_t unused1, uint32_t unused2, char* outLarge);
    int32_t automateHostParameter(int paramIndex, float value);
};

class Bitmap {
public:
    Bitmap(Bitmap* pThis, uint32_t resId);
    void deleteBitmap();
    void unregisterBitmap();
    void blit(GDIDrawingContext* drawingContext, RECT* destRect, POINT* srcPoint);
    void drawMasked(GDIDrawingContext* drawingContext, RECT* destRect, POINT* srcPoint);
    HBITMAP createMaskBitmap(HDC hdcRef, HANDLE hBitmapSrc, COLORREF colorKey);
};

class DropTarget {
public:
    DropTarget(DropTarget* pThis, Window* frame);
    DropTarget* destructor(bool deleteObject);
    void destroy();
    uint32_t queryInterface(void* riid, void* * ppvObject);
    int addRef();
    int release();
    HRESULT dragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
    HRESULT dragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
    uint32_t returnZero();
    uint32_t drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
    uint32_t resolveShortcutTarget();
    void __chkstk();
};

class EditorBase {
public:
    void generic18();
    void invalidate();
    uint32_t generic1();
    EditorBase(EditorBase* pThis, DelayLamaPlugin* delayLama);
    EditorBase* destructor(bool destroyObject);
    void destroy();
    void draw();
    bool open(HWND hParent);
    void onIdle();
    void setKnobMode(int mode);
    bool unkownFunction(uint32_t param_1);
    void sleep(DWORD miliseconds);
    void idleHandler();
    void getRect(Rect* * outRect);
    void dispatcher();
};

class GDIDrawingContext {
public:
    uint32_t setModuleHandle(uint32_t param_1);
    GDIDrawingContext(GDIDrawingContext* pThis, Window* parentFramePtr, HDC hDC, HWND hWnd);
    void* Destructor_0x0(bool deleteObject);
    void cleanResources();
    void moveToEx(POINT* target);
    void lineTo(POINT* tageet);
    void setPenDashMode(Color color);
    void drawRectangleOutline(RECT* param_1);
    void fillRectangleInset(RECT* param_1);
    void setTextColor(Color color);
    void setPenColor(Color color);
    void setBackgroundColorAndBrush(Color color);
    void getRelativeMousePos(POINT* outRelMousePos);
    void copyToScreen(GDIDrawingContext* dest, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcX, int srcY);
    void setCursor(int cursorType);
    bool registerWindowClass();
    void unregisterClass();
    HBRUSH pluginWndProc(HWND hWnd, uint32_t uMsg, HDC hdc, LPARAM lParam);
};

class UnusedClass {
public:
    UnusedClass(UnusedClass* pThis);
    uint32_t zeroParam(uint32_t* param_1);
    uint32_t setParam(int param_1);
    void checkValue();
};

class View {
public:
    void generic3();
    void setEnabled(bool enabled);
    bool getEnabled();
    void setAbsRect(RECT* rect);
    void getRect(RECT* outRect);
    void setUseAlphaBlending(bool useAlphaBlending);
    bool getUseAlphaBlending();
    void setParent(Window* parent);
    bool generic4(Window* frame);
    int getReferenceCount();
    uint8_t GetPressedModifiersAndMouseButtons();
    View(View* pThis, RECT* pRect);
    bool isDirty(int param_1);
    void setDirty(bool dirty);
    View* destructor(bool deleteObject);
    void onMouseDown();
    bool routeMessage();
    bool onMouseWheel(GDIDrawingContext* drawingContext, void* drawingContextMemory, void* param_3);
    void update(GDIDrawingContext* drawingContext);
    void setRect(RECT* rect);
    void release();
    void useBitmap(Bitmap* bmp);
};

class AudioBaseExtended : public AudioBase {
public:
    AudioBaseExtended(AudioBaseExtended* pThis, HostCallback hostCallback, int presetCount, int parameterCount);
    AudioBaseExtended* destructAudioBase(bool destroyObject);
    void destroy();
    int32_t dispatchPluginCallback(int32_t opcode, int32_t* index, int32_t* value, void* ptr, float opt);
    uint32_t getEditor();
    long double returnZeroFloat();
    void requestMidiSupport(float value);
    uint32_t generic6();
    uint8_t generic5();
    void getHostTransportTime(char* outText);
    void getHostTempoAtSample(float pos);
    uint32_t getPluginCategory();
    uint32_t generic15();
    uint32_t getHostAutomatableParameterCount();
    uint32_t getParameterStepSize();
    bool notifyHostIoConfigurationChanged();
    void setAudioSettings(uint32_t hostBlockSize, uint32_t sampleRate);
    bool needIdle();
    bool sizeWindow(POINT* size);
    double* getHostSampleRate();
    uint32_t companySpecific();
    uint32_t getBlockSize();
    uint32_t getInputLatencySamples();
    uint8_t generic8();
    uint32_t getOutputLatencySamples();
    uint8_t generic9();
    uint32_t getPreviousPlugin();
    int getDamVersion();
    uint32_t getNextPlugin();
    int32_t getMidiProgram(uint32_t index, void* out);
    uint32_t willReplaceOrAccumulate();
    uint32_t getCurrentProcessLevel();
    uint32_t getAutomationState();
    void setOverwritingCapability(bool param_1);
    void setOfflineProcessingCapability(bool enable);
    bool offlineRead(void* task, int32_t sampleFrames, float* * audioBuffers);
    bool offlineWrite(float* * audioBuffers);
    bool startOfflineProcessing(int32_t* param_2_00, int32_t* param_2, int32_t param_3);
    bool getCurrentPass();
    bool getCurrentMetaPass();
    uint32_t setHostOutputSampleRate(float sampleRate);
    bool getOutputSpeakerArrangement(int32_t arrangement, void* param_2);
    bool getHostCompanyString(char* outText);
    bool getHostProductString(char* outText);
    uint32_t getHostVersion();
    int32_t* callCompanySpecific(int32_t index, int valueHigh, int valueLow, void* context);
    bool hostSupports(char* target);
    void setIsSynth(bool isSynth);
    void setSupportsOutputOverwrite(bool supportsOutputOverwrite);
    uint32_t getHostLanguage();
    void openWindow(float window);
    bool closePluginEditorOnHost(HWND windowHandle);
    char* getHostWorkingDirectory();
    bool requestHostEditorRefresh();
    bool notifyHostClientBeginningParameterEdit(int32_t parameterId);
    bool notifyHostClientEndingParameterEdit(int32_t parameterId);
    bool openFileDialogOnHost(char* outText);
    uint32_t generic13();
    uint8_t generic16();
};

class DelayLamaEditor : public EditorBase {
public:
    DelayLamaEditor(DelayLamaEditor* pThis, DelayLamaPlugin* delayLama);
    DelayLamaEditor* destructor(bool deleteFlag);
    void destroy();
    void open(HWND hParent);
    void close();
    void dispatcher(int parameterIndex);
};

class Control : public View {
public:
    void onIdle();
    void setValue(float value);
    long double getValue();
    void setMin(float min);
    long double getMin();
    void setMax(float max);
    long double getMax();
    long double getPreviousValue();
    long double getDefaultValue();
    void setParameterId(int parameterId);
    uint32_t getBitmap();
    void setWheelSensitivity(float sensitivity);
    long double getWheelSensitivity();
    Control(Control* pThis, RECT* pRect, DrawControl* * drawControl, int parameterId, Bitmap* bmp);
    Control* destructor(uint8_t param_1);
    void destroy();
    bool isDirty();
    void setDirty(bool isDirty);
    void changeBitmap(Bitmap* newBitmap);
    void clampValue();
    bool onMouseWheel(GDIDrawingContext* drawingContext, POINT* mousePos, float wheelDelta);
};

class Window : public View {
public:
    void resetVtable(Window* frame);
    Window(Window* pThis, RECT* pRect, HWND hParent, DelayLamaEditor* editor);
    Window* destructor(bool deleteObject);
    void cleanup();
    bool closeWindow();
    bool openPluginWindow(HWND hParent);
    bool setDragAndDropState(bool enable);
    void onDraw(GDIDrawingContext* drawingContext);
    void drawControlOrSelf(void* target);
    void onMouseDown(GDIDrawingContext* drawingContext, POINT* mousePos);
    uint8_t routeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT* mousePos);
    void onMouseWheel(int unused, int unused2, int hoverState);
    void update(GDIDrawingContext* drawingContext);
    bool needsRedraw(Window* frame);
    void refresh(Window* frame);
    void setBackgroundBitmap(Bitmap* param_1);
    bool registerChild(Control* control);
    bool removeChild(Control* child, bool shouldRelease);
    bool destroyChildren(bool* callExtraFlag);
    bool containsChild(Control* target);
    uint32_t setModalView(View* view);
    void beginEdit(int controlId);
    void endEdit(int controlId);
    Control* getChildAtMousePos();
    void getLocalMousePos(POINT* mousePos);
    DropTarget* createDropTarget();
};

class DelayLamaAudio : public AudioBaseExtended {
public:
    bool sendEventsToHost(void* eventsPtr);
    DelayLamaAudio(DelayLamaAudio* pThis, HostCallback hostCallback);
    DelayLamaAudio* destructAudioBase(bool deleteObject);
    void destroy();
    void loadPresetByIndex(int currentProgram);
    void setCurrentPresetName(char* newName);
    void getCurrentPresetName(char* outText);
    void getParameterUnitLabel(char* label);
    void getParameterValueString(int parameterId, char* outText);
    void getParameterName(int parameterId, char* outBuffer);
    void setParameterValue(int parameterId, float value);
    long double getParameterValue(int parameter);
    uint32_t getOutputBusProperties(uint32_t index, char* properties);
    bool getPresetNameByIndex(int category, int index, char* outText);
    bool copyPreset(int param_1);
    void getEffectName(char* outBuffer);
    uint32_t getCompanyName(char* outText);
    uint32_t getProductName(char* outText);
    int pluginSupports(char* targetText);
    void initPresets();
    void setSampleRate(uint32_t sampleRate);
    void setMaxFramesPerProcess(uint32_t blocksize);
    void disableAudioProcessing();
    void enableAudioProcessing();
    void initialize();
    void buildPiecewiseCubicTable(int32_t* controlPoints, float* outSamples);
    void invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames);
    void processAudio(float* * inputs, float* * outputs, void * sampleFrames);
    void dispatchMidiEvents(int sampleIdx, int sampleFrame);
    void addSynthesisToExcitation(int offsetIncrement);
    void updateVowelFilter(float vowelX);
    void processEvents(DamEventList* eventList);
    void handleNoteEvent(int midiData1, int midiData2);
    void handleControlChange(int midiData1, int midiData2);
    long double getRandomFloat();
    void sendMidiToHost(uint8_t status, uint8_t data1, uint8_t data2);
    void iteratePresetBlocks(Preset* context[5], int stride, int iterationCount, void* callback, void* extra);
    void _conditionalRunCallback(void* context, int param2, int unknownLocal, void* extraParam);
    void __vector_destructor_iterator(int param_1, int param_2, int param_3, void* param_4);
    void _conditionalCleanup();
    void _forEachPresetBlockReverse(void* startPtr, int step, int count, void* callback);
};

class HorizontalSlider : public Control {
public:
    void setIsHandleTransparent(uint8_t transparent);
    void setSnapToMouse(bool snapToMouse);
    uint8_t getSnapToMouse();
    void setHandlePos(POINT* handlePos);
    uint32_t getTrackLength();
    void setCurrentValue(float currentValue);
    long double getCurrentValue();
    void changeHandle(Bitmap* newHandle);
    HorizontalSlider(HorizontalSlider* pThis, RECT* pRect, DrawControl* * drawControl, int controlId, int minValue, int maxValue, Bitmap* handleBmp, Bitmap* backgroundBmp, Range* range, int stepSize);
    HorizontalSlider* destructor(bool deleteObject);
    void destroy();
    void onDraw(GDIDrawingContext* drawingContext);
    void onMouseDown(GDIDrawingContext* drawingContext, POINT* mousePos);
};

class RotaryControl : public Control {
public:
    RotaryControl(RotaryControl* pThis, RECT* pRect, DrawControl* * drawControl, int controlId, Bitmap* bmp1, Bitmap* bmp2, POINT* srcPoint);
    RotaryControl* destructor(bool deleteObject);
    void destroy();
    void onDraw(GDIDrawingContext* drawingContext);
    void drawIndicator(GDIDrawingContext* drawingContext);
    void onMouseDown(GDIDrawingContext* drawingContext, POINT* mousePos);
    void setStartAngle(float startAngle);
    void setTotalRange(uint32_t totalRange);
    void updateMathConstants();
    void calculateXYFromValue(POINT* outPoint);
    long double calculateAngleFromPoint(POINT* point);
    void setIndicatorShadowColor(Color color);
    void setIndicatorHighlightColor(Color color);
    void setBitmap(Bitmap* bmp);
    long double getStartAngle();
    long double getTotalRange();
    void setKnobRadius(uint32_t radius);
    void setFineTuneDivider(float divider);
    long double getFineTuneDivider();
};

class SplashScreen : public Control {
public:
    SplashScreen(SplashScreen* pThis, RECT* pRect, DrawControl* * drawControl, int controlId, Bitmap* bmp, RECT* destRect, POINT* srcPoint);
    SplashScreen* destructor(bool deleteObject);
    void destroy();
    void onDraw(GDIDrawingContext* drawingContect);
    void onMouseDown(GDIDrawingContext* drawingContext);
};

class TileGrid : public Control {
public:
    TileGrid(TileGrid* pThis, RECT* pRect, DrawControl* * drawControl, int controlId, int tileWidth, int tileHeight, Bitmap* bmp, POINT* srcOffset);
    TileGrid* destructor(bool deleteObject);
    void destoy();
    void onDraw(GDIDrawingContext* drawingContext);
};

class VerticalSlider : public Control {
public:
    VerticalSlider(VerticalSlider* pThis, RECT* pRect, DrawControl* * drawControl, int controlId, int minValue, int maxValue, Bitmap* handleBmp, Bitmap* backgroundBmp, Range* range, int stepSize);
    VerticalSlider* destructor(bool deleteObject);
    void destroy();
    void onDraw(GDIDrawingContext* drawingContext);
    void onMouseDown(GDIDrawingContext* drawingContext, POINT* relativeMousePos);
};

class DelayLamaPlugin : public DelayLamaAudio {
public:
    DelayLamaPlugin(DelayLamaPlugin* pThis, HostCallback hostCallback);
    DelayLamaPlugin* destructAudioBase(bool deleteObject);
    void destroy();
    void setParameterValue(int parameterIndex, float parameterValue);
};

class TwoAxisSlider : public HorizontalSlider {
public:
    TwoAxisSlider(TwoAxisSlider* pThis, RECT* bounds, DrawControl* * drawControl, int controlId, int minValue, int maxValue, int trackLength, Bitmap* bitmap, Range* range, int stepSize);
    TwoAxisSlider* destructor(bool deleteObject);
    void onMouseDown(GDIDrawingContext* drawContext, POINT* mousePos);
};

class Knob : public RotaryControl {
public:
    Knob(Knob* pThis, RECT* pRect, DrawControl* * drawControl, int parameterId, int totalFrames, int frameHeight, Bitmap* bmp, POINT* srcPoint);
    Knob* destructor(bool deleteObject);
    void destroy();
    void onDraw(GDIDrawingContext* drawingContext);
};

class Monk : public TileGrid {
public:
    Monk(Monk* pThis, RECT* pRect, DrawControl* * drawControl, int controlId, uint32_t tileWidth, int tileHeight, Bitmap* bmp, POINT* srcOffset);
    Monk* destructor(bool destroyObject);
    void onDraw(GDIDrawingContext* drawingContext);
};

class OffscreenGDIDrawingContext {
public:
    OffscreenGDIDrawingContext(Window* parentFramePtr, int width, int height, Color color);
    void* destructOffscreenGDIDrawingContext(bool deleteObject);
    void destroy();
};