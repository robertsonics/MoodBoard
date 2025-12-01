// ****************************************************************************
//     Filename: MainComponent.cpp
// Date Created: 11/9/2025
//
//     Comments: Juce main omponent module
//               Build Environment: VSC, CMake, Juce
//
// This file is part of the MoodBoard Project.
//
// MIT License
//
// Copyright (c) 2025 Jamie Robertson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// ****************************************************************************

#include "MainComponent.h"


// ****************************************************************************
MainComponent::MainComponent() {

    pluginsActive = false;

    menuBar = std::make_unique<juce::MenuBarComponent>(this);
    addAndMakeVisible(menuBar.get());

    RuntimePermissions::request (RuntimePermissions::recordAudio,
                                [this] (bool granted) {
                                    int numInputChannels = granted ? 2 : 0;
                                    audioDeviceManager.initialise (numInputChannels, 2, nullptr, true, {}, nullptr);
                                });

    // Initialize our device manager and fetch the available devices and the
    //  current setup.
    audioDeviceManager.initialise (2, 2, nullptr, true);
    auto devSetup = audioDeviceManager.getAudioDeviceSetup();
    auto& types = audioDeviceManager.getAvailableDeviceTypes();

    // Search for the Focusrite ASIO driver and switch to it if found
    for (auto* type : types)
    {
        if (type->getTypeName() == "ASIO")
        {
            audioDeviceManager.setCurrentAudioDeviceType ("ASIO", true);
            auto devices = type->getDeviceNames();
            for (auto& d : devices)
            {
                //if (d.containsIgnoreCase ("Focusrite USB ASIO") || d.containsIgnoreCase ("ASIO4ALL"))
                if (d.containsIgnoreCase ("Focusrite USB ASIO"))
                {
                    devSetup.inputDeviceName = d;
                    devSetup.outputDeviceName = d;
                    devSetup.inputChannels = 0;
                    devSetup.outputChannels = 0;
                    devSetup.sampleRate = mySampleRate;
                    devSetup.bufferSize = myBufferSize;
                    devSetup.useDefaultInputChannels = true;
                    devSetup.useDefaultOutputChannels = true;
                    juce::String error = audioDeviceManager.setAudioDeviceSetup(devSetup, true);
                    if (error.isNotEmpty())
                        juce::Logger::writeToLog ("ASIO Found");
                    break;
                }
            }
        }
    }

    // Load our main window background image
    auto* data = BinaryData::pedalboard_jpg;
    int   size = BinaryData::pedalboard_jpgSize;
    backgroundImage = juce::ImageFileFormat::loadFrom(data, size);

    // Set up our level meter
    addAndMakeVisible(levelMeter);

    audioDeviceManager.initialiseWithDefaultDevices(2, 2);
    audioDeviceManager.addAudioCallback(this);

    peakReset = true;
    startTimer(30);

    setSize (800, 600);

    // Call the function to look for and load our plugins. We do it this way
    //  so that our main window has a chance to get painted before we put up
    //  the scanning plugins dialog window
    juce::MessageManager::callAsync(
    [safe = juce::Component::SafePointer<MainComponent>(this)]
    {
        if (safe != nullptr)
            safe->startPluginScan();
    });

}

// ****************************************************************************
MainComponent::~MainComponent() {

    scanDialog.reset();
    audioDeviceManager.removeAudioCallback(this);
    granularPluginWindow = nullptr;
    if (granularPlugin != nullptr) {
        granularPlugin->releaseResources();
        granularPlugin.reset();
    }
    granularPluginBuffer.setSize (0, 0);
}

// ****************************************************************************
void MainComponent::paint (juce::Graphics& g) {

    if (backgroundImage.isValid()) {
        // The image will automatically scale to fill the new size
        g.drawImage(backgroundImage, getLocalBounds().toFloat(),
        juce::RectanglePlacement::fillDestination);
    }
    else {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
        g.setFont (juce::FontOptions (16.0f));
        g.setColour (juce::Colours::white);
        g.drawText ("Mood Board", getLocalBounds(), juce::Justification::centred, true);
    }
}

// ****************************************************************************
void MainComponent::resized() {

    menuBar->setBounds (0, 0, getWidth(), 24);
    auto bounds = getLocalBounds();
        
    // Create status bar at bottom
    auto statusBar = bounds.removeFromBottom(20);
        
    // Position meter in status bar with some padding
    auto meterBounds = statusBar.reduced(1);
    levelMeter.setBounds(meterBounds);
}

// ****************************************************************************
juce::StringArray MainComponent::getMenuBarNames() {

    return { "File", "Settings", "Help" };
}

// ****************************************************************************
juce::PopupMenu MainComponent::getMenuForIndex (int topLevelMenuIndex, const juce::String&) {

    juce::PopupMenu menu;
    if (topLevelMenuIndex == 0) {
        menu.addItem (1, "Open...");
        menu.addItem (2, "Save");
        menu.addSeparator();
        menu.addItem (3, "Quit");
    }
    else if (topLevelMenuIndex == 1) {
        menu.addItem (4, "Audio Driver");
    }
    else if (topLevelMenuIndex == 2) {
        menu.addItem (5, "About");
    }
    return menu;
}

// ****************************************************************************
void MainComponent::menuItemSelected (int menuItemID, int) {

    switch(menuItemID) {
        case 4:
            auto* popup = new Settings(audioDeviceManager);
            popup->setSize (600, 400);
            juce::DialogWindow::LaunchOptions options;
            options.content.setOwned (popup);                 // take ownership
            options.dialogTitle                = "Audio Device Settings";
            options.dialogBackgroundColour     = juce::Colours::darkgrey;
            options.escapeKeyTriggersCloseButton = true;
            options.useNativeTitleBar          = true;
            options.resizable                  = false;
            options.componentToCentreAround    = this;        // center on parent
            options.launchAsync();
        break;
    }
}

