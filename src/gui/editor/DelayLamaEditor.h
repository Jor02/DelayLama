#pragma once
#include "damsdk/api/EditorBase.h"
#include "damsdk/gui/controls/Control.h"
#include "damsdk/gui/platform/windows/Bitmap.h"

// Forward declarations
namespace DelayLama {
    namespace Core { class DelayLamaPlugin; }
}

using DamSDK::Gui::Platform::Windows::Bitmap;
using DamSDK::Gui::Controls::Control;

namespace DelayLama {
namespace Gui{
    class DelayLamaEditor : public DamSDK::Api::EditorBase {
        public:
            char unused[3];
            void* drawControl;
            Control* monk;
            Control* twoAxisSlider;
            Control* singingVerticalSlider;
            Control* singingHorizontalSlider;
            Control* reverbSlider;
            Control* leftKnob;
            Control* knob;
            Control* splashScreen;
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
            void dispatcher(int parameterIndex) override;
    };   
}
}