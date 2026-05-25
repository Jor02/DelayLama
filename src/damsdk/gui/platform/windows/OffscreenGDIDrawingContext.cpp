#include "OffscreenGDIDrawingContext.h"
#include "Window.h"
#include "Bitmap.h"

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    // FUNCTION: DELAYLAMA 0x10006f80
    OffscreenGDIDrawingContext::OffscreenGDIDrawingContext(Window* parentFramePtr, int width, int height, COLORREF color) : GDIDrawingContext(parentFramePtr, nullptr, nullptr) {
        this->bmp = nullptr;
        this->unknown = 0;
        this->width = width;
        this->height = height;
        this->color = color;
        this->flag = true;
        HWND hWnd = parentFramePtr->hWnd;
        HDC hdc = GetDC(hWnd);
        HDC pHVar1 = CreateCompatibleDC(hdc);
        this->hDC = pHVar1;
        HBITMAP h = CreateCompatibleBitmap(hdc,width,height);
        this->hWnd = (HWND)h;
        
        SelectObject(this->hDC,h);
        ReleaseDC(hWnd,hdc);

        RECT rect;
        rect.left = 0;
        rect.top = 0;
        rect.right = width;
        rect.bottom = height;

        this->setBackgroundColorAndBrush(color);
        this->setPenColor(color);
        this->fillRectangleInset(&rect);
        this->drawRectangleOutline(&rect);
    }

    // FUNCTION: DELAYLAMA 0x10006f60
    OffscreenGDIDrawingContext::~OffscreenGDIDrawingContext() {
        this->destroy();
    }

    // FUNCTION: DELAYLAMA 0x10007050
    void OffscreenGDIDrawingContext::destroy() {
        if (this->bmp != nullptr) {
          Bitmap::unregisterBitmap(this->bmp);
        }

        HDC hdc = this->hDC;
        if (hdc != (HDC)0x0) {
          DeleteDC(hdc);
        }

        
        if (this->flag != false) {
            HWND ho = this->hWnd;
            if (ho != nullptr) {
                DeleteObject(ho);
            }    
        }

        GDIDrawingContext::cleanResources();
    }
}
}
}
}
