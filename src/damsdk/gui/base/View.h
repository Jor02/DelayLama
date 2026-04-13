#pragma once
#include <Windows.h>
#include <windef.h>
#include <utils/portable_stdint.h>

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
            static void useBitmap(Platform::Windows::Bitmap *bmp);
            bool getEnabled();
            void setAbsRect(RECT* rect);
            void getRect(RECT* outRect);
            void setUseAlphaBlending(bool useAlphaBlending);
            bool getUseAlphaBlending();
            void setParent(Platform::Windows::Window* parent);
            void returnTrue1(Platform::Windows::Window *frame);
            void returnTrue2(Platform::Windows::Window *frame);
            int getReferenceCount();
            uint8_t GetPressedModifiersAndMouseButtons();
            bool routeMessage();
            void setRect(RECT* rect);
            void release();

    };
}
}
}