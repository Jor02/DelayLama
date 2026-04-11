
namespace DelayLama {
namespace Core {
    // Constructor
    DelayLamaPlugin::DelayLamaPlugin(DamSDK::dispatchFunc hostCallback) : DelayLamaAudio(hostCallback)  {
        this->plugin.id = 'AnDl';
        Gui::DelayLamaEditor* editor = new Gui::DelayLamaEditor(this);
        this->editor = editor;
        if (editor == nullptr)
            g_bInitFailed = true;
    }

    // Destructor
    DelayLamaPlugin::~DelayLamaPlugin() {
    }
}
}