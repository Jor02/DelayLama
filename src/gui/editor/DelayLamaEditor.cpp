#include "DelayLamaEditor.h"
#include "gui/Resources.h"
#include "damsdk/api/AudioBaseExtended.h"
#include "damsdk/api/DamPlugin.h"
#include "core/PluginConfig.h"

// Components
#include "damsdk/gui/platform/windows/Window.h"
#include "damsdk/gui/controls/HorizontalSlider.h"
#include "damsdk/gui/controls/VerticalSlider.h"
#include "damsdk/gui/controls/TwoAxisSlider.h"
#include "damsdk/gui/controls/Knob.h"
#include "gui/controls/Monk.h"
#include "gui/controls/SplashScreen.h"

// Logging
#include "utils/Logger.h"

namespace DelayLama {
namespace Gui{

    // FUNCTION: DELAYLAMA 0x10003640
    DelayLamaEditor::DelayLamaEditor(Core::DelayLamaPlugin* pluginInstance) : DamSDK::Api::EditorBase((DamSDK::Api::AudioBaseExtended*)pluginInstance) {
        Utils::log("DelayLamaEditor::ctor\n");
        this->reverbHandleBitmap = nullptr;
        this->monkSpriteSheetBitmap = nullptr;
        this->singingYHandleBitmap = nullptr;
        this->singingXHandleBitmap = nullptr;
        this->aboutScreenBitmap = nullptr;
        this->leftKnobBitmap = nullptr;
        this->rightKnobBitmap = nullptr;
        this->reverbSlider = nullptr;
        this->singingVerticalSlider = nullptr;
        this->singingHorizontalSlider = nullptr;
        this->leftKnob = nullptr;
        this->rightKnob = nullptr;
        this->monk = nullptr;
        this->splashScreen = nullptr;

        Bitmap* background = new Bitmap(IDB_BACKGROUND);
        this->backgroundBitmap = background;
        this->rect.top = 0;
        this->rect.left = 0;
        this->rect.bottom = (int16_t)background->width;
        this->rect.right = (int16_t)background->height;
    }
    
    // FUNCTION: DELAYLAMA 0x10004210
    void DelayLamaEditor::valueChanged(GDIDrawingContext *drawingContext, Control *control)
    {
        int parameterId = control->parameterId;
        switch (parameterId)
        {
            case GlideKnobParameterId:
            case SingingVerticalSliderParameterId:
            case DelaySliderParameterId:
            case VoiceKnobParameterId:
            case SingingHorizontalSliderParameterId:
            case MonkSpriteParameterId:
                // Also tells the host, so it can record the change as automation
                this->mainPlugin->automateHostParameter(parameterId, control->getValue());
                control->update(drawingContext);
                break;

            case TwoAxisSliderParameterId:
            {
                // The singing pad reports several things through one value:
                // -2..3 the pitch (x axis), 98..103 the vowel (100 + inverted y),
                // 200 / 201 singing off / on.
                float value = control->getValue();
                if (-2.0f < value && value < 3.0f)
                    this->mainPlugin->setParameterValue(PadPitchParameterId, value);

                if (98.0f < value && value < 103.0f) {
                    value = (float)((value - 100.0f) * -1.0f) + 1.0f;
                    this->mainPlugin->setParameterValue(PadVowelParameterId, value);
                }

                if (value == 200.0f)
                    this->mainPlugin->setParameterValue(SingingEnabledParameterId, 0.0f);

                if (value == 201.0f)
                    this->mainPlugin->setParameterValue(SingingEnabledParameterId, 1.0f);

                control->update(drawingContext);
                break;
            }
        }
    }

    // FUNCTION: DELAYLAMA 0x10003740
    DelayLamaEditor::~DelayLamaEditor() {
        Utils::log("DelayLamaEditor::destroy\n");
        if (this->backgroundBitmap != nullptr) {
          this->backgroundBitmap->unregisterBitmap();
        }
        this->backgroundBitmap = nullptr;

        if (this->reverbHandleBitmap != nullptr) {
          this->reverbHandleBitmap->unregisterBitmap();
        }
        this->reverbHandleBitmap = nullptr;

        if (this->singingYHandleBitmap != nullptr) {
          this->singingYHandleBitmap->unregisterBitmap();
        }
        this->singingYHandleBitmap = nullptr;

        if (this->singingXHandleBitmap != nullptr) {
          this->singingXHandleBitmap->unregisterBitmap();
        }
        this->singingXHandleBitmap = nullptr;

        if (this->monkSpriteSheetBitmap != nullptr) {
          this->monkSpriteSheetBitmap->unregisterBitmap();
        }
        this->monkSpriteSheetBitmap = nullptr;

        if (this->aboutScreenBitmap != nullptr) {
          this->aboutScreenBitmap->unregisterBitmap();
        }
        this->aboutScreenBitmap = nullptr;

        if (this->leftKnobBitmap != nullptr) {
          this->leftKnobBitmap->unregisterBitmap();
        }
        this->leftKnobBitmap = nullptr;

        if (this->rightKnobBitmap != nullptr) {
          this->rightKnobBitmap->unregisterBitmap();
        }
        this->rightKnobBitmap = nullptr;
    }

