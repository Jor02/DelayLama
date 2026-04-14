#pragma once
#include "Monk.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/utils/portable_stdint.h"

namespace DelayLama {
namespace Gui {
namespace Controls {

    // FUNCTION DELAYLAMA: 0x10004600
    Monk::Monk(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, int tileWidth, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset) : TileGrid(pRect, callback, parameterId, tileWidth, tileHeight, bmp, srcOffset)
    {
    }

    // FUNCTION: DELAYLAMA 0x10004650
    Monk::~Monk() {
        TileGrid::destroy();
    }

    // FUNCTION DELAYLAMA: 0x100046a0
    void Monk::onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext)
    {
        this->tileHeight = 311;

        const float frameValue = this->value;
        if (this->value > 1.0f) {
            this->value = 1.0f;
        }

        POINT srcPoint;
        srcPoint.x = 0;
        srcPoint.y = 0;

        int xOffsetAccumulator = 0;

        if (this->value > 0.0f) {
            const int frameIndex = static_cast<int>(frameValue);

            const int row = frameIndex % 6;
            const int column = frameIndex / 6;

            srcPoint.y = row * 311;
            xOffsetAccumulator = column * 314;
        }

        srcPoint.y += this->srcOffset.y;
        srcPoint.x = xOffsetAccumulator + this->srcOffset.x;

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