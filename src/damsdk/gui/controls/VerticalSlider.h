#pragma once
#include "control.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000bfd4
    class VerticalSlider : public Control {
        public:
            void *range;
            int numOutputs;
            int flags;
            bool snapToMouse;
            Platform::Windows::Bitmap *backgroundBitmap;
        public:
            VerticalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, void *range, int flags);
            ~VerticalSlider();
            void destroy();
            void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
            void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* relativeMousePos);
    };
}
}
}