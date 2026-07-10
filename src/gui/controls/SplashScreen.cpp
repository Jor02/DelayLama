#include "SplashScreen.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/gui/platform/windows/Window.h"
#include "utils/Logger.h"

namespace DelayLama {
namespace Gui {
namespace Controls {
    
    // FUNCTION DELAYLAMA: 0x1000a530
    SplashScreen::SplashScreen(RECT *pRect, DamSDK::Gui::Controls::callbackCallback callback, int parameterId, DamSDK::Gui::Platform::Windows::Bitmap *bmp, RECT *destRect, POINT *srcPoint) : DamSDK::Gui::Controls::Control(pRect, callback, parameterId, bmp)
    {
        Utils::log("SplashScreen::ctor\n");
        this->destRect.left = destRect->left;
        this->destRect.top = destRect->top;
        this->destRect.right = destRect->right;
        this->destRect.bottom = destRect->bottom;

        this->absRect.left = 0;
        this->absRect.top = 0;
        this->absRect.right = 0;
        this->absRect.bottom = 0;

        this->srcPoint.x = srcPoint->x;
        this->srcPoint.y = srcPoint->y;
    }

    // FUNCTION DELAYLAMA: 0x1000a5a0
    SplashScreen::~SplashScreen() {
        this->destroy();
    }

    // FUNCTION DELAYLAMA: 0x1000a5c0
    void SplashScreen::destroy() {
        Control::destroy();
    }

    // FUNCTION DELAYLAMA: 0x1000a5d0
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

    // FUNCTION DELAYLAMA: 0x1000a630
    void SplashScreen::onMouseDown(DamSDK::Gui::Platform::Windows::GDIDrawingContext* drawingContext) {
        if (this->isEnabled && (View::GetPressedModifiersAndMouseButtons() & 0x1) != 0) {
            bool isDismissing = (this->value == 0.0f);
            Utils::logf("SplashScreen::onMouseDown %s\n", isDismissing ? "show" : "dismiss");
            this->value = (float)(int)isDismissing;

            if (isDismissing) {
                // Dismiss: restore rect from absRect and clear modal view
                this->rect.left = this->absRect.left;
                this->rect.top = this->absRect.top;
                this->rect.right = this->absRect.right;
                this->rect.bottom = this->absRect.bottom;

                DamSDK::Gui::Platform::Windows::Window* parentPtr = this->parent;
                if (parentPtr != nullptr) {
                    parentPtr->setModalView(nullptr);
                    parentPtr->onDraw(drawingContext);
                }

                this->callback(drawingContext, this);
            }
            else {
                // Show: set modal view and save current rect to absRect
                DamSDK::Gui::Platform::Windows::Window* parentPtr = this->parent;
                if (parentPtr != nullptr && parentPtr->setModalView((DamSDK::Gui::Base::View *)this) != 0) {
                    RECT* rectPtr = &this->rect;

                    this->absRect.left = rectPtr->left;
                    this->absRect.top = rectPtr->top;
                    this->absRect.right = rectPtr->right;
                    this->absRect.bottom = rectPtr->bottom;

                    rectPtr->left = this->destRect.left;
                    rectPtr->top = this->destRect.top;
                    rectPtr->right = this->destRect.right;
                    rectPtr->bottom = this->destRect.bottom;

                    this->onDraw(drawingContext);
                    this->callback(drawingContext, this);
                    this->setDirty(true);
                    return;
                }
            }
            this->setDirty(true);
        }
    }
}
}
}
