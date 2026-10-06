cmake_minimum_required(VERSION 3.15)
project(MonomachineFilterDist LANGUAGES CXX)

# Header-only component; this does not fetch JUCE or build a plugin.
add_library(monomachine_filter_dist INTERFACE)
add_library(mnm132::filter_dist ALIAS monomachine_filter_dist)
target_include_directories(monomachine_filter_dist INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(monomachine_filter_dist INTERFACE cxx_std_17)

if(TARGET juce::juce_audio_basics)
    add_library(monomachine_filter_dist_juce INTERFACE)
    add_library(mnm132::juce_filter_dist ALIAS monomachine_filter_dist_juce)
    target_link_libraries(monomachine_filter_dist_juce INTERFACE
        monomachine_filter_dist juce::juce_audio_basics)
endif()
