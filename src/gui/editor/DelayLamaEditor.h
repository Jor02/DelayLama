#pragma once
#include "damsdk/api/EditorBase.h"
#include "damsdk/gui/controls/Control.h"
#include "damsdk/gui/platform/windows/Bitmap.h"

// Forward declarations
namespace DamSDK {
    namespace Gui {
        namespace Controls {
            class HorizontalSlider;
            class VerticalSlider;
            class TwoAxisSlider;
            class Knob;
        }
    }
}

namespace DelayLama {
    namespace Core { class DelayLamaPlugin; }
    namespace Gui {
        namespace Controls {
            class Monk;
            class SplashScreen;
        }
    }
}

using namespace DamSDK::Gui::Platform::Windows;
using namespace DamSDK::Gui::Controls;
using namespace DelayLama::Gui::Controls;

namespace DelayLama {
namespace Gui{
    class DelayLamaEditor : public DamSDK::Api::EditorBase {
        public:
            char unused[3];
            void* drawControl;
            Monk* monk;
            TwoAxisSlider* singingController;
            VerticalSlider* singingVerticalSlider;
            HorizontalSlider* singingHorizontalSlider;
            HorizontalSlider* reverbSlider;
            Knob* leftKnob;
            Knob* rightKnob;
            SplashScreen* splashScreen;
            Bitmap* backgroundBitmap;
            Bitmap* monkSpriteSheetBitmap;
            Bitmap* singingYHandleBitmap;
            Bitmap* singingXHandleBitmap;
            Bitmap* reverbHandleBitmap;
            Bitmap* leftKnobBitmap;
            Bitmap* rightKnobBitmap;
            Bitmap* aboutScreenBitmap;
        public:
            DelayLamaEditor(Core::DelayLamaPlugin* pluginInstance);
            ~DelayLamaEditor();
            void open(HWND hParent) override;
            void dispatcher(int parameterIndex) override;
    };   
}
}