class PluginWindow : public juce::DocumentWindow
{
public:
    PluginWindow (juce::AudioPluginInstance& instance)
        : DocumentWindow (instance.getName(),
                          juce::Colours::darkgrey,
                          DocumentWindow::closeButton),
          plugin (instance) {
            
        setUsingNativeTitleBar (true);

        // createEditorIfNeeded() will reuse the existing editor if there is one,
        // or create a new one if not.
        setContentOwned (plugin.createEditorIfNeeded(), true);

        // Size to the editor
        if (auto* ed = plugin.getActiveEditor())
            setResizable (true, false),
            setResizeLimits (200, 100, 2000, 2000),
            centreAroundComponent (ed, ed->getWidth(), ed->getHeight());

        setVisible (true);
    }

    void closeButtonPressed() override {
        // Just hide the window; the plugin instance stays alive.
        setVisible (false);
    }

private:
    juce::AudioPluginInstance& plugin;
};
