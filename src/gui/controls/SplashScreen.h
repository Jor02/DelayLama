#pragma once
#include <Windows.h>
#include <windef.h>
#include "damsdk/gui/controls/control.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    // VTABLE: DELAYLAMA 0x1000c154
    class SplashScreen : public DamSDK::Gui::Controls::Control {
        public:
            RECT destRect;
            RECT keepRect;  // the control's own rect while the splash is shown
            POINT srcPoint;
            
        public:
            SplashScreen(RECT *pRect, DamSDK::Gui::Controls::ControlListener* listener, int parameterId, DamSDK::Gui::Platform::Windows::Bitmap *bmp, RECT *destRect,POINT *srcPoint);
            ~SplashScreen();
            virtual void onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContect) override;
            virtual void onMouseDown(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos) override;
    };
}
}
}