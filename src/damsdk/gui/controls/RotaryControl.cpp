#pragma once
#include <cmath>
#include "RotaryControl.h"

namespace DamSDK {
namespace Gui {
namespace Controls {
    // STUB: DELAYLAMA 0x10008e40
    RotaryControl::RotaryControl(RECT* pRect, callbackCallback callback, int parameterId, Platform::Windows::Bitmap* bmp1, Platform::Windows::Bitmap* bmp2, POINT* srcPoint) : Control(pRect, callback, parameterId, bmp1) {
        // int iVar1;
        // int iVar2;
        // undefined4 uVar3;
        // float fVar4;
        // undefined4 unaff_ESI;
        // undefined4 unaff_EDI;
        // LONG y;
        //
        // Control::Control((Control *)this,pRect,callback,parameterId,bmp1);
        // this->srcPoint = srcPoint->x;
        // y = srcPoint->y;
        // this->bmp = bmp2;
        // this->y = y;
        // this->vtable = &RotaryControlVTable_1000bd78;
        // if (bmp2 == (Bitmap *)0x0) {
        //   this->knobRadius = 4.2039e-45;
        // }
        // else {
        //   View::useBitmap(bmp2);
        //   fVar4 = (float)_ftol((double)CONCAT44(unaff_ESI,unaff_EDI));
        //   this->knobRadius = fVar4;
        // }
        // uVar3 = DAT_1000d854;
        // this->indicatorShadowColor.bytes.r = (byte)DAT_1000d854;
        // this->indicatorShadowColor.bytes.g = (byte)((uint)uVar3 >> 8);
        // this->indicatorShadowColor.bytes.b = (byte)((uint)uVar3 >> 0x10);
        // this->indicatorShadowColor.bytes.a = (byte)((uint)uVar3 >> 0x18);
        // uVar3 = DAT_FOREGROUND_COLOR;
        // this->indicatorHighlightColor.bytes.r = (byte)DAT_FOREGROUND_COLOR;
        // this->indicatorHighlightColor.bytes.g = (byte)((uint)uVar3 >> 8);
        // this->indicatorHighlightColor.bytes.b = (byte)((uint)uVar3 >> 0x10);
        // this->indicatorHighlightColor.bytes.a = (byte)((uint)uVar3 >> 0x18);
        // iVar1 = pRect->right;
        // iVar2 = pRect->left;
        // this->totalRange = 1.0;
        // this->center = (float)(iVar1 - iVar2) * FLOAT_1000bb34;
        // setStartAngle(this,3.9269907);
        // setTotalRange(this,0xc096cbe4);
        // this->fineTuneDivider = 1.5;
        // return this;
    }

    // STUB: DELAYLAMA 0x10008f60
    RotaryControl::~RotaryControl() {
        // destroy(this);
        // if (deleteObject) {
        //   operator_delete(this);
        // }
        // return this;
    }

    // STUB: DELAYLAMA 0x10008f80
    void RotaryControl::destroy() {
        // Bitmap *bmp;
        //
        // this->vtable = &RotaryControlVTable_1000bd78;
        // bmp = this->bmp;
        // if (bmp != (Bitmap *)0x0) {
        //   Bitmap::unregisterBitmap(bmp);
        // }
        // Control::destroy((Control *)this);
        // return;
    }

    // STUB: DELAYLAMA 0x10008fe0
    void RotaryControl::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        // RECT *destRect;
        // undefined4 *srcPoint;
        // Bitmap *bitmap;
        //
        // bitmap = this->control.bitmap;
        // if (bitmap != (Bitmap *)0x0) {
        //   destRect = &this->control.viewMembers.rect;
        //   srcPoint = &this->srcPoint;
        //   if (this->control.viewMembers.useAlphaBlending == false) {
        //     Bitmap::blit(bitmap,drawingContext,destRect,(POINT *)srcPoint);
        //   }
        //   else {
        //     Bitmap::drawMasked(bitmap,drawingContext,destRect,(POINT *)srcPoint);
        //   }
        // }
        // (*this->vtable->drawIndicator)(drawingContext);
        // (*(this->vtable->control).view.setDirty)(0);
        // return;
    }

