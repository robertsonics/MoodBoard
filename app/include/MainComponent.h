// ****************************************************************************
//     Filename: MainComponent.h
// Date Created: 11/9/2025
//
//     Comments: Main Juce component module header
//               Build Environment: VSC, CMake, Juce
//
// COPYRIGHT (c) 2025, Robertsonics, All Rights Reserved
//
// This file is part of the Tapestry Project.
//
// Programmers: Jamie Robertson
//              robertsonics@gmail.com
//
// ****************************************************************************
#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "Settings.h"
#include "LevelMeter.h"
#include "PluginWindow.h"
//#include "PluginScanner.h"

// ****************************************************************************
// This component lives inside our window, and this is where you should put all
//   your controls and content.

class MainComponent final : public juce::Component,
                            public juce::AudioIODeviceCallback,
                            public juce::Timer,
                            public juce::MenuBarModel
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

    void menuItemSelected (int menuItemID, int) override;
    void mouseDown(const MouseEvent& event) override;
    
    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex (int topLevelMenuIndex, const juce::String&);

    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceIOCallbackWithContext(
        const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const juce::AudioIODeviceCallbackContext& context) override;
    void audioDeviceStopped() override;

    void startPluginScan();
    void pluginScanner();
    void pluginScanComplete();

    static constexpr double mySampleRate = 44100.0;
    static constexpr int myBufferSize = 256;

private:
    AudioDeviceManager audioDeviceManager;

    juce::Image backgroundImage;
    juce::ComponentBoundsConstrainer constrainer;

    std::unique_ptr<juce::MenuBarComponent> menuBar;

    HorizontalMeter levelMeter;
    bool peakReset;
    std::atomic<float> currentLevel { -100.0f };

    juce::AudioPluginFormatManager formatManager;

    bool pluginsActive;

    std::unique_ptr<juce::AudioPluginInstance> granularPlugin;
    juce::AudioBuffer<float> granularPluginBuffer;
    std::unique_ptr<PluginWindow> granularPluginWindow;

    std::unique_ptr<juce::DialogWindow> scanDialog;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
