#include "control.h"
#include "utils/portable_stdint.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // FUNCTION: DELAYLAMA 0x10008c80
    Control::Control(RECT *pRect, void *unknown, int parameterId, Platform::Windows::Bitmap *bmp) : Base::View(pRect) {
        this->unknown = unknown;
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

    // FUNCTION: DELAYLAMA 0x10001960 FOLDED
    void Control::setDefaultValue(float defaultValue) {
        this->defaultValue = defaultValue;
    }

    // FUNCTION: DELAYLAMA 0x100047b0
    void Control::setValue(float newValue) {
        this->value = newValue;
    }
}
}
}