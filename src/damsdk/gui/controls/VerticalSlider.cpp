#pragma once
#include "VerticalSlider.h"
#include "damsdk/gui/platform/windows/OffscreenGDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // STUB: DELAYLAMA 0x10009a40
    VerticalSlider::VerticalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, POINT* offset, int flags) : Control(pRect, callback, parameterId, handleBmp)
    {
        this->backgroundOffset.x = offset->x;
        this->backgroundOffset.y = offset->y;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->flags = flags;
        this->handleImage = handleBmp;
        this->trackMinY = minValue;
        this->trackMaxY = maxValue;
        this->snapToMouse = true;
        this->isHandleTransparent = true;
        if (handleBmp == nullptr) {
            this->handleWidth = 1;
            this->handleHeight = 1;
        }
        else {
            View::useBitmap(handleBmp);
            this->handleWidth = this->handleImage->width;
            this->handleHeight = this->handleImage->height;
        }
        this->trackWidth = this->rect.right - this->rect.left;
        this->trackTopY = maxValue;
        this->trackHeight = this->rect.bottom - this->rect.top;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->fineTuneDivider = 10.0f;
        this->handleMinPos = minValue - this->rect.top;
        this->handleMaxPos = (this->handleHeight - this->rect.top) + maxValue;
    }

    // FUNCTION: DELAYLAMA 0x10009bd0
    VerticalSlider::~VerticalSlider() {
        this->destroy();
    }

    // FUNCTION: DELAYLAMA 0x10009bf0
    void VerticalSlider::destroy() {
        if (this->handleImage != nullptr) {
          Platform::Windows::Bitmap::unregisterBitmap(this->handleImage);
        }
        Control::destroy();
    }

    // FUNCTION: DELAYLAMA 0x10009c50
    void VerticalSlider::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        Platform::Windows::OffscreenGDIDrawingContext* offscreenContext = new Platform::Windows::OffscreenGDIDrawingContext(
            this->parent,
            this->trackWidth,
            this->trackHeight,
            Platform::Windows::DAT_BACK_COLOR
        );

        RECT trackRect = { 0, 0, this->trackWidth, this->trackHeight };
        Platform::Windows::Bitmap* trackBitmap = this->bitmap;

        if (trackBitmap != nullptr) {
            POINT srcOffset = this->backgroundOffset;
            if (!this->useAlphaBlending) {
                trackBitmap->blit(offscreenContext, &trackRect, &srcOffset);
            } else {
                trackBitmap->drawMasked(offscreenContext, &trackRect, &srcOffset);
            }
        }

        int valueRange = this->trackMaxY - this->trackMinY;
        int handleOffset = static_cast<int>(this->fineTuneDivider * valueRange);
        int handleY = this->handlePos.y + handleOffset;

        if (handleY < this->handleMinPos) handleY = this->handleMinPos;
        int handleBottom = handleY + this->handleHeight;
        if (handleBottom > this->handleMaxPos) handleBottom = this->handleMaxPos;

        RECT handleRect;
        handleRect.left   = this->handlePos.x;
        handleRect.top    = handleY;
        handleRect.right  = this->handlePos.x + this->handleWidth;
        handleRect.bottom = handleBottom;

        Platform::Windows::Bitmap* handleImage = this->handleImage;
        if (handleImage != nullptr) {
            POINT srcPoint = { 0, 0 };
            if (!this->isHandleTransparent) {
                handleImage->blit(offscreenContext, &handleRect, &srcPoint);
            } else {
                handleImage->drawMasked(offscreenContext, &handleRect, &srcPoint);
            }
        }

        RECT destRect = this->rect;
        offscreenContext->copyToScreen(drawingContext, destRect.left, destRect.top, destRect.right, destRect.bottom, 0, 0);

        delete offscreenContext;

        this->trackTopY = destRect.top + handleRect.top;
        this->setDirty(true);
    }

    // FUNCTION: DELAYLAMA 0x10009e10
    void VerticalSlider::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* relativeMousePos) {
        if (this->isEnabled != false) {

            uint8_t modifiers = View::GetPressedModifiersAndMouseButtons();

            // Ctrl + Left Click -> Reset
            if (modifiers == 0x11) { 
                this->value = this->getDefaultValue();
                if (this->isDirty()) {
                    this->callback(drawingContext, this);
                }
                return;
            }

            // Left Click dragging
            if ((modifiers & 0x01) != 0) {
                int verticalAnchor;
                
                int trackMinY = this->trackMinY;
                int trackMaxY = this->trackMaxY;

                if (!this->snapToMouse) {
                    trackMaxY = this->rect.left;
                    trackTopY = this->trackTopY;
                    if (relativeMousePos->x < trackMaxY) {
                        return;
                    }
                    if (this->handleWidth + trackMaxY < relativeMousePos->x) {
                        return;
                    }
                    verticalAnchor = relativeMousePos->y;
                    if (verticalAnchor < trackTopY) {
                        return;
                    }
                    if (this->handleHeight + trackTopY < verticalAnchor) {
                        return;
                    }
                    verticalAnchor -= trackTopY;
                } else {
                    verticalAnchor = (this->handleHeight / 2) - 1;
                }

                verticalAnchor += trackMinY;
                
                float previousValue = this->value;
                this->parent->beginEdit(this->parameterId);

                while ((modifiers & 0x01) != 0) {
                    float calculatedValue = (float)(relativeMousePos->y - verticalAnchor) / (float)(trackMaxY - trackMinY);
                    
                    // Reverse if flag 0x40 is set (Standard for "Inverted" vertical sliders)
                    if ((this->flags & 0x40) != 0) {
                        calculatedValue = 1.0f - calculatedValue;
                    }

                    // Fine-tuning logic (usually Shift key = 0x08)
                    if ((modifiers & 0x08) != 0) {
                        this->value = ((calculatedValue - previousValue) / this->fineTuneDivider) + previousValue;
                    } else {
                        this->value = calculatedValue;
                        previousValue = calculatedValue;
                    }

                    this->clampValue();

                    if (this->isDirty()) {
                        this->callback(drawingContext, this);
                    }

                    drawingContext->getRelativeMousePos(relativeMousePos);
                    this->onIdle();
                    modifiers = View::GetPressedModifiersAndMouseButtons();
                }

                this->parent->endEdit(this->parameterId);
            }
        }
    }
}
}
}