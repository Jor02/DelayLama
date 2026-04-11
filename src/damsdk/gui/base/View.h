#pragma once
#include <Windows.h>
#include <windef.h>

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows { class GDIDrawingContext; class Bitmap; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Base {

    class View {
        private:
            bool _isDirty;
        public:
            int referenceCount;
            RECT rect;
            RECT absRect;
            struct Window *parent;
            int unused;
            bool isEnabled;
            bool useAlphaBlending;
        public:
            View(RECT *pRect);
            virtual bool isDirty();
            virtual void setDirty(bool isDirty);
            virtual void update(Platform::Windows::GDIDrawingContext *drawingContext);
            virtual void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
            virtual void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* point);
            static void useBitmap(Platform::Windows::Bitmap *bmp);
    };
}
}
}