    // STUB: DELAYLAMA 0x10009030
    void RotaryControl::drawIndicator(Platform::Windows::GDIDrawingContext* drawingContext) {
        // int centerX;
        // int centerY;
        // POINT sourceOffset;
        // RECT destRect;
        // GDIDrawingContext *drawContext;
        // Bitmap *knobBitmap;
        //
        // centerY = 0;
        // sourceOffset.x = 0;
        // (*this->vtable->calculateXYFromValue)(&centerY);
        // knobBitmap = this->bmp;
        //                   /* BITMAP RENDERING */
        // if (knobBitmap != (Bitmap *)0x0) {
        //   destRect.left =
        //        centerX + (this->control.viewMembers.rect.left - (int)knobBitmap->width / 2);
        //   centerY = centerY + (this->control.viewMembers.rect.top - (int)knobBitmap->height / 2)
        //   ;
        //   destRect.bottom = centerY + knobBitmap->height;
        //   destRect.right = knobBitmap->width + destRect.left;
        //   sourceOffset.x = 0;
        //   sourceOffset.y = 0;
        //   destRect.top = centerY;
        //   Bitmap::drawMasked(knobBitmap,drawContext,&destRect,&sourceOffset);
        //   return;
        // }
        //                   /* VECTOR RENDERING (Fallback) */
        // sourceOffset.y = this->control.viewMembers.rect.top;
        // sourceOffset.x =
        //      (this->control.viewMembers.rect.right -
        //      this->control.viewMembers.rect.left) / 2 + -1 +
        //      this->control.viewMembers.rect.left;
        // centerY = centerY + sourceOffset.y;
        // sourceOffset.y =
        //      (this->control.viewMembers.rect.bottom - sourceOffset.y) / 2 + sourceOffset.y;
        // GDIDrawingContext::setPenColor(drawContext,this->indicatorShadowColor);
        // GDIDrawingContext::moveToEx(drawContext,(POINT *)&stack0xffffffdc);
        // GDIDrawingContext::lineTo(drawContext,&sourceOffset);
        // centerY = centerY + -1;
        // sourceOffset.x = sourceOffset.x + 1;
        // sourceOffset.y = sourceOffset.y + -1;
        // GDIDrawingContext::setPenColor(drawContext,this->indicatorHighlightColor);
        // GDIDrawingContext::moveToEx(drawContext,(POINT *)&stack0xffffffdc);
        // GDIDrawingContext::lineTo(drawContext,&sourceOffset);
        // return;
    }

