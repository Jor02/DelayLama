#include "EditorBase.h"
#include "AudioBase.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DamSDK {
namespace Api {

    // GLOBAL: DELAYLAMA 0x1000d874
    int GLOBAL_KNOB_MODE = 0;

    // FUNCTION: DELAYLAMA 0x10006680
    EditorBase::EditorBase(AudioBase* mainPlugin) {
        this->mainPlugin = mainPlugin;
        this->needsRedraw = false;
        this->window = nullptr;
        this->isInIdleUpdate = false;

        if (this == nullptr) {
            mainPlugin->plugin.flags &= ~PluginFlags::HasEditor;
        }
        else {
            mainPlugin->plugin.flags |= PluginFlags::HasEditor;
        }

        this->hParent = nullptr;
        this->lastIdleTick = GetTickCount();
        OleInitialize(NULL);
    }

    // FUNCTION: DELAYLAMA 0x10006790
    EditorBase::~EditorBase() {}

    // FUNCTION: DELAYLAMA 0x100067f0
    void EditorBase::open(HWND hParent) {
        this->invalidate();
        this->hParent = hParent;
    }

    // FUNCTION: DELAYLAMA 0x10006940
    void EditorBase::getRect(Rect** outRect) {
        *outRect = &this->rect;
    }

    // STUB: DELAYLAMA 0x100015b0 FOLDED
    void EditorBase::close() {

    }

    // FUNCTION: DELAYLAMA 0x10006810
    void EditorBase::onIdle() {
        if (this->isInIdleUpdate == false) {
            if (this->needsRedraw != false) {
                this->needsRedraw = false;
                //this->unimplemented();
            }

            if (this->window != nullptr) {
                this->window->refresh();
            }
        }
    }

    // FUNCTION: DELAYLAMA 0x10003810 FOLDED
    int32_t EditorBase::keyDown(KeyCode* keycode) {
        return -1;
    }
    
    // FUNCTION: DELAYLAMA 0x10003810 FOLDED
    int32_t EditorBase::keyUp(KeyCode* keycode) {
        return -1;
    }

    // FUNCTION: DELAYLAMA 0x10006840
    void EditorBase::setKnobMode(int32_t mode) {
        GLOBAL_KNOB_MODE = mode;
    }

    // FUNCTION: DELAYLAMA 0x10006ae0
    void EditorBase::dispatcher(int parameterIndex, float value) {
        this->invalidate();
    }

    // FUNCTION: DELAYLAMA 0x100067d0
    void EditorBase::draw() {
        if (this->window != nullptr) {
            this->window->drawControlOrSelf(NULL);
        }
    }

    // FUNCTION: DELAYLAMA 0x100068c0
    void EditorBase::idleHandler() {
        DWORD currentTick = GetTickCount();

        this->onIdle();
        
        if (currentTick < this->lastIdleTick + 100) {
            this->sleep(4);
            currentTick += 4;
            if (currentTick < this->lastIdleTick + 50) {
                return;
            }
        }

        MSG msg;
        if (PeekMessage(&msg, nullptr, WM_PAINT, WM_PAINT, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }
    
        this->lastIdleTick = currentTick;
        isInIdleUpdate = true;
        if (this->mainPlugin)
        {
            mainPlugin->sendIdleToHost();
        }
        isInIdleUpdate = false;
    }

    // FUNCTION: DELAYLAMA 0x10003730
    void EditorBase::invalidate() {
        this->needsRedraw = true;
    }

    // FUNCTION: DELAYLAMA 0x10006860
    bool EditorBase::onMouseWheel(float wheelDelta) {
        if (this->window != nullptr) {
            this->window->onMouseWheel(nullptr, nullptr, wheelDelta);
            return true;
        }
        return false;
    }

    // FUNCTION: DELAYLAMA 0x100068a0
    void EditorBase::sleep(DWORD milliseconds) {
        Sleep(milliseconds);
    }
}
}