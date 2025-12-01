# MoodBoard - A virtual pedalboard for ambient guitar
<br><br/>
![ALT TEXT](/app/assets/pedalboard.jpg)

## Introduction

This project grew out of a desire to assemble a pedalboard for ambient guitar. It occured to me that most of the pedals that one would use for such a rig have many plugin equivalents, and that one could eliminate all of the stompboxes, power supplies and cables by creating a low-latency plugin host that would implement the same functions/fx, routing and switching.

This is a Juce Plugin Host GUI application, plumbed to create 2 signal paths from a mono guitar input. The signal first goes to an A/B switch which routes the guitar to one of two stereo paths:

* Path A: Looper -> Granular Synth -> Delay -> Reverb ->
* Path B: Guitar Multi-FX ->

The two paths are combined via a simple mixer before going to the mains output.

This allows you to build up a looping ambient pad and then play on top of that with normal guitar tones and FX. You get to choose what plugins to install in each of the positions, and the app exposes the plugin's GUI's for real-time control.

## Development Environment

* Visual Studio Code Version 1.106.3

* Juce Version 8.0.10

* Cmake Version 4.1.2

Initially, the plugins that I personally want to use are available only for Windows and Mac (not Linux), so I'm starting with support for just VST3 on Windows 11.

**Note as of 12/1/2025:** To be completed.

## Code Structure

To be completed.

## Theory of Operation

To be completed

## Building and Running the Project

To be completed

## To-Do

To be completed