    // STUB: DELAYLAMA 0x10009190
    void RotaryControl::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos) {
        // float fVar1;
        // bool isDirty1;
        // bool isDirty2;
        // undefined3 _padding1;
        // undefined3 _padding2;
        // byte inputMask;
        // int lastMouseX;
        // float10 defaultVal;
        // float stepSize;
        // float defaultValue;
        // float valueRange;
        // float relativeY;
        // float lastValue;
        // int currentModifiers;
        // float halfRange;
        // LONG startMouseX;
        // LONG startMouseY;
        // bool isLinearMode;
        // int mouseX;
        // int rectTop;
        // RotaryControlVTable *vtable;
        //
        // if (this->control.viewMembers.isEnabled == false) {
        //   return;
        // }
        // inputMask = View::GetPressedModifiersAndMouseButtons();
        // currentModifiers = CONCAT31(_padding1,inputMask);
        //                   /* Exit if Left Click is not down */
        // if ((inputMask & 1) == 0) {
        //   return;
        // }
        //                   /* CTRL + Click: Reset to Default */
        // if (currentModifiers == 0x11) {
        //   defaultVal = (float10)(*(this->vtable->control).getDefaultValue)();
        //   vtable = this->vtable;
        //   this->control.value = (float)defaultVal;
        //   isDirty1 = (bool)(*(vtable->control).view.isDirty)();
        //   if (isDirty1 == false) {
        //     return;
        //   }
        //   (*(code *)**(undefined4 **)this->control.callback)(drawingContext,this);
        //   return;
        // }
        // valueRange = this->control.max - this->control.min;
        // defaultValue = this->control.prevValue;
        // lastValue = this->control.value;
        // startMouseX = 0;
        // startMouseY = 0;
        // halfRange = valueRange * FLOAT_1000bb34;
        // isLinearMode = false;
        // stepSize = valueRange * _DAT_1000be4c;
        // if (GLOBAL_KNOB_MODE == 2) {
        //                   /* If Alt is held, use Radial/Angular mode */
        //   if ((inputMask & 0x20) != 0) {
        // LAB_RADIAL_MODE:
        //     valueRange = (float)(mousePos->x - this->control.viewMembers.rect.left);
        //     relativeY = (float)(mousePos->y - this->control.viewMembers.rect.top);
        //     defaultVal = (float10)(*this->vtable->calculateAngleFromPoint)(&valueRange);
        //     defaultValue = (float)defaultVal;
        //     goto LAB_START_DRAG_LOOP;
        //   }
        // }
        // else if ((inputMask & 0x20) == 0) goto LAB_RADIAL_MODE;
        // stepSize = FLOAT_1000b8f8;
        //                   /* Linear Mode Setup */
        // if ((inputMask & 8) != 0) {
        //                   /* Shift-key for fine-tuning */
        //   stepSize = this->fineTuneDivider * FLOAT_1000b8f8;
        // }
        // stepSize = valueRange / stepSize;
        // isLinearMode = true;
        // startMouseX = mousePos->x;
        // startMouseY = mousePos->y;
        // LAB_START_DRAG_LOOP:
        // lastMouseX = -1;
        // relativeY = -NAN;
        // Window::beginEdit(this->control.viewMembers.parent,this->control.parameterId);
        // do {
        //   inputMask = View::GetPressedModifiersAndMouseButtons();
        //   mouseX = mousePos->x;
        //   if ((mouseX != lastMouseX) || ((float)mousePos->y != relativeY)) {
        //     relativeY = (float)mousePos->y;
        //     lastMouseX = mousePos->x;
        //     if (isLinearMode) {
        //       mouseX = ((mouseX - mousePos->y) - startMouseX) + startMouseY;
        //       if (CONCAT31(_padding2,inputMask) != currentModifiers) {
        //         fVar1 = FLOAT_1000b8f8;
        //         if ((inputMask & 8) != 0) {
        //           fVar1 = this->fineTuneDivider * FLOAT_1000b8f8;
        //         }
        //         fVar1 = (this->control.max - this->control.min) / fVar1;
        //         lastValue = (stepSize - fVar1) * (float)mouseX + lastValue;
        //         stepSize = fVar1;
        //         currentModifiers = CONCAT31(_padding2,inputMask);
        //       }
        //       vtable = this->vtable;
        //       this->control.value = (float)mouseX * stepSize + lastValue;
        //       (*(vtable->control).clampValue)();
        //     }
        //     else {
        //       rectTop = this->control.viewMembers.rect.top;
        //       mousePos->x = mouseX - this->control.viewMembers.rect.left;
        //       mousePos->y = mousePos->y - rectTop;
        //       defaultVal = (float10)(*this->vtable->calculateAngleFromPoint)(mousePos);
        //       this->control.value = (float)defaultVal;
        //       if ((float10)defaultValue - defaultVal <= (float10)halfRange) {
        //         if (defaultVal - (float10)defaultValue <= (float10)halfRange) {
        //           defaultValue = (float)defaultVal;
        //         }
        //         else {
        //           this->control.value = this->control.min;
        //         }
        //       }
        //       else {
        //         this->control.value = this->control.max;
        //       }
        //     }
        //     isDirty2 = (bool)(*(this->vtable->control).view.isDirty)();
        //     if (isDirty2 != false) {
        //       (*(code *)**(undefined4 **)this->control.callback)(drawingContext,this);
        //     }
        //   }
        //   GDIDrawingContext::getRelativeMousePos(drawingContext,mousePos);
        //   (*(this->vtable->control).onIdle)();
        // } while ((inputMask & 1) != 0);
        // Window::endEdit(this->control.viewMembers.parent,this->control.parameterId);
        // return;
    }

