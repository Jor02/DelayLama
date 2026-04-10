#pragma once
#include <cstdint>
#include <windows.h>
#include "damsdk/DamPlugin.h"
#include "core/DelayLamaAudio.h"
#include "reference/OriginalInternal.h"

namespace DelayLama {
namespace Core {

    extern bool g_bInitFailed;

    class DelayLamaPlugin : public DelayLamaAudio {
    public:
        explicit DelayLamaPlugin(DamSDK::dispatchFunc hostCallback);
        virtual ~DelayLamaPlugin();
    };
}
}