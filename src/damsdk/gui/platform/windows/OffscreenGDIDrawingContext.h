#pragma once
#include <Windows.h>
#include <windef.h>
#include "GDIDrawingContext.h"
#include <utils/portable_stdint.h>

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows { class Window; class Bitmap; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    extern HINSTANCE g_hInstance;

    // VTABLE: DELAYLAMA 0x1000bbe4
    class OffscreenGDIDrawingContext : public GDIDrawingContext {
        public:
            bool flag; // 0x74
            char unused[3]; // 0x75
            Bitmap* bmp; // 0x78
            int unknown; // 0x7c
            int height; // 0x80
            int width; // 0x84
            COLORREF color; // 0x88
        public:
            OffscreenGDIDrawingContext(Window* parentFramePtr, int width, int height, COLORREF color);
            ~OffscreenGDIDrawingContext();
            void destroy();
    };
}
}
}
}