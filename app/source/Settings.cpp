// ****************************************************************************
//     Filename: Settings.cpp
// Date Created: 11/9/2025
//
//     Comments: Settings component module
//               Build Environment: VSC, CMake, Juce
//
// COPYRIGHT (c) 2025, Robertsonics, All Rights Reserved
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
