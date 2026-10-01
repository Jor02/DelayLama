#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "SplashScreen.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/gui/platform/windows/Window.h"
#include "utils/Logger.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    
    // FUNCTION: DELAYLAMA 0x1000a530
    SplashScreen::SplashScreen(RECT *pRect, DamSDK::Gui::Controls::ControlListener* listener, int parameterId, DamSDK::Gui::Platform::Windows::Bitmap *bmp, RECT *destRect, POINT *srcPoint) : DamSDK::Gui::Controls::Control(pRect, listener, parameterId, bmp)
    {
        Utils::log("SplashScreen::ctor\n");
        this->destRect.left = destRect->left;
        this->destRect.top = destRect->top;
        this->destRect.right = destRect->right;
        this->destRect.bottom = destRect->bottom;

        this->keepRect.left = 0;
        this->keepRect.top = 0;
        this->keepRect.right = 0;
        this->keepRect.bottom = 0;

        this->srcPoint.x = srcPoint->x;
        this->srcPoint.y = srcPoint->y;
    }

    // FUNCTION: DELAYLAMA 0x1000a5c0
    SplashScreen::~SplashScreen() {
    }

    // FUNCTION: DELAYLAMA 0x1000a5d0
    void SplashScreen::onDraw(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext) {
        DamSDK::Gui::Platform::Windows::Bitmap* bitmap = this->bitmap;

        if (this->value != 0.0f && bitmap != nullptr) {
            if (this->useAlphaBlending) {
                bitmap->drawMasked(drawingContext, &this->destRect, &this->srcPoint);
                this->setDirty(false);
                return;
            }
            bitmap->blit(drawingContext, &this->destRect, &this->srcPoint);
        }
        this->setDirty(false);
    }

    // FUNCTION: DELAYLAMA 0x1000a630
    void SplashScreen::onMouseDown(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos) {
        if (!this->isEnabled)
            return;
        int buttons = drawingContext->getMouseButtons();
        if (!(buttons & DamSDK::Gui::kLButton))
            return;

        this->value = !this->value;
        if (this->value) {
            // Show: grow to the splash rect and take over the mouse
            if (this->parent != nullptr && this->parent->setModalView(this)) {
                this->keepRect = this->rect;
                this->rect = this->destRect;
                this->onDraw(drawingContext);
                this->listener->valueChanged(drawingContext, this);
            }
        }
        else {
            // Dismiss: back to the control's own rect
            this->rect = this->keepRect;
            if (this->parent != nullptr) {
                this->parent->setModalView(nullptr);
                this->parent->onDraw(drawingContext);
            }
            this->listener->valueChanged(drawingContext, this);
        }
        this->setDirty(true);
    }
}
}
}
