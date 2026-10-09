cmake_minimum_required(VERSION 3.22)

project(CombScannerLab VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    # Как в Monomachine-Nova-1.6.27 stable: скрипты/подписи в UTF-8, иначе MSVC портит кириллицу.
    add_compile_options(/utf-8)
endif()

option(COMBLAB_FETCH_JUCE     "Скачать JUCE автоматически, если JUCE_DIR не задан" ON)
option(COMBLAB_BUILD_RENDERER "Собрать оффлайн-рендерер WAV (без JUCE)" ON)
option(COMBLAB_COPY_PLUGIN    "Копировать VST3 в системную папку после сборки" OFF)

set(JUCE_DIR "" CACHE PATH "Путь к исходникам JUCE 8 (если пусто — берётся из интернета)")

# 8.0.15 по умолчанию: в 8.0.6 проверка CMAKE_C_COMPILE_OBJECT в JUCEModuleSupport.cmake
# конфликтует с CMake 4.x (а именно такой CMake идёт вместе с Visual Studio 2026).
# Можно вернуться на 8.0.6: -DCOMBLAB_JUCE_TAG=8.0.6
set(COMBLAB_JUCE_TAG "8.0.15" CACHE STRING "Версия JUCE для FetchContent")

# ---------------------------------------------------------------------------
# JUCE
# ---------------------------------------------------------------------------
if(JUCE_DIR)
    if(NOT EXISTS "${JUCE_DIR}/CMakeLists.txt")
        message(FATAL_ERROR "JUCE_DIR=\"${JUCE_DIR}\" не похож на клон JUCE (нет CMakeLists.txt).")
    endif()
    add_subdirectory("${JUCE_DIR}" JUCE)
elseif(COMBLAB_FETCH_JUCE)
    include(FetchContent)
    FetchContent_Declare(JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG ${COMBLAB_JUCE_TAG}
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(JUCE)
else()
    message(FATAL_ERROR "Задайте JUCE_DIR или включите COMBLAB_FETCH_JUCE")
endif()

# ---------------------------------------------------------------------------
# DSP-ядро: движки из other/zigcomb + адаптеры. JUCE не требуется,
# поэтому тот же код использует и оффлайн-рендерер.
# ---------------------------------------------------------------------------
add_library(comblab_dsp STATIC
    Source/lab/EngineRegistry.cpp
    Source/engines/CombScannerVariants1Fav.cpp
    Source/engines/CombScannerVariants2.cpp
    Source/engines/CombScannerVariants3.cpp
    Source/engines/CombScannerVariants4.cpp
    Source/engines/CsMmVariants.cpp
    Source/engines/ZigZag.cpp)

target_include_directories(comblab_dsp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_compile_features(comblab_dsp PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(comblab_dsp PRIVATE /W4 /permissive-)
else()
    target_compile_options(comblab_dsp PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# Плагин
# ---------------------------------------------------------------------------
juce_add_plugin(CombScannerLab
    COMPANY_NAME "MonomachineNova"
    COMPANY_WEBSITE "https://github.com/glassg333/mmnova"
    BUNDLE_ID "com.monomachinenova.combscannerlab"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    VST3_CATEGORIES Fx
    FORMATS VST3 Standalone
    PLUGIN_MANUFACTURER_CODE MnOn
    PLUGIN_CODE CsLb
    PRODUCT_NAME "Comb Scanner Lab"
    DESCRIPTION "7 вариантов Comb Scanner из other/zigcomb в одном плагине, для сравнения на слух"
    COPY_PLUGIN_AFTER_BUILD ${COMBLAB_COPY_PLUGIN})

juce_generate_juce_header(CombScannerLab)

target_sources(CombScannerLab PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/ui/SlotPage.cpp)

target_include_directories(CombScannerLab PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_compile_definitions(CombScannerLab PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(CombScannerLab PRIVATE
    comblab_dsp
    juce::juce_audio_utils
    juce::juce_audio_devices
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags)

# ---------------------------------------------------------------------------
# Оффлайн-рендерер (без JUCE): ./combscannerlab_render <папка> [секунды] [all|first]
# ---------------------------------------------------------------------------
if(COMBLAB_BUILD_RENDERER)
    add_executable(combscannerlab_render tools/render_engines.cpp)
    target_link_libraries(combscannerlab_render PRIVATE comblab_dsp)
    target_compile_features(combscannerlab_render PRIVATE cxx_std_17)

    if(MSVC)
        target_compile_options(combscannerlab_render PRIVATE /W4 /permissive-)
    else()
        target_compile_options(combscannerlab_render PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
