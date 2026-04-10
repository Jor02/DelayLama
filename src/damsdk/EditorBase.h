#pragma once
#include <cstdint>
#include <windows.h>
#include "AudioBase.h"
#include "damsdk/DamPlugin.h"

namespace DamSDK {

    extern int GLOBAL_KNOB_MODE;

    class EditorBase {
        public:
            AudioBase* mainPlugin;
            HWND hParent;
            bool needsRedraw;
            Rect rect;
            void* window; //Window* window; // (not yet implemented)
            DWORD lastIdleTick;
            bool isInIdleUpdate;
        public:
            EditorBase(AudioBase* plugin);
            virtual void open(HWND hParent);
            virtual void getRect(Rect** outRect);
            virtual void close();
            virtual void onIdle();
            virtual int32_t keyDown(DamSDK::KeyCode* keycode);
            virtual int32_t keyUp(DamSDK::KeyCode* keycode);
            virtual void setKnobMode(int32_t mode);
            
            virtual void dispatcher(int parameterIndex);
            virtual void draw();
            virtual void idleHandler();
            virtual void invalidate();
            virtual bool onMouseWheel(float wheelDelta);
            virtual void sleep(DWORD milliseconds);
    };
}