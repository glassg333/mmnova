cmake_minimum_required(VERSION 3.22)
project(MonomachineNovaUnifiedSynth VERSION 1.6.19 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(MSVC)
    # 1.6.5: Russian UI strings are UTF-8 in the sources; /utf-8 keeps MSVC from
    # mangling them into mojibake (source+execution charset UTF-8).
    add_compile_options(/utf-8)
endif()
set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE" CACHE PATH "Path to JUCE 8 source checkout")
if(NOT TARGET juce::juce_audio_utils)
    if(NOT EXISTS "${JUCE_DIR}/CMakeLists.txt")
        message(FATAL_ERROR "Set JUCE_DIR to a JUCE 8 checkout, or clone JUCE into this project.")
    endif()
    add_subdirectory("${JUCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/JUCE")
endif()
juce_add_plugin(MonomachineNovaUnifiedSynth
    COMPANY_NAME "MonomachineNova"
    BUNDLE_ID "com.monomachinenova.unified.synth"
    PLUGIN_MANUFACTURER_CODE MnOn
    PLUGIN_CODE NvSy
    PRODUCT_NAME "Monomachine Nova Synth"
    FORMATS VST3
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    VST3_CATEGORIES Instrument Synth
    COPY_PLUGIN_AFTER_BUILD FALSE)
juce_generate_juce_header(MonomachineNovaUnifiedSynth)
target_sources(MonomachineNovaUnifiedSynth PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp Source/dsp/ChorusCore.cpp Source/dsp/AudioAdapter.cpp)
target_include_directories(MonomachineNovaUnifiedSynth PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_compile_definitions(MonomachineNovaUnifiedSynth PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(MonomachineNovaUnifiedSynth PRIVATE juce::juce_audio_utils)
option(NOVA_BUILD_TESTS "Build offline integration tests" OFF)
if(NOVA_BUILD_TESTS)
    enable_testing()
    add_executable(MonomachineNovaUnifiedSynthTests tests/DeliveryTests.cpp)
    target_link_libraries(MonomachineNovaUnifiedSynthTests PRIVATE MonomachineNovaUnifiedSynth)
    # The tests use the generated JuceHeader.h and the same module declarations.
    target_include_directories(MonomachineNovaUnifiedSynthTests PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedSynth_artefacts/JuceLibraryCode"
        "${JUCE_DIR}/modules")
    target_compile_definitions(MonomachineNovaUnifiedSynthTests PRIVATE JucePlugin_Name="Monomachine Nova Synth" JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME MonomachineNovaUnifiedSynthIntegration COMMAND MonomachineNovaUnifiedSynthTests)
endif()

if(NOVA_BUILD_TESTS)
    add_executable(NovaChorusTests tests/ChorusTests.cpp Source/dsp/ChorusCore.cpp Source/dsp/AudioAdapter.cpp)
    target_include_directories(NovaChorusTests PRIVATE Source)
    add_test(NAME NovaChorusTests COMMAND NovaChorusTests)
endif()

if(NOVA_BUILD_TESTS)
    add_executable(NovaFXControls tests/FXControlsTests.cpp)
    target_link_libraries(NovaFXControls PRIVATE MonomachineNovaUnifiedSynth)
    target_include_directories(NovaFXControls PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedSynth_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaFXControls PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaFXControls COMMAND NovaFXControls)
endif()

if(NOVA_BUILD_TESTS)
    # Firmware ("mnm") DSP core checks: real tables, envelope slope, FM ratio law, delay line.
    add_executable(MnmCoreTests tests/MnmCoreTests.cpp)
    target_include_directories(MnmCoreTests PRIVATE Source)
    add_test(NAME MnmCoreTests COMMAND MnmCoreTests)
endif()
