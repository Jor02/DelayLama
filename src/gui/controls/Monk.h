#pragma once
#include "damsdk/gui/controls/Control.h"
#include "TileGrid.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    class Monk : public TileGrid {
        public:
        public:
            Monk(RECT *pRect, void *unknown, int parameterId, int tileWidth, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset)
                : TileGrid(pRect, unknown, parameterId, tileWidth, tileHeight, bmp, srcOffset)
            {
            }
    };
    }
}
}