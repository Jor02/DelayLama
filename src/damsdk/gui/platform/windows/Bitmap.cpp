#include <Windows.h>
#include <windef.h>
#include <wingdi.h>

#include "Bitmap.h"
#include "GDIDrawingContext.h"

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    Bitmap::Bitmap(int resId) {        
        this->resourceId = resId;
        this->refCount = 1;
        this->width = 0;
        this->height = 0;
        this->maskBitmap = 0;
        
        HBITMAP hBitmap = LoadBitmapA(g_hInstance,(LPCSTR)(resId & 0xffff));
        this->bitmap = hBitmap;
        if (hBitmap != NULL) {
            tagBITMAP bitmapInfo;
            int bytesWritten = GetObjectA(hBitmap,0x18,&bitmapInfo);
            if (bytesWritten != 0) {
                this->width = bitmapInfo.bmWidth;
                this->height = bitmapInfo.bmHeight;
            }
        }
    }

    Bitmap::~Bitmap() {
        if (this->bitmap != nullptr) {
        DeleteObject(this->bitmap);
        }
        if (this->maskBitmap != nullptr) {
            DeleteObject(this->maskBitmap);
        }
        return;
    }

    void Bitmap::blit(GDIDrawingContext *drawingContext, RECT *destRect, POINT *srcPoint) {
        if (this->bitmap != nullptr) {
            HDC hdc = CreateCompatibleDC(drawingContext->hDC);
            HGDIOBJ h = SelectObject(hdc,this->bitmap);
            BitBlt(
                drawingContext->hDC,
                drawingContext->drawOffset.x + destRect->left,
                drawingContext->drawOffset.y + destRect->top,
                destRect->right - destRect->left,
                destRect->bottom - destRect->top,
                hdc,
                srcPoint->x,
                srcPoint->y,
                SRCCOPY
            );
            SelectObject(hdc,h);
            DeleteDC(hdc);
        }
    }

    HBITMAP Bitmap::createMaskBitmap(HDC hdcRef,HANDLE hBitmapSrc,COLORREF colorKey) {
        HDC hdcSrc = CreateCompatibleDC(hdcRef);
        SelectObject(hdcSrc,hBitmapSrc);
        
        BITMAP bm;
        GetObjectA(hBitmapSrc,sizeof(BITMAP),&bm);
        
        tagPOINT bitmapSize = {bm.bmWidth, bm.bmHeight};
        DPtoLP(hdcSrc,&bitmapSize,1);
        
        HDC hdcMask = CreateCompatibleDC(hdcRef);
        
        HBITMAP hMaskBitmap = CreateBitmap(bitmapSize.x,bitmapSize.y,1,1,NULL);
        HGDIOBJ oldMaskBitmap = SelectObject(hdcMask,hMaskBitmap);

        int oldMapMode = GetMapMode(hdcRef);
        SetMapMode(hdcSrc,oldMapMode);
        
        COLORREF oldBkColor = SetBkColor(hdcSrc,colorKey);
        BitBlt(hdcMask,0,0,bitmapSize.x,bitmapSize.y,hdcSrc,0,0,SRCCOPY);
        SetBkColor(hdcSrc,oldBkColor);
        SelectObject(hdcMask,oldMaskBitmap);
        
        DeleteDC(hdcMask);
        DeleteDC(hdcSrc);
        
        return hMaskBitmap;
    }

    void Bitmap::drawMasked(GDIDrawingContext* drawingContext, RECT* destRect, POINT* srcPoint)
    {
        if (this->maskBitmap == nullptr)
        {
            COLORREF colorKey = RGB(255, 255, 255);
            this->maskBitmap = createMaskBitmap(drawingContext->hDC, this->bitmap, colorKey);
        }

        BITMAP bm;
        GetObjectA(this->bitmap, sizeof(BITMAP), &bm);
        SIZE bitmapSize = { bm.bmWidth, bm.bmHeight };

        HDC srcDC = CreateCompatibleDC(drawingContext->hDC);
        SelectObject(srcDC, this->bitmap);

        DPtoLP(srcDC, (LPPOINT)&bitmapSize, 1);

        HDC maskDC       = CreateCompatibleDC(drawingContext->hDC);
        HDC maskBitmapDC = CreateCompatibleDC(drawingContext->hDC);
        HDC destTempDC   = CreateCompatibleDC(drawingContext->hDC);
        HDC srcTempDC    = CreateCompatibleDC(drawingContext->hDC);

        HBITMAP monoBitmap     = CreateBitmap(bitmapSize.cx, bitmapSize.cy, 1, 1, nullptr);
        HBITMAP tempColor1     = CreateCompatibleBitmap(drawingContext->hDC, bitmapSize.cx, bitmapSize.cy);
        HBITMAP tempColor2     = CreateCompatibleBitmap(drawingContext->hDC, bitmapSize.cx, bitmapSize.cy);

        HGDIOBJ oldMono        = SelectObject(maskDC, monoBitmap);
        HGDIOBJ oldMask        = SelectObject(maskBitmapDC, this->maskBitmap);
        HGDIOBJ oldTemp1       = SelectObject(destTempDC, tempColor1);
        HGDIOBJ oldTemp2       = SelectObject(srcTempDC, tempColor2);

        BitBlt(srcTempDC, 0, 0, bitmapSize.cx, bitmapSize.cy, srcDC, 0, 0, SRCCOPY);
        BitBlt(maskDC, 0, 0, bitmapSize.cx, bitmapSize.cy, maskBitmapDC, 0, 0, 0x330008);

        int destX = destRect->left   + drawingContext->drawOffset.x;
        int destY = destRect->top    + drawingContext->drawOffset.y;
        int srcX  = srcPoint->x;
        int srcY  = srcPoint->y;
        BitBlt(destTempDC, 0, 0, bitmapSize.cx, bitmapSize.cy,
            drawingContext->hDC,
            destX - srcX, destY - srcY,
            SRCCOPY);

        BitBlt(destTempDC, 0, 0, bitmapSize.cx, bitmapSize.cy,
            maskBitmapDC, 0, 0, SRCAND);

        BitBlt(srcDC, 0, 0, bitmapSize.cx, bitmapSize.cy,
            maskDC, 0, 0, SRCAND);

        BitBlt(destTempDC, 0, 0, bitmapSize.cx, bitmapSize.cy,
            srcDC, 0, 0, SRCPAINT);

        BitBlt(drawingContext->hDC,
            destX, destY,
            destRect->right - destRect->left, destRect->bottom - destRect->top,
            destTempDC,
            srcX, srcY,
            SRCCOPY);

        BitBlt(srcDC, 0, 0, bitmapSize.cx, bitmapSize.cy,
            srcTempDC, 0, 0, SRCCOPY);

        SelectObject(maskDC, oldMono);
        DeleteObject(monoBitmap);
        SelectObject(destTempDC, oldTemp1);
        DeleteObject(tempColor1);
        SelectObject(srcTempDC, oldTemp2);
        DeleteObject(tempColor2);
        SelectObject(maskBitmapDC, oldMask);

        DeleteDC(destTempDC);
        DeleteDC(maskDC);
        DeleteDC(maskBitmapDC);
        DeleteDC(srcTempDC);
        DeleteDC(srcDC);
    }

    void Bitmap::unregisterBitmap(Bitmap* bitmap)
    {
        if (bitmap->refCount > 0 && --bitmap->refCount == 0)
            delete bitmap;
    }
}
}
}
}