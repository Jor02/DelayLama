#pragma once
#include "control.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "damsdk/api/DamPlugin.h"
#include "utils/portable_stdint.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000c094
    class HorizontalSlider : public Control {
        public:
            void *range;
            int numOutputs;
            int flags;
            bool snapToMouse;
            int handleWidth;
            int handleHeight;
            int trackMinX;
            int trackMaxX;
            int trackLeftX;
            int handleMinPos;
            int handleMaxPos;
            int trackWidth;
            int trackHeight;
            float currentValue;
            bool isHandleTransparent;
            POINT backgroundOffset;
            POINT handlePos;
            Platform::Windows::Bitmap *handleImage;
        public:
            HorizontalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, Api::Range *range, int flags);
            ~HorizontalSlider();
            virtual void setSnapToMouse(bool snapToMouse);
            virtual void setIsHandleTransparent(bool transparent);
            virtual bool getSnapToMouse();
            virtual void setHandlePos(POINT* handlePos);
            virtual Platform::Windows::Bitmap* HorizontalSlider::getHandleImage();
            virtual void setCurrentValue(float currentValue);
            virtual float getCurrentValue();
            virtual void changeHandle(Platform::Windows::Bitmap* newHandle);
            virtual void destroy();
            virtual void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
            virtual void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos);
    };
}
}
}