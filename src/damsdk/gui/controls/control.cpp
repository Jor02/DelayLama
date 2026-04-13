#include "control.h"
#include "damsdk/api/EditorBase.h"
#include "damsdk/gui/platform/windows/Window.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "utils/portable_stdint.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // FUNCTION: DELAYLAMA 0x10008c80
    Control::Control(RECT *pRect, callbackCallback callback, int parameterId, Platform::Windows::Bitmap *bmp) : Base::View(pRect) {
        this->callback = callback;
        this->prevValue = 1.0f;
        this->max = 1.0f;
        this->parameterId = parameterId;
        this->defaultValue = 0.5f;
        this->value = 0.0f;
        this->min = 0.0f;
        this->wheelSensitivity = 0.1f;
        this->bitmap = bmp;
        this->useAlphaBlending = false;
        this->isEnabled = true;

        if (bmp != nullptr) {
            View::useBitmap(bmp);
        }
    }

    // FUNCTION: DELAYLAMA 0x10008d10
    Control::~Control() {
        destroy();
    }

    // FUNCTION: DELAYLAMA 0x10001960 FOLDED
    void Control::setDefaultValue(float defaultValue) {
        this->defaultValue = defaultValue;
    }

    // FUNCTION: DELAYLAMA 0x100047b0
    void Control::setValue(float newValue) {
        this->value = newValue;
    }

    // FUNCTION: DELAYLAMA 0x10004790
    void Control::onIdle() {
        Platform::Windows::Window* parentFrame = this->parent;
        Api::EditorBase* editor = parentFrame->editor;
        if (parentFrame != nullptr && editor != nullptr) {
            editor->idleHandler();
        }
    }

    // FUNCTION: DELAYLAMA 0x100047c0
    float Control::getValue() {
        return this->value;
    }

    // FUNCTION: DELAYLAMA 0x100047d0
    void Control::setMin(float min) {
        this->min = min;
    }

    // FUNCTION: DELAYLAMA 0x100047e0
    float Control::getMin() {
        return this->min;
    }

    // FUNCTION: DELAYLAMA 0x100047f0
    void Control::setMax(float max) {
        this->max = max;
    }

    // FUNCTION: DELAYLAMA 0x10004800
    float Control::getMax() {
        return this->max;
    }

    // FUNCTION: DELAYLAMA 0x10004810
    float Control::getPreviousValue() {
        return this->prevValue;
    }

    // FUNCTION: DELAYLAMA 0x10004820
    float Control::getDefaultValue() {
        return this->defaultValue;
    }

    // FUNCTION: DELAYLAMA 0x10004830
    void Control::setParameterId(int parameterId) {
        this->parameterId = parameterId;
    }

    // FUNCTION: DELAYLAMA 0x10004840
    Platform::Windows::Bitmap* Control::getBitmap() {
        return this->bitmap;
    }

    // FUNCTION: DELAYLAMA 0x10004850
    void Control::setWheelSensitivity(float sensitivity) {
        this->wheelSensitivity = sensitivity;
    }

    // FUNCTION: DELAYLAMA 0x10004860
    float Control::getWheelSensitivity() {
        return this->wheelSensitivity;
    }

    // FUNCTION: DELAYLAMA 0x10008d30
    void Control::destroy() {
        Platform::Windows::Bitmap* bmp = this->bitmap;
        if (bmp != nullptr) {
          Platform::Windows::Bitmap::unregisterBitmap(bmp);
        }
    }

    // FUNCTION: DELAYLAMA 0x10008d90
    bool Control::isDirty() {
        if ((this->prevValue == this->value) &&
           (this->_isDirty == false)) {
          return false;
        }
    }

    // FUNCTION: DELAYLAMA 0x10008db0
    void Control::setDirty(bool isDirty) {
        float prevValue;
        
        this->_isDirty = isDirty;
        if (isDirty) {
          prevValue = -1.0;
          if (this->value == -1.0) {
            this->prevValue = 0.0;
            return;
          }
        }
        else {
          prevValue = this->value;
        }
        this->prevValue = prevValue;
    }

    // FUNCTION: DELAYLAMA 0x10008de0
    void Control::changeBitmap(Platform::Windows::Bitmap* newBitmap) {
        Platform::Windows::Bitmap* oldBitmap = this->bitmap;
        if (oldBitmap != nullptr) {
          Platform::Windows::Bitmap::unregisterBitmap(oldBitmap);
        }
        this->bitmap = newBitmap;
        if (newBitmap != nullptr) {
          View::useBitmap(newBitmap);
        }
    }

    // FUNCTION: DELAYLAMA 0x10008e10
    void Control::clampValue() {
        if (this->max < this->value) {
          this->value = this->max;
          return;
        }
        if (this->value < this->min) {
          this->value = this->min;
        }
    }

    // FUNCTION: DELAYLAMA 0x10009410
    bool Control::onMouseWheel(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos, float wheelDelta) {
        if (this->isEnabled == false) {
          return false;
        }

        byte inputMask = View::GetPressedModifiersAndMouseButtons();
        float valueChange = wheelDelta * this->wheelSensitivity;

        if ((inputMask & 8) != 0) {
          valueChange = valueChange * 1.0f;
        }

        this->value = valueChange + this->value;
        this->clampValue();

        bool isDirty = this->isDirty();
        if (isDirty != false) {
            this->callback(drawingContext,this);
        }
        return true;
    }
}
}
}