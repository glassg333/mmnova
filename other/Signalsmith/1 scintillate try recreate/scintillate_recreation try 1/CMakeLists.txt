cmake_minimum_required(VERSION 3.24)
project(ScintillateRecreation VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(juce)

FetchContent_Declare(
    signalsmith_fft
    GIT_REPOSITORY https://github.com/Signalsmith-Audio/fft.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(signalsmith_fft)

juce_add_plugin(ScintillateRecreation
    COMPANY_NAME "Prototype Audio"
    PLUGIN_MANUFACTURER_CODE PtAu
    PLUGIN_CODE ScR1
    FORMATS VST3 Standalone
    PRODUCT_NAME "Scintillate Recreation"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(ScintillateRecreation PRIVATE
    plugin/PluginProcessor.cpp
    plugin/PluginProcessor.h
    plugin/PluginEditor.cpp
    plugin/PluginEditor.h
    src/ScintillateDSP.cpp
    include/scintillate/ScintillateDSP.h
)

target_include_directories(ScintillateRecreation PRIVATE
    include
    ${signalsmith_fft_SOURCE_DIR}
)

target_compile_features(ScintillateRecreation PRIVATE cxx_std_17)

target_link_libraries(ScintillateRecreation PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

target_compile_definitions(ScintillateRecreation PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
