#include <cstdio>
#include <windowsx.h>
#include "Window.h"
#include "damsdk/gui/controls/Control.h"
#include "damsdk/api/AudioBaseExtended.h"
#include "damsdk/api/EditorBase.h"
#include "GDIDrawingContext.h"
#include "DropTarget.h"
#include "Bitmap.h"

// Logging
#include "utils/Logger.h"

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    static LRESULT CALLBACK pluginWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static char g_szWindowClassName[64];
    static int g_RegistrationCount = 0;

    // FUNCTION: DELAYLAMA 0x100072a0
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

    // STUB: DELAYLAMA 0x10007330
    Window::~Window() {
        cleanup();
    }

    // STUB: DELAYLAMA 0x100071e0
    void Window::resetVtable(Window* frame) {
        // frame->vtable = (WindowVTable *)&ViewVTable_1000bbec;
        // return;
    }

    // FUNCTION: DELAYLAMA 0x10007520
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

    // FUNCTION: DELAYLAMA 0x100078b0
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

    // FUNCTION: DELAYLAMA 0x10007450
    bool Window::openPluginWindow(HWND hParent) {
        HWND hChild;
  
        if (hParent == nullptr) {
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

    // FUNCTION: DELAYLAMA 0x10007a10
    void Window::setBackgroundBitmap(Bitmap *background) {
        if (this->backgroundBitmap != nullptr) {
            Bitmap::unregisterBitmap(this->backgroundBitmap);
        }
        this->backgroundBitmap = background;
        
        if (background != nullptr) {
            View::useBitmap(background);
        }
    }

    // FUNCTION: DELAYLAMA 0x100082a0
    bool Window::registerWindowClass()
    {
        g_RegistrationCount++;
        if (g_RegistrationCount == 1)
        {
            // Generate unique class name: "Plugin" + hex instance handle
            sprintf(g_szWindowClassName, "Plugin%08x", (unsigned int)Windows::g_hInstance);

            WNDCLASSA windowClass;
            windowClass.style         = CS_GLOBALCLASS;
            windowClass.lpfnWndProc   = pluginWndProc;
            windowClass.cbClsExtra = 0;
            windowClass.cbWndExtra = 0;
            windowClass.hInstance     = Windows::g_hInstance;
            windowClass.hIcon = nullptr;
            windowClass.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
            windowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
            windowClass.lpszMenuName = nullptr;
            windowClass.lpszClassName = (LPCSTR)&g_szWindowClassName;

            RegisterClassA(&windowClass);
        }
        return true;
    }

    // FUNCTION: DELAYLAMA 0x10008360
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

    // STUB: DELAYLAMA 0x100077e0
    bool Window::onMouseWheel(GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta) {
        // Control *child;
        // HDC hDC;
        // GDIDrawingContext *newDrawingContext;
        // void *drawingContextMemory;
        // LONG LStack_10;
        // undefined1 *puStack_8;
        // undefined4 __som;
        
        // __som = 0xffffffff;
        // puStack_8 = &LAB_1000accb;
        // child = getChildAtMousePos(this);
        // newDrawingContext = NULL;
        // if (child != NULL) {
        //     hDC = GetDC(this->hWnd);
        //     drawingContextMemory = operator_new(0x74);
        //     __som = 0;
        //     if (drawingContextMemory != NULL) {
        //     newDrawingContext =
        //         GDIDrawingContext::GDIDrawingContext(drawingContextMemory,this,hDC,this->hWnd);
        //     }
        //     __som = 0xffffffff;
        //     if (newDrawingContext != NULL) {
        //     drawingContextMemory = NULL;
        //     LStack_10 = 0;
        //     getLocalMousePos(this,(POINT *)&drawingContextMemory);
        //     (*(child->vtable->view).onMouseWheel)(newDrawingContext,&drawingContextMemory,scrollDelta);
        //     (*newDrawingContext->vtable->Destructor_0x0)(1);
        //     }
        //     ReleaseDC(this->hWnd,hDC);
        // }
        // return;
        return false;
    }

    // STUB: DELAYLAMA 0x100075c0
    void Window::drawControlOrSelf(Controls::Control *target) {        
        Controls::Control* targetControl = NULL;
        if (target != NULL) {
            int i = 0;
            if (0 < (int)this->numChildren) {
            Controls::Control ** currentChild = this->children;
            do {
                if (*currentChild == target) {
                targetControl = this->children[i];
                break;
                }
                i += 1;
                currentChild = currentChild + 1;
            } while (i < (int)this->numChildren);
            }
        }
        HDC hDC = GetDC(this->hWnd);

        GDIDrawingContext* drawingContext = new GDIDrawingContext(this,hDC,this->hWnd);
        if (drawingContext != nullptr) {
            if (targetControl == nullptr) {
                this->onDraw(drawingContext);
            }
            else {
                targetControl->onDraw(drawingContext);
            }
            delete drawingContext;
        }
        ReleaseDC(this->hWnd,hDC);
        return;
    }

    // FUNCTION: DELAYLAMA 0x10007960
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

    // STUB: DELAYLAMA 0x10007920
    bool Window::needsRedraw() {
        return true; // TODO: Implement actual implementation.
    }

    // FUNCTION: DELAYLAMA 0x10007a40
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

    // STUB: DELAYLAMA 0x10007350
    void Window::cleanup() {
        // bool callExtraFlag;
        // Window *local_10;
        // undefined1 *puStack_8;
        // undefined4 local_4;
        //
        // puStack_8 = &LAB_1000ac88;
        // this->vtable = &WindowVTable_1000bc48;
        // local_4 = 0;
        // local_10 = this;
        // GDIDrawingContext::setCursor((GDIDrawingContext *)this,0);
        // setDragAndDropState(this,false);
        // callExtraFlag = true;
        // destroyChildren(this,&callExtraFlag);
        // if (this->backgroundBitmap != (Bitmap *)0x0) {
        //   Bitmap::unregisterBitmap(this->backgroundBitmap);
        // }
        // if (this->hWnd != (HWND)0x0) {
        //   SetWindowLongA(this->hWnd,-0x15,0);
        //   DestroyWindow(this->hWnd);
        //   GDIDrawingContext::unregisterClass();
        // }
        // if (this->isActive != false) {
        //   closeWindow(this);
        // }
        // if (this->closeParameter != (void *)0x0) {
        //   free(this->closeParameter);
        // }
        // resetVtable(this);
        // return;
    }

    // STUB: DELAYLAMA 0x10007410
    bool Window::closeWindow() {
        // if (((this->isActive != false) && (this->visible != false)) && (this->handle != (HWND)0x0)) {
        //   (*(((this->editor->guiEditor).mainPlugin)->vtable->audioVtable).audioEffectX.
        //     closePluginEditorOnHost)(this->closeParameter);
        //   this->handle = (HWND)0x0;
        //   return true;
        // }
        return false;
    }

    // STUB: DELAYLAMA 0x100074c0
    bool Window::setDragAndDropState(bool enable) {
        // DropTarget *pDropTarget;
        //
        // if ((this->field13_0x56 != '\0') || (enable)) {
        //   if (this->hWnd == (HWND)0x0) {
        //     return false;
        //   }
        //   if (enable) {
        //     pDropTarget = createDropTarget(this);
        //     RegisterDragDrop(this->hWnd,(LPDROPTARGET)pDropTarget);
        //     this->field13_0x56 = 1;
        //     return true;
        //   }
        //   RevokeDragDrop(this->hWnd);
        //   this->field13_0x56 = 0;
        // }
        return true;
    }

    // STUB: DELAYLAMA 0x10007690
    void Window::onMouseDown(GDIDrawingContext* drawingContext, POINT* mousePos) {
        // bool childEnabled;
        // Control *currentChild;
        // int i;
        // View *modal;
        //
        // if (this->colors != (Color *)0x0) {
        //   (**(code **)(this->colors->rgba + 0x18))(0);
        //   this->colors = (Color *)0x0;
        // }
        // modal = this->modalView;
        // if (modal == nullptr) {
        //   i = this->numChildren;
        //   do {
        //     do {
        //       i = i + -1;
        //       if (i < 0) {
        //         return;
        //       }
        //       childEnabled = (bool)(*(this->children[i]->vtable->view).getEnabled)();
        //     } while (childEnabled == false);
        //     currentChild = this->children[i];
        //   } while ((((mousePos->x < (currentChild->members).viewMembers.rect.left) ||
        //             ((currentChild->members).viewMembers.rect.right < mousePos->x)) ||
        //            (mousePos->y < (currentChild->members).viewMembers.rect.top)) ||
        //           ((currentChild->members).viewMembers.rect.bottom < mousePos->y));
        //   (*(this->children[i]->vtable->view).onMouseDown)(drawingContext,mousePos);
        // }
        // else if ((((modal->members).rect.left <= mousePos->x) &&
        //          (mousePos->x <= (modal->members).rect.right)) &&
        //         (((modal->members).rect.top <= mousePos->y &&
        //          (mousePos->y <= (modal->members).rect.bottom)))) {
        //   (*modal->vtable->onMouseDown)(drawingContext,mousePos);
        //   return;
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10007740
    bool Window::routeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT* mousePos) {
        // Control *pCVar1;
        // bool childEnabled;
        // bool cVar2;
        // int i;
        //
        // if ((this->modalView != nullptr) || (this->colors != (Color *)0x0)) {
        //   return false;
        // }
        // i = this->numChildren + -1;
        // if (i < 0) {
        //   return false;
        // }
        // do {
        //   childEnabled = (bool)(*(this->children[i]->vtable->view).getEnabled)();
        //   if (childEnabled != false) {
        //     pCVar1 = this->children[i];
        //     if (((((pCVar1->members).viewMembers.rect.left <= mousePos->x) &&
        //          (mousePos->x <= (pCVar1->members).viewMembers.rect.right)) &&
        //         ((pCVar1->members).viewMembers.rect.top <= mousePos->y)) &&
        //        ((mousePos->y <= (pCVar1->members).viewMembers.rect.bottom &&
        //         (cVar2 = (bool)(*(pCVar1->vtable->view).routeMessage)(uMsg,wParam,lParam,mousePos),
        //         cVar2 != false)))) {
        //       return true;
        //     }
        //   }
        //   i = i + -1;
        //   if (i < 0) {
        //     return false;
        //   }
        // } while( true );
        return false;
    }

    // STUB: DELAYLAMA 0x10007ac0
    bool Window::removeChild(Controls::Control* child, bool shouldRelease) {
        // bool bVar1;
        // int iVar2;
        // undefined3 in_stack_00000009;
        //
        // iVar2 = 0;
        // bVar1 = false;
        // if (0 < (int)this->numChildren) {
        //   do {
        //     if (bVar1) {
        //       this->children[iVar2 + -1] = this->children[iVar2];
        //     }
        //     if (this->children[iVar2] == child) {
        //       (*(child->vtable->view).attached)(this);
        //       if (*_shouldRelease != '\0') {
        //         (*(child->vtable->view).release)();
        //       }
        //       bVar1 = true;
        //     }
        //     iVar2 = iVar2 + 1;
        //   } while (iVar2 < (int)this->numChildren);
        //   if (bVar1) {
        //     this->numChildren = this->numChildren + -1;
        //   }
        // }
        return true;
    }

    // STUB: DELAYLAMA 0x10007b30
    bool Window::destroyChildren(bool* callExtraFlag) {
        // int i;
        // int curChild;
        //
        // if (this->colors != (Color *)0x0) {
        //   (**(code **)(this->colors->rgba + 0x18))(0);
        //   this->colors = (Color *)0x0;
        // }
        // if (this->children == (Control **)0x0) {
        //   return true;
        // }
        // i = 0;
        // if (0 < (int)this->numChildren) {
        //   do {
        //     (*(this->children[i]->vtable->view).attached)(this);
        //     if (*callExtraFlag != false) {
        //       (*(this->children[i]->vtable->view).release)();
        //     }
        //     curChild = i + 1;
        //     this->children[i] = (Control *)0x0;
        //     i = curChild;
        //   } while (curChild < (int)this->numChildren);
        // }
        // free(this->children);
        // this->children = (Control **)0x0;
        // this->numChildren = 0;
        // this->maxChildren = 0;
        return true;
    }

    // STUB: DELAYLAMA 0x10007bb0
    bool Window::containsChild(Controls::Control* target) {
        bool output = false;
        int i = 0;
        if (0 < (int)this->numChildren) {
          Controls::Control ** children = this->children;
          while (*children != target) {
            i = i + 1;
            children = children + 1;
            if ((int)this->numChildren <= i) {
              return output;
            }
          }
          output = true;
        }
        return output;
    }

    // STUB: DELAYLAMA 0x10007be0
    int32_t Window::setModalView(Base::View* view) {
        if ((view != nullptr) && (this->modalView != nullptr)) {
          return 0;
        }
        if (this->modalView != nullptr) {
          this->modalView->returnTrue1(this);
        }
        this->modalView = view;
        if (view != nullptr) {
          view->returnTrue2(this);
        }
        return 1;
    }

    // FUNCTION: DELAYLAMA 0x10007c20
    void Window::beginEdit(int parameterId) {
        if (this->editor != nullptr) {
            this->editor->mainPlugin->notifyHostClientBeginningParameterEdit(parameterId);
        }
    }

    // FUNCTION: DELAYLAMA 0x10007c40
    void Window::endEdit(int parameterId) {
        if (this->editor != nullptr) {
            this->editor->mainPlugin->notifyHostClientEndingParameterEdit(parameterId);
        }
    }

    // FUNCTION: DELAYLAMA 0x10007c60
    Controls::Control* Window::getChildAtMousePos() {
        POINT mousePos;
        mousePos.x = 0;
        mousePos.y = 0;

        this->getLocalMousePos(&mousePos);
        
        int i = this->numChildren + -1;
        if (-1 < i) {
            Controls::Control ** childIter = this->children + i;
            do {
                Controls::Control* child = *childIter;
                if (((child != nullptr) && (child->rect.left <= mousePos.x)) && (mousePos.x <= child->rect.right) && ((child->rect.top <= mousePos.y && (mousePos.y <= child->rect.bottom))))
                {
                    return this->children[i];
                }
                i = i + -1;
                childIter = childIter + -1;
            } while (-1 < i);
        }
        return nullptr;
    }

    // FUNCTION: DELAYLAMA 0x10007cd0
    void Window::getLocalMousePos(POINT* mousePos) {
        HWND hWnd = this->hWnd;
        tagPOINT mousePosTarget;
        GetCursorPos(&mousePosTarget);
        mousePos->x = mousePosTarget.x;
        mousePos->y = mousePosTarget.y;
        
        if (hWnd != nullptr) {
          GetWindowRect(hWnd,&rect);
          mousePos->x = mousePos->x - rect.left;
          mousePos->y = mousePos->y - rect.top;
        }
    }

    // FUNCTION: DELAYLAMA 0x10008690
    DropTarget* Window::createDropTarget() {

        DropTarget* target = new DropTarget(this);
        if (target != nullptr) {;
          return target;
        }
        return nullptr;
    }

}
}
}
}