#include "HorizontalSlider.h"
#include "damsdk/gui/platform/windows/OffscreenGDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // FUNCTION: DELAYLAMA 0x1000a010
    HorizontalSlider::HorizontalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, POINT* offset, int flags) : Control(pRect, callback, parameterId, backgroundBmp)
    {
        this->backgroundOffset.x = offset->x;
        this->backgroundOffset.y = offset->y;
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
        this->trackWidth = this->rect.right - this->rect.left;
        this->trackHeight = this->rect.bottom - this->rect.top;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->handleMinPos = minValue - this->rect.left;
        this->handleMaxPos = (this->handleWidth - this->rect.left) + maxValue;
        this->fineTuneDivider = 10.0;
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
    void HorizontalSlider::setFinetuneDivider(float currentValue) {
        this->fineTuneDivider = currentValue;
    }

    // FUNCTION: DELAYLAMA 0x10009bc0
    float HorizontalSlider::getFinetuneDivider() {
        return this->fineTuneDivider;
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
        Control::destroy();
    }

    // FUNCTION: DELAYLAMA 0x1000a1a0
    void HorizontalSlider::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        Platform::Windows::OffscreenGDIDrawingContext* offscreenContext = new Platform::Windows::OffscreenGDIDrawingContext(
            this->parent,
            this->trackWidth,
            this->trackHeight,
            Platform::Windows::DAT_BACK_COLOR
        );

        RECT trackRect = { 0, 0, this->trackWidth, this->trackHeight };
        Platform::Windows::Bitmap* trackBitmap = this->bitmap;

        // Draw the track background if a bitmap is provided.
        if (trackBitmap != nullptr) {
            POINT srcOffset = this->backgroundOffset;
            if (!this->useAlphaBlending) {
                trackBitmap->blit(offscreenContext, &trackRect, &srcOffset);
            } else {
                trackBitmap->drawMasked(offscreenContext, &trackRect, &srcOffset);
            }
        }

        // Compute the handle's horizontal position based on the current normalized value.
        int valueRange = this->trackMaxX - this->trackMinX;
        int handleOffset = static_cast<int>(this->fineTuneDivider * valueRange);
        int handleX = this->handlePos.x + handleOffset;

        // Clamp to allowed range.
        if (handleX < this->handleMinPos) handleX = this->handleMinPos;
        int handleRight = handleX + this->handleWidth;
        if (handleRight > this->handleMaxPos) handleRight = this->handleMaxPos;

        RECT handleRect;
        handleRect.left   = handleX;
        handleRect.top    = this->handlePos.y;
        handleRect.right  = handleRight;
        handleRect.bottom = this->handlePos.y + this->handleHeight;

        // Draw the handle image.
        Platform::Windows::Bitmap* handleImage = this->handleImage;
        if (handleImage != nullptr) {
            POINT srcPoint = { 0, 0 };  // drawn from top‑left of the handle bitmap
            if (!this->isHandleTransparent) {
                handleImage->blit(offscreenContext, &handleRect, &srcPoint);
            } else {
                handleImage->drawMasked(offscreenContext, &handleRect, &srcPoint);
            }
        }

        // Copy the completed offscreen buffer to the actual screen.
        RECT destRect = this->rect;
        offscreenContext->copyToScreen(drawingContext, destRect.left, destRect.top, destRect.right, destRect.bottom, 0, 0);

        // Clean up offscreen context.
        delete offscreenContext;

        // Update the left coordinate of the track (used by parent for layout) and mark dirty.
        this->trackLeftX = destRect.left + handleRect.left;
        this->setDirty(true);
    }

    // FUNCTION: DELAYLAMA 0x1000a360
    void HorizontalSlider::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos) {
        if (this->isEnabled != false) {
            uint8_t modifiers = View::GetPressedModifiersAndMouseButtons();
            // Ctrl + Left Click -> Reset to default value
            if (modifiers == 0x11) {  // 0x10 (Ctrl) | 0x01 (Left Button)
                float defaultValue = this->getDefaultValue();
                this->value = defaultValue;
                if (this->isDirty()) {
                    this->callback(drawingContext, this);
                }
                return;
            }
            else {

                int horizontalAnchor;

                //Check for Left Click (0x01)
                if ((modifiers & 1) != 0) {
                    int trackMinX = this->trackMinX;
                    if (this->snapToMouse == false) {
                        int trackLeft = this->trackLeftX;
                        int trackTop = this->rect.top;
                        int mouseX = mousePos->x;
                        if (mouseX < trackLeft) {
                        return;
                        }
                        if (this->handleWidth + trackLeft < mouseX) {
                            return;
                        }
                        if (mousePos->y < trackTop) {
                            return;
                        }
                        if (this->handleHeight + trackTop < mousePos->y) {
                            return;
                        }
                        horizontalAnchor = trackMinX + (mouseX - trackLeft);
                    }
                    else {
                        horizontalAnchor = trackMinX + -1 + this->handleWidth / 2;
                    }

                    trackMaxX = this->trackMaxX;
                    float curValue = this->value;
                    this->parent->beginEdit(this->parameterId);

                    modifiers = View::GetPressedModifiersAndMouseButtons();
                    uint32_t previousModifiers = modifiers;

                    float previousValue = this->value;
                    while ((modifiers & 1) != 0) {
                        if (modifiers != previousModifiers) {
                            if ((modifiers & 8) != 0) {
                                previousValue = this->value;
                            }
                            previousModifiers = modifiers;
                        }
                        else {
                            curValue = this->value;
                        }
                        flags = this->flags;
                        float calculatedValue = (float)(mousePos->x - horizontalAnchor) / (float)(trackMaxX - trackMinX);
                        this->value = calculatedValue;
                        
                        if ((flags & 0x10) != 0) {
                            this->value = 1.0f - calculatedValue;
                        }

                        if ((modifiers  & 8) != 0) {
                            this->value = (this->value - (float)curValue) / this->fineTuneDivider + (float)curValue;
                        }

                        this->clampValue();
                        bool isDirty = this->isDirty();
                        if (isDirty != false) {
                            this->callback(drawingContext,this);
                        }
                        drawingContext->getRelativeMousePos(mousePos);
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
}