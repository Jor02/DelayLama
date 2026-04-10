#include "DelayLamaAudio.h"
#include "PluginConfig.h"

namespace DelayLama {
namespace Core {
    DelayLamaAudio::DelayLamaAudio(DamSDK::dispatchFunc hostCallback) : DamSDK::AudioBaseExtended(hostCallback, PRESET_COUNT, PARAMETER_COUNT) {
        this->plugin.inputChannelCount = 0;
        this->plugin.outputChannelCount = 2;
        this->setSupportsInPlaceProcessing(true);
        this->setReportsLoudnessToHost(false);
        this->setHasClip(false);
        this->setIsSynthesizer(true);
        this->plugin.id = 'AnDl';

    }
    DelayLamaAudio::~DelayLamaAudio() {}

    bool DelayLamaAudio::getPluginName(char *outText) {
        strcpy(outText, "Delay Lama");
        return true;
    }
    bool DelayLamaAudio::getCompanyName(char *outText) {
        strcpy(outText, "AudioNerdz");
        return true;
    }
}
}