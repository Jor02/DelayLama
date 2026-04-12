#pragma once
#include "damsdk/gui/controls/Control.h"
#include "TileGrid.h"

namespace DelayLama {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000b9d4
    class Monk : public TileGrid {
        public:
        public:
            Monk(RECT *pRect, void *unknown, int parameterId, int tileWidth, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset);
            virtual void onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext);
    };
    }
}
}