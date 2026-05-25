#pragma once
#include "damsdk/gui/controls/Control.h"
#include "TileGrid.h"

namespace DelayLama {
namespace Gui {
namespace Controls {

    #define X_TILES      5
    #define Y_TILES      6
    #define TILE_WIDTH   314
    #define TILE_HEIGHT  311

    #define TOTAL_TILES  (X_TILES * Y_TILES)
    #define MAX_Y_OFFSET (Y_TILES * TILE_HEIGHT)

    #define MONK_FRAME_VAL(tx, ty) ((float)((tx) * Y_TILES + (ty)) / (float)TOTAL_TILES)

    // VTABLE: DELAYLAMA 0x1000b9d4
    class Monk : public TileGrid {
        public:
        public:
            Monk(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, int frameCount, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset);
            ~Monk();
            virtual void onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext);
    };
    }
}
}