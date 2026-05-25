#pragma once
#include <windows.h>
#include <windef.h>
#include "damsdk/gui/base/View.h"
#include "damsdk/utils/portable_stdint.h"

namespace DamSDK {
    namespace Api { class EditorBase; }
    namespace Gui {
        namespace Controls { class Control; }
        namespace Platform {
            namespace Windows { class DropTarget; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    // VTABLE: DELAYLAMA 0x1000bc48
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
            COLORREF* unknownClass;
            bool redrawPending;
            bool visible;
            char unused3[2];
            HWND hWnd;
            bool isActive;
            char unused4[3];
            void *closeParameter;
        public:
            Window(RECT *pRect, HWND hParent, Api::EditorBase *editor);
            ~Window();
            virtual void update(GDIDrawingContext *drawingContext) override;
            virtual void onDraw(GDIDrawingContext *drawingContext) override;
            virtual bool openPluginWindow(HWND hParent);
            virtual bool needsRedraw();
            virtual bool registerControl(Controls::Control *control);
            virtual bool onMouseWheel(GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta) override;
            virtual void drawControlOrSelf(Controls::Control *target);
            virtual void refresh();
            virtual void setBackgroundBitmap(Bitmap *background);
            virtual void resetVtable(Window* frame);
            virtual void cleanup();
            virtual bool closeWindow();
            virtual bool setDragAndDropState(bool enable);
            virtual void onMouseDown(GDIDrawingContext* drawingContext, POINT* mousePos);
            virtual bool routeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT* mousePos);
            virtual bool removeChild(Controls::Control* child, bool shouldRelease);
            virtual bool destroyChildren(bool* callExtraFlag);
            virtual bool containsChild(Controls::Control* target);
            virtual int32_t setModalView(Base::View* view);
            virtual void beginEdit(int parameterId);
            virtual void endEdit(int parameterId);
            virtual Controls::Control* getChildAtMousePos();
            virtual void getLocalMousePos(POINT* mousePos);
            virtual DropTarget* createDropTarget();
            static bool registerWindowClass();
            static void unregisterWindowClass();
    };
}
}
}
}