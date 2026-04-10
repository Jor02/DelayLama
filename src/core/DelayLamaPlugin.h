#pragma once
#include <cstdint>
#include <windows.h>
#include "damsdk/DamPlugin.h"
#include "core/DelayLamaAudio.h"
#include "reference/OriginalInternal.h"

namespace DelayLama {
namespace Core {
    class DelayLamaPlugin : public DelayLamaAudio {
    public:
        explicit DelayLamaPlugin(DamSDK::dispatchFunc hostCallback);
        virtual ~DelayLamaPlugin();
    };
}
}