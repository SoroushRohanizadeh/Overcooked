cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
message(${CMAKE_CURRENT_SOURCE_DIR})
include("gcc-arm-none-eabi.cmake")
project(Undercooked LANGUAGES C ASM)

#enable_language(C ASM)
#add_library(stm32cubemx INTERFACE)

add_subdirectory(HB_aux_BP)
add_subdirectory(HB_navi_BP)