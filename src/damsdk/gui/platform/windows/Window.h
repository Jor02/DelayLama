#pragma once
#include <Windows.h>
#include <windef.h>
#include "damsdk/gui/base/View.h"

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
            struct Bitmap *backgroundBitmap;
            int numChildren;
            int maxChildren;
            struct Controls::Control **children;
            struct View *modalView;
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
            void onMouseWheel(int unused1, POINT unused2, int hoverState);
            void drawControlOrSelf(Controls::Control *target);
            void refresh();
            static bool registerWindowClass();
            void setBackgroundBitmap(Bitmap *background);
    };
}
}
}
}