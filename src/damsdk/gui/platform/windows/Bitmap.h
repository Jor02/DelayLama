#pragma once

// Forward declarations
namespace DelayLama {
namespace Platform {
    struct GDIDrawingContext;
}
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    class Bitmap {
        public:
            int resourceId;
            int refCount;
            int width;
            int height;
            HBITMAP bitmap;
            HBITMAP maskBitmap;

        public:
            Bitmap(int resId);
            ~Bitmap();
            void blit(GDIDrawingContext *drawingContext, RECT *destRect,POINT *srcPoint);
            HBITMAP createMaskBitmap(HDC hdcRef,HANDLE hBitmapSrc,COLORREF colorKey);
            void drawMasked(GDIDrawingContext *drawingContext, RECT *destRect,POINT *srcPoint);
            static void unregisterBitmap(Bitmap* bitmap);
    };
}
}
}
}