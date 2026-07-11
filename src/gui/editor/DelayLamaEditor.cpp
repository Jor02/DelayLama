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

    DelayLamaEditor* DelayLamaEditor::currentEditor = nullptr;

    // FUNCTION DELAYLAMA: 0x10003640
    DelayLamaEditor::DelayLamaEditor(Core::DelayLamaPlugin* pluginInstance) : DamSDK::Api::EditorBase((DamSDK::Api::AudioBaseExtended*)pluginInstance) {
        Utils::log("DelayLamaEditor::ctor\n");
        currentEditor = this;
        this->callback = &onControlChangedThunk;
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
    
    // Not in the original binary, just couldn't figure out how else to implement this.
    void DelayLamaEditor::onControlChangedThunk(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext, DamSDK::Gui::Controls::Control* control)
    {
        if (currentEditor != nullptr)
        {
            currentEditor->onControlChanged(drawingContext, control);
        }
    }

    // FUNCTION: DELAYLAMA 0x10004210
    void DelayLamaEditor::onControlChanged(GDIDrawingContext *drawingContext, Control *control)
    {
        if (control == nullptr)
            return;

        const int parameterId = control->parameterId;
        const float value = control->getValue();
        Utils::logf("DelayLamaEditor::onControlChanged id=%d value=%f\n", parameterId, value);

        switch (parameterId)
        {
            case LeftVoiceKnobParameterId:
            case SingingVerticalSliderParameterId:
            case ReverbSliderParameterId:
            case RightGlideKnobParameterId:
            case SingingHorizontalSliderParameterId:
            case MonkSpriteParameterId:
            {
                this->mainPlugin->setParameterValue(parameterId, value);
                control->update(drawingContext);
                return;
            }

             case TwoAxisSliderParameterId:
             {
                 // Update horizontal and vertical sliders when clicking on TwoAxisSlider
                 float xValue = value;
                 float yValue = 0.0f;
                 
                 if (control->prevValue > 98.0f && control->prevValue < 103.0f) {
                     // Previous Y value was in 100-101 range, extract it
                     yValue = 1.0f - (control->prevValue - 100.0f);
                 }
                 
                 if (value > -2.0f && value < 3.0f)
                 {
                     // This is an X-axis update (vibrato amount, 0-1 normalized)
                     xValue = value;
                     this->mainPlugin->setParameterValue(VibratoAmountParameterId, xValue);
                     
                     // Update the horizontal slider position
                     if (this->singingHorizontalSlider != nullptr) {
                         this->singingHorizontalSlider->setValue(xValue);
                         this->singingHorizontalSlider->setDirty(true);
                     }
                 }

                  if (value > 98.0f && value < 103.0f)
                  {
                      // This is a Y-axis update (pitch, 100-101 range)
                      const float pitchValue = 1.0f - (value - 100.0f);
                      this->mainPlugin->setParameterValue(PitchValueParameterId, pitchValue);
                      
                      // Update the vertical slider position
                      // The vertical slider has flags=64, and its onDraw inverts when flags & 0x20 == 0
                      // Since 64 & 32 == 0, onDraw will invert, so we set the pitchValue directly
                      // and let onDraw handle the inversion
                      if (this->singingVerticalSlider != nullptr) {
                          this->singingVerticalSlider->setValue(pitchValue);
                          this->singingVerticalSlider->setDirty(true);
                      }
                  }

                 if (value == 200.0f)
                 {
                     this->mainPlugin->setParameterValue(SingingEnabledParameterId, 0.0f);
                 }

                 if (value == 201.0f)
                 {
                     this->mainPlugin->setParameterValue(SingingEnabledParameterId, 1.0f);
                 }

                 control->update(drawingContext);
                 
                 // Update both sliders visually
                 if (this->singingHorizontalSlider != nullptr && this->singingHorizontalSlider->isDirty()) {
                     this->singingHorizontalSlider->onDraw(drawingContext);
                 }
                 if (this->singingVerticalSlider != nullptr && this->singingVerticalSlider->isDirty()) {
                     this->singingVerticalSlider->onDraw(drawingContext);
                 }
                 
                 return;
             }

            default:
                return;
        }
    }

    // FUNCTION: DELAYLAMA 0x10003710
    DelayLamaEditor::~DelayLamaEditor() {
        destroy();
    }

    // FUNCTION DELAYLAMA: 0x10003820
    void DelayLamaEditor::open(HWND parentWnd)
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

        // [Knob::ctor] this=08A417F8 rect={21,448,71,498} onChangeCallback=08A42704 paramId=0 totalFrames=60 frameHeight=50 bmp=08A41648 srcPoint={0,0}
        RECT leftKnobRect;

        int desiredTop = (int)this->leftKnobBitmap->height / 60 + 448;
        int knobWidth = this->leftKnobBitmap->width + 21;

        if (knobWidth < 22) {
            leftKnobRect.right = 21;
            leftKnobRect.left = knobWidth;
        }
        else {
            leftKnobRect.left = 21;
            leftKnobRect.right = knobWidth;
        }

        if (desiredTop < 449) {
            leftKnobRect.bottom = 448;
            leftKnobRect.top = desiredTop;
        }
        else {
            leftKnobRect.top = 448;
            leftKnobRect.bottom = desiredTop;
        }

        this->leftKnob = new DamSDK::Gui::Controls::Knob(&leftKnobRect, this->callback, LeftVoiceKnobParameterId, 60, 50, this->leftKnobBitmap, &origin);
        this->leftKnob->setValue(this->mainPlugin->getParameterValue(LeftVoiceKnobParameterId));
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->leftKnob);

        //[Knob::ctor] this=08A45808 rect={293,447,343,497} onChangeCallback=08A42704 paramId=3 totalFrames=60 frameHeight=50 bmp=08A41328 srcPoint={0,0}
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

        // Delay Slider
        //[HorizontalSlider::ctor] this=08A44420 rect={104,479,256,504} onChangeCallback=08A42704 controlId=2 min=104 max=235 handle=08A414E8 bg=08A412A8 backgroundOffset={104,479} flags=8
        RECT reverbSliderRect;
        reverbSliderRect.left   = 104;
        reverbSliderRect.top    = 479;
        reverbSliderRect.right  = 256;
        reverbSliderRect.bottom = 504;

        POINT reverbSliderBackgroundOffset;
        reverbSliderBackgroundOffset.x = reverbSliderRect.left;
        reverbSliderBackgroundOffset.y = reverbSliderRect.top;

        this->reverbSlider = new DamSDK::Gui::Controls::HorizontalSlider(&reverbSliderRect, this->callback, ReverbSliderParameterId, 104, 255 - this->reverbHandleBitmap->width, this->reverbHandleBitmap, this->backgroundBitmap, &reverbSliderBackgroundOffset, 8);
        this->reverbSlider->setValue(this->mainPlugin->getParameterValue(ReverbSliderParameterId));
        this->reverbSlider->setDefaultValue(0.75f);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->reverbSlider);

        // - - - Singing Sliders - - -
        // Two Axis slider
        //[TwoAxisSlider::ctor] this=08A444C8 bounds={96,362,259,440} onChangeCallback=08A42704 controlId=7 min=96 max=259 handle=00000000 bg=00000000 backgroundOffset={0,0} flags=8
        RECT twoAxisRect;
        twoAxisRect.left = 96;
        twoAxisRect.top = 362;
        twoAxisRect.right = 259;
        twoAxisRect.bottom = 440;

        POINT twoAxisBackgroundOffset;
        twoAxisBackgroundOffset.x = 0;
        twoAxisBackgroundOffset.y = 0;

        this->singingController = new DamSDK::Gui::Controls::TwoAxisSlider(&twoAxisRect, this->callback, TwoAxisSliderParameterId, 96, 259, nullptr, nullptr, &twoAxisBackgroundOffset, 1); // flags=1 to match original behavior (flags & 8 == 0 means invert value)
        this->singingController->setSnapToMouse(true);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->singingController);

        // Singing Vertical Slider
        //[VerticalSlider::ctor] this=08A445A0 rect={86,358,96,446} onChangeCallback=08A42704 controlId=1 min=358 max=437 handle=08A415E8 bg=08A412A8 backgroundOffset={86,358} flags=64
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

        // Singing Horizontal Slider
        //[HorizontalSlider::ctor] this=08A41D08 rect={93,352,265,362} onChangeCallback=08A42704 controlId=5 min=93 max=254 handle=08A415C8 bg=08A412A8 backgroundOffset={93,352} flags=8
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

        //[SplashScreen::ctor] this=08A437A0 rect={284,300,327,335} onChangeCallback=08A42704 controlId=12 bmp=08A414A8 destRect={57,13,310,288} srcPoint={0,0}
        RECT splashBoundsRect;
        splashBoundsRect.left = 284;
        splashBoundsRect.top = 300;
        splashBoundsRect.right = 327;
        splashBoundsRect.bottom = 335;

        RECT splashRect;
        splashRect.left = 57;
        splashRect.top = 13;
        splashRect.right = 310;
        splashRect.bottom = 288;

        this->splashScreen = new Controls::SplashScreen(&splashBoundsRect, this->callback, SplashScreenParameterId, this->aboutScreenBitmap, &splashRect, &origin);
        this->window->registerControl((DamSDK::Gui::Controls::Control*)this->splashScreen);
    }

    // FUNCTION DELAYLAMA: 0x100040c0
    void DelayLamaEditor::dispatcher(int32_t parameterIndex, float parameterValue)
    {
        Utils::logf("DelayLamaEditor::dispatcher id=%d value=%f\n", parameterIndex, parameterValue);
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
        Utils::log("DelayLamaEditor::destroy\n");
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
        Utils::log("DelayLamaEditor::close\n");
        Window* frame = this->window;
        if (frame != nullptr) {
            delete frame;
        }
        this->window = nullptr;
    }
}
}