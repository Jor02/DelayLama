#include "GDIDrawingContext.h"

namespace DelayLama {
namespace Platform{
    HINSTANCE g_hInstance = NULL;
    
    int32_t GDIDrawingContext::setModuleHandle(HINSTANCE hInstance)
    {
        g_hInstance = hInstance;
        return 1;
    }
}
}