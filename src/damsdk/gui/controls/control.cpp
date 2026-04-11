#include "control.h"

namespace DamSDK {
namespace Gui {
namespace Controls {
    Control::Control(RECT *pRect, void *unknown, int parameterId, Platform::Windows::Bitmap *bmp) : Base::View(pRect) {
        this->unknown = unknown;
        this->prevValue = 1.0;
        this->max = 1.0;
        this->parameterId = parameterId;
        this->defaultValue = 0.5;
        this->value = 0.0;
        this->min = 0.0;
        this->wheelSensitivity = 0.1;
        this->bitmap = bmp;
        this->useAlphaBlending = false;
        this->isEnabled = true;

        if (bmp != nullptr) {
            View::useBitmap(bmp);
        }
    }

    void Control::setDefaultValue(float defaultValue) {
        this->defaultValue = defaultValue;
    }

    void Control::setValue(float newValue) {
        this->value = newValue;
    }
}
}
}