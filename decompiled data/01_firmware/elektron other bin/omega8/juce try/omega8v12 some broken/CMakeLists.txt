# Omega8 — JUCE + Visual Studio build (alternative to Omega8.jucer)
# Usage:
#   1) Put JUCE next to this folder (or set JUCE_DIR), e.g. C:\JUCE
#   2) cmake -B build -G "Visual Studio 17 2022" -DJUCE_DIR=C:/JUCE
#   3) cmake --build build --config Release
cmake_minimum_required(VERSION 3.22)
project(Omega8 VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED JUCE_DIR)
    set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE")
endif()
add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/juce EXCLUDE_FROM_ALL)

juce_add_plugin(Omega8
    COMPANY_NAME "Glassg"
    BUNDLE_ID com.glassg.omega8
    PLUGIN_MANUFACTURER_CODE GlSs
    PLUGIN_CODE OmE8
    FORMATS VST3 Standalone
    PRODUCT_NAME "Omega8"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES Instrument Synth)



target_sources(Omega8 PRIVATE
    Source/PluginProcessor.cpp)

target_compile_definitions(Omega8 PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_STRICT_LEAKCHECKER=1)

target_link_libraries(Omega8
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags)
