cmake_minimum_required(VERSION 3.22)
project(MonomachineNovaUnifiedFX VERSION 1.9.8 LANGUAGES C CXX)
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
juce_add_plugin(MonomachineNovaUnifiedFX
    COMPANY_NAME "MonomachineNova"
    BUNDLE_ID "com.monomachinenova.unified.fx"
    PLUGIN_MANUFACTURER_CODE MnOn
    PLUGIN_CODE NvFx
    PRODUCT_NAME "Monomachine Nova FX"
    FORMATS VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    VST3_CATEGORIES Fx
    COPY_PLUGIN_AFTER_BUILD FALSE)
juce_generate_juce_header(MonomachineNovaUnifiedFX)
target_sources(MonomachineNovaUnifiedFX PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp Source/dsp/ChorusCore.cpp Source/dsp/ChorusNative.cpp Source/dsp/AudioAdapter.cpp)
target_include_directories(MonomachineNovaUnifiedFX PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_compile_definitions(MonomachineNovaUnifiedFX PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(MonomachineNovaUnifiedFX PRIVATE juce::juce_audio_utils)
option(NOVA_BUILD_TESTS "Build offline integration tests" OFF)
if(NOVA_BUILD_TESTS)
    enable_testing()
    add_executable(MonomachineNovaUnifiedFXTests tests/DeliveryTests.cpp)
    target_link_libraries(MonomachineNovaUnifiedFXTests PRIVATE MonomachineNovaUnifiedFX)
    # The tests use the generated JuceHeader.h and the same module declarations.
    target_include_directories(MonomachineNovaUnifiedFXTests PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode"
        "${JUCE_DIR}/modules")
    target_compile_definitions(MonomachineNovaUnifiedFXTests PRIVATE JucePlugin_Name="Monomachine Nova FX" JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME MonomachineNovaUnifiedFXIntegration COMMAND MonomachineNovaUnifiedFXTests)
endif()

if(NOVA_BUILD_TESTS)
    add_executable(NovaChorusTests tests/ChorusTests.cpp Source/dsp/ChorusCore.cpp Source/dsp/ChorusNative.cpp Source/dsp/AudioAdapter.cpp)
    target_include_directories(NovaChorusTests PRIVATE Source)
    add_test(NAME NovaChorusTests COMMAND NovaChorusTests)
endif()

if(NOVA_BUILD_TESTS)
    add_executable(NovaFXControls tests/FXControlsTests.cpp)
    target_link_libraries(NovaFXControls PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(NovaFXControls PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaFXControls PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaFXControls COMMAND NovaFXControls)
endif()

if(NOVA_BUILD_TESTS)
    add_executable(NovaTailClearTests tests/TailClearTests.cpp)
    target_link_libraries(NovaTailClearTests PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(NovaTailClearTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaTailClearTests PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaTailClearTests COMMAND NovaTailClearTests)
endif()

if(NOVA_BUILD_TESTS)
    add_executable(NovaP2RoutingTests tests/P2RoutingTests.cpp)
    target_link_libraries(NovaP2RoutingTests PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(NovaP2RoutingTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaP2RoutingTests PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaP2RoutingTests COMMAND NovaP2RoutingTests)

    add_executable(NovaP2DeclickTests tests/P2DeclickTests.cpp)
    target_link_libraries(NovaP2DeclickTests PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(NovaP2DeclickTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaP2DeclickTests PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaP2DeclickTests COMMAND NovaP2DeclickTests)

    # Documented P1/P2 TrackChain topology and explicit VOL/PAN vs DELAY split.
    add_executable(NovaTrackTopologyTests tests/TrackTopologyTests.cpp)
    target_link_libraries(NovaTrackTopologyTests PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(NovaTrackTopologyTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaTrackTopologyTests PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaTrackTopologyTests COMMAND NovaTrackTopologyTests)

    # Seven-stage P1/P2 route default plus legacy six-stage state migration.
    add_executable(NovaClassicRouteTopologyTests tests/ClassicRouteTopologyTests.cpp)
    target_link_libraries(NovaClassicRouteTopologyTests PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(NovaClassicRouteTopologyTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaClassicRouteTopologyTests PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaClassicRouteTopologyTests COMMAND NovaClassicRouteTopologyTests)

    # APVTS migration for withdrawn FMA/source-derived values and P2 selector.
    add_executable(NovaRollbackStateTests tests/RollbackStateTests.cpp)
    target_link_libraries(NovaRollbackStateTests PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(NovaRollbackStateTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(NovaRollbackStateTests PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME NovaRollbackStateTests COMMAND NovaRollbackStateTests)

endif()

if(NOVA_BUILD_TESTS)
    # Firmware ("mnm") DSP core checks: real tables, envelope slope, FM ratio law, delay line.
    add_executable(MnmCoreTests tests/MnmCoreTests.cpp)
    target_include_directories(MnmCoreTests PRIVATE Source)
    add_test(NAME MnmCoreTests COMMAND MnmCoreTests)

    # DBAS/DWID feedback routing, optional Q stability, NEW delay state
    # isolation and appended mode/schema compatibility without JUCE.
    add_executable(DelayFeedbackDspTests tests/DelayFeedbackDspTests.cpp)
    target_include_directories(DelayFeedbackDspTests PRIVATE Source)
    add_test(NAME DelayFeedbackDspTests COMMAND DelayFeedbackDspTests)

    # Native/base-width and imported Korg/Odin filter continuity, including
    # the regression that redundant host-rate setup must not reset an adapter
    # at each audio callback.
    add_executable(MnmRealFilterTests tests/MnmRealFilterTests.cpp)
    target_include_directories(MnmRealFilterTests PRIVATE Source)
    add_test(NAME MnmRealFilterTests COMMAND MnmRealFilterTests)

    # Closed local-only hybrid DSP smoke test. This standalone target does not
    # link the plugin/JUCE target; it verifies the K35, six-response ladder,
    # and Fold/Zero/Clamp state machines. Public release packaging remains untouched.
    add_executable(HybridDspTests tests/HybridDspTests.cpp)
    target_include_directories(HybridDspTests PRIVATE Source)
    add_test(NAME HybridDspTests COMMAND HybridDspTests)

    # Neutral P1/P2 FILT-extra bridge and Korg/Odin saturation-law regression.
    add_executable(FilterExtrasDspTests tests/FilterExtrasDspTests.cpp)
    target_include_directories(FilterExtrasDspTests PRIVATE Source)
    add_test(NAME FilterExtrasDspTests COMMAND FilterExtrasDspTests)

    add_executable(ModEnvMatrixTests tests/ModEnvMatrixTests.cpp)
    target_include_directories(ModEnvMatrixTests PRIVATE Source)
    add_test(NAME ModEnvMatrixTests COMMAND ModEnvMatrixTests)

    # Live 1..64 ARP scheduling is host-independent and covered without JUCE.
    add_executable(ArpWindowTests tests/ArpWindowTests.cpp)
    target_include_directories(ArpWindowTests PRIVATE Source)
    add_test(NAME ArpWindowTests COMMAND ArpWindowTests)

    # Canonical direct LFO PAGE/DEST compatibility and P1/P2 mirror mapping.
    add_executable(LfoMappingTests tests/LfoMappingTests.cpp)
    target_link_libraries(LfoMappingTests PRIVATE MonomachineNovaUnifiedFX)
    target_include_directories(LfoMappingTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/MonomachineNovaUnifiedFX_artefacts/JuceLibraryCode" "${JUCE_DIR}/modules")
    target_compile_definitions(LfoMappingTests PRIVATE JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
    add_test(NAME LfoMappingTests COMMAND LfoMappingTests)

    # Two-choice native/old registry and legacy experimental-state collapse.
    add_executable(ModeRollbackTests tests/ModeRollbackTests.cpp)
    target_include_directories(ModeRollbackTests PRIVATE Source)
    add_test(NAME ModeRollbackTests COMMAND ModeRollbackTests)
endif()

# 1.6.40: LAYOUT EDITOR -- отдельное приложение "только интерфейс" (без аудио/DAW):
# тот же редактор поверх живого процессора, сразу включает LAYOUT EDIT, расклад
# пишется в общий layout.json. Только CMake (.jucer второй таргет не держит).
juce_add_gui_app(MonomachineNovaLayoutFX
    PRODUCT_NAME "Monomachine Nova Layout FX"
    COMPANY_NAME "MonomachineNova"
    BUNDLE_ID "com.monomachinenova.layout.fx")
juce_generate_juce_header(MonomachineNovaLayoutFX)
target_sources(MonomachineNovaLayoutFX PRIVATE
    Source/PluginProcessor.cpp Source/PluginEditor.cpp Source/LayoutEditorMain.cpp
    Source/dsp/ChorusCore.cpp Source/dsp/ChorusNative.cpp Source/dsp/AudioAdapter.cpp)
target_include_directories(MonomachineNovaLayoutFX PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_compile_definitions(MonomachineNovaLayoutFX PRIVATE
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0
    JucePlugin_Name="Monomachine Nova FX") # PluginProcessor.h читает JucePlugin_Name
target_link_libraries(MonomachineNovaLayoutFX PRIVATE juce::juce_audio_utils)
