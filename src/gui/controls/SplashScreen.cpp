#include "SplashScreen.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    
    // STUB: DELAYLAMA 0x1000a530
    SplashScreen::SplashScreen(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, DamSDK::Gui::Platform::Windows::Bitmap *bmp, RECT *destRect,POINT *srcPoint) : DamSDK::Gui::Controls::Control(pRect, callback, parameterId, bmp)
    {
        // Control::Control((Control *)this,pRect,callback,parameterId,bmp);
        // (this->destRect).left = destRect->left;
        // (this->destRect).top = destRect->top;
        // (this->destRect).right = destRect->right;
        // (this->destRect).bottom = destRect->bottom;
        // (this->absRect).left = 0;
        // (this->absRect).top = 0;
        // (this->absRect).right = 0;
        // (this->absRect).bottom = 0;
        // (this->srcPoint).x = srcPoint->x;
        // (this->srcPoint).y = srcPoint->y;
        // this->vtable = &vftable_1000c154_1000c154;
        // return this;
    }

    // STUB: DELAYLAMA 0x1000a5a0
    SplashScreen::~SplashScreen() {
        // this->destroy();
        // if (deleteObject) {
        //   operator_delete(this);
        // }
        // return this;
    }

    // STUB: DELAYLAMA 0x1000a5c0
    void SplashScreen::destroy() {
        // this->vtable = &vftable_1000c154_1000c154;
        // Control::destroy((Control *)this);
        // return;
    }

    // STUB: DELAYLAMA 0x1000a5d0
    void SplashScreen::onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContect) {
        // Bitmap *bitmap;
        //
        // if (((this->control).value != FLOAT_1000b208) &&
        //    (bitmap = (this->control).bitmap, bitmap != (Bitmap *)0x0)) {
        //   if ((this->control).viewMembers.useAlphaBlending != false) {
        //     Bitmap::drawMasked(bitmap,drawingContect,&this->destRect,&this->srcPoint);
        //     (*(this->vtable->control).view.setDirty)(0);
        //     return;
        //   }
        //   Bitmap::blit(bitmap,drawingContect,&this->destRect,&this->srcPoint);
        // }
        // (*(this->vtable->control).view.setDirty)(0);
        // return;
    }

    // STUB: DELAYLAMA 0x1000a630
    void SplashScreen::onMouseDown(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext) {
        // bool anyKeyPressed;
        // int32_t result;
        // GDIDrawingContext *drawingContect;
        // Window *parentPtr;
        // RECT *rectPtr;
        // SplashScreenVTable *vtablePtr;
        //
        // drawingContect = drawingContext;
        // if (((this->control).viewMembers.isEnabled != false) &&
        //    (anyKeyPressed = (bool)View::GetPressedModifiersAndMouseButtons(), (anyKeyPressed & 0x1U) != 0)
        //    ) {
        //   drawingContext = (GDIDrawingContext *)(uint)((this->control).value == FLOAT_1000b208);
        //   (this->control).value = (float)(int)drawingContext;
        //   if ((float)(int)drawingContext == FLOAT_1000b208) {
        //     (this->control).viewMembers.rect.left = (this->absRect).left;
        //     (this->control).viewMembers.rect.top = (this->absRect).top;
        //     (this->control).viewMembers.rect.right = (this->absRect).right;
        //     (this->control).viewMembers.rect.bottom = (this->absRect).bottom;
        //     parentPtr = (this->control).viewMembers.parent;
        //     if (parentPtr != (Window *)0x0) {
        //       Window::setModalView(parentPtr,(View *)0x0);
        //       (*(((this->control).viewMembers.parent)->vtable->view).onDraw)(drawingContect);
        //     }
        //     (*(code *)**(undefined4 **)(this->control).callback)(drawingContect,this);
        //   }
        //   else {
        //     parentPtr = (this->control).viewMembers.parent;
        //     if ((parentPtr != (Window *)0x0) &&
        //        (result = Window::setModalView(parentPtr,(View *)this), result != 0)) {
        //       rectPtr = &(this->control).viewMembers.rect;
        //       (this->absRect).left = rectPtr->left;
        //       (this->absRect).top = (this->control).viewMembers.rect.top;
        //       (this->absRect).right = (this->control).viewMembers.rect.right;
        //       (this->absRect).bottom = (this->control).viewMembers.rect.bottom;
        //       rectPtr->left = (this->destRect).left;
        //       (this->control).viewMembers.rect.top = (this->destRect).top;
        //       (this->control).viewMembers.rect.right = (this->destRect).right;
        //       vtablePtr = this->vtable;
        //       (this->control).viewMembers.rect.bottom = (this->destRect).bottom;
        //       (*(vtablePtr->control).view.onDraw)(drawingContect);
        //       (*(code *)**(undefined4 **)(this->control).callback)(drawingContect,this);
        //       (*(this->vtable->control).view.setDirty)(1);
        //       return;
        //     }
        //   }
        //   (*(this->vtable->control).view.setDirty)(1);
        // }
        // return;
    }
}
}
}