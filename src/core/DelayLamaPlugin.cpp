#include "DelayLamaPlugin.h"

namespace DelayLama {
namespace Core {
    // Constructor
    DelayLamaPlugin::DelayLamaPlugin(DamSDK::dispatchFunc hostCallback) : DelayLamaAudio(hostCallback)  {
    }

    // Destructor
    DelayLamaPlugin::~DelayLamaPlugin() {
    }
}
}