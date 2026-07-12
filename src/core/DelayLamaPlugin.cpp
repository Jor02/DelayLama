#include "DelayLamaPlugin.h"
#include "gui/editor/DelayLamaEditor.h"
#include "utils/Logger.h"

namespace DelayLama {
namespace Core {

    // FUNCTION DELAYLAMA: 0x10003470
    DelayLamaPlugin::DelayLamaPlugin(DamSDK::Api::dispatchFunc hostCallback) : DelayLamaAudio(hostCallback)  {
        Utils::log("DelayLamaPlugin::ctor\n");

        this->plugin.id = 'AnDl';
        Gui::DelayLamaEditor* editor = new Gui::DelayLamaEditor(this);
        this->editor = editor;
        if (editor == nullptr) {
            Utils::log("DelayLamaPlugin::ctor - editor allocation failed\n");
            g_bInitFailed = true;
        }
    }

    // FUNCTION DELAYLAMA: 0x10003500
    DelayLamaPlugin::~DelayLamaPlugin() {
    }

    // FUNCTION DELAYLAMA: 0x10003530
    void DelayLamaPlugin::setParameterValue(int32_t parameterIndex,float parameterValue) {
        Utils::logf("DelayLamaPlugin::setParameterValue id=%d value=%f\n", parameterIndex, parameterValue);
        DelayLamaAudio::setParameterValue(parameterIndex,parameterValue);

        Gui::DelayLamaEditor* editor = (Gui::DelayLamaEditor*)this->editor;
        if (editor != nullptr) {
            editor->dispatcher(parameterIndex,parameterValue);
        }
    }

    // STUB: DELAYLAMA 0x10003520
    void DelayLamaPlugin::destroy() {
        Utils::log("DelayLamaPlugin::destroy\n");
        // this->vtable = &DelayLama_vtable;
        // DelayLamaAudio::destroy((DelayLamaAudio *)this);
        // return;
    }
}
}