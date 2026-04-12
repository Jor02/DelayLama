#include "View.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Bitmap.h"

namespace DamSDK {
namespace Gui {
namespace Base {

    // FUNCTION: DELAYLAMA 0x10007140
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

    // FUNCTION: DELAYLAMA 0x100071a0
    bool View::isDirty() { return _isDirty; }

    // FUNCTION: DELAYLAMA 0x100071b0
    void View::setDirty(bool isDirty) { _isDirty = isDirty; }

    // STUB: DELAYLAMA 0x10007220
    void View::update(Platform::Windows::GDIDrawingContext *drawingContext) { }

    // STUB: DELAYLAMA 0x10004450 FOLDED
    void View::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) { }

    // STUB: DELAYLAMA 0x10007210
    void View::onMouseWheel(Platform::Windows::GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta) {}

    // STUB: DELAYLAMA 0x100071f0
    void View::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* point) { }

    // FUNCTION: DELAYLAMA 0x10004570
    void View::setEnabled(bool enabled) { this->isEnabled = enabled; }

    // FUNCTION: DELAYLAMA 0x10007ec0
    void View::useBitmap(Platform::Windows::Bitmap *bmp) {
        bmp->refCount = bmp->refCount + 1;
    }
}
}
}