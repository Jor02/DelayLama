#pragma once
#include "DelayLamaAudio.h"

namespace DelayLama {
namespace Core {

    extern bool g_bInitFailed;

    // VTABLE: DELAYLAMA 0x1000b694
    class DelayLamaPlugin : public DelayLamaAudio {
    public:
        explicit DelayLamaPlugin(DamSDK::Api::dispatchFunc hostCallback);
        virtual ~DelayLamaPlugin();
        virtual void setParameterValue(int32_t parameterIndex,float parameterValue) override;
        virtual void destroy() override;
    };
}
}