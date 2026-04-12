#pragma once
#include "control.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000be60
    class Knob : public Control {
        public:
            POINT origin;
        public:
            Knob(RECT *pRect, void *unknown, int parameterId, int minValue, int maxValue,
                Platform::Windows::Bitmap *bmp, POINT *origin)
                : Control(pRect, unknown, parameterId, bmp)
            {
                this->min = static_cast<float>(minValue);
                this->max = static_cast<float>(maxValue);
                this->origin = *origin;
            }
    };
}
}
}