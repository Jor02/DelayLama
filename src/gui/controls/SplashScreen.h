#pragma once
#include <Windows.h>
#include <windef.h>
#include "damsdk/gui/controls/Control.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    // VTABLE: DELAYLAMA 0x1000c154
    class SplashScreen : public DamSDK::Gui::Controls::Control {
        public:
        public:
            SplashScreen(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, DamSDK::Gui::Platform::Windows::Bitmap *bmp, RECT *destRect,POINT *srcPoint);
            ~SplashScreen();
            void destroy();
            void onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContect);
            void onMouseDown(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext);
    };
}
}
}