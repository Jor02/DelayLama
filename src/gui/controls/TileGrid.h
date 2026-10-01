#pragma once
#include "damsdk/gui/controls/control.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    // VTABLE: DELAYLAMA 0x1000bf34
    class TileGrid : public DamSDK::Gui::Controls::Control {
        public:
            POINT srcOffset;
            int frameCount;
            int tileHeight;
        public:
            TileGrid(RECT *pRect, DamSDK::Gui::Controls::ControlListener* listener, int parameterId, int frameCount, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset);
            ~TileGrid();
            virtual void onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext) override;
    };
    }
}
}