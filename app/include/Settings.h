// ****************************************************************************
//     Filename: Settings.h
// Date Created: 11/9/2025
//
//     Comments: Settings module header
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
#pragma once

#include <JuceHeader.h>

// ****************************************************************************
class Settings    : public juce::Component
{
public:
	Settings(juce::AudioDeviceManager& devManager);
    ~Settings();
    
    void paint (Graphics&);
    void resized();
    

private:

    std::unique_ptr<AudioDeviceSelectorComponent> audioSetupComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Settings)
};
