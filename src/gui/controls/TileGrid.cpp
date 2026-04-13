#pragma once
#include "TileGrid.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "utils/portable_stdint.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    
    // FUNCTION DELAYLAMA: 0x10009900
    TileGrid::TileGrid(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, int tileWidth, int tileHeight, DamSDK::Gui::Platform::Windows::Bitmap *bmp, POINT *srcOffset) : DamSDK::Gui::Controls::Control(pRect, callback, parameterId, bmp)
    {
        this->srcOffset = *srcOffset;
        this->tileWidth = tileWidth;
        this->tileHeight = tileHeight;
    }

    // FUNCTION DELAYLAMA: 0x10009980
    void TileGrid::onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext) {
        POINT srcPoint;
        srcPoint.x = this->srcOffset.x;
        srcPoint.y = this->srcOffset.y;

        // Clamp value to [0, 1]
        if (this->value > 1.0f) {
            this->value = 1.0f;
        }

        if (this->value > 0.0f) {
            int tileIndex = static_cast<int>(this->value);
            srcPoint.y += tileIndex * this->tileHeight;
        }

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

        // Mark clean
        this->setDirty(false);
    }

    // STUB: DELAYLAMA 0x10009950
    TileGrid::~TileGrid() {
        // destoy(this);
        // if (deleteObject) {
        //   operator_delete(this);
        // }
        // return this;
    }

    // STUB: DELAYLAMA 0x10009970
    void TileGrid::destoy() {
        // this->vtable = &TileGridVTable_1000bf34;
        // Control::destroy((Control *)this);
        // return;
    }
}
}
}