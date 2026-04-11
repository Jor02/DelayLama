#pragma once

namespace DamSDK {
namespace Gui {
namespace Base {
    class Window : public View {
        public:
            char unused1;
            DelayLamaEditor *editor;
            HWND handle;
            struct Bitmap *backgroundBitmap;
            int numChildren;
            int maxChildren;
            struct Control **children;
            struct View *modalView;
            COLORREF* colors;
            bool redrawPending;
            bool visible;
            char unused3[2];
            HWND hWnd;
            bool isActive;
            char unused4[3];
            void *closeParameter;
        public:
            Window(RECT *pRect,HWND hParent,DelayLamaEditor *editor);
            bool openPluginWindow(HWND hParent);
            static bool registerWindowClass();
    };
}
}
}

