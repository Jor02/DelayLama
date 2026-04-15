#pragma once
#include "Monk.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/utils/portable_stdint.h"
#include "utils/Logger.h"

namespace DelayLama {
namespace Gui {
namespace Controls {

    // FUNCTION DELAYLAMA: 0x10004600
    Monk::Monk(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, int frameCount, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset) : TileGrid(pRect, callback, parameterId, frameCount, tileHeight, bmp, srcOffset)
    {
    }

    // FUNCTION: DELAYLAMA 0x10004650
    Monk::~Monk() {
        TileGrid::destroy();
    }

    // FUNCTION DELAYLAMA: 0x100046a0
    void Monk::onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext)
    {
        this->tileHeight = TILE_HEIGHT;

        if (this->value > 1.0f) {
            this->value = 1.0f;
        }

        int xOffset = 0;
        int yOffset = 0;

        if (this->value > 0.0f) {
            int maxFrames = this->frameCount - 1; 
            int frameIndex = static_cast<int>((maxFrames * this->value) + 0.5f);

            int row = frameIndex % Y_TILES;
            int column = frameIndex / Y_TILES;

            yOffset = row * TILE_HEIGHT;
            xOffset = column * TILE_WIDTH;
        }

        POINT srcPoint;
        srcPoint.x = xOffset + this->srcOffset.x;
        srcPoint.y = yOffset + this->srcOffset.y;

        DamSDK::Gui::Platform::Windows::Bitmap* fullGridBitmap = this->bitmap;
        if (fullGridBitmap != nullptr) {
            if (this->useAlphaBlending) {
                fullGridBitmap->drawMasked(
                    drawingContext,
                    &this->rect,
                    &srcPoint
                );
            } else {
                fullGridBitmap->blit(
                    drawingContext,
                    &this->rect,
                    &srcPoint
                );
            }
        }

        this->setDirty(false);
    }
}
}
}