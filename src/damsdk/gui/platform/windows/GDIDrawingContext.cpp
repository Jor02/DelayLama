#include "GDIDrawingContext.h"
#include "Window.h"

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    HINSTANCE g_hInstance = NULL;

    static COLORREF DAT_FOREGROUND_COLOR = RGB(255, 255, 255);
    static COLORREF DAT_BACK_COLOR = RGB(0, 0, 0);

    // FUNCTION: DELAYLAMA 0x10006960
    GDIDrawingContext::GDIDrawingContext(Window *parentFramePtr,HDC hDC,HWND hWnd) {
        this->screenPos.x = 0;
        this->screenPos.y = 0;
        this->drawOffset.x = 0;
        this->drawOffset.y = 0;
        this->hDC = hDC;
        this->hWnd = hWnd;
        this->parentFrame = parentFramePtr;

        memset(this->unused2, 0, sizeof(this->unused2));
        this->penWidth = 1;
        this->penDashEnabled = false;
        memset(this->unused3, 0, sizeof(this->unused3));

        this->rect.left = 0;
        this->rect.top = 0;
        this->rect.right = 0;
        this->rect.bottom = 0;

        this->obj1 = NULL;
        this->obj2 = NULL;
        this->obj3 = NULL;
        this->originalPen = NULL;
        this->OriginalBrush = NULL;
        this->OriginalFont = NULL;

        // Set the drawing rect
        if (parentFramePtr == NULL)
        {
            this->rect.left = 0;
            this->rect.top = 0;
            this->rect.right = 1000;
            this->rect.bottom = 1000;
        }
        else
        {
            this->rect.left   = parentFramePtr->rect.left;
            this->rect.top    = parentFramePtr->rect.top;
            this->rect.right  = parentFramePtr->rect.right;
            this->rect.bottom = parentFramePtr->rect.bottom;
        }

        // Colors
        this->penColor = DAT_FOREGROUND_COLOR;
        this->backgroundColor = DAT_BACK_COLOR;
        this->textColor = DAT_FOREGROUND_COLOR;

        this->screenPos.x = 0;
        this->screenPos.y = 0;
        this->drawOffset.x = 0;
        this->drawOffset.y = 0;

        if (hDC != NULL)
        {
            this->originalPen   = GetCurrentObject(hDC, OBJ_PEN);
            this->OriginalBrush = GetCurrentObject(hDC, OBJ_BRUSH);
            this->OriginalFont  = GetCurrentObject(hDC, OBJ_FONT);

            SetBkMode(hDC, TRANSPARENT);
        }

        this->penStyle = 0;

        if (hWnd != NULL)
        {
            RECT windowRect;
            GetWindowRect(hWnd, &windowRect);

            this->screenPos.x = windowRect.left;
            this->screenPos.y = windowRect.top;
        }

        if (hDC != NULL)
        {
            this->setPenColor(this->penColor);
            this->setPenDashMode(this->penDashEnabled);
            this->setBackgroundColorAndBrush(this->backgroundColor);
            this->setTextColor(this->textColor);
        }
    }
    
    // FUNCTION: DELAYLAMA 0x10006da0
    void GDIDrawingContext::setPenColor(COLORREF color)
    {
        this->penColor = color;

        LOGPEN pen;
        pen.lopnWidth.x = this->penWidth;
        pen.lopnWidth.y = this->penWidth;
        pen.lopnStyle   = this->penStyle;
        pen.lopnColor   = this->penColor;

        HPEN newPen = CreatePenIndirect(&pen);
        if (newPen == NULL)
            return;

        SelectObject(this->hDC, newPen);

        if (this->obj2 != NULL)
        {
            DeleteObject(this->obj2);
        }

        this->obj2 = newPen;
    }

    // FUNCTION: DELAYLAMA 0x10006be0
    void GDIDrawingContext::setPenDashMode(bool penDashEnabled)
    {
        this->penDashEnabled = penDashEnabled;

        if (penDashEnabled == true)
            this->penStyle = PS_DASH;
        else
            this->penStyle = PS_SOLID;

        LOGPEN pen;
        pen.lopnWidth.x = this->penWidth;
        pen.lopnWidth.y = this->penWidth;
        pen.lopnStyle   = this->penStyle;
        pen.lopnColor   = this->penColor;

        HPEN newPen = CreatePenIndirect(&pen);
        if (newPen == NULL)
            return;

        SelectObject(this->hDC, newPen);

        if (this->obj2 != NULL)
        {
            DeleteObject(this->obj2);
        }

        this->obj2 = newPen;
    }

    // FUNCTION: DELAYLAMA 0x10006e20
    void GDIDrawingContext::setBackgroundColorAndBrush(COLORREF color)
    {
        this->backgroundColor = color;

        SetBkColor(this->hDC, this->backgroundColor);

        LOGBRUSH brush;
        brush.lbStyle = BS_SOLID;
        brush.lbColor = this->backgroundColor;
        brush.lbHatch = 0;

        HBRUSH newBrush = CreateBrushIndirect(&brush);
        if (newBrush == NULL)
        {
            GetLastError();
            return;
        }

        SelectObject(this->hDC, newBrush);

        if (this->obj1 != NULL)
        {
            DeleteObject(this->obj1);
        }

        this->obj1 = newBrush;
    }

    // FUNCTION: DELAYLAMA 0x10006d60
    void GDIDrawingContext::setTextColor(COLORREF color)
    {
        this->textColor = color;
        SetTextColor(this->hDC, this->textColor);
    }

    // FUNCTION: DELAYLAMA 0x10003620
    int32_t GDIDrawingContext::setModuleHandle(HINSTANCE hInstance)
    {
        g_hInstance = hInstance;
        return 1;
    }
}
}
}
}