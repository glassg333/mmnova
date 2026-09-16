cmake_minimum_required(VERSION 3.22)
project(MonomachineNovaUnifiedFX VERSION 1.3.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE" CACHE PATH "Path to JUCE 8 source checkout")
if(NOT TARGET juce::juce_audio_utils)
    if(NOT EXISTS "${JUCE_DIR}/CMakeLists.txt")
        message(FATAL_ERROR "Set JUCE_DIR to a JUCE 8 checkout, or clone JUCE into this project.")
    endif()
    add_subdirectory("${JUCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/JUCE")
endif()
juce_add_plugin(MonomachineNovaUnifiedFX
    COMPANY_NAME "MonomachineNova"
    BUNDLE_ID "com.monomachinenova.unified.fx"
    PLUGIN_MANUFACTURER_CODE MnOn
    PLUGIN_CODE NvFx
    PRODUCT_NAME "MonomachineNova Unified FX"
    FORMATS VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    VST3_CATEGORIES Fx
    COPY_PLUGIN_AFTER_BUILD FALSE)
juce_generate_juce_header(MonomachineNovaUnifiedFX)
target_sources(MonomachineNovaUnifiedFX PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp Source/dsp/ChorusCore.cpp Source/dsp/AudioAdapter.cpp)
target_include_directories(MonomachineNovaUnifiedFX PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_compile_definitions(MonomachineNovaUnifiedFX PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(MonomachineNovaUnifiedFX PRIVATE juce::juce_audio_utils)
option(NOVA_BUILD_TESTS "Build offline integration tests" OFF)
if(NOVA_BUILD_TESTS)
    enable_testing()
    add_executable(MonomachineNovaUnifiedFXTests tests/IntegrationTests.cpp)
    target_link_libraries(MonomachineNovaUnifiedFXTests PRIVATE MonomachineNovaUnifiedFX)
    # The tests use the generated JuceHeader.h and the same module declarations.
    target_include_directories(MonomachineNovaUnifiedFXTests PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode"
        "${JUCE_DIR}/modules")
    target_compile_definitions(MonomachineNovaUnifiedFXTests PRIVATE JucePlugin_Name="MonomachineNova Unified FX" JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME MonomachineNovaUnifiedFXIntegration COMMAND MonomachineNovaUnifiedFXTests)
endif()
