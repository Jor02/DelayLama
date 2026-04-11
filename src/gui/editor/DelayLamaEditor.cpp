#include "DelayLamaEditor.h"
#include "gui/Resources.h"
#include "damsdk/api/AudioBase.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DelayLama {
namespace Gui{
    DelayLamaEditor::DelayLamaEditor(Core::DelayLamaPlugin* pluginInstance) : DamSDK::Api::EditorBase((DamSDK::Api::AudioBase*)pluginInstance) {
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
        this->knob = nullptr;
        this->monk = nullptr;
        this->splashScreen = nullptr;

        Bitmap* background = new Bitmap(IDB_BACKGROUND);
        this->backgroundBitmap = background;
        this->rect.top = 0;
        this->rect.left = 0;
        this->rect.bottom = (int16_t)background->width;
        this->rect.right = (int16_t)background->height;

        // Utils::logf("{%d, %d, %d, %d}", this->rect.left, this->rect.top, this->rect.right, this->rect.bottom);
    }

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
        RECT frameRect;

        frameRect.bottom = this->backgroundBitmap->height;
        frameRect.right = this->backgroundBitmap->width;
        frameRect.left = 0;
        frameRect.top = 0;

        this->window = new DamSDK::Gui::Platform::Windows::Window(&frameRect, parentWnd, this);
        this->window->setBackgroundBitmap(backgroundBitmap);

        // auto orderedRect = [](int x1, int y1, int x2, int y2) -> RECT
        // {
        //     RECT r{};
        //     r.left   = std::min(x1, x2);
        //     r.top    = std::min(y1, y2);
        //     r.right  = std::max(x1, x2);
        //     r.bottom = std::max(y1, y2);
        //     return r;
        // };

        // POINT origin{0, 0};

        // RECT leftKnobRect = orderedRect(
        //     21,
        //     448,
        //     21 + static_cast<int>(leftKnobBitmap->width),
        //     448 + static_cast<int>(leftKnobBitmap->height / 60)
        // );

        // leftKnob = new Knob(
        //     &leftKnobRect,
        //     &drawControl,
        //     LeftVoiceKnobId,
        //     60,
        //     50,
        //     leftKnobBitmap,
        //     &origin
        // );

        // leftKnob->setValue(this->mainPlugin->getParameterValue(/* left voice param index */));
        // this->window->registerControl(leftKnob);

        // RECT rightKnobRect = orderedRect(
        //     293,
        //     447,
        //     293 + static_cast<int>(rightKnobBitmap->width),
        //     447 + static_cast<int>(rightKnobBitmap->height / 60)
        // );

        // POINT rightKnobOrigin{0, 0};

        // knob = new Knob(
        //     &rightKnobRect,
        //     &drawControl,
        //     RightGlideKnobId,
        //     60,
        //     50,
        //     rightKnobBitmap,
        //     &rightKnobOrigin
        // );

        // knob->setValue(this->mainPlugin->getParameterValue(/* right glide param index */));
        // this->window->registerControl(knob);

        // RECT reverbSliderRect = {
        //     /* left   */ 0,
        //     /* top    */ 0,
        //     /* right  */ 0,
        //     /* bottom */ 0
        // };

        // Range reverbRange{
        //     /* min */ 0,
        //     /* max */ 0
        // };

        // reverbSlider = new HorizontalSlider(
        //     &reverbSliderRect,
        //     &drawControl,
        //     ReverbSliderId,
        //     0x68,
        //     0xff - static_cast<int>(reverbHandleBitmap->width),
        //     reverbHandleBitmap,
        //     backgroundBitmap,
        //     &reverbRange,
        //     8
        // );

        // reverbSlider->setValue(this->mainPlugin->getParameterValue(/* reverb param index */));
        // reverbSlider->setNumOutputs();
        // this->window->registerControl(reverbSlider);

        // RECT twoAxisRect = {
        //     /* left   */ 0,
        //     /* top    */ 0,
        //     /* right  */ 0,
        //     /* bottom */ 0
        // };

        // Range twoAxisRange{
        //     /* min */ 0,
        //     /* max */ 0
        // };

        // autoClass10 = new TwoAxisSlider(
        //     &twoAxisRect,
        //     &drawControl,
        //     7,
        //     96,
        //     259,
        //     TwoAxisSliderId,
        //     nullptr,
        //     &twoAxisRange,
        //     8
        // );

        // autoClass10->setSnapToMouse(-1);
        // this->window->registerControl(autoClass10);

