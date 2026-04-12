#include "DelayLamaPlugin.h"
#include "gui/editor/DelayLamaEditor.h"

namespace DelayLama {
namespace Core {
    // Constructor
    DelayLamaPlugin::DelayLamaPlugin(DamSDK::Api::dispatchFunc hostCallback) : DelayLamaAudio(hostCallback)  {
        this->plugin.id = 'AnDl';
        Gui::DelayLamaEditor* editor = new Gui::DelayLamaEditor(this);
        this->editor = editor;
        if (editor == nullptr)
            g_bInitFailed = true;
    }

    // Destructor
    DelayLamaPlugin::~DelayLamaPlugin() {
    }

    void DelayLamaPlugin::setParameterValue(int parameterIndex,float parameterValue) {
        DelayLamaAudio::setParameterValue(parameterIndex,parameterValue);

        Gui::DelayLamaEditor* editor = (Gui::DelayLamaEditor*)this->editor;
        if (editor != nullptr) {
            editor->dispatcher(parameterIndex,parameterValue);
        }
    }

}
}