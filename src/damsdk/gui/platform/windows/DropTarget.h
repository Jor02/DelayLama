#include <windows.h>
#include <ole2.h>
#include <objidl.h>
#include "utils/portable_stdint.h"

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows { class Window; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {
    class DropTarget {
        int refCount;
        bool canAcceptDrop;
        char field3_0x9;
        char field4_0xa;
        char field5_0xb;
        Window *parentFrame;
    public:
        DropTarget(Window* frame);
        void destroy();
        HRESULT queryInterface(IID* riid, void* * ppvObject);
        int addRef();
        int release();
        HRESULT dragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
        HRESULT dragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
        HRESULT returnZero();
        void drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
        HRESULT resolveShortcutTarget();
        void __chkstk();
    };
}
}
}
}