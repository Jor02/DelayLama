#pragma once
#include "control.h"
#include "damsdk/api/DamPlugin.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000bfd4
    class VerticalSlider : public Control {
        public:
            POINT backgroundOffset; //0x58
            POINT handlePos; //0x60
            Platform::Windows::Bitmap * handleImage; //0x68
            int handleWidth; //0x6c
            int handleHeight; //0x70
            int trackMinY; //0x74
            int trackMaxY; //0x78
            int flags; //0x7c
            int trackTopY; //0x80
            int handleMinPos; //0x84
            int handleMaxPos; //0x88
            int trackWidth; //0x8c
            int trackHeight; //0x90
            float fineTuneDivider; //0x94
            bool isHandleTransparent; //0x98
            bool snapToMouse; //0x99
            Platform::Windows::Bitmap *backgroundBitmap;
        public:
            VerticalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, POINT* offset, int flags);
            ~VerticalSlider();
            void destroy();
            void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
            void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* relativeMousePos);
    };
}
}
}