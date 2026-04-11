#pragma once
#include "DelayLamaAudio.h"

namespace DelayLama {
namespace Core {

    extern bool g_bInitFailed;

    class DelayLamaPlugin : public DelayLamaAudio {
    public:
        explicit DelayLamaPlugin(DamSDK::Api::dispatchFunc hostCallback);
        virtual ~DelayLamaPlugin();
    };
}
}