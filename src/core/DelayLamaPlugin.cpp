#include "DelayLamaPlugin.h"
#include "gui/editor/DelayLamaEditor.h"

namespace DelayLama {
namespace Core {

    // FUNCTION DELAYLAMA: 0x10003470
    DelayLamaPlugin::DelayLamaPlugin(DamSDK::Api::dispatchFunc hostCallback) : DelayLamaAudio(hostCallback)  {
        this->plugin.id = 'AnDl';
        Gui::DelayLamaEditor* editor = new Gui::DelayLamaEditor(this);
        this->editor = editor;
        if (editor == nullptr)
            g_bInitFailed = true;
    }

    // FUNCTION DELAYLAMA: 0x10003500
    DelayLamaPlugin::~DelayLamaPlugin() {
    }

    // FUNCTION DELAYLAMA: 0x10003530
    void DelayLamaPlugin::setParameterValue(int parameterIndex,float parameterValue) {
        DelayLamaAudio::setParameterValue(parameterIndex,parameterValue);

        Gui::DelayLamaEditor* editor = (Gui::DelayLamaEditor*)this->editor;
        if (editor != nullptr) {
            editor->dispatcher(parameterIndex,parameterValue);
        }
    }

}
}