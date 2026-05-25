#pragma once
#include <Windows.h>
#include <windef.h>
#include <damsdk/utils/portable_stdint.h>

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows { class GDIDrawingContext; class Bitmap; class Window; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Base {

    // VTABLE: DELAYLAMA 0x1000bbec
    class View {
        public:
            int referenceCount;
            RECT rect;
            RECT absRect;
            DamSDK::Gui::Platform::Windows::Window *parent;
            int unused;
            bool _isDirty;
            bool isEnabled;
            bool useAlphaBlending;
        public:
            View(RECT *pRect);
            ~View();
            virtual bool isDirty();
            virtual void setDirty(bool isDirty);
            virtual void update(Platform::Windows::GDIDrawingContext *drawingContext);
            virtual void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
            virtual void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* point);
            virtual bool onMouseWheel(Platform::Windows::GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta);
            virtual void setEnabled(bool enabled);
            virtual bool getEnabled();
            virtual void setAbsRect(RECT* rect);
            virtual void getRect(RECT* outRect);
            virtual void setUseAlphaBlending(bool useAlphaBlending);
            virtual bool getUseAlphaBlending();
            virtual void setParent(Platform::Windows::Window* parent);
            virtual void returnTrue1(Platform::Windows::Window *frame);
            virtual void returnTrue2(Platform::Windows::Window *frame);
            virtual int getReferenceCount();
            virtual bool routeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT* mousePos);
            virtual void setRect(RECT* rect);
            virtual void release();
            
            static void useBitmap(Platform::Windows::Bitmap *bmp);
            static uint32_t GetPressedModifiersAndMouseButtons();

    };
}
}
}