    // STUB: DELAYLAMA 0x10009470
    void RotaryControl::setStartAngle(float startAngle) {
        this->startAngle = startAngle;
        updateMathConstants();
    }

    // STUB: DELAYLAMA 0x10009480
    void RotaryControl::setTotalRange(float totalRange) {
        this->totalRange = totalRange;
        updateMathConstants();
    }

    // STUB: DELAYLAMA 0x10009490
    void RotaryControl::updateMathConstants() {
        float valueWidth = (this->max - this->min) / this->totalRange;
        this->angleRange = valueWidth;
        this->angleOffset = this->min - valueWidth * this->startAngle;
        this->deadZoneSize = (6.2831855f - abs(this->totalRange)) * 0.5f;
        this->setDirty(true);
    }

    // STUB: DELAYLAMA 0x100094d0
    void RotaryControl::calculateXYFromValue(POINT* outPoint) {
        // long normalizedValue;
        // undefined4 unaff_ESI;
        // undefined4 unaff_EDI;
        // unkbyte10 extraout_ST0;
        //
        // fcos(((float10)this->control.value - (float10)(float)this->angleOffset) /
        //      (float10)(float)this->angleRange);
        // normalizedValue = _ftol((double)CONCAT44(unaff_ESI,unaff_EDI));
        // fsin(extraout_ST0);
        // outPoint->x = normalizedValue;
        // normalizedValue = _ftol((double)CONCAT44(unaff_ESI,unaff_EDI));
        // outPoint->y = normalizedValue;
        // return;
    }

    // STUB: DELAYLAMA 0x10009530
    float RotaryControl::calculateAngleFromPoint(POINT* point) {
        // float10 normalizedAngle;
        // float10 rawAngle;
        // float rangePlusDeadzone;
        //
        // rawAngle = (float10)fpatan((float10)this->center - (float10)point->y,
        //                            (float10)point->x - (float10)this->center);
        // if (rawAngle < (float10)FLOAT_1000b208) {
        //   rawAngle = rawAngle + (float10)_DAT_1000be54;
        // }
        // rawAngle = rawAngle - (float10)this->startAngle;
        // if (FLOAT_1000b208 <= this->totalRange) {
        //   if ((float10)FLOAT_1000b208 <= rawAngle) {
        //     normalizedAngle = rawAngle;
        //     if ((float10)_DAT_1000be58 < rawAngle) {
        //       normalizedAngle = rawAngle - (float10)_DAT_1000be54;
        //     }
        //   }
        //   else {
        //     normalizedAngle = rawAngle + (float10)_DAT_1000be54;
        //   }
        //   rangePlusDeadzone = this->totalRange + (float)this->deadZoneSize;
        //   if ((float10)rangePlusDeadzone < normalizedAngle) {
        //     return (float10)this->control.min;
        //   }
        //   if ((float10)this->totalRange < normalizedAngle) {
        //     return (float10)this->control.max;
        //   }
        //   if ((float10)rangePlusDeadzone < rawAngle) {
        //     return (rawAngle - (float10)_DAT_1000be54) * (float10)(float)this->angleRange +
        //            (float10)this->control.min;
        //   }
        //   if (rawAngle < -(float10)(float)this->deadZoneSize) {
        //     rawAngle = rawAngle + (float10)_DAT_1000be54;
        //   }
        //   return rawAngle * (float10)(float)this->angleRange +
        //          (float10)this->control.min;
        // }
        // rawAngle = rawAngle - (float10)this->totalRange;
        // if ((float10)FLOAT_1000b208 <= rawAngle) {
        //   normalizedAngle = rawAngle;
        //   if ((float10)_DAT_1000be58 < rawAngle) {
        //     normalizedAngle = rawAngle - (float10)_DAT_1000be54;
        //   }
        // }
        // else {
        //   normalizedAngle = rawAngle + (float10)_DAT_1000be54;
        // }
        // rangePlusDeadzone = (float)this->deadZoneSize - this->totalRange;
        // if ((float10)rangePlusDeadzone < normalizedAngle) {
        //   return (float10)this->control.max;
        // }
        // if (-(float10)this->totalRange < normalizedAngle) {
        //   return (float10)this->control.min;
        // }
        // if ((float10)rangePlusDeadzone < rawAngle) {
        //   return (rawAngle - (float10)_DAT_1000be54) * (float10)(float)this->angleRange +
        //          (float10)this->control.max;
        // }
        // if (rawAngle < -(float10)(float)this->deadZoneSize) {
        //   rawAngle = rawAngle + (float10)_DAT_1000be54;
        // }
        // return rawAngle * (float10)(float)this->angleRange +
        //        (float10)this->control.max;
        return 0.0f;
    }

