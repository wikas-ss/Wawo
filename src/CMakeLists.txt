cmake_minimum_required(VERSION 3.21)

project(AstralFPSOptimizer VERSION 0.1.1 LANGUAGES CXX)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK environment variable is not set")
endif()

add_subdirectory(
    "$ENV{GEODE_SDK}"
    "${CMAKE_CURRENT_BINARY_DIR}/geode"
)

setup_geode_mod(
    AstralFPSOptimizer
)
