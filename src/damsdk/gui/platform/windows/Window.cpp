#include <cstdio>
#include <windowsx.h>
#include "Window.h"
#include "damsdk/gui/controls/Control.h"
#include "GDIDrawingContext.h"
#include "Bitmap.h"

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    static LRESULT CALLBACK pluginWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static char g_szWindowClassName[64];
    static int g_RegistrationCount = 0;

    Window::Window(RECT *pRect,HWND hParent, Api::EditorBase *editor) : View(pRect) {
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

    void Window::onDraw(GDIDrawingContext *drawingContext) {
        if (this->redrawPending != false) {
            this->redrawPending = false;
        }

        Bitmap* background = this->backgroundBitmap;
        if (background != nullptr) {

            RECT destRect;
            destRect.bottom = background->height;
            destRect.right = background->width;
            destRect.left = 0;
            destRect.top = 0;

            POINT srcPoint;
            srcPoint.x = 0;
            srcPoint.y = 0;

            background->blit(drawingContext,&destRect,&srcPoint);
        }
        int i = 0;
        if (0 < (int)this->numChildren) {
            do {
                this->children[i]->isDirty();
                this->children[i]->onDraw(drawingContext);
                this->children[i]->setDirty(false);
            i += 1;
            } while (i < (int)this->numChildren);
        }
        if (this->modalView != NULL) {
            this->modalView->onDraw(drawingContext);
        }
    }

    void Window::update(GDIDrawingContext *drawingContext)
    {
        bool isActive;
        int i;
        
        if (this->visible != false) {
            if (this->modalView != nullptr) {
                this->modalView->update(drawingContext);
                return;
            }

            isActive = this->isDirty();

            if (isActive != false) {
                this->onDraw(drawingContext);
                this->setDirty(false);
                return;
            }

            i = 0;
            if (0 < (int)this->numChildren) {
            do {
                this->children[i]->update(drawingContext);
                i += 1;
            } while (i < (int)this->numChildren);
            }
        }
    }

    bool Window::openPluginWindow(HWND hParent) {
        HWND hChild;
  
        if (hParent == NULL) {
            return false;
        }

        Window::registerWindowClass();

        int width  = this->rect.right - this->rect.left;
        int height = this->rect.bottom - this->rect.top;

        hChild = CreateWindowExA(
            0,
            (LPCSTR)g_szWindowClassName,
            "Window",
            WS_CHILD | WS_VISIBLE,
            0, 0,
            width, height,
            this->handle,
            nullptr,
            Windows::g_hInstance,
            nullptr
        );
        this->hWnd = hChild;

        SetWindowLongA(hChild,GWL_USERDATA,(LONG)this);
        // setDragAndDropState(this,true);
        
        return true;
    }

    void Window::setBackgroundBitmap(Bitmap *background) {
        if (this->backgroundBitmap != nullptr) {
            Bitmap::unregisterBitmap(this->backgroundBitmap);
        }
        this->backgroundBitmap = background;
        
        if (background != nullptr) {
            View::useBitmap(background);
        }
    }

    bool Window::registerWindowClass()
    {
        g_RegistrationCount++;
        if (g_RegistrationCount == 1)
        {
            // Generate unique class name: "Plugin" + hex instance handle
            sprintf(g_szWindowClassName, "Plugin%08x", (unsigned int)Windows::g_hInstance);

            WNDCLASSA wc;
            wc.style         = CS_GLOBALCLASS;
            wc.lpfnWndProc   = pluginWndProc;
            wc.hInstance     = Windows::g_hInstance;
            wc.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
            wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
            wc.lpszClassName = g_szWindowClassName;

            RegisterClassA(&wc);
        }
        return true;
    }

    static LRESULT CALLBACK pluginWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        #ifdef GetWindowLongPtr
            Window* parentFramePtr = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        #else
            // Will break if it were to be compiled in 64-bit
            Window* parentFramePtr = (Window*)GetWindowLong(hWnd, GWL_USERDATA);
        #endif

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
                    
                    GDIDrawingContext* drawingContext = new GDIDrawingContext(parentFramePtr, hdcPaint, hWnd);
                    parentFramePtr->onDraw(drawingContext);
                    
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
                    GDIDrawingContext* drawingContext = new GDIDrawingContext(parentFramePtr, hdcPaint, hWnd);

                    // The decompiler was manually constructing a Point struct from lParam bytes
                    POINT pt;
                    pt.x = GET_X_LPARAM(lParam);
                    pt.y = GET_Y_LPARAM(lParam);

                    parentFramePtr->onMouseDown(drawingContext, &pt);

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

    void Window::onMouseWheel(GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta) {}
    void Window::drawControlOrSelf(Controls::Control *target) {}
    void Window::refresh() {
        if ((this->visible != false) && (this->redrawPending == false)) {
            if (needsRedraw()) {
                HDC hDC = GetDC(this->hWnd);
                GDIDrawingContext* drawingContext = new GDIDrawingContext(this, hDC, this->hWnd);
                if (drawingContext != NULL) {
                    this->update(drawingContext);
                    delete drawingContext;
                }
                ReleaseDC(this->hWnd,hDC);
            }
        }
    }

    bool Window::needsRedraw() {
        return true; // TODO: Implement actual implementation.
    }

    bool Window::registerControl(Controls::Control *control)
    {
        if (numChildren == maxChildren) {
            int newCapacity = maxChildren + 10;
            maxChildren = newCapacity;

            Controls::Control** newChildren;
            if (children == nullptr) {
                newChildren = static_cast<Controls::Control**>(malloc(newCapacity * sizeof(Controls::Control*)));
            } else {
                newChildren = static_cast<Controls::Control**>(realloc(children, newCapacity * sizeof(Controls::Control*)));
            }

            if (newChildren == nullptr) {
                maxChildren = 0;
                return false;
            }

            children = newChildren;
        }

        children[numChildren++] = control;

        // Set parent
        control->parent = this;

        // Call virtual function
        // control->returnTrue(this);

        return true;
    }
}
}
}
}