        // RECT singingVerticalRect = {
        //     /* left   */ 0,
        //     /* top    */ 0,
        //     /* right  */ 0,
        //     /* bottom */ 0
        // };

        // Range singingVerticalRange{
        //     /* min */ 0,
        //     /* max */ 0
        // };

        // singingVerticalSlider = new VerticalSlider(
        //     &singingVerticalRect,
        //     &drawControl,
        //     SingingVerticalSliderId,
        //     0x166,
        //     0x1bf - static_cast<int>(singingYHandleBitmap->height),
        //     singingYHandleBitmap,
        //     backgroundBitmap,
        //     &singingVerticalRange,
        //     0x40
        // );

        // singingVerticalSlider->view.setEnabled(-1);
        // singingVerticalSlider->setValue(this->mainPlugin->getParameterValue(/* singing vertical param index */));
        // singingVerticalSlider->setNumOutputs();
        // this->window->registerControl(singingVerticalSlider);

        // RECT singingHorizontalRect = {
        //     /* left   */ 0,
        //     /* top    */ 0,
        //     /* right  */ 0,
        //     /* bottom */ 0
        // };

        // Range singingHorizontalRange{
        //     /* min */ 0,
        //     /* max */ 0
        // };

        // singingHorizontalSlider = new HorizontalSlider(
        //     &singingHorizontalRect,
        //     &drawControl,
        //     SingingHorizontalSiderId,
        //     93,
        //     264 - static_cast<int>(singingXHandleBitmap->width),
        //     singingXHandleBitmap,
        //     backgroundBitmap,
        //     &singingHorizontalRange,
        //     8
        // );

        // singingHorizontalSlider->view.setEnabled(-1);
        // singingHorizontalSlider->setValue(this->mainPlugin->getParameterValue(/* singing horizontal param index */));
        // singingHorizontalSlider->setNumOutputs(0);
        // this->window->registerControl(singingHorizontalSlider);

        // RECT monkRect = {
        //     /* left   */ 0,
        //     /* top    */ 0,
        //     /* right  */ 0,
        //     /* bottom */ 0
        // };

        // POINT monkOrigin{0, 0};

        // monk = new Monk(
        //     &monkRect,
        //     &drawControl,
        //     MonkId,
        //     30,
        //     static_cast<int>(monkSpriteSheetBitmap->height / 30),
        //     monkSpriteSheetBitmap,
        //     &monkOrigin
        // );

        // monk->tileGrid.setValue(this->mainPlugin->getParameterValue(6));
        // this->window->registerControl(monk);

        // RECT splashRect = {
        //     /* left   */ 0,
        //     /* top    */ 0,
        //     /* right  */ 0,
        //     /* bottom */ 0
        // };

        // POINT splashOrigin{0, 0};

        // splashScreen = new SplashScreen(
        //     &splashRect,
        //     &drawControl,
        //     SplashScreenId,
        //     aboutScreenBitmap,
        //     &reverbSliderRect,
        //     &splashOrigin
        // );

        // this->window->registerControl(splashScreen);
    }

        void DelayLamaEditor::dispatcher(int32_t parameterIndex)
        {
            if (this->window == nullptr)
                return;

            switch (parameterIndex)
            {
                case 0:
                    if (this->leftKnob != nullptr) {
                        this->leftKnob->setValue(this->mainPlugin->getParameterValue(0));
                        invalidate();
                    }
                    break;
                case 1:
                    if (this->singingVerticalSlider != nullptr) {
                        this->singingVerticalSlider->setValue(this->mainPlugin->getParameterValue(1));
                        invalidate();
                    }
                    break;
                case 2:
                    if (this->reverbSlider != nullptr) {
                        this->reverbSlider->setValue(this->mainPlugin->getParameterValue(2));
                        invalidate();
                    }
                    break;
                case 3:
                    if (this->knob != nullptr) {
                        this->knob->setValue(this->mainPlugin->getParameterValue(3));
                        invalidate();
                    }
                    break;
                case 5:
                    if (this->singingHorizontalSlider != nullptr) {
                        this->singingHorizontalSlider->setValue(this->mainPlugin->getParameterValue(5));
                    }
                    break;
                case 6:
                    if (this->monk != nullptr) {
                        this->monk->setValue(this->mainPlugin->getParameterValue(6));
                        invalidate();
                    }
                    break;
                default:
                    break;
            }

            invalidate();
        }
}
}