#pragma once
#include <Windows.h>
#include <windef.h>
#include "DamPlugin.h"

// Forward declarations
namespace DamSDK {
    namespace Api { class AudioBase; }
    namespace Gui {
        namespace Platform {
            namespace Windows { class Window; }
        }
    }
}

namespace DamSDK {
namespace Api {

    extern int GLOBAL_KNOB_MODE;

    // VTABLE: DELAYLAMA 0x1000bb80
    class EditorBase {
        public:
            AudioBase* mainPlugin;
            HWND hParent;
            bool needsRedraw;
            Rect rect;
            Gui::Platform::Windows::Window* window;
            DWORD lastIdleTick;
            bool isInIdleUpdate;
        public:
            EditorBase(AudioBase* plugin);
            ~EditorBase();
            virtual void open(HWND hParent);
            virtual void getRect(Rect** outRect);
            virtual void close();
            virtual void onIdle();
            virtual int32_t keyDown(KeyCode* keycode);
            virtual int32_t keyUp(KeyCode* keycode);
            virtual void setKnobMode(int32_t mode);
            
            virtual void dispatcher(int parameterIndex, float value);
            virtual void draw();
            virtual void idleHandler();
            virtual void invalidate();
            virtual bool onMouseWheel(float wheelDelta);
            virtual void sleep(DWORD milliseconds);
    };
}
}