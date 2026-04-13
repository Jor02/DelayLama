#pragma once
#include "TwoAxisSlider.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // STUB: DELAYLAMA 0x10004350
    TwoAxisSlider::TwoAxisSlider(RECT *bounds, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handle, Platform::Windows::Bitmap *background, Api::Range* range, int flags) : HorizontalSlider(bounds,callback,parameterId,minValue,maxValue,handle,background,range,flags)
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
        // byte inputMask;
        // undefined3 _padding1;
        // int isStillDown;
        // undefined3 _padding3;
        // int yTop;
        // undefined3 _padding2;
        // DrawControl *drawFunc;
        // int handleWidth;
        // int minX;
        // TwoAxisSliderVTable *vtablePtr;
        //
        // if ((this->slider).control.viewMembers.isEnabled != false) {
        //   inputMask = View::GetPressedModifiersAndMouseButtons();
        //   if ((inputMask & 1) != 0) {
        //     if (CONCAT31(_padding1,inputMask) != 0) {
        //       drawFunc = (this->slider).control.callback;
        //       (this->slider).control.value = 201.0;
        //       (*(code *)**(undefined4 **)drawFunc)(drawContext,this);
        //     }
        //     handleWidth = (this->slider).handleWidth;
        //     minX = (this->slider).trackMinX;
        //     Window::beginEdit((this->slider).control.viewMembers.parent,(this->slider).control.parameterId
        //                      );
        //     inputMask = View::GetPressedModifiersAndMouseButtons();
        //     isStillDown = CONCAT31(_padding2,inputMask);
        //     while (isStillDown != 0) {
        //       vtablePtr = this->vtable;
        //       (this->slider).control.value =
        //            (float)(mousePos->x - (handleWidth / 2 + minX)) / this->xValueRange;
        //       (*(vtablePtr->slider).control.clampValue)();
        //       (*(code *)**(undefined4 **)(this->slider).control.callback)(drawContext,this);
        //       vtablePtr = this->vtable;
        //       (this->slider).control.value = (float)(mousePos->y - yTop) / this->yValueRange;
        //       (*(vtablePtr->slider).control.clampValue)();
        //       drawFunc = (this->slider).control.callback;
        //       (this->slider).control.value = (this->slider).control.value + FLOAT_1000b8b0;
        //       (*(code *)**(undefined4 **)drawFunc)(drawContext,this);
        //       GDIDrawingContext::getRelativeMousePos(drawContext,mousePos);
        //       (*(this->vtable->slider).control.onIdle)();
        //       inputMask = View::GetPressedModifiersAndMouseButtons();
        //       isStillDown = CONCAT31(_padding3,inputMask);
        //     }
        //     drawFunc = (this->slider).control.callback;
        //     (this->slider).control.value = 200.0;
        //     (*(code *)**(undefined4 **)drawFunc)(drawContext,this);
        //   }
        // }
        // return;
    }
}
}
}