cmake_minimum_required(VERSION 4.1.2)

project(MoodBoard VERSION 0.1.0)

juce_add_gui_app(${PROJECT_NAME}
    PRODUCT_NAME "MoodBoard"
    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/Source/JUCEAppIcon.png"

)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="MoodBoard"
        JUCE_APPLICATION_VERSION_STRING="Version 0.1.0"
        JUCE_PLUGINHOST_VST3=1
        JUCE_WASAPI=0
        JUCE_ASIO=1
)

juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        source/Main.cpp
        source/MainComponent.cpp
        source/Settings.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        # GuiAppData            # If we'd created a binary data target, we'd link to it here
        juce::juce_gui_extra
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_processors_headless
        juce::juce_audio_utils

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

