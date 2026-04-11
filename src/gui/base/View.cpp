#include "View.h"

namespace DelayLama {
namespace Gui {
namespace Base {
    View::View(RECT * pRect) {
        this->referenceCount = 1;
        this->rect.left = pRect->left;
        this->rect.top = pRect->top;
        this->rect.right = pRect->right;
        this->rect.bottom = pRect->bottom;
        this->absRect.left = pRect->left;
        this->absRect.top = pRect->top;
        this->absRect.right = pRect->right;
        this->absRect.bottom = pRect->bottom;
        this->parent = nullptr;
        this->unused = 0;
        this->isDirty = true;
        this->isEnabled = true;
        this->useAlphaBlending = false;
    }

    void View::onDraw(Platform::GDIDrawingContext* drawingContext) {}
    void View::onMouseDown(Platform::GDIDrawingContext* drawingContext, POINT* point) {}
}
}
}