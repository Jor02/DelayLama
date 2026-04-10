#include "gui/editor/DelayLamaEditor.h"
#include "DelayLamaEditor.h"
#include "platform/Resource.h"
#include "damsdk/EditorBase.h"
#include "utils/Logger.h"

namespace DelayLama {
namespace Gui{
    DelayLamaEditor::DelayLamaEditor(Core::DelayLamaPlugin* pluginInstance) : DamSDK::EditorBase(pluginInstance) {
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

        Platform::Bitmap* background = new Platform::Bitmap(IDB_BACKGROUND);
        this->backgroundBitmap = background;
        this->rect.top = 0;
        this->rect.left = 0;
        this->rect.bottom = (int16_t)background->width;
        this->rect.right = (int16_t)background->height;

        // Utils::logf("{%d, %d, %d, %d}", this->rect.left, this->rect.top, this->rect.right, this->rect.bottom);
    }

    DelayLamaEditor::~DelayLamaEditor() {
        if (this->backgroundBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->backgroundBitmap);
        }
        this->backgroundBitmap = NULL;
        if (this->reverbHandleBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->reverbHandleBitmap);
        }
        this->reverbHandleBitmap = NULL;
        if (this->singingYHandleBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->singingYHandleBitmap);
        }
        this->singingYHandleBitmap = NULL;
        if (this->singingXHandleBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->singingXHandleBitmap);
        }
        this->singingXHandleBitmap = NULL;
        if (this->monkSpriteSheetBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->monkSpriteSheetBitmap);
        }
        this->monkSpriteSheetBitmap = NULL;
        if (this->aboutScreenBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->aboutScreenBitmap);
        }
        this->aboutScreenBitmap = NULL;
        if (this->leftKnobBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->leftKnobBitmap);
        }
        this->leftKnobBitmap = NULL;
        if (this->rightKnobBitmap != NULL) {
            Platform::Bitmap::unregisterBitmap(this->rightKnobBitmap);
        }
        this->rightKnobBitmap = NULL;
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