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

    // FUNCTION: DELAYLAMA 0x100071c0
    View::~View() {
        // this->resetVtable(); This implies that view is a window, but nothing else implies that this is the case.
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
    bool View::onMouseWheel(Platform::Windows::GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta) { return false; }

    // STUB: DELAYLAMA 0x100071f0
    void View::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* point) { }

    // FUNCTION: DELAYLAMA 0x10004570
    void View::setEnabled(bool enabled) { this->isEnabled = enabled; }

    // FUNCTION: DELAYLAMA 0x10007ec0
    void View::useBitmap(Platform::Windows::Bitmap *bmp) {
        bmp->refCount = bmp->refCount + 1;
    }

    // FUNCTION: DELAYLAMA 0x10004580
    bool View::getEnabled() {
        return this->isEnabled;
    }

    // FUNCTION: DELAYLAMA 0x10004590
    void View::setAbsRect(RECT* rect) {
        this->absRect.left = rect->left;
        this->absRect.top = rect->top;
        this->absRect.right = rect->right;
        this->absRect.bottom = rect->bottom;
        return;
    }

    // FUNCTION: DELAYLAMA 0x100045b0
    void View::getRect(RECT* outRect) {
        outRect->left = this->absRect.left;
        outRect->top = this->absRect.top;
        outRect->right = this->absRect.right;
        outRect->bottom = this->absRect.bottom;
        return;
    }

    // FUNCTION: DELAYLAMA 0x100045e0
    void View::setUseAlphaBlending(bool useAlphaBlending) {
        this->useAlphaBlending = useAlphaBlending;
        return;
    }

    // FUNCTION: DELAYLAMA 0x100045f0
    bool View::getUseAlphaBlending() {
        return this->useAlphaBlending;
    }

    // FUNCTION: DELAYLAMA 0x10004640
    void View::setParent(Platform::Windows::Window* parent) {
        this->parent = parent;
        return;
    }

    // FUNCTION: DELAYLAMA 0x10004690
    int View::getReferenceCount() {
        return this->referenceCount;
    }

    // FUNCTION: DELAYLAMA 0x10006ec0
    uint8_t View::GetPressedModifiersAndMouseButtons() {
        SHORT keyState = GetAsyncKeyState(VK_LBUTTON);
        byte combinedMask = keyState < 0;
        keyState = GetAsyncKeyState(VK_MBUTTON);
        if (keyState < 0) {
          combinedMask = combinedMask | 2;
        }
        keyState = GetAsyncKeyState(VK_RBUTTON);
        if (keyState < 0) {
          combinedMask = combinedMask | 4;
        }
        keyState = GetAsyncKeyState(VK_SHIFT);
        if (keyState < 0) {
          combinedMask = combinedMask | 8;
        }
        keyState = GetAsyncKeyState(VK_CONTROL);
        if (keyState < 0) {
          combinedMask = combinedMask | 0x10;
        }
        keyState = GetAsyncKeyState(VK_MENU);
        if (keyState < 0) {
          combinedMask = combinedMask | 0x20;
        }
        return combinedMask;
    }

    // FUNCTION: DELAYLAMA 0x10007200
    bool View::routeMessage() {
        return false;
    }

    // STUB: DELAYLAMA 0x10007250
    void View::setRect(RECT* rect) {
        // this->rect.left = rect->left;
        // this->rect.top = rect->top;
        // this->rect.right = rect->right;
        // this->rect.bottom = rect->bottom;
        // (*this->vtable->setDirty)(1);
        // return;
    }

    void View::returnTrue1(Platform::Windows::Window *frame) {}
    void View::returnTrue2(Platform::Windows::Window *frame) {}

    // STUB: DELAYLAMA 0x10007280
    void View::release() {
        // int referenceCount;
        //
        // referenceCount = this->referenceCount;
        // if (((0 < referenceCount) &&
        //     (referenceCount = referenceCount + -1, this->referenceCount = referenceCount,
        //     referenceCount == 0)) && (this != (View *)0x0)) {
        //   (*this->vtable->destructor)(1);
        // }
        // return;
    }
}
}
}