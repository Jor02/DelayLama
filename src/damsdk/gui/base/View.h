#pragma once
#include <Windows.h>
#include <windef.h>

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows { class GDIDrawingContext; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Base {

    class View {
        public:
            int referenceCount;
            RECT rect;
            RECT absRect;
            struct Window *parent;
            int unused;
            bool isDirty;
            bool isEnabled;
            bool useAlphaBlending;
        public:
            View(RECT *pRect);
            void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
            void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* point);
    };
}
}
}