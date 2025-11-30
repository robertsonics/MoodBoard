// ****************************************************************************
//     Filename: Settings.cpp
// Date Created: 11/9/2025
//
//     Comments: Settings module
//               Build Environment: VSC, CMake, Juce
//
// COPYRIGHT (c) 2025, Robertsonics, All Rights Reserved
//
// This file is part of the MoodBoard Project.
//
// Programmers: Jamie Robertson
//              robertsonics@gmail.com
//
// ****************************************************************************

#include "Settings.h"

// ****************************************************************************
Settings::Settings(AudioDeviceManager& devManager) {
 
    audioSetupComp.reset (new AudioDeviceSelectorComponent (devManager, 0, 16, 0, 16, false, false, true, true));
    addAndMakeVisible (audioSetupComp.get());
}

// ****************************************************************************
Settings::~Settings() {
}

// ****************************************************************************
void Settings::paint (Graphics& g) {

	g.fillAll(Colours::black);
}

// ****************************************************************************
void Settings::resized() {

    auto r =  getLocalBounds().reduced (4);
    audioSetupComp->setBounds (r.removeFromTop (proportionOfHeight (0.65f)));
}
