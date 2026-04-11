
namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    HINSTANCE g_hInstance = NULL;
    
    int32_t GDIDrawingContext::setModuleHandle(HINSTANCE hInstance)
    {
        g_hInstance = hInstance;
        return 1;
    }
}
}
}
}