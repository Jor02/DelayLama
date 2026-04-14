#pragma once
#include "TwoAxisSlider.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // STUB: DELAYLAMA 0x10004350
    TwoAxisSlider::TwoAxisSlider(RECT *bounds, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handle, Platform::Windows::Bitmap *background, POINT* offset, int flags) : HorizontalSlider(bounds,callback,parameterId,minValue,maxValue,handle,background,offset,flags)
    {
        int handleHeight = this->handleHeight;
        
        int halfHandleHeight = handleHeight / 2;
        this->halfHandleHeight2 = halfHandleHeight;
        this->yTop = bounds->top;
        
        int handleMaxY = bounds->bottom - halfHandleHeight;
        this->yTrackBottom = 0;
        this->yBottom = handleMaxY;
        this->xRange = (float)((handleMaxY - bounds->top) + handleHeight);
        
        int handleWidth = this->handleWidth;
        this->xMinOffset = minValue - bounds->left;
        this->xMaxOffset = (handleWidth - bounds->left) + maxValue;
        this->xValueRange = (float)(maxValue - minValue);
        this->yValueRange = (float)(handleMaxY - this->yTop);
    }

    // STUB: DELAYLAMA 0x10004420
    TwoAxisSlider::~TwoAxisSlider() {
        HorizontalSlider::destroy();
    }

    // STUB: DELAYLAMA 0x10004460
    void TwoAxisSlider::onMouseDown(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawContext, POINT* mousePos) {
        if (this->isEnabled != false) {
            uint8_t inputMask = View::GetPressedModifiersAndMouseButtons();
            
            if ((inputMask & 1) != 0) {

                if (inputMask != 0) {
                    this->value = 201.0f;
                    this->callback(drawContext, this);
                }

                int handleWidth = this->handleWidth;
                int minX = this->trackMinX;

                this->parent->beginEdit(this->parameterId);

                inputMask = View::GetPressedModifiersAndMouseButtons();
                int isStillDown = (int)inputMask;

                while (isStillDown != 0) {
                    // X-Axis Update
                    this->value = (float)(mousePos->x - (handleWidth / 2 + minX)) / this->xValueRange;
                    this->clampValue();
                    this->callback(drawContext, this);

                    // Y-Axis Update
                    this->value = (float)(mousePos->y - this->yTop) / this->yValueRange;
                    this->clampValue();
                    
                    this->value = this->value + 100.0f; 
                    this->callback(drawContext, this);

                    drawContext->getRelativeMousePos(mousePos);
                    this->onIdle();

                    inputMask = View::GetPressedModifiersAndMouseButtons();
                    isStillDown = (int)inputMask;
                }

                this->value = 200.0f;
                this->callback(drawContext, this);
            }
        }
    }
}
}
}