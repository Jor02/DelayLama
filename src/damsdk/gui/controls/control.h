#include "damsdk/gui/base/View.h"

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows { class Bitmap; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Controls {
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
            void *unknown;
        public:
            Control(RECT *pRect, void *unknown, int parameterId, Platform::Windows::Bitmap *bmp);
            void setValue(float newValue);
    };
}
}
}