    // Sets a rect from two corners in any order (VSTGUI's CRect::operator())
    static inline void setRect(RECT* rect, int left, int top, int right, int bottom)
    {
        if (left < right)
            rect->left = left, rect->right = right;
        else
            rect->left = right, rect->right = left;
        if (top < bottom)
            rect->top = top, rect->bottom = bottom;
        else
            rect->top = bottom, rect->bottom = top;
    }

    // FUNCTION: DELAYLAMA 0x10003820
    int32_t DelayLamaEditor::open(HWND parentWnd)
    {
        Utils::log("DelayLamaEditor::open\n");
        EditorBase::open(parentWnd);
        
        if (this->reverbHandleBitmap == NULL) {
            this->reverbHandleBitmap = new Bitmap(IDB_HANDLE_DELAY);
        }
        if (this->monkSpriteSheetBitmap == NULL) {
            this->monkSpriteSheetBitmap = new Bitmap(IDB_MONK_SPRITES);
        }
        if (this->aboutScreenBitmap == NULL) {
            this->aboutScreenBitmap = new Bitmap(IDB_SPLASH);
        }
        if (this->singingYHandleBitmap == NULL) {
            this->singingYHandleBitmap = new Bitmap(IDB_HANDLE_Y);
        }
        if (this->singingXHandleBitmap == NULL) {
            this->singingXHandleBitmap = new Bitmap(IDB_HANDLE_X);
        }
        if (this->leftKnobBitmap == NULL) {
            this->leftKnobBitmap = new Bitmap(IDB_KNOB_GLIDE);
        }
        if (this->rightKnobBitmap == NULL) {
            this->rightKnobBitmap = new Bitmap(IDB_KNOB_VOICE);
        }
        
        RECT size = {0, 0, this->backgroundBitmap->width, this->backgroundBitmap->height};
        this->window = new DamSDK::Gui::Platform::Windows::Window(&size, parentWnd, this);
        this->window->setBackgroundBitmap(this->backgroundBitmap);

        POINT point = {0, 0};
        POINT offset = {1, 0};

        // Knobs: 60 frames stacked vertically
        setRect(&size, 21, 448, 21 + this->leftKnobBitmap->width, 448 + this->leftKnobBitmap->height / 60);
        this->leftKnob = new DamSDK::Gui::Controls::Knob(&size, this, GlideKnobParameterId, 60, 50, this->leftKnobBitmap, &point);
        this->leftKnob->setValue(this->mainPlugin->getParameterValue(GlideKnobParameterId));
        this->window->registerControl(this->leftKnob);

        setRect(&size, 293, 447, 293 + this->rightKnobBitmap->width, 447 + this->rightKnobBitmap->height / 60);
        this->rightKnob = new DamSDK::Gui::Controls::Knob(&size, this, VoiceKnobParameterId, 60, 50, this->rightKnobBitmap, &point);
        this->rightKnob->setValue(this->mainPlugin->getParameterValue(VoiceKnobParameterId));
        this->window->registerControl(this->rightKnob);

        // Delay slider
        setRect(&size, 104, 479, 256, 504);
        offset.x = 104;
        offset.y = 479;
        this->reverbSlider = new DamSDK::Gui::Controls::HorizontalSlider(&size, this, DelaySliderParameterId, 104, 255 - this->reverbHandleBitmap->width, this->reverbHandleBitmap, this->backgroundBitmap, &offset, 8);
        this->reverbSlider->setValue(this->mainPlugin->getParameterValue(DelaySliderParameterId));
        this->reverbSlider->setDefaultValue(0.75f);
        this->window->registerControl(this->reverbSlider);

        // Singing pad
        setRect(&size, 96, 362, 259, 440);
        point.x = 0;
        point.y = 0;
        offset.x = 0;
        offset.y = 0;
        this->singingController = new DamSDK::Gui::Controls::TwoAxisSlider(&size, this, TwoAxisSliderParameterId, 96, 259, nullptr, nullptr, &point, 8);
        this->singingController->setSnapToMouse(true);
        this->window->registerControl(this->singingController);

        // The two handles next to the pad only show its position; they ignore the mouse
        setRect(&size, 96 - this->singingYHandleBitmap->width, 358, 96, 446);
        offset.x = 96 - this->singingYHandleBitmap->width;
        offset.y = 358;
        this->singingVerticalSlider = new DamSDK::Gui::Controls::VerticalSlider(&size, this, SingingVerticalSliderParameterId, 358, 447 - this->singingYHandleBitmap->height, this->singingYHandleBitmap, this->backgroundBitmap, &offset, 64);
        this->singingVerticalSlider->setEnabled(false);
        this->singingVerticalSlider->setValue(this->mainPlugin->getParameterValue(SingingVerticalSliderParameterId));
        this->singingVerticalSlider->setDefaultValue(0.5f);
        this->window->registerControl(this->singingVerticalSlider);

        setRect(&size, 93, 362 - this->singingXHandleBitmap->height, 265, 362);
        offset.x = 93;
        offset.y = 362 - this->singingXHandleBitmap->height;
        this->singingHorizontalSlider = new DamSDK::Gui::Controls::HorizontalSlider(&size, this, SingingHorizontalSliderParameterId, 93, 264 - this->singingXHandleBitmap->width, this->singingXHandleBitmap, this->backgroundBitmap, &offset, 8);
        this->singingHorizontalSlider->setEnabled(false);
        this->singingHorizontalSlider->setValue(this->mainPlugin->getParameterValue(SingingHorizontalSliderParameterId));
        this->singingHorizontalSlider->setDefaultValue(0.0f);
        this->window->registerControl(this->singingHorizontalSlider);

        // Monk: 5 x 6 tiles
        setRect(&size, 22, 5, 22 + this->monkSpriteSheetBitmap->width / 5, 5 + this->monkSpriteSheetBitmap->height / 6);
        this->monk = new Controls::Monk(&size, this, MonkSpriteParameterId, 30, this->monkSpriteSheetBitmap->height / 30, this->monkSpriteSheetBitmap, &point);
        this->monk->setValue(this->mainPlugin->getParameterValue(MonkSpriteParameterId));
        this->window->registerControl(this->monk);

        // About screen: clicking the logo shows it over the monk
        setRect(&size, 284, 300, 327, 335);
        point.x = 0;
        point.y = 0;
        RECT toDisplay = {57, 13, 57 + this->aboutScreenBitmap->width, 13 + this->aboutScreenBitmap->height};
        this->splashScreen = new Controls::SplashScreen(&size, this, SplashScreenParameterId, this->aboutScreenBitmap, &toDisplay, &point);
        this->window->registerControl(this->splashScreen);

        return 1;
    }

