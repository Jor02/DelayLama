#pragma once
#include "VerticalSlider.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // STUB: DELAYLAMA 0x10009a40
    VerticalSlider::VerticalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, void *range, int flags) : Control(pRect, callback, parameterId, handleBmp)
    {
        // int iVar1;
        // int iVar2;
        //
        // Control::Control((Control *)this,pRect,callback,parameterId,backgroundBmp);
        // (this->backgroundOffset).x = range->min;
        // (this->backgroundOffset).y = range->max;
        // (this->handlePos).x = 0;
        // (this->handlePos).y = 0;
        // this->flags = stepSize;
        // this->handleImage = handleBmp;
        // this->trackMin = minValue;
        // this->trackMaxY = maxValue;
        // this->snapToMouse = true;
        // this->vtable = &VerticalSliderVTable_1000bfd4;
        // this->isHandleTransparent = true;
        // if (handleBmp == (Bitmap *)0x0) {
        //   this->handleWidth = 1;
        //   this->handleHeight = 1;
        // }
        // else {
        //   View::useBitmap(handleBmp);
        //   this->handleWidth = this->handleImage->width;
        //   this->handleHeight = this->handleImage->height;
        // }
        // this->trackWidth = (this->control).viewMembers.rect.right - (this->control).viewMembers.rect.left;
        // iVar1 = (this->control).viewMembers.rect.bottom;
        // iVar2 = (this->control).viewMembers.rect.top;
        // this->trackTopY = maxValue;
        // this->trackHeight = iVar1 - iVar2;
        // (this->handlePos).x = 0;
        // (this->handlePos).y = 0;
        // iVar1 = (this->control).viewMembers.rect.top;
        // this->fineTuneDivider = 0x41200000;
        // this->handleMinPos = minValue - iVar1;
        // this->handleMaxPos = (this->handleHeight - iVar1) + maxValue;
        // return this;
    }

    // STUB: DELAYLAMA 0x10009bd0
    VerticalSlider::~VerticalSlider() {
        // destroy(this);
        // if (deleteObject) {
        //   operator_delete(this);
        // }
        // return this;
    }

    // STUB: DELAYLAMA 0x10009bf0
    void VerticalSlider::destroy() {
        // this->vtable = &VerticalSliderVTable_1000bfd4;
        // if (this->handleImage != (Bitmap *)0x0) {
        //   Bitmap::unregisterBitmap(this->handleImage);
        // }
        // Control::destroy((Control *)this);
        // return;
    }

    // STUB: DELAYLAMA 0x10009c50
    void VerticalSlider::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        // Bitmap *pBVar1;
        // OffscreenGDIDrawingContext *drawingContext_00;
        // long lVar2;
        // void *unaff_ESI;
        // HWND unaff_EDI;
        // OffscreenGDIDrawingContextVTable *in_stack_ffffffa4;
        // int in_stack_ffffffa8;
        // Window *in_stack_ffffffac;
        // OffscreenGDIDrawingContext *in_stack_ffffffb0;
        // int srcX;
        // HDC srcY;
        // OffscreenGDIDrawingContext *local_34;
        // LONG LStack_30;
        // RECT local_2c;
        // Color local_1c;
        // char local_18 [4];
        // LONG local_14;
        // void *local_10;
        // void *pvStack_c;
        // undefined1 *puStack_8;
        // HGDIOBJ local_4;
        //
        // local_4 = (HGDIOBJ)0xffffffff;
        // puStack_8 = &LAB_1000ae0b;
        // pvStack_c = ExceptionList;
        // ExceptionList = &pvStack_c;
        // local_34 = operator_new(0x8c);
        // local_4 = (HGDIOBJ)0x0;
        // if (local_34 == (OffscreenGDIDrawingContext *)0x0) {
        //   drawingContext_00 = (OffscreenGDIDrawingContext *)0x0;
        // }
        // else {
        //   in_stack_ffffffac = (this->control).viewMembers.parent;
        //   in_stack_ffffffb0 = (OffscreenGDIDrawingContext *)this->trackWidth;
        //   in_stack_ffffffa8 = 0x10009cc4;
        //   drawingContext_00 =
        //        OffscreenGDIDrawingContext::offscreenGDIDrawingContext
        //                  (local_34,in_stack_ffffffac,(int)in_stack_ffffffb0,this->trackHeight,
        //                   DAT_BACK_COLOR);
        // }
        // pBVar1 = (this->control).bitmap;
        // local_14 = this->trackWidth;
        // local_10 = (void *)this->trackHeight;
        // local_4 = (HGDIOBJ)0xffffffff;
        // local_1c.rgba = 0;
        // local_18[0] = '\0';
        // local_18[1] = '\0';
        // local_18[2] = '\0';
        // local_18[3] = '\0';
        // if (pBVar1 != (Bitmap *)0x0) {
        //   if ((this->control).viewMembers.useAlphaBlending == false) {
        //     in_stack_ffffffac = (Window *)0x10009d12;
        //     in_stack_ffffffb0 = drawingContext_00;
        //     Bitmap::blit(pBVar1,(GDIDrawingContext *)drawingContext_00,(RECT *)&local_1c.bytes,
        //                  &this->backgroundOffset);
        //   }
        //   else {
        //     in_stack_ffffffac = (Window *)0x10009d0b;
        //     in_stack_ffffffb0 = drawingContext_00;
        //     Bitmap::drawMasked(pBVar1,(GDIDrawingContext *)drawingContext_00,(RECT *)&local_1c.bytes,
        //                        &this->backgroundOffset);
        //   }
        // }
        // local_2c.left = (this->handlePos).x;
        // local_2c.right = this->handleWidth + local_2c.left;
        // local_34 = (OffscreenGDIDrawingContext *)(this->trackMaxY - this->trackMin);
        // lVar2 = _ftol((double)CONCAT44(unaff_ESI,unaff_EDI));
        // local_2c.top = lVar2 + (this->handlePos).y;
        // if (local_2c.top < this->handleMinPos) {
        //   local_2c.top = this->handleMinPos;
        // }
        // local_2c.bottom = this->handleHeight + local_2c.top;
        // if (this->handleMaxPos < local_2c.bottom) {
        //   local_2c.bottom = this->handleMaxPos;
        // }
        // pBVar1 = this->handleImage;
        // if (pBVar1 != (Bitmap *)0x0) {
        //   if (this->isHandleTransparent == false) {
        //     local_34 = (OffscreenGDIDrawingContext *)0x0;
        //     LStack_30 = 0;
        //     in_stack_ffffffac = (Window *)0x10009dac;
        //     in_stack_ffffffb0 = drawingContext_00;
        //     Bitmap::blit(pBVar1,(GDIDrawingContext *)drawingContext_00,&local_2c,(POINT *)&local_34);
        //   }
        //   else {
        //     local_34 = (OffscreenGDIDrawingContext *)0x0;
        //     LStack_30 = 0;
        //     in_stack_ffffffac = (Window *)0x10009d92;
        //     in_stack_ffffffb0 = drawingContext_00;
        //     Bitmap::drawMasked(pBVar1,(GDIDrawingContext *)drawingContext_00,&local_2c,(POINT *)&local_34)
        //     ;
        //   }
        // }
        // srcY = (HDC)0x0;
        // srcX = 0;
        // local_34 = (OffscreenGDIDrawingContext *)&stack0xffffffa4;
        // copyRect(&stack0xffffffa4,&(this->control).viewMembers.rect);
        // GDIDrawingContext::copyToScreen
        //           ((GDIDrawingContext *)drawingContext_00,drawingContext,(int)in_stack_ffffffa4,
        //            in_stack_ffffffa8,(int)in_stack_ffffffac,(int)in_stack_ffffffb0,srcX,(int)srcY);
        // if (drawingContext_00 != (OffscreenGDIDrawingContext *)0x0) {
        //   (*drawingContext_00->vtable->destructor)();
        // }
        // this->trackTopY = (this->control).viewMembers.rect.top + local_2c.top;
        // (*(this->vtable->control).view.setDirty)();
        // ExceptionList = local_10;
        // return;
    }

    // STUB: DELAYLAMA 0x10009e10
    void VerticalSlider::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* relativeMousePos) {
        // byte inputMask;
        // bool isDirtyResult;
        // bool isDirty;
        // undefined3 _padding1;
        // uint currentModifiers;
        // undefined3 _padding2;
        // float10 defaultValue;
        // int verticalAnchor;
        // uint loopModifiers;
        // undefined3 _padding3;
        // float calculatedValue;
        // POINT *pMousePos;
        // int trackMaxY;
        // int trackMinY;
        // int trackTopY;
        // VerticalSliderVTable *vtable;
        //
        // pMousePos = relativeMousePos;
        // if ((this->control).viewMembers.isEnabled != false) {
        //   inputMask = View::GetPressedModifiersAndMouseButtons();
        //   loopModifiers = CONCAT31(_padding1,inputMask);
        //                   /* Check for Ctrl (0x10) + Left Click (0x01) = 0x11 */
        //   if (loopModifiers == 0x11) {
        //     defaultValue = (float10)(*(this->vtable->control).getDefaultValue)();
        //     vtable = this->vtable;
        //     (this->control).value = (float)defaultValue;
        //     isDirtyResult = (bool)(*(vtable->control).view.isDirty)();
        //     if (isDirtyResult != false) {
        //       (*(code *)**(undefined4 **)(this->control).callback)(drawingContext,this);
        //       return;
        //     }
        //   }
        //   else {
        //                   /* Check for Left Click (0x01) */
        //     if ((inputMask & 1) != 0) {
        //       trackMinY = this->trackMin;
        //       if (this->snapToMouse == false) {
        //         trackMaxY = (this->control).viewMembers.rect.left;
        //         trackTopY = this->trackTopY;
        //         if (relativeMousePos->x < trackMaxY) {
        //           return;
        //         }
        //         if (this->handleWidth + trackMaxY < relativeMousePos->x) {
        //           return;
        //         }
        //         verticalAnchor = relativeMousePos->y;
        //         if (verticalAnchor < trackTopY) {
        //           return;
        //         }
        //         if (this->handleHeight + trackTopY < verticalAnchor) {
        //           return;
        //         }
        //         verticalAnchor = verticalAnchor - trackTopY;
        //       }
        //       else {
        //         verticalAnchor = this->handleHeight / 2 + -1;
        //       }
        //       verticalAnchor = trackMinY + verticalAnchor;
        //       trackMaxY = this->trackMaxY;
        //       relativeMousePos = (POINT *)(this->control).value;
        //       Window::beginEdit((this->control).viewMembers.parent,(this->control).parameterId);
        //       inputMask = View::GetPressedModifiersAndMouseButtons();
        //       currentModifiers = CONCAT31(_padding3,inputMask);
        //       while ((inputMask & 1) != 0) {
        //         if ((loopModifiers == currentModifiers) || ((currentModifiers & 8) == 0)) {
        //           if ((currentModifiers & 8) == 0) {
        //             relativeMousePos = (POINT *)(this->control).value;
        //           }
        //         }
        //         else {
        //           relativeMousePos = (POINT *)(this->control).value;
        //           loopModifiers = currentModifiers;
        //         }
        //         inputMask = *(byte *)&this->flags;
        //         calculatedValue = (float)(pMousePos->y - verticalAnchor) / (float)(trackMaxY - trackMinY);
        //         (this->control).value = calculatedValue;
        //         if ((inputMask & 0x40) != 0) {
        //           (this->control).value = FLOAT_1000b8fc - calculatedValue;
        //         }
        //         if ((currentModifiers & 8) != 0) {
        //           (this->control).value =
        //                ((this->control).value - (float)relativeMousePos) / (float)this->fineTuneDivider +
        //                (float)relativeMousePos;
        //         }
        //         (*(this->vtable->control).clampValue)();
        //         isDirty = (bool)(*(this->vtable->control).view.isDirty)();
        //         if (isDirty != false) {
        //           (*(code *)**(undefined4 **)(this->control).callback)(drawingContext,this);
        //         }
        //         GDIDrawingContext::getRelativeMousePos(drawingContext,pMousePos);
        //         (*(this->vtable->control).onIdle)();
        //         inputMask = View::GetPressedModifiersAndMouseButtons();
        //         currentModifiers = CONCAT31(_padding2,inputMask);
        //       }
        //       Window::endEdit((this->control).viewMembers.parent,(this->control).parameterId);
        //     }
        //   }
        // }
        // return;
    }
}
}
}