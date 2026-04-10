#pragma once
#include <cstdint>
#include <windows.h>
#include "damsdk/DamPlugin.h"

namespace DamSDK {
    class EditorBase {
        public:
            virtual int open(HWND* hParent);
            virtual int getRect(Rect** outRect);
            virtual void close();
            virtual void onIdle();
            virtual bool keyDown(DamSDK::KeyCode* keycode);
            virtual bool keyUp(DamSDK::KeyCode* keycode);
            virtual bool setKnobMode(int32_t mode);
    };
}