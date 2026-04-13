#include "HorizontalSlider.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // FUNCTION: DELAYLAMA 0x1000a010
    HorizontalSlider::HorizontalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, Api::Range *range, int flags) : Control(pRect, callback, parameterId, backgroundBmp)
    {
        this->backgroundOffset.x = range->min;
        this->backgroundOffset.y = range->max;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->flags = flags;
        this->handleImage = handleBmp;
        this->trackMinX = minValue;
        this->trackMaxX = maxValue;
        this->snapToMouse = true;
        this->isHandleTransparent = true;
        if (handleBmp == nullptr) {
            this->handleWidth = 1;
            this->handleHeight = 1;
        }
        else {
            View::useBitmap(handleBmp);
            handleImage = this->handleImage;
            this->handleWidth = handleImage->width;
            this->handleHeight = handleImage->height;
        }
        this->trackWidth =
            this->rect.right -
            this->rect.left;
        this->trackHeight =
            this->rect.bottom -
            this->rect.top;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->handleMinPos = minValue - this->rect.left;
        this->handleMaxPos = (this->handleWidth - this->rect.left) + maxValue;
        this->currentValue = 10.0;
    }
    
    // FUNCTION: DELAYLAMA 0x1000a120
    HorizontalSlider::~HorizontalSlider() {
        this->destroy();
    }

    // FUNCTION: DELAYLAMA 0x10009b50
    void HorizontalSlider::setIsHandleTransparent(bool transparent) {
        this->isHandleTransparent = transparent;
    }

    // FUNCTION: DELAYLAMA 0x10009b60
    void HorizontalSlider::setSnapToMouse(bool snapToMouse) {
        this->snapToMouse = snapToMouse;
    }

    // FUNCTION: DELAYLAMA 0x10009b70
    bool HorizontalSlider::getSnapToMouse() {
        return this->snapToMouse;
    }

    // FUNCTION: DELAYLAMA 0x10009b80
    void HorizontalSlider::setHandlePos(POINT* handlePos) {
        this->handlePos.x = handlePos->x;
        this->handlePos.y = handlePos->y;
    }

    // FUNCTION: DELAYLAMA 0x10009ba0
    Platform::Windows::Bitmap* HorizontalSlider::getHandleImage() {
        return this->handleImage;
    }

    // FUNCTION: DELAYLAMA 0x10009bb0
    void HorizontalSlider::setCurrentValue(float currentValue) {
        this->currentValue = currentValue;
    }

    // FUNCTION: DELAYLAMA 0x10009bc0
    float HorizontalSlider::getCurrentValue() {
        return this->currentValue;
    }

    // FUNCTION: DELAYLAMA 0x10009fe0
    void HorizontalSlider::changeHandle(Platform::Windows::Bitmap* newHandle) {
        if (this->handleImage != nullptr) {
            Platform::Windows::Bitmap::unregisterBitmap(this->handleImage);
        }
        this->handleImage = newHandle;
        if (newHandle != nullptr) {
            View::useBitmap(newHandle);
        }
    }

    // FUNCTION: DELAYLAMA 0x1000a140
    void HorizontalSlider::destroy() {
        if (this->handleImage != nullptr) {
            Platform::Windows::Bitmap::unregisterBitmap(this->handleImage);
        }
        this->destroy();
    }

    // STUB: DELAYLAMA 0x1000a1a0
    void HorizontalSlider::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        // HorizontalSliderVTable *pHVar1;
        // OffscreenGDIDrawingContext *tempDrawingContext;
        // long lVar2;
        // undefined4 unaff_ESI;
        // undefined4 unaff_EDI;
        // int in_stack_ffffffa0;
        // int in_stack_ffffffa4;
        // Window *in_stack_ffffffa8;
        // OffscreenGDIDrawingContext *in_stack_ffffffac_00;
        // int srcY;
        // int valueRange;
        // LONG LStack_30;
        // RECT handleRect;
        // RECT trackRect;
        // undefined1 *puStack_8;
        // undefined4 local_4;
        // POINT *sourcePoint;
        // int in_stack_ffffffac;
        // Bitmap *background;
        // int handleMinPos;
        // RECT *rect;
        //
        // local_4 = 0xffffffff;
        // puStack_8 = &LAB_1000ae6b;
        // valueRange = (int)operator_new(0x8c);
        // local_4 = 0;
        // if ((OffscreenGDIDrawingContext *)valueRange == (OffscreenGDIDrawingContext *)0x0) {
        //   tempDrawingContext = (OffscreenGDIDrawingContext *)0x0;
        // }
        // else {
        //   in_stack_ffffffa8 = this->control.viewMembers.parent;
        //   in_stack_ffffffac_00 = (OffscreenGDIDrawingContext *)this->trackWidth;
        //   in_stack_ffffffa4 = 0x1000a215;
        //   tempDrawingContext =
        //        OffscreenGDIDrawingContext::offscreenGDIDrawingContext
        //                  ((OffscreenGDIDrawingContext *)valueRange,in_stack_ffffffa8,
        //                   (int)in_stack_ffffffac_00,this->trackHeight,DAT_BACK_COLOR);
        // }
        // background = this->control.bitmap;
        // trackRect.right = this->trackWidth;
        // trackRect.bottom = this->trackHeight;
        // local_4 = 0xffffffff;
        // trackRect.left = 0;
        // trackRect.top = 0;
        // if (background != (Bitmap *)0x0) {
        //   sourcePoint = &this->backgroundOffset;
        //   if (this->control.viewMembers.useAlphaBlending == false) {
        //     in_stack_ffffffa8 = (Window *)0x1000a263;
        //     in_stack_ffffffac_00 = tempDrawingContext;
        //     Bitmap::blit(background,(GDIDrawingContext *)tempDrawingContext,&trackRect,sourcePoint);
        //   }
        //   else {
        //     in_stack_ffffffa8 = (Window *)0x1000a25c;
        //     in_stack_ffffffac_00 = tempDrawingContext;
        //     Bitmap::drawMasked(background,(GDIDrawingContext *)tempDrawingContext,&trackRect,sourcePoint);
        //   }
        // }
        // handleRect.top = this->handlePos.y;
        // handleRect.bottom = this->handleHeight + handleRect.top;
        // valueRange = this->trackMaxX - this->trackMinX;
        // lVar2 = _ftol((double)CONCAT44(unaff_ESI,unaff_EDI));
        // handleMinPos = this->handleMinPos;
        // handleRect.left = lVar2 + this->handlePos.x;
        // if (handleRect.left < handleMinPos) {
        //   handleRect.left = handleMinPos;
        // }
        // handleRect.right = this->handleWidth + handleRect.left;
        // handleMinPos = this->handleMaxPos;
        // if (handleMinPos < handleRect.right) {
        //   handleRect.right = handleMinPos;
        // }
        // background = this->handleImage;
        // if (background != (Bitmap *)0x0) {
        //   if (this->isHandleTransparent == false) {
        //     valueRange = 0;
        //     LStack_30 = 0;
        //     in_stack_ffffffa8 = (Window *)0x1000a2fd;
        //     in_stack_ffffffac_00 = tempDrawingContext;
        //     Bitmap::blit(background,(GDIDrawingContext *)tempDrawingContext,&handleRect,
        //                  (POINT *)&valueRange);
        //   }
        //   else {
        //     valueRange = 0;
        //     LStack_30 = 0;
        //     in_stack_ffffffa8 = (Window *)0x1000a2e3;
        //     in_stack_ffffffac_00 = tempDrawingContext;
        //     Bitmap::drawMasked(background,(GDIDrawingContext *)tempDrawingContext,&handleRect,
        //                        (POINT *)&valueRange);
        //   }
        // }
        // srcY = 0;
        // handleMinPos = 0;
        // valueRange = (int)&stack0xffffffa0;
        // rect = &this->control.viewMembers.rect;
        // copyRect(&stack0xffffffa0,rect);
        // GDIDrawingContext::copyToScreen
        //           ((GDIDrawingContext *)tempDrawingContext,drawingContext,in_stack_ffffffa0,
        //            in_stack_ffffffa4,(int)in_stack_ffffffa8,(int)in_stack_ffffffac_00,handleMinPos,srcY);
        // if (tempDrawingContext != (OffscreenGDIDrawingContext *)0x0) {
        //   (*tempDrawingContext->vtable->destructor)();
        // }
        // pHVar1 = this->vtable;
        // this->trackLeftX = rect->left + handleRect.left;
        // (*(pHVar1->control).view.setDirty)();
        // return;
    }

    // STUB: DELAYLAMA 0x1000a360
    void HorizontalSlider::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos) {
        // bool _isDirty;
        // byte inputMask;
        // bool isDirty;
        // undefined3 _padding1;
        // uint loopModifiers;
        // undefined3 _padding3;
        // float10 defaultValue;
        // int horizontalAnchor;
        // int flags;
        // int trackMaxX;
        // int trackLeft;
        // uint currentModifiers;
        // undefined3 _padding2;
        // GDIDrawingContext *_drawingContext;
        // float calculatedValue;
        // int mouseX;
        // int trackMinX;
        // int trackTop;
        // HorizontalSliderVTable *vtable;
        //
        // _drawingContext = drawingContext;
        // if (this->control.viewMembers.isEnabled != false) {
        //   inputMask = View::GetPressedModifiersAndMouseButtons();
        //   currentModifiers = CONCAT31(_padding1,inputMask);
        //                   /* Check for Ctrl (0x10) + Left Click (0x01) = 0x11 */
        //   if (currentModifiers == 0x11) {
        //     defaultValue = (float10)(*(this->vtable->control).getDefaultValue)();
        //     vtable = this->vtable;
        //     this->control.value = (float)defaultValue;
        //     isDirty = (bool)(*(vtable->control).view.isDirty)();
        //     if (isDirty != false) {
        //       (*(code *)**(undefined4 **)this->control.callback)(drawingContext,this);
        //       return;
        //     }
        //   }
        //   else {
        //                   /* Check for Left Click (0x01) */
        //     if ((inputMask & 1) != 0) {
        //       trackMinX = this->trackMinX;
        //       if (this->snapToMouse == false) {
        //         trackLeft = this->trackLeftX;
        //         trackTop = this->control.viewMembers.rect.top;
        //         mouseX = mousePos->x;
        //         if (mouseX < trackLeft) {
        //           return;
        //         }
        //         if (this->handleWidth + trackLeft < mouseX) {
        //           return;
        //         }
        //         if (mousePos->y < trackTop) {
        //           return;
        //         }
        //         if (this->handleHeight + trackTop < mousePos->y) {
        //           return;
        //         }
        //         horizontalAnchor = trackMinX + (mouseX - trackLeft);
        //       }
        //       else {
        //         horizontalAnchor = trackMinX + -1 + this->handleWidth / 2;
        //       }
        //       trackMaxX = this->trackMaxX;
        //       drawingContext = (GDIDrawingContext *)this->control.value;
        //       Window::beginEdit(this->control.viewMembers.parent,
        //                         this->control.parameterId);
        //       inputMask = View::GetPressedModifiersAndMouseButtons();
        //       loopModifiers = CONCAT31(_padding2,inputMask);
        //       while ((inputMask & 1) != 0) {
        //         if ((currentModifiers == loopModifiers) || ((loopModifiers & 8) == 0)) {
        //           if ((loopModifiers & 8) == 0) {
        //             drawingContext = (GDIDrawingContext *)this->control.value;
        //           }
        //         }
        //         else {
        //           drawingContext = (GDIDrawingContext *)this->control.value;
        //           currentModifiers = loopModifiers;
        //         }
        //         flags = this->flags;
        //         calculatedValue = (float)(mousePos->x - horizontalAnchor) / (float)(trackMaxX - trackMinX)
        //         ;
        //         this->control.value = calculatedValue;
        //         if ((flags & 0x10) != 0) {
        //           this->control.value = FLOAT_1000b8fc - calculatedValue;
        //         }
        //         if ((loopModifiers & 8) != 0) {
        //           this->control.value =
        //                (this->control.value - (float)drawingContext) /
        //                this->currentValue + (float)drawingContext;
        //         }
        //         (*(this->vtable->control).clampValue)();
        //         _isDirty = (bool)(*(this->vtable->control).view.isDirty)();
        //         if (_isDirty != false) {
        //           (*(code *)**(undefined4 **)this->control.callback)(_drawingContext,this);
        //         }
        //         GDIDrawingContext::getRelativeMousePos(_drawingContext,mousePos);
        //         (*(this->vtable->control).onIdle)();
        //         inputMask = View::GetPressedModifiersAndMouseButtons();
        //         loopModifiers = CONCAT31(_padding3,inputMask);
        //       }
        //       Window::endEdit(this->control.viewMembers.parent,
        //                       this->control.parameterId);
        //     }
        //   }
        // }
        // return;
    }
}
}
}