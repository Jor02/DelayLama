#include "damsdk/EditorBase.h"
#include "DamPlugin.h"
#include "EditorBase.h"

namespace DamSDK {

    int GLOBAL_KNOB_MODE = 0;

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

    void EditorBase::open(HWND hParent) {
        this->invalidate();
        this->hParent = hParent;
    }

    void EditorBase::getRect(Rect** outRect) {
        *outRect = &this->rect;
    }

    void EditorBase::close() {

    }

    void EditorBase::onIdle() {
        if (this->isInIdleUpdate == false) {
            if (this->needsRedraw != false) {
                this->needsRedraw = false;
                //this->unimplemented();
            }

            if (this->window != nullptr) {
                //Window::refresh(frame);
            }
        }
    }

    int32_t EditorBase::keyDown(DamSDK::KeyCode* keycode) {
        return -1;
    }
    
    int32_t EditorBase::keyUp(DamSDK::KeyCode* keycode) {
        return -1;
    }

    void EditorBase::setKnobMode(int32_t mode) {
        GLOBAL_KNOB_MODE = mode;
    }

    void EditorBase::dispatcher(int parameterIndex) {
        this->invalidate();
    }

    void EditorBase::draw() {
        // if (this->window != nullptr) {
        //     this->window->drawControlOrSelf(frame,NULL);
        // }
    }

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

    void EditorBase::invalidate() {
        this->needsRedraw = true;
    }

    bool EditorBase::onMouseWheel(float wheelDelta) {
        if (this->window != nullptr) {
            POINT zeroPoint = {0, 0};
            //this->window.onMouseWheel(0, zeroPoint, wheelDelta)
            return true;
        }
        return false;
    }

    void EditorBase::sleep(DWORD milliseconds) {
        Sleep(milliseconds);
    }
}