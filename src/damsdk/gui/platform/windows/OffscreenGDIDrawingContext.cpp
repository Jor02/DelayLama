#include "OffscreenGDIDrawingContext.h"

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    // STUB: DELAYLAMA 0x10006f80
    OffscreenGDIDrawingContext::OffscreenGDIDrawingContext(Window* parentFramePtr, int width, int height, COLORREF color) {
        // HWND hWnd;
        // HDC hdc;
        // HDC pHVar1;
        // HBITMAP h;
        // RECT rect;
        //
        // GDIDrawingContext::GDIDrawingContext((GDIDrawingContext *)this,parentFramePtr,(HDC)0x0,(HWND)0x0);
        // this->bmp = (Bitmap *)0x0;
        // this->field5_0x7c = 0;
        // this->width = width;
        // this->height = height;
        // this->color = color;
        // this->vtable = &OffscreenGDIDrawingContextVTable_1000bbe8;
        // this->field2_0x74 = true;
        // hWnd = parentFramePtr->hWnd;
        // hdc = GetDC(hWnd);
        // pHVar1 = CreateCompatibleDC(hdc);
        // (this->gdiDrawingContext).hDC = pHVar1;
        // h = CreateCompatibleBitmap(hdc,width,height);
        // (this->gdiDrawingContext).hWnd = (HWND)h;
        // SelectObject((this->gdiDrawingContext).hDC,h);
        // ReleaseDC(hWnd,hdc);
        // rect.left = 0;
        // rect.top = 0;
        // rect.right = width;
        // rect.bottom = height;
        // GDIDrawingContext::setBackgroundColorAndBrush((GDIDrawingContext *)this,color);
        // GDIDrawingContext::setPenColor((GDIDrawingContext *)this,color);
        // GDIDrawingContext::fillRectangleInset((GDIDrawingContext *)this,&rect);
        // GDIDrawingContext::drawRectangleOutline((GDIDrawingContext *)this,&rect);
        // return this;
    }

    // STUB: DELAYLAMA 0x10006f60
    OffscreenGDIDrawingContext::~OffscreenGDIDrawingContext() {
        // destroy(this);
        // if (deleteObject) {
        //   operator_delete(this);
        // }
        // return this;
    }

    // STUB: DELAYLAMA 0x10007050
    void OffscreenGDIDrawingContext::destroy() {
        // HDC hdc;
        // HWND ho;
        //
        // this->vtable = &OffscreenGDIDrawingContextVTable_1000bbe8;
        // if (this->bmp != (Bitmap *)0x0) {
        //   Bitmap::unregisterBitmap(this->bmp);
        // }
        // hdc = (this->gdiDrawingContext).hDC;
        // if (hdc != (HDC)0x0) {
        //   DeleteDC(hdc);
        // }
        // if ((this->field2_0x74 != false) && (ho = (this->gdiDrawingContext).hWnd, ho != (HWND)0x0)) {
        //   DeleteObject(ho);
        // }
        // GDIDrawingContext::cleanResources((GDIDrawingContext *)this);
        // return;
    }
}
}
}
}
