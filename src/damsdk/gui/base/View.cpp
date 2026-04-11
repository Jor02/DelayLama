#include "View.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Bitmap.h"

namespace DamSDK {
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
        this->_isDirty = true;
        this->isEnabled = true;
        this->useAlphaBlending = false;
    }

    bool View::isDirty() { return _isDirty; }
    void View::setDirty(bool isDirty) { _isDirty = isDirty; }
    void View::update(Platform::Windows::GDIDrawingContext *drawingContext) { }
    void View::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) { }
    void View::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* point) { }
    void View::setEnabled(bool enabled) { this->isEnabled = enabled; }
    void View::useBitmap(Platform::Windows::Bitmap *bmp) {
        bmp->refCount = bmp->refCount + 1;
    }
}
}
}