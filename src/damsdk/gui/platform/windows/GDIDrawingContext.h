#pragma once
#include <Windows.h>
#include <windef.h>
#include <damsdk/utils/portable_stdint.h>

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

    extern COLORREF DAT_FOREGROUND_COLOR;
    extern COLORREF DAT_GRAY_COLOR;
    extern COLORREF DAT_BACK_COLOR;

    // VTABLE: DELAYLAMA 0x1000bbe4
    class GDIDrawingContext {
        public:
            POINT screenPos;
            POINT drawOffset;
            HDC hDC;
            HWND hWnd;
            void*	parentFrame;
            char unused1[8];
            COLORREF textColor;
            char unused2[8];
            LONG penWidth;
            COLORREF penColor;
            COLORREF backgroundColor;
            bool penDashEnabled;
            char unused3[4];
            RECT rect;
            HBRUSH obj1;
            HPEN obj2;
            HBRUSH obj3;
            HGDIOBJ originalPen;
            HGDIOBJ OriginalBrush;
            HGDIOBJ OriginalFont;
            UINT penStyle;

        public:
            GDIDrawingContext(Window *parentFramePtr,HDC hDC,HWND hWnd);
            ~GDIDrawingContext();
            static int32_t setModuleHandle(HINSTANCE hInstance);
            virtual void setPenColor(COLORREF color);
            virtual void setPenDashMode(bool penDashEnabled);
            virtual void setBackgroundColorAndBrush(COLORREF color);
            virtual void setTextColor(COLORREF color);
            virtual void cleanResources();
            virtual void moveToEx(POINT* target);
            virtual void lineTo(POINT* tageet);
            virtual void drawRectangleOutline(RECT* param_1);
            virtual void fillRectangleInset(RECT* param_1);
            virtual void getRelativeMousePos(POINT* outRelMousePos);
            virtual void copyToScreen(GDIDrawingContext* dest, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcX, int srcY);
            static void setCursor(int cursorType);
    };
}
}
}
}