// ****************************************************************************
void MainComponent::timerCallback(void) {

    levelMeter.setLevel(currentLevel.load());
    peakReset = true;
}

// ****************************************************************************
void MainComponent::audioDeviceAboutToStart(juce::AudioIODevice* device) {
}

// ****************************************************************************
void MainComponent::audioDeviceIOCallbackWithContext(
    const float* const* inputChannelData,
    int numInputChannels,
    float* const* outputChannelData,
    int numOutputChannels,
    int numSamples,
    const juce::AudioIODeviceCallbackContext& context) {

    if ((numInputChannels >= 1 ) && (numOutputChannels >= 2)) {

        if (!pluginsActive) {
            if ((inputChannelData[0] != nullptr) && 
                (outputChannelData[0] != nullptr) && (outputChannelData[1] != nullptr)) {
                memcpy(outputChannelData[0], inputChannelData[0], numSamples * sizeof(float));
                memcpy(outputChannelData[1], inputChannelData[0], numSamples * sizeof(float));
            }
        }
        else {
            for (int ch = 0; ch < numOutputChannels; ++ch) {
                if (auto* out = outputChannelData[ch]) {
                    juce::FloatVectorOperations::clear (out, numSamples);
                }
            }
            granularPluginBuffer.clear();

            granularPluginBuffer.copyFrom(0, 0, inputChannelData[0], numSamples);
            granularPluginBuffer.copyFrom(1, 0, inputChannelData[0], numSamples);
 
            juce::MidiBuffer midi; // empty MIDI for now
            granularPlugin->processBlock (granularPluginBuffer, midi);

            const float* pSrcL = granularPluginBuffer.getReadPointer(0);
            const float* pSrcR = granularPluginBuffer.getReadPointer(1);
            memcpy(outputChannelData[0], pSrcL, numSamples * sizeof(float));
            memcpy(outputChannelData[1], pSrcR, numSamples * sizeof(float));            
        }
    }    

    if (peakReset) {
        peakReset = false;
        currentLevel.store(-100.0f);
    }
    float peak = 0.0f;
    for (int i = 0; i < numSamples; i++) {
        float absolute = abs(inputChannelData[0][i]);
        if (absolute > peak)
            peak = absolute;
    }
    float peakDb = Decibels::gainToDecibels(peak);

    if (peakDb > currentLevel.load())
        currentLevel.store(peakDb);
}


// ****************************************************************************
void MainComponent::audioDeviceStopped() {

    granularPluginWindow = nullptr;

    if (granularPlugin != nullptr) {
        granularPlugin->releaseResources();
        granularPlugin.reset();
    }
    granularPluginBuffer.setSize (0, 0);
}

// ****************************************************************************
void MainComponent::startPluginScan() {

    auto* dialog = new juce::DialogWindow("MoodBoard", juce::Colours::lightgrey, true);
    auto* label = new juce::Label();
    label->setText("Scanning Plugins...", juce::dontSendNotification);
    label->setJustificationType(juce::Justification::centred);
    dialog->setContentOwned(label, true);
    dialog->centreWithSize(300, 120);
    dialog->setAlwaysOnTop(true);
    dialog->setVisible(true);
    scanDialog.reset(dialog);
    addDefaultFormatsToManager(formatManager);
    juce::Timer::callAfterDelay (2000, [this]() {
        pluginScanner();
    });
}

// ****************************************************************************
void MainComponent::pluginScanner() {

    auto* format = formatManager.getFormat(0);  // VST3 on Windows/macOS
    if (format == nullptr)
        return;

    juce::OwnedArray<juce::PluginDescription> types;
    juce::File pluginFile ("C:/Program Files/Common Files/VST3/Velvet Machine.vst3");
    format->findAllTypesForFile (types, pluginFile.getFullPathName());
    if (types.isEmpty()) {
        DBG ("Plugin not found!");
        return;
    }

    juce::String error;
    auto instance = formatManager.createPluginInstance (*types[0], mySampleRate, myBufferSize, error);
    if (instance == nullptr) {
        DBG ("Failed to create plugin instance: " << error);
        return;
    }

    granularPlugin = std::move (instance);

    auto layout = granularPlugin->getBusesLayout();
    auto stereo = juce::AudioChannelSet::stereo();
    layout.getChannelSet(true, 0)  = stereo;  // input
    layout.getChannelSet(false, 0) = stereo;  // output
    if (granularPlugin->checkBusesLayoutSupported(layout)) {
        granularPlugin->setBusesLayout(layout);
    }

    granularPluginBuffer.setSize (granularPlugin->getTotalNumOutputChannels(), myBufferSize);
    granularPlugin->prepareToPlay (mySampleRate, myBufferSize);

    scanDialog = nullptr;
    pluginsActive = true;
}

// ****************************************************************************
void MainComponent::mouseDown(const MouseEvent& event) {

int x, y;

    MouseEvent me = event.getEventRelativeTo(this);
    Point<int> p = me.getMouseDownPosition();
    x = p.getX();
    y = p.getY();

    if (! granularPlugin)
        return;

    // Only create the window if we don’t already have one
    if (granularPluginWindow == nullptr)
        granularPluginWindow = std::make_unique<PluginWindow> (*granularPlugin);
    else
        granularPluginWindow->setVisible (true);
}
