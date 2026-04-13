#pragma once
#include "control.h"
#include "RotaryControl.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000be60
    class Knob : public RotaryControl {
        public:
            POINT origin;
        public:
            Knob(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *bmp, POINT *origin);
            ~Knob();
            Knob* destructor(bool deleteObject);
            void destroy();
            void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
    };
}
}
}