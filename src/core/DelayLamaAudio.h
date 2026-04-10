#pragma once
#include <cstdint>
#include <windows.h>
#include "damsdk/AudioBaseExtended.h"

namespace DelayLama {
namespace Core {
    class DelayLamaAudio : public DamSDK::AudioBaseExtended {
        public:
            DelayLamaAudio(DamSDK::dispatchFunc hostCallback);
            ~DelayLamaAudio();
    };
}
}