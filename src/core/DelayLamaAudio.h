#pragma once
#include "damsdk/api/AudioBaseExtended.h"

namespace DelayLama {
namespace Core {
    class DelayLamaAudio : public DamSDK::Api::AudioBaseExtended {
        public:
            DelayLamaAudio(DamSDK::Api::dispatchFunc hostCallback);
            ~DelayLamaAudio();

            bool getPluginName(char *outText) override;
            bool getCompanyName(char *outText) override;
    };
}
}