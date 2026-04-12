#pragma once
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <windef.h>
#include "damsdk/gui/base/View.h"
#include "utils/portable_stdint.h"

namespace DamSDK {
    namespace Api { class EditorBase; }
    namespace Gui {
        namespace Controls { class Control; }
    }
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {
    class Window : public Base::View {
        public:
            char unused1;
            Api::EditorBase *editor;
            HWND handle;
            Bitmap *backgroundBitmap;
            int numChildren;
            int maxChildren;
            Controls::Control **children;
            Base::View *modalView;
            COLORREF* colors;
            bool redrawPending;
            bool visible;
            char unused3[2];
            HWND hWnd;
            bool isActive;
            char unused4[3];
            void *closeParameter;
        public:
            Window(RECT *pRect, HWND hParent, Api::EditorBase *editor);
            void update(GDIDrawingContext *drawingContext) override;
            void onDraw(GDIDrawingContext *drawingContext) override;
            bool openPluginWindow(HWND hParent);
            bool needsRedraw();
            bool registerControl(Controls::Control *control);
            void onMouseWheel(GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta) override;
            void drawControlOrSelf(Controls::Control *target);
            void refresh();
            static bool registerWindowClass();
            void setBackgroundBitmap(Bitmap *background);
    };
}
}
}
}