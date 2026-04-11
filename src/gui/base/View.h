#pragma once
#include <Windows.h>
#include <windef.h>
#include "platform/GDIDrawingContext.h"

namespace DelayLama {
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
            void onDraw(Platform::GDIDrawingContext* drawingContext);
            void onMouseDown(Platform::GDIDrawingContext* drawingContext, POINT* point);
    };
}
}
}