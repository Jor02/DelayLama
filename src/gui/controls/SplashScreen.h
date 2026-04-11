#pragma once
#include <Windows.h>
#include <windef.h>
#include "damsdk/gui/controls/Control.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    class SplashScreen : public DamSDK::Gui::Controls::Control {
        public:
        public:
            SplashScreen(RECT *pRect, void * unknown, int controlId, DamSDK::Gui::Platform::Windows::Bitmap *bmp, RECT *destRect,POINT *srcPoint)
                : DamSDK::Gui::Controls::Control(pRect, unknown, parameterId, bmp)
            {
            }
    };
}
}
}