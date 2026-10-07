cmake_minimum_required(VERSION 3.22)

project(CombScanner VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(COMBSCANNER_FETCH_JUCE "Download JUCE when JUCE_DIR is not supplied" ON)

if (JUCE_DIR)
    add_subdirectory("${JUCE_DIR}" JUCE)
elseif (COMBSCANNER_FETCH_JUCE)
    include(FetchContent)
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.6
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(JUCE)
else()
    message(FATAL_ERROR "Set JUCE_DIR or enable COMBSCANNER_FETCH_JUCE")
endif()

add_library(combscanner_dsp STATIC src/CombScannerDSP.cpp)
target_include_directories(combscanner_dsp PUBLIC include)
target_compile_features(combscanner_dsp PUBLIC cxx_std_17)

juce_add_plugin(CombScanner
    COMPANY_NAME Restrange
    COMPANY_WEBSITE https://github.com/restrange
    COMPANY_EMAIL dev@restrange.example
    BUNDLE_ID com.restrange.combscanner
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Rstr
    PLUGIN_CODE CbS1
    FORMATS VST3 Standalone
    PRODUCT_NAME "Comb Scanner"
    DESCRIPTION "Scanning bank of nested comb filters"
)

target_sources(CombScanner PRIVATE
    plugin/PluginProcessor.cpp
    plugin/PluginProcessor.h
    plugin/PluginEditor.cpp
    plugin/PluginEditor.h
)
target_include_directories(CombScanner PRIVATE include plugin)
target_link_libraries(CombScanner PRIVATE
    combscanner_dsp
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
juce_generate_juce_header(CombScanner)

add_executable(combscanner_demo tools/render_demo.cpp src/CombScannerDSP.cpp)
target_include_directories(combscanner_demo PRIVATE include)
target_compile_features(combscanner_demo PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(combscanner_dsp PRIVATE /W4 /permissive-)
    target_compile_options(combscanner_demo PRIVATE /W4 /permissive-)
else()
    target_compile_options(combscanner_dsp PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(combscanner_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
