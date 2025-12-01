// **************************************************************************
//     Filename: LevelMeter.h
// Date Created: 1/30/2022
//
//     Comments: Level meter component module header
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
// **************************************************************************

#pragma once

#include <JuceHeader.h>

//==============================================================================
class VerticalMeter  : public juce::Component {
public:
    VerticalMeter() {

        currentLevel = -60.0f;
    }

    ~VerticalMeter() override {
    }

    void setLevel(float newLevel) {
        targetLevel = newLevel;
        if (targetLevel > currentLevel)
            currentLevel = targetLevel;
        else if ((currentLevel - targetLevel) > 1.5f)
            currentLevel -= 1.5f;
        else
            currentLevel = targetLevel;
        repaint();
    }

    void paint (juce::Graphics& g) override {

        auto bounds = getLocalBounds().toFloat();
        g.setColour(Colour(0xff323232));
        g.fillRect(bounds);
        g.setGradientFill(gradient);
        const auto scaledY = jmap(currentLevel, -60.0f, 6.0f, 0.0f, static_cast<float>(getHeight()));
        g.fillRect(bounds.removeFromBottom(scaledY));
    }

    void resized() override {

        auto bounds = getLocalBounds().toFloat();
        gradient = ColourGradient {
            Colours::deepskyblue,
            bounds.getBottomLeft(),
            Colours::red,
            bounds.getTopLeft(),
            false
        };
        gradient.addColour(0.75, Colours::skyblue);
    }

private:

    ColourGradient gradient;
    float currentLevel;
    float targetLevel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VerticalMeter)
};

//==============================================================================
class HorizontalMeter  : public juce::Component {
public:
    HorizontalMeter() {

        currentLevel = -30.0f;
    }

    ~HorizontalMeter() override {
    }

    void setLevel(float newLevel) {
        targetLevel = newLevel;
        if (targetLevel > currentLevel)
            currentLevel = targetLevel;
        else if ((currentLevel - targetLevel) > 0.5f)
            currentLevel -= 0.5f;
        else
            currentLevel = targetLevel;
        repaint();
    }

    void paint (juce::Graphics& g) override {

        auto bounds = getLocalBounds().toFloat();
        g.setColour(Colour(0xff323232));
        g.fillRect(bounds);
        g.setGradientFill(gradient);
        const auto scaledX = jmap(currentLevel, -30.0f, 0.0f, 0.0f, static_cast<float>(getWidth()));
        g.fillRect(bounds.removeFromLeft(scaledX));
    }

    void resized() override {

        auto bounds = getLocalBounds().toFloat();
        gradient = ColourGradient {
            Colours::deepskyblue,
            bounds.getBottomLeft(),
            Colours::red,
            bounds.getBottomRight(),
            false
        };
        gradient.addColour(0.75, Colours::skyblue);
    }

private:

    ColourGradient gradient;
    float currentLevel;
    float targetLevel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HorizontalMeter)
};