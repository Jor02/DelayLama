#include <windowsx.h>
#include <cstdio>
#include "Window.h"
#include "gui/editor/DelayLamaEditor.h"
#include "platform/GDIDrawingContext.h"

namespace DelayLama {
namespace Gui {
namespace Base {

    static LRESULT CALLBACK pluginWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static char g_szWindowClassName[64];
    static int g_RegistrationCount = 0;

    Window::Window(RECT *pRect,HWND hParent, DelayLamaEditor *editor) : View(pRect) {
        this->editor = editor;
        this->handle = hParent;
        this->backgroundBitmap = NULL;
        this->numChildren = 0;
        this->maxChildren = 0;
        this->children = NULL;
        this->modalView = NULL;
        this->colors = nullptr;
        this->redrawPending = true;
        this->unused3[1] = 0;
        this->isActive = false;
        this->visible = true;
        this->closeParameter = nullptr;
        this->hWnd = nullptr;
        openPluginWindow(hParent);
    }

    bool Window::openPluginWindow(HWND hParent) {
        HWND hChild;
  
        if (hParent == NULL) {
            return false;
        }

        Window::registerWindowClass();

        int width  = this->modalView->rect.right - this->modalView->rect.left;
        int height = this->modalView->rect.bottom - this->modalView->rect.top;

        hChild = CreateWindowExA(
            0,
            (LPCSTR)g_szWindowClassName,
            "Window",
            WS_CHILD | WS_VISIBLE,
            0, 0,
            width, height,
            this->handle,
            nullptr,
            Platform::g_hInstance,
            nullptr
        );
        this->hWnd = hChild;

        SetWindowLongA(hChild,GWL_USERDATA,(LONG)this);
        // setDragAndDropState(this,true);
        
        return true;
    }

    bool Window::registerWindowClass()
    {
        g_RegistrationCount++;
        if (g_RegistrationCount == 1)
        {
            // Generate unique class name: "Plugin" + hex instance handle
            sprintf(g_szWindowClassName, "Plugin%08x", (unsigned int)Platform::g_hInstance);

            WNDCLASSA wc = {};
            wc.style         = CS_GLOBALCLASS;
            wc.lpfnWndProc   = pluginWndProc;
            wc.hInstance     = Platform::g_hInstance;
            wc.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
            wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
            wc.lpszClassName = g_szWindowClassName;

            RegisterClassA(&wc);
        }
        return true;
    }

    static LRESULT CALLBACK pluginWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Window* parentFramePtr = (Window*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

        switch (uMsg)
        {
            case WM_DESTROY:
            {
                if (parentFramePtr != nullptr) {
                    parentFramePtr->visible = false;
                    parentFramePtr->handle = nullptr;
                }
                break;
            }

            case WM_PAINT:
            {
                if (parentFramePtr != nullptr && GetUpdateRect(hWnd, nullptr, FALSE)) {
                    PAINTSTRUCT ps;
                    HDC hdcPaint = BeginPaint(hWnd, &ps);
                    
                    Platform::GDIDrawingContext* drawingContext = new Platform::GDIDrawingContext(parentFramePtr, hdcPaint, hWnd);
                    parentFramePtr->modalView->onDraw(drawingContext);
                    
                    delete drawingContext;
                    
                    EndPaint(hWnd, &ps);
                    return 0;
                }
                break;
            }

            case WM_CTLCOLOREDIT:
            {
                if (parentFramePtr != nullptr && parentFramePtr->colors != nullptr) {
                    HDC hdc = (HDC)wParam; 

                    uint32_t rawTextColor = parentFramePtr->colors[30];
                    COLORREF textColor = RGB(rawTextColor & 0xFF, (rawTextColor >> 8) & 0xFF, (rawTextColor >> 16) & 0xFF);
                    SetTextColor(hdc, textColor);

                    uint32_t rawBgColor = parentFramePtr->colors[31];
                    COLORREF bgColor = RGB(rawBgColor & 0xFF, (rawBgColor >> 8) & 0xFF, (rawBgColor >> 16) & 0xFF);
                    SetBkColor(hdc, bgColor);

                    if (parentFramePtr->colors[37] != 0) {
                        DeleteObject((HGDIOBJ)parentFramePtr->colors[37]);
                    }

                    HBRUSH hBrush = CreateSolidBrush(bgColor);
                    parentFramePtr->colors[37] = (uint32_t)hBrush;

                    return (LRESULT)hBrush;
                }
                break;
            }

            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            {
                if (parentFramePtr != nullptr) {
                    HDC hdcPaint = GetDC(hWnd);
                    Platform::GDIDrawingContext* drawingContext = new Platform::GDIDrawingContext(parentFramePtr, hdcPaint, hWnd);

                    // The decompiler was manually constructing a Point struct from lParam bytes
                    POINT pt;
                    pt.x = GET_X_LPARAM(lParam);
                    pt.y = GET_Y_LPARAM(lParam);

                    parentFramePtr->modalView->onMouseDown(drawingContext, &pt);

                    delete drawingContext;
                    ReleaseDC(hWnd, hdcPaint);
                    return 0;
                }
                break;
            }
        }

        // LAB_100084e8: Default window procedure
        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }
}
}
}

