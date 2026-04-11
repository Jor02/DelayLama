#include "DelayLamaEditor.h"
#include "gui/Resources.h"
#include "damsdk/api/AudioBase.h"
#include "damsdk/gui/platform/windows/Window.h"

#include "utils/Logger.h"

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
        RECT windowRect;
        
        windowRect.bottom = this->backgroundBitmap->height;
        windowRect.right = this->backgroundBitmap->width;
        windowRect.left = 0;
        windowRect.top = 0;

        this->window = new DamSDK::Gui::Platform::Windows::Window(&windowRect, parentWnd, this);
        
        this->window->setBackgroundBitmap(backgroundBitmap);


        this->window->registerControl(splashScreen);
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