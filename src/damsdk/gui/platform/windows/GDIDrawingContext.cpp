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

    // STUB: DELAYLAMA 0x10006af0
    GDIDrawingContext::~GDIDrawingContext()
    {
        // cleanResources(this);
        // if (deleteObject) {
        //   operator_delete(this);
        // }
        // return this;
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

    // STUB: DELAYLAMA 0x10006b10
    void GDIDrawingContext::cleanResources() {
        // HGDIOBJ pvVar1;
        //
        // pvVar1 = this->originalPen;
        // this->vtable = &GDIDrawingContextVtable_1000bbe4;
        // if (pvVar1 != (HGDIOBJ)0x0) {
        //   SelectObject(this->hDC,pvVar1);
        // }
        // pvVar1 = this->OriginalBrush;
        // if (pvVar1 != (HGDIOBJ)0x0) {
        //   SelectObject(this->hDC,pvVar1);
        // }
        // pvVar1 = this->OriginalFont;
        // if (pvVar1 != (HGDIOBJ)0x0) {
        //   SelectObject(this->hDC,pvVar1);
        // }
        // pvVar1 = this->obj1;
        // if (pvVar1 != (HGDIOBJ)0x0) {
        //   DeleteObject(pvVar1);
        // }
        // pvVar1 = this->obj2;
        // if (pvVar1 != (HGDIOBJ)0x0) {
        //   DeleteObject(pvVar1);
        // }
        // pvVar1 = this->obj3;
        // if (pvVar1 != (HGDIOBJ)0x0) {
        //   DeleteObject(pvVar1);
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10006b80
    void GDIDrawingContext::moveToEx(POINT* target) {
        // MoveToEx(this->hDC,target->x + this->drawOffset.x,
        //          target->y + this->drawOffset.y,(LPPOINT)0x0);
        // return;
    }

    // STUB: DELAYLAMA 0x10006bb0
    void GDIDrawingContext::lineTo(POINT* tageet) {
        // LineTo(this->hDC,tageet->x + this->drawOffset.x,
        //        tageet->y + this->drawOffset.y);
        // return;
    }

    // STUB: DELAYLAMA 0x10006c60
    void GDIDrawingContext::drawRectangleOutline(RECT* param_1) {
        // int bottom;
        // int top;
        // int right;
        // int left;
        //
        // top = this->drawOffset.y;
        // right = this->drawOffset.x;
        // bottom = param_1->bottom + top;
        // top = param_1->top + top;
        // left = param_1->left + right;
        // right = param_1->right + right;
        // MoveToEx(this->hDC,left,top,(LPPOINT)0x0);
        // LineTo(this->hDC,right,top);
        // LineTo(this->hDC,right,bottom);
        // LineTo(this->hDC,left,bottom);
        // LineTo(this->hDC,left,top);
        // return;
    }

    // STUB: DELAYLAMA 0x10006ce0
    void GDIDrawingContext::fillRectangleInset(RECT* param_1) {
        // HGDIOBJ pvVar1;
        // RECT rect;
        // int xOffset;
        // int yOffset;
        //
        // xOffset = this->drawOffset.x;
        // yOffset = this->drawOffset.y;
        // rect.right = param_1->right + xOffset;
        // rect.bottom = param_1->bottom + yOffset;
        // rect.left = param_1->left + xOffset + 1;
        // rect.top = param_1->top + yOffset + 1;
        // pvVar1 = GetStockObject(8);
        // pvVar1 = SelectObject(this->hDC,pvVar1);
        // FillRect(this->hDC,&rect,this->obj1);
        // SelectObject(this->hDC,pvVar1);
        // return;
    }

    // STUB: DELAYLAMA 0x10006f20
    void GDIDrawingContext::getRelativeMousePos(POINT* outRelMousePos) {
        // tagPOINT mousePos;
        // int yPos;
        //
        // GetCursorPos(&mousePos);
        // outRelMousePos->x = mousePos.x;
        // outRelMousePos->y = mousePos.y;
        // yPos = this->screenPos.y;
        // outRelMousePos->x = mousePos.x - this->screenPos.x;
        // outRelMousePos->y = mousePos.y - yPos;
        // return;
    }

    // STUB: DELAYLAMA 0x100070f0
    void GDIDrawingContext::copyToScreen(GDIDrawingContext* dest, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcX, int srcY) {
        // BitBlt((dest->members).hDC,(dest->members).drawOffset.x + dstLeft,
        //        (dest->members).drawOffset.y + dstTop,dstRight - dstLeft,dstBottom - dstTop,
        //        this->hDC,srcX,srcY,0xcc0020);
        // return;
    }

    // STUB: DELAYLAMA 0x10007d30
    void GDIDrawingContext::setCursor(int cursorType) {
        // HCURSOR cursor1;
        // HCURSOR cursor2;
        //
        // if (this->originalPen == (HGDIOBJ)0x0) {
        //   cursor1 = GetCursor();
        //   this->originalPen = cursor1;
        // }
        // switch(cursorType) {
        // case 0:
        //   SetCursor(this->originalPen);
        //   return;
        // case 1:
        //   cursor2 = LoadCursorA((HINSTANCE)0x0,&lpCursorName_00007f02);
        //   SetCursor(cursor2);
        //   return;
        // case 2:
        //   cursor2 = LoadCursorA((HINSTANCE)0x0,&lpCursorName_00007f84);
        //   SetCursor(cursor2);
        //   return;
        // case 3:
        //   cursor2 = LoadCursorA((HINSTANCE)0x0,&lpCursorName_00007f85);
        //   SetCursor(cursor2);
        //   return;
        // case 4:
        //   cursor2 = LoadCursorA((HINSTANCE)0x0,&lpCursorName_00007f86);
        //   SetCursor(cursor2);
        //   break;
        // case 5:
        //   cursor2 = LoadCursorA((HINSTANCE)0x0,&lpCursorName_00007f83);
        //   SetCursor(cursor2);
        //   return;
        // case 6:
        //   cursor2 = LoadCursorA((HINSTANCE)0x0,&lpCursorName_00007f82);
        //   SetCursor(cursor2);
        //   return;
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10008340
    void GDIDrawingContext::unregisterClass() {
        // g_RegistrationCount = g_RegistrationCount + -1;
        // if (g_RegistrationCount == 0) {
        //   UnregisterClassA((LPCSTR)&lpClassName_1000d80c,g_hInstance);
        // }
        // return;
    }
}
}
}
}