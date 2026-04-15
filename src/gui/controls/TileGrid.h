#pragma once
#include "damsdk/gui/controls/Control.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    class TileGrid : public DamSDK::Gui::Controls::Control {
        public:
            POINT srcOffset;
            int frameCount;
            int tileHeight;
        public:
            TileGrid(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, int frameCount, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset);
            ~TileGrid();
            void destroy();
            virtual void onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext);
    };
    }
}
}