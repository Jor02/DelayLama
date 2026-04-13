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
            HGDIOBJ obj1;
            HGDIOBJ obj2;
            HGDIOBJ obj3;
            HGDIOBJ originalPen;
            HGDIOBJ OriginalBrush;
            HGDIOBJ OriginalFont;
            UINT penStyle;

        public:
            GDIDrawingContext(Window *parentFramePtr,HDC hDC,HWND hWnd);
            ~GDIDrawingContext();
            static int32_t setModuleHandle(HINSTANCE hInstance);
            void setPenColor(COLORREF color);
            void setPenDashMode(bool penDashEnabled);
            void setBackgroundColorAndBrush(COLORREF color);
            void setTextColor(COLORREF color);
            void cleanResources();
            void moveToEx(POINT* target);
            void lineTo(POINT* tageet);
            void drawRectangleOutline(RECT* param_1);
            void fillRectangleInset(RECT* param_1);
            void getRelativeMousePos(POINT* outRelMousePos);
            void copyToScreen(GDIDrawingContext* dest, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcX, int srcY);
            void setCursor(int cursorType);
            void unregisterClass();
    };
}
}
}
}