#pragma once
#include <cstdint>
#include "core/DelayLamaPlugin.h"
#include "damsdk/DamPlugin.h"
#include "damsdk/EditorBase.h"
#include "gui/controls/control.h"
#include "platform/Bitmap.h"

namespace DelayLama {
namespace Gui{
    class DelayLamaEditor : public DamSDK::EditorBase {
        public:
            char unused[3];
            void* drawControl;
            Controls::Control* monk;
            Controls::Control* twoAxisSlider;
            Controls::Control* singingVerticalSlider;
            Controls::Control* singingHorizontalSlider;
            Controls::Control* reverbSlider;
            Controls::Control* leftKnob;
            Controls::Control* knob;
            Controls::Control* splashScreen;
            Platform::Bitmap* backgroundBitmap;
            Platform::Bitmap* monkSpriteSheetBitmap;
            Platform::Bitmap* singingYHandleBitmap;
            Platform::Bitmap* singingXHandleBitmap;
            Platform::Bitmap* reverbHandleBitmap;
            Platform::Bitmap* leftKnobBitmap;
            Platform::Bitmap* rightKnobBitmap;
            Platform::Bitmap* aboutScreenBitmap;
        public:
            DelayLamaEditor(Core::DelayLamaPlugin* pluginInstance);
            ~DelayLamaEditor();
            void dispatcher(int parameterIndex) override;
    };   
}
}