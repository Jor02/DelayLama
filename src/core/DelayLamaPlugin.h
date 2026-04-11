#pragma once

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