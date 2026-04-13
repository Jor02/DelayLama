#pragma once
#include "damsdk/gui/base/View.h"

namespace DamSDK {
    namespace Gui {
        namespace Controls { class Control; }
        namespace Platform {
            namespace Windows { class Bitmap; class GDIDrawingContext; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Controls {

    typedef void (*callbackCallback)(Platform::Windows::GDIDrawingContext*, Control*);

    // VTABLE: DELAYLAMA 0x1000bcd4
    class Control : public Base::View {
        public:
            int parameterId;
            float prevValue;
            float value;
            float min;
            float max;
            float defaultValue;
            float wheelSensitivity;
            Platform::Windows::Bitmap *bitmap;
            bool useAlphaBlending;
            bool isEnabled;
            void (*callback)(Platform::Windows::GDIDrawingContext*, Control*);
        public:
            Control(RECT *pRect, callbackCallback callback, int parameterId, Platform::Windows::Bitmap *bmp);
            ~Control();
            void setDefaultValue(float defaultValue = 0);
            void setValue(float newValue);
            void onIdle();
            float getValue();
            void setMin(float min);
            float getMin();
            void setMax(float max);
            float getMax();
            float getPreviousValue();
            float getDefaultValue();
            void setParameterId(int parameterId);
            Platform::Windows::Bitmap* getBitmap();
            void setWheelSensitivity(float sensitivity);
            float getWheelSensitivity();
            void destroy();
            bool isDirty();
            void setDirty(bool isDirty);
            void changeBitmap(Platform::Windows::Bitmap* newBitmap);
            void clampValue();
            bool onMouseWheel(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos, float wheelDelta);
    };
}
}
}