    // FUNCTION: DELAYLAMA 0x100040c0
    void DelayLamaEditor::dispatcher(int parameterIndex, float parameterValue)
    {
        Utils::logf("DelayLamaEditor::dispatcher id=%d value=%f\n", parameterIndex, parameterValue);
        if (this->window == nullptr)
            return;

        switch (parameterIndex)
        {
            case GlideKnobParameterId:
                if (this->leftKnob != nullptr)
                    this->leftKnob->setValue(this->mainPlugin->getParameterValue(GlideKnobParameterId));
                break;
            case VoiceKnobParameterId:
                if (this->rightKnob != nullptr)
                    this->rightKnob->setValue(this->mainPlugin->getParameterValue(VoiceKnobParameterId));
                break;
            case SingingVerticalSliderParameterId:
                if (this->singingVerticalSlider != nullptr)
                    this->singingVerticalSlider->setValue(this->mainPlugin->getParameterValue(SingingVerticalSliderParameterId));
                break;
            case MonkSpriteParameterId:
                if (this->monk != nullptr)
                    this->monk->setValue(this->mainPlugin->getParameterValue(MonkSpriteParameterId));
                break;
            case DelaySliderParameterId:
                if (this->reverbSlider != nullptr)
                    this->reverbSlider->setValue(this->mainPlugin->getParameterValue(DelaySliderParameterId));
                break;
            case SingingHorizontalSliderParameterId:
                if (this->singingHorizontalSlider != nullptr)
                    this->singingHorizontalSlider->setValue(this->mainPlugin->getParameterValue(SingingHorizontalSliderParameterId));
                break;
            default:
                break;
        }

        invalidate();
    }

    // FUNCTION: DELAYLAMA 0x100040a0
    void DelayLamaEditor::close() {
        Utils::log("DelayLamaEditor::close\n");
        Window* frame = this->window;
        if (frame != nullptr) {
            delete frame;
        }
        this->window = nullptr;
    }
}
}