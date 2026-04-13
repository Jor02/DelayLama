#pragma once
#include "Knob.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // STUB: DELAYLAMA 0x100097c0
    Knob::Knob(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *bmp, POINT *origin) : RotaryControl(pRect, callback, parameterId, bmp, nullptr, origin)
    {
        this->min = static_cast<float>(minValue);
        this->max = static_cast<float>(maxValue);
        this->origin = *origin;
    }

    // STUB: DELAYLAMA 0x10009810
    Knob::~Knob() {
        destroy();
    }

    // STUB: DELAYLAMA 0x10009830
    void Knob::destroy() {
        RotaryControl::destroy();
    }

    // STUB: DELAYLAMA 0x10009840
    void Knob::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        // int currentY;
        // undefined4 unaff_ESI;
        // int maxSrcY;
        // undefined4 unaff_EDI;
        // POINT src;
        // Bitmap *bitmap;
        // int frameHeight;
        //
        // src.x = 0;
        // if ((this->rotaryControl).control.value <= FLOAT_1000b208) {
        //   src.y = 0;
        // }
        // else {
        //   frameHeight = this->frameHeight;
        //   maxSrcY = (this->totalFrames + -1) * frameHeight;
        //   src.y = _ftol((double)CONCAT44(unaff_ESI,unaff_EDI));
        //   currentY = 0;
        //   if (-1 < maxSrcY) {
        //     do {
        //       if (src.y < currentY) {
        //         src.y = currentY - frameHeight;
        //         break;
        //       }
        //       currentY = currentY + frameHeight;
        //     } while (currentY <= maxSrcY);
        //   }
        // }
        // bitmap = (this->rotaryControl).control.bitmap;
        // if (bitmap != (Bitmap *)0x0) {
        //   if ((this->rotaryControl).control.viewMembers.useAlphaBlending != false) {
        //     Bitmap::drawMasked(bitmap,drawingContext,&(this->rotaryControl).control.viewMembers.rect,&src)
        //     ;
        //     (*(this->vtable->rotaryControl).control.view.setDirty)(0);
        //     return;
        //   }
        //   Bitmap::blit(bitmap,drawingContext,&(this->rotaryControl).control.viewMembers.rect,&src);
        // }
        // (*(this->vtable->rotaryControl).control.view.setDirty)(0);
        // return;
    }
}
}
}