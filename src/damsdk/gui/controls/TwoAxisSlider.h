#pragma once
#include "control.h"
#include "HorizontalSlider.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000b914
    class TwoAxisSlider : public HorizontalSlider {
        public:
            void *range;
            int numOutputs;
            int snapToMouse;
        public:
            TwoAxisSlider(RECT *bounds, void *unknown, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handle, Platform::Windows::Bitmap *background, void *range, int flags) : HorizontalSlider(bounds,unknown,parameterId,minValue,maxValue,handle,background,range,flags)
            {
            }
    };
}
}
}