    // STUB: DELAYLAMA 0x100096c0
    void RotaryControl::setIndicatorShadowColor(Api::ColorRGBA color) {
        this->indicatorShadowColor.bytes.r = color.bytes.r;
        this->indicatorShadowColor.bytes.g = color.bytes.g;
        this->indicatorShadowColor.bytes.b = color.bytes.b;
        this->indicatorShadowColor.bytes.a = color.bytes.a;
        this->setDirty(true);
    }

    // STUB: DELAYLAMA 0x100096f0
    void RotaryControl::setIndicatorHighlightColor(Api::ColorRGBA color) {
        this->indicatorHighlightColor.bytes.r = color.bytes.r;
        this->indicatorHighlightColor.bytes.g = color.bytes.g;
        this->indicatorHighlightColor.bytes.b = color.bytes.b;
        this->indicatorHighlightColor.bytes.a = color.bytes.a;
        this->setDirty(true);
    }

    // STUB: DELAYLAMA 0x10009720
    void RotaryControl::setBitmap(Platform::Windows::Bitmap* bmp) {
        // float knobRadius;
        // undefined4 unaff_ESI;
        // undefined4 unaff_retaddr;
        // Bitmap *oldBitmap;
        //
        // oldBitmap = this->bmp;
        // if (oldBitmap != (Bitmap *)0x0) {
        //   Bitmap::unregisterBitmap(oldBitmap);
        //   this->bmp = (Bitmap *)0x0;
        // }
        // if (bmp != (Bitmap *)0x0) {
        //   this->bmp = bmp;
        //   View::useBitmap(bmp);
        //   knobRadius = (float)_ftol((double)CONCAT44(unaff_retaddr,unaff_ESI));
        //   this->knobRadius = knobRadius;
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10009770
    float RotaryControl::getStartAngle() {
        return this->startAngle;
    }

    // STUB: DELAYLAMA 0x10009780
    float RotaryControl::getTotalRange() {
        return this->totalRange;
    }

    // STUB: DELAYLAMA 0x10009790
    void RotaryControl::setKnobRadius(float radius) {
        this->knobRadius = radius;
    }

    // STUB: DELAYLAMA 0x100097a0
    void RotaryControl::setFineTuneDivider(float divider) {
        this->fineTuneDivider = divider;
    }

    // STUB: DELAYLAMA 0x100097b0
    float RotaryControl::getFineTuneDivider() {
        return this->fineTuneDivider;
    }
}
}
}