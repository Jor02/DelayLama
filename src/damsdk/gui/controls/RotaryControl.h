#pragma once
#include "control.h"
#include "utils/portable_stdint.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/api/DamPlugin.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000be60
    class RotaryControl : public Control {
            POINT srcPoint;
            Api::ColorRGBA indicatorHighlightColor;
            Api::ColorRGBA indicatorShadowColor;
            Platform::Windows::Bitmap *bmp;
            float knobRadius;
            float startAngle;
            float totalRange;
            float deadZoneSize;
            float angleRange;
            float angleOffset;
            float center;
            float fineTuneDivider;
        public:
            RotaryControl(RECT* pRect, callbackCallback callback, int parameterId, Platform::Windows::Bitmap* bmp1, Platform::Windows::Bitmap* bmp2, POINT* srcPoint);
            ~RotaryControl();
            void destroy();
            void onDraw(Platform::Windows::GDIDrawingContext* drawingContext);
            void drawIndicator(Platform::Windows::GDIDrawingContext* drawingContext);
            void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos);
            void setStartAngle(float startAngle);
            void setTotalRange(float totalRange);
            void updateMathConstants();
            void calculateXYFromValue(POINT* outPoint);
            float calculateAngleFromPoint(POINT* point);
            void setIndicatorShadowColor(Api::ColorRGBA color);
            void setIndicatorHighlightColor(Api::ColorRGBA color);
            void setBitmap(Platform::Windows::Bitmap* bmp);
            float getStartAngle();
            float getTotalRange();
            void setKnobRadius(float radius);
            void setFineTuneDivider(float divider);
            float getFineTuneDivider();
    };
}
}
}