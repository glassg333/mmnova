cmake_minimum_required(VERSION 3.22)

project(MnmFltDist VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE: fetch, если нет локальной копии
include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.4
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(MnmFltDist
    COMPANY_NAME "mmnova-port"
    PLUGIN_MANUFACTURER_CODE Mmnv
    PLUGIN_CODE Mflt
    FORMATS VST3 Standalone
    PRODUCT_NAME "MnmFltDist")

target_sources(MnmFltDist PRIVATE
    Source/MnmFltDistProcessor.cpp)

target_include_directories(MnmFltDist PRIVATE Source)

target_compile_definitions(MnmFltDist PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(MnmFltDist PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags)

# самопроверка ядра без JUCE (g++ -std=c++17 напрямую)
add_executable(test_sanity EXCLUDE_FROM_ALL Tests/test_sanity.cpp)
target_include_directories(test_sanity PRIVATE Source)
