#pragma once
#include <cmath>
#include "RotaryControl.h"
#include "damsdk/api/EditorBase.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DamSDK {
namespace Gui {
namespace Controls {
    // FUNCTION: DELAYLAMA 0x10008e40
    RotaryControl::RotaryControl(RECT* pRect, callbackCallback callback, int parameterId, Platform::Windows::Bitmap* bmp1, Platform::Windows::Bitmap* bmp2, POINT* srcPoint) : Control(pRect, callback, parameterId, bmp1) {
        this->srcPoint.x = srcPoint->x;
        LONG y = srcPoint->y;
        this->bmp = bmp2;
        this->srcPoint.y = y;
        if (bmp2 == nullptr) {
            this->knobRadius = 3;
        }
        else {
            View::useBitmap(bmp2);
            int result = static_cast<int>(bmp2->width * 0.5f + 2.5f);
            this->knobRadius = result;
        }

        COLORREF grayColor = Platform::Windows::DAT_GRAY_COLOR;
        this->indicatorShadowColor.bytes.r = (byte)grayColor;
        this->indicatorShadowColor.bytes.g = (byte)(grayColor >> 8);
        this->indicatorShadowColor.bytes.b = (byte)(grayColor >> 0x10);
        this->indicatorShadowColor.bytes.a = (byte)(grayColor >> 0x18);

        COLORREF backgroundColor = Platform::Windows::DAT_FOREGROUND_COLOR;
        this->indicatorHighlightColor.bytes.r = (byte)backgroundColor;
        this->indicatorHighlightColor.bytes.g = (byte)(backgroundColor >> 8);
        this->indicatorHighlightColor.bytes.b = (byte)(backgroundColor >> 0x10);
        this->indicatorHighlightColor.bytes.a = (byte)(backgroundColor >> 0x18);

        int iVar1 = pRect->right;
        int iVar2 = pRect->left;

        this->totalRange = 1.0f;
        this->center = (float)(iVar1 - iVar2) * 0.5f;
        setStartAngle(3.9269907f);
        setTotalRange(-4.712389f);
        this->fineTuneDivider = 1.5;
    }

    // FUNCTION: DELAYLAMA 0x10008f60
    RotaryControl::~RotaryControl() {
        this->destroy();
    }

    // FUNCTION: DELAYLAMA 0x10008f80
    void RotaryControl::destroy() {
        Platform::Windows::Bitmap* bmp = this->bmp;
        if (bmp != nullptr) {
            Platform::Windows::Bitmap::unregisterBitmap(bmp);
        }
        Control::destroy();
    }

