#pragma once
#include "control.h"

namespace DamSDK {
namespace Gui {
namespace Controls {
    class VerticalSlider : public Control {
        public:
            void *range;
            int numOutputs;
            int flags;
            bool snapToMouse;
            Platform::Windows::Bitmap *backgroundBitmap;
        public:
            VerticalSlider(RECT *pRect, void *unknown, int parameterId, int minValue, int maxValue,
                            Platform::Windows::Bitmap *handleBmp,
                            Platform::Windows::Bitmap *backgroundBmp,
                            void *range, int flags)
                : Control(pRect, unknown, parameterId, handleBmp)
            {
                this->backgroundBitmap = backgroundBmp;
                this->range = range;
                this->flags = flags;
            }
            void setSnapToMouse(bool snap) { this->snapToMouse = snap; }
    };
}
}
}