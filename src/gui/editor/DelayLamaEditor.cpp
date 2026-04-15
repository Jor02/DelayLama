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

    // FUNCTION DELAYLAMA: 0x10003640
    DelayLamaEditor::DelayLamaEditor(Core::DelayLamaPlugin* pluginInstance) : DamSDK::Api::EditorBase((DamSDK::Api::AudioBaseExtended*)pluginInstance) {
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
    
    // FUNCTION: DELAYLAMA 0x10003710
    DelayLamaEditor::~DelayLamaEditor() {
        destroy();
    }

    // FUNCTION DELAYLAMA: 0x10003820
    void DelayLamaEditor::open(HWND parentWnd)
    {
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
        
        // Extract view rectangle dimensions
        RECT windowRect;
        windowRect.bottom = this->backgroundBitmap->height;
        windowRect.right = this->backgroundBitmap->width;
        windowRect.left = 0;
        windowRect.top = 0;

        this->window = new DamSDK::Gui::Platform::Windows::Window(&windowRect, parentWnd, this);
        this->window->setBackgroundBitmap(this->backgroundBitmap);

        POINT origin;
        origin.x = 0;
        origin.y = 0;

        RECT knobRect;

        int desiredTop = (int)this->leftKnobBitmap->height / 60 + 448;
        int knobWidth = this->leftKnobBitmap->width + 21;

        if (knobWidth < 22) {
            knobRect.right = 21;
            knobRect.left = knobWidth;
        }
        else {
            knobRect.left = 21;
            knobRect.right = knobWidth;
        }

        if (desiredTop < 449) {
            knobRect.bottom = 448;
            knobRect.top = desiredTop;
        }
        else {
            knobRect.top = 448;
            knobRect.bottom = desiredTop;
        }

        this->leftKnob = new DamSDK::Gui::Controls::Knob(&knobRect, this->callback, LeftVoiceKnobParameterId, 60, 50, this->leftKnobBitmap, &origin);
        this->leftKnob->setValue(this->mainPlugin->getParameterValue(LeftVoiceKnobParameterId));
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->leftKnob);

        RECT rightKnobRect;
        desiredTop = (int)this->rightKnobBitmap->height / 60 + 447;
        int desiredLeft = this->rightKnobBitmap->width + 293;

        if (desiredLeft < 294) {
            rightKnobRect.left = desiredLeft;
            rightKnobRect.right = 293;
        }
        else {
            rightKnobRect.left = 293;
            rightKnobRect.right = desiredLeft;
        }

        if (desiredTop < 448) {
            rightKnobRect.top = desiredTop;
            rightKnobRect.bottom = 447;
        }
        else {
            rightKnobRect.top = 447;
            rightKnobRect.bottom = desiredTop;
        }

        this->rightKnob = new DamSDK::Gui::Controls::Knob(&rightKnobRect, this->callback, RightGlideKnobParameterId, 60, 50, this->rightKnobBitmap, &origin);
        this->rightKnob->setValue(this->mainPlugin->getParameterValue(RightGlideKnobParameterId));
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->rightKnob);

        RECT reverbSliderRect;
        reverbSliderRect.left = 97;
        reverbSliderRect.top = 475;
        reverbSliderRect.right = 260;
        reverbSliderRect.bottom = 475 + this->reverbHandleBitmap->height;

        POINT reverbSliderBackgroundOffset;
        reverbSliderBackgroundOffset.x = reverbSliderRect.left;
        reverbSliderBackgroundOffset.y = reverbSliderRect.top;

        this->reverbSlider = new DamSDK::Gui::Controls::HorizontalSlider(&reverbSliderRect, this->callback, ReverbSliderParameterId, 0x68, 0xff - this->reverbHandleBitmap->width, this->reverbHandleBitmap, this->backgroundBitmap, &reverbSliderBackgroundOffset, 8);
        this->reverbSlider->setValue(this->mainPlugin->getParameterValue(ReverbSliderParameterId));
        this->reverbSlider->setDefaultValue(0.75f);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->reverbSlider);

        RECT twoAxisRect;
        twoAxisRect.left = 0;
        twoAxisRect.top = 0;
        twoAxisRect.right = 0;
        twoAxisRect.bottom = 0;

        POINT twoAxisBackgroundOffset;
        twoAxisBackgroundOffset.x = 0;
        twoAxisBackgroundOffset.y = 0;

        this->singingController = new DamSDK::Gui::Controls::TwoAxisSlider(&twoAxisRect, this->callback, TwoAxisSliderParameterId, 96, 259, nullptr, nullptr, &twoAxisBackgroundOffset, 8);
        this->singingController->setSnapToMouse(true);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->singingController);

        RECT singingVerticalRect;
        singingVerticalRect.left = 96 - this->singingYHandleBitmap->width;
        singingVerticalRect.top = 358;
        singingVerticalRect.right = 96;
        singingVerticalRect.bottom = 446;

        POINT singingVerticalSliderBackgroundOffset;
        singingVerticalSliderBackgroundOffset.x = singingVerticalRect.left;
        singingVerticalSliderBackgroundOffset.y = singingVerticalRect.top;

        this->singingVerticalSlider = new DamSDK::Gui::Controls::VerticalSlider(&singingVerticalRect, this->callback, SingingVerticalSliderParameterId, 358, 447 - this->singingYHandleBitmap->height, this->singingYHandleBitmap, this->backgroundBitmap, &singingVerticalSliderBackgroundOffset, 64);
        this->singingVerticalSlider->setEnabled(true);
        this->singingVerticalSlider->setValue(this->mainPlugin->getParameterValue(SingingVerticalSliderParameterId));
        this->singingVerticalSlider->setDefaultValue(0.5f);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->singingVerticalSlider);

        RECT singingHorizontalRect;
        singingHorizontalRect.left = 93;
        singingHorizontalRect.top = 362 - this->singingXHandleBitmap->height;
        singingHorizontalRect.right = 265;
        singingHorizontalRect.bottom = 362;

        POINT singingHorizontalBackgroundOffset;
        singingHorizontalBackgroundOffset.x = singingHorizontalRect.left;
        singingHorizontalBackgroundOffset.y = singingHorizontalRect.top;

        this->singingHorizontalSlider = new DamSDK::Gui::Controls::HorizontalSlider(&singingHorizontalRect, this->callback, SingingHorizontalSliderParameterId, 93, 264 - this->singingXHandleBitmap->width, this->singingXHandleBitmap, this->backgroundBitmap, &singingHorizontalBackgroundOffset, 8);
        this->singingHorizontalSlider->setEnabled(true);
        this->singingHorizontalSlider->setValue(this->mainPlugin->getParameterValue(SingingHorizontalSliderParameterId));
        this->singingHorizontalSlider->setDefaultValue(0.0f);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->singingHorizontalSlider);

        RECT monkRect;
        monkRect.left = 22;
        monkRect.top = 5;
        monkRect.right = (monkSpriteSheetBitmap->width / 5) + 22;
        monkRect.bottom = (monkSpriteSheetBitmap->height / 6) + 5;

        this->monk = new Controls::Monk(&monkRect, this->callback, MonkSpriteParameterId, 30, this->monkSpriteSheetBitmap->height / 30, this->monkSpriteSheetBitmap, &origin);
        this->monk->setValue(this->mainPlugin->getParameterValue(MonkSpriteParameterId));
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->monk);

        RECT splashRect;
        splashRect.left = 0;
        splashRect.top = 0;
        splashRect.right = 0;
        splashRect.bottom = 0;

        this->splashScreen = new Controls::SplashScreen(&splashRect, this->callback, SplashScreenParameterId, this->aboutScreenBitmap, &splashRect, &origin);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->splashScreen);
    }

    // FUNCTION DELAYLAMA: 0x100040c0
    void DelayLamaEditor::dispatcher(int32_t parameterIndex, float parameterValue)
    {
        if (this->window == nullptr)
            return;

        switch (parameterIndex)
        {
            case LeftVoiceKnobParameterId:
                if (this->leftKnob != nullptr) {
                    this->leftKnob->setValue(this->mainPlugin->getParameterValue(LeftVoiceKnobParameterId));
                    invalidate();
                }
                break;
            case SingingVerticalSliderParameterId:
                if (this->singingVerticalSlider != nullptr) {
                    this->singingVerticalSlider->setValue(this->mainPlugin->getParameterValue(SingingVerticalSliderParameterId));
                    invalidate();
                }
                break;
            case ReverbSliderParameterId:
                if (this->reverbSlider != nullptr) {
                    this->reverbSlider->setValue(this->mainPlugin->getParameterValue(ReverbSliderParameterId));
                    invalidate();
                }
                break;
            case RightGlideKnobParameterId:
                if (this->rightKnob != nullptr) {
                    this->rightKnob->setValue(this->mainPlugin->getParameterValue(RightGlideKnobParameterId));
                    invalidate();
                }
                break;
            case SingingHorizontalSliderParameterId:
                if (this->singingHorizontalSlider != nullptr) {
                    this->singingHorizontalSlider->setValue(this->mainPlugin->getParameterValue(SingingHorizontalSliderParameterId));
                }
                break;
            case MonkSpriteParameterId:
                if (this->monk != nullptr) {
                    this->monk->setValue(this->mainPlugin->getParameterValue(MonkSpriteParameterId));
                    invalidate();
                }
                break;
            default:
                break;
        }

        invalidate();
    }

    // FUNCTION: DELAYLAMA 0x10003740
    void DelayLamaEditor::destroy() {
        if (this->backgroundBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->backgroundBitmap);
        }
        this->backgroundBitmap = nullptr;

        if (this->reverbHandleBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->reverbHandleBitmap);
        }
        this->reverbHandleBitmap = nullptr;

        if (this->singingYHandleBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->singingYHandleBitmap);
        }
        this->singingYHandleBitmap = nullptr;

        if (this->singingXHandleBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->singingXHandleBitmap);
        }
        this->singingXHandleBitmap = nullptr;

        if (this->monkSpriteSheetBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->monkSpriteSheetBitmap);
        }
        this->monkSpriteSheetBitmap = nullptr;

        if (this->aboutScreenBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->aboutScreenBitmap);
        }
        this->aboutScreenBitmap = nullptr;

        if (this->leftKnobBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->leftKnobBitmap);
        }
        this->leftKnobBitmap = nullptr;

        if (this->rightKnobBitmap != nullptr) {
          Bitmap::unregisterBitmap(this->rightKnobBitmap);
        }
        this->rightKnobBitmap = nullptr;

        EditorBase::destroy();
    }

    // FUNCTION: DELAYLAMA 0x100040a0
    void DelayLamaEditor::close() {
        Window* frame = this->window;
        if (frame != nullptr) {
            delete frame;
        }
        this->window = nullptr;
    }
}
}