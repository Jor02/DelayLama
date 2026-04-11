#pragma once
#include <Windows.h>
#include <minwindef.h>
#include <windef.h>
#include <cstdint>

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

    struct GDIDrawingContext {
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
            COLORREF penDashColor;
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
            static int32_t setModuleHandle(HINSTANCE hInstance);
    };
}
}
}
}