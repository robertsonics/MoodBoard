// ****************************************************************************
//     Filename: PluginWindow.h
// Date Created: 11/9/2025
//
//     Comments: Plugin window module header
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
#pragma once

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
