#pragma once
#include <Windows.h>
#include <windef.h>
#include <utils/portable_stdint.h>

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows { class Window; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    extern HINSTANCE g_hInstance;

    // VTABLE: DELAYLAMA 0x1000bbe4
    class OffscreenGDIDrawingContext {
        public:
            OffscreenGDIDrawingContext(Window* parentFramePtr, int width, int height, COLORREF color);
            ~OffscreenGDIDrawingContext();
            void destroy();
    };
}
}
}
}