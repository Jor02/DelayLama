#pragma once
#include "Knob.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // FUNCTION: DELAYLAMA 0x100097c0
    Knob::Knob(RECT *pRect, callbackCallback callback, int parameterId, int totalFrames, int frameHeight, Platform::Windows::Bitmap *bmp, POINT *origin) : RotaryControl(pRect, callback, parameterId, bmp, nullptr, origin)
    {
        this->frameHeight = frameHeight;
        this->totalFrames = totalFrames;
        this->knobRadius = 0.0;
    }

    // FUNCTION: DELAYLAMA 0x10009810
    Knob::~Knob() {
        destroy();
    }

    // FUNCTION: DELAYLAMA 0x10009830
    void Knob::destroy() {
        RotaryControl::destroy();
    }

    // FUNCTION: DELAYLAMA 0x10009840
    void Knob::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        POINT src;
        src.x = 0;

        if (!(this->value > 0.0f))
        {
            src.y = 0;
        }
        else
        {
            int frameHeight = this->frameHeight;
            int totalFrames = this->totalFrames;
            int maxSrcY = (totalFrames - 1) * frameHeight;

            long rawY = static_cast<long>(static_cast<float>(maxSrcY) * this->value);

            int currentY = 0;

            if (maxSrcY >= 0)
            {
                for (;;)
                {
                    if (rawY < currentY)
                    {
                        currentY -= frameHeight;
                        src.y = currentY;
                        break;
                    }

                    currentY += frameHeight;

                    if (currentY > maxSrcY)
                    {
                        break;
                    }
                }
            }
        }

        Platform::Windows::Bitmap* bitmap = this->bitmap;
        if (bitmap != nullptr)
        {
            if (this->useAlphaBlending != false)
            {
                bitmap->drawMasked(drawingContext, &this->rect, &src);
                this->setDirty(false);
                return;
            }

            bitmap->blit(drawingContext, &this->rect, &src);
        }

        this->setDirty(false);
    }
}
}
}