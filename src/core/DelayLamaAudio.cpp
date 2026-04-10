#include "DelayLamaAudio.h"
#include "PluginConfig.h"

namespace DelayLama {
namespace Core {
    DelayLamaAudio::DelayLamaAudio(DamSDK::dispatchFunc hostCallback) : DamSDK::AudioBaseExtended(hostCallback, PRESET_COUNT, PARAMETER_COUNT) {}
    DelayLamaAudio::~DelayLamaAudio() {}
}
}