    // FUNCTION: DELAYLAMA 0x10008fe0
    void RotaryControl::onDraw(Platform::Windows::GDIDrawingContext *drawingContext)
    {
        Platform::Windows::Bitmap *bitmap = this->bitmap;
        if (bitmap != nullptr)
        {
            RECT *destRect = &this->rect;
            POINT *srcPoint = &this->srcPoint;
            if (this->useAlphaBlending == false)
            {
                bitmap->blit(drawingContext, destRect, srcPoint);
            }
            else
            {
                bitmap->drawMasked(drawingContext, destRect, srcPoint);
            }
        }
        this->drawIndicator(drawingContext);
        this->setDirty(false);
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
        //                   // BITMAP RENDERING
        // if (knobBitmap != (Bitmap *)0x0) {
        //   destRect.left =
        //        centerX + (this->rect.left - (int)knobBitmap->width / 2);
        //   centerY = centerY + (this->rect.top - (int)knobBitmap->height / 2)
        //   ;
        //   destRect.bottom = centerY + knobBitmap->height;
        //   destRect.right = knobBitmap->width + destRect.left;
        //   sourceOffset.x = 0;
        //   sourceOffset.y = 0;
        //   destRect.top = centerY;
        //   Bitmap::drawMasked(knobBitmap,drawContext,&destRect,&sourceOffset);
        //   return;
        // }
        //                   // VECTOR RENDERING (Fallback)
        // sourceOffset.y = this->rect.top;
        // sourceOffset.x =
        //      (this->rect.right -
        //      this->rect.left) / 2 + -1 +
        //      this->rect.left;
        // centerY = centerY + sourceOffset.y;
        // sourceOffset.y =
        //      (this->rect.bottom - sourceOffset.y) / 2 + sourceOffset.y;
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

    // FUNCTION: DELAYLAMA 0x10009190
    void RotaryControl::onMouseDown(Platform::Windows::GDIDrawingContext *drawingContext, POINT *mousePos)
    {
        float halfRange;
        bool isDirty;
        uint32_t currentModifiers;
        int prevMouseX;
        int prevMouseY;
        float defaultParamValue;
        float calculatedAngle;
        float valuePerPixel;
        float paramRange;
        float previousAngle;
        float baseParamValue;
        uint32_t prevModifiers;
        POINT dragStartMouse;
        bool isLinearMode;
        int currentMouseX;
        int currentMouseY;
        int linearDelta;
        int boundsTop;
        float sensitivityPixels;
        POINT relativeMousePos;

        if (this->isEnabled == false)
        {
            return;
        }

        prevModifiers = View::GetPressedModifiersAndMouseButtons();

        // Exit if Left Click is not down
        if ((prevModifiers & 1) == 0)
        {
            return;
        }

        // CTRL + Click: Reset to Default
        if (prevModifiers == 0x11)
        {
            defaultParamValue = this->getDefaultValue();
            this->value = (float)defaultParamValue;
            isDirty = this->isDirty();
            if (isDirty == false)
            {
                return;
            }
            this->callback(drawingContext, this);
            return;
        }

        paramRange = (float)(this->max - this->min);
        previousAngle = this->prevValue;
        baseParamValue = this->value;
        dragStartMouse.x = 0;
        dragStartMouse.y = 0;
        halfRange = paramRange * 0.5f;
        isLinearMode = false;
        valuePerPixel = paramRange * 0.005f;
        sensitivityPixels = 200.0f;

        if (Api::GLOBAL_KNOB_MODE == 2)
        {
            // If Alt is held, use Radial/Angular mode
            if ((prevModifiers & 0x20) != 0)
            {
            LAB_RADIAL_MODE:
                relativeMousePos.x = mousePos->x - this->rect.left;
                relativeMousePos.y = mousePos->y - this->rect.top;
                calculatedAngle = this->calculateAngleFromPoint(&relativeMousePos);
                previousAngle = (float)calculatedAngle;
                goto LAB_START_DRAG_LOOP;
            }
        }
        else if ((prevModifiers & 0x20) == 0)
            goto LAB_RADIAL_MODE;

        // Linear Mode Setup
        if ((prevModifiers & 8) != 0)
        {
            // Shift-key for fine-tuning
            sensitivityPixels = this->fineTuneDivider * 200.0f;
        }
        valuePerPixel = paramRange / sensitivityPixels;
        isLinearMode = true;
        dragStartMouse.x = mousePos->x;
        dragStartMouse.y = mousePos->y;

    LAB_START_DRAG_LOOP:
        prevMouseX = -1;
        prevMouseY = -1;
        this->parent->beginEdit(this->parameterId);
        do
        {
            currentModifiers = View::GetPressedModifiersAndMouseButtons();
            currentMouseX = mousePos->x;
            currentMouseY = mousePos->y;

            if ((currentMouseX != prevMouseX) || (currentMouseY != prevMouseY))
            {
                prevMouseY = currentMouseY;
                prevMouseX = currentMouseX;

                if (isLinearMode)
                {
                    linearDelta = ((currentMouseX - currentMouseY) - dragStartMouse.x) + dragStartMouse.y;
                    if (currentModifiers != prevModifiers)
                    {
                        sensitivityPixels = 200.0f;
                        if ((currentModifiers & 8) != 0)
                        {
                            sensitivityPixels = this->fineTuneDivider * 200.0f;
                        }
                        sensitivityPixels = (this->max - this->min) / sensitivityPixels;
                        baseParamValue = (valuePerPixel - sensitivityPixels) * (float)linearDelta + baseParamValue;
                        valuePerPixel = sensitivityPixels;
                        prevModifiers = currentModifiers;
                    }
                    this->value = (float)linearDelta * valuePerPixel + baseParamValue;
                    this->clampValue();
                }
                else
                {
                    boundsTop = this->rect.top;
                    relativeMousePos.x = currentMouseX - this->rect.left;
                    relativeMousePos.y = currentMouseY - boundsTop;
                    calculatedAngle = this->calculateAngleFromPoint(&relativeMousePos);
                    this->value = (float)calculatedAngle;

                    // Wrap-around logic bounds checking
                    if (previousAngle - calculatedAngle <= halfRange)
                    {
                        if (calculatedAngle - previousAngle <= halfRange)
                        {
                            previousAngle = (float)calculatedAngle;
                        }
                        else
                        {
                            this->value = this->min;
                        }
                    }
                    else
                    {
                        this->value = this->max;
                    }
                }

                isDirty = this->isDirty();
                if (isDirty != false)
                {
                    this->callback(drawingContext, this);
                }
            }
            drawingContext->getRelativeMousePos(mousePos);
            this->onIdle();
        } while ((currentModifiers & 1) != 0);

        this->parent->endEdit(this->parameterId);
        return;
    }

    // FUNCTION: DELAYLAMA 0x10009470
    void RotaryControl::setStartAngle(float startAngle) {
        this->startAngle = startAngle;
        updateMathConstants();
    }

    // FUNCTION: DELAYLAMA 0x10009480
    void RotaryControl::setTotalRange(float totalRange) {
        this->totalRange = totalRange;
        updateMathConstants();
    }

    // FUNCTION: DELAYLAMA 0x10009490
    void RotaryControl::updateMathConstants() {
        float valueWidth = (this->max - this->min) / this->totalRange;
        this->angleRange = valueWidth;
        this->angleOffset = this->min - valueWidth * this->startAngle;
        this->deadZoneSize = (6.2831855f - abs((float)this->totalRange)) * 0.5f;
        this->setDirty(true);
    }

    // FUNCTION: DELAYLAMA 0x100094d0
    void RotaryControl::calculateXYFromValue(POINT* outPoint) {
        float angle = (this->value - this->angleOffset) / this->angleRange;

        float cosAngle = cos(angle);
        float sinAngle = sin(angle);

        int radius = static_cast<int>(this->knobRadius);

        // X coordinate: center - radius * cos(angle) + 0.5f
        float tempX = (this->center - static_cast<float>(radius) * cosAngle) + 0.5f;
        outPoint->x = static_cast<LONG>(tempX);

        // Y coordinate: center - radius * sin(angle) + 0.5f
        float tempY = (this->center - static_cast<float>(radius) * sinAngle) + 0.5f;
        outPoint->y = static_cast<LONG>(tempY);
    }

    // FUNCTION: DELAYLAMA 0x10009530
    float RotaryControl::calculateAngleFromPoint(POINT* point) {

        float rawAngle = atan2(this->center - point->y, point->x - this->center);
        if (rawAngle < 0.0f) {
            rawAngle = rawAngle + 6.2831855f;
        }
        
        rawAngle = rawAngle - this->startAngle;
        float normalizedAngle;
        float rangePlusDeadzone;
        if (0.0f <= this->totalRange) {
            if (0.0f <= rawAngle) {
            normalizedAngle = rawAngle;
            if (6.283185307179586f < rawAngle) {
                normalizedAngle = rawAngle - 6.2831855f;
            }
            }
            else {
                normalizedAngle = rawAngle + 6.2831855f;
            }

            rangePlusDeadzone = this->totalRange + this->deadZoneSize;
            if (rangePlusDeadzone < normalizedAngle) {
                return this->min;
            }
            if (this->totalRange < normalizedAngle) {
                return this->max;
            }
            if (rangePlusDeadzone < rawAngle) {
                return (rawAngle - 6.2831855f) * (float)this->angleRange + this->min;
            }
            if (rawAngle < -(float)this->deadZoneSize) {
                rawAngle = rawAngle + 6.2831855f;
            }

            return rawAngle * (float)this->angleRange + this->min;
        }

        rawAngle = rawAngle - this->totalRange;
        if (0.0f <= rawAngle) {
            normalizedAngle = rawAngle;
            if (6.283185307179586f < rawAngle) {
                normalizedAngle = rawAngle - 6.2831855f;
            }
        }
        else {
            normalizedAngle = rawAngle + 6.2831855f;
        }

        rangePlusDeadzone = this->deadZoneSize - this->totalRange;

        if (rangePlusDeadzone < normalizedAngle) {
            return this->max;
        }

        if (-this->totalRange < normalizedAngle) {
            return this->min;
        }

        if (rangePlusDeadzone < rawAngle) {
            return (rawAngle - 6.2831855f) * (float)this->angleRange +
                this->max;
        }

        if (rawAngle < -this->deadZoneSize) {
            rawAngle = rawAngle + 6.2831855f;
        }

        return rawAngle * this->angleRange + this->max;
    }

    // FUNCTION: DELAYLAMA 0x100096c0
    void RotaryControl::setIndicatorShadowColor(Api::ColorRGBA color) {
        this->indicatorShadowColor.bytes.r = color.bytes.r;
        this->indicatorShadowColor.bytes.g = color.bytes.g;
        this->indicatorShadowColor.bytes.b = color.bytes.b;
        this->indicatorShadowColor.bytes.a = color.bytes.a;
        this->setDirty(true);
    }

    // FUNCTION: DELAYLAMA 0x100096f0
    void RotaryControl::setIndicatorHighlightColor(Api::ColorRGBA color) {
        this->indicatorHighlightColor.bytes.r = color.bytes.r;
        this->indicatorHighlightColor.bytes.g = color.bytes.g;
        this->indicatorHighlightColor.bytes.b = color.bytes.b;
        this->indicatorHighlightColor.bytes.a = color.bytes.a;
        this->setDirty(true);
    }

    // FUNCTION: DELAYLAMA 0x10009720
    void RotaryControl::setBitmap(Platform::Windows::Bitmap* bmp) {
        Platform::Windows::Bitmap* oldBitmap = this->bmp;
        if (oldBitmap != nullptr) {
            Platform::Windows::Bitmap::unregisterBitmap(oldBitmap);
            this->bmp = nullptr;
        }
        if (bmp != nullptr) {
            this->bmp = bmp;
            View::useBitmap(bmp);
            int bitmapWidth = bmp->width;
            int knobRadius = static_cast<int>(static_cast<float>(bitmapWidth) * 0.5f + 2.5f);
            this->knobRadius = knobRadius;
        }
    }

    // FUNCTION: DELAYLAMA 0x10009770
    float RotaryControl::getStartAngle() {
        return this->startAngle;
    }

    // FUNCTION: DELAYLAMA 0x10009780
    float RotaryControl::getTotalRange() {
        return this->totalRange;
    }

    // FUNCTION: DELAYLAMA 0x10009790
    void RotaryControl::setKnobRadius(float radius) {
        this->knobRadius = radius;
    }

    // FUNCTION: DELAYLAMA 0x100097a0
    void RotaryControl::setFineTuneDivider(float divider) {
        this->fineTuneDivider = divider;
    }

    // FUNCTION: DELAYLAMA 0x100097b0
    float RotaryControl::getFineTuneDivider() {
        return this->fineTuneDivider;
    }
}
}
}