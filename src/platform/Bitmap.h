#include <windows.h>
#include <windef.h>
#include "platform/GDIDrawingContext.h"

namespace DelayLama {
namespace Platform{
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