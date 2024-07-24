cmake_minimum_required(VERSION 3.22)

# Set the project name
set(NAVI_PROJECT HB_navi_BP)

# Include toolchain file

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
#project(${CMAKE_PROJECT_NAME})
#
#file(GLOB_RECURSE NAVI_SRC Core/*.c)
#file(GLOB_RECURSE NAVI_MID Middlewares/*.c)

# Create an executable object type
add_executable(${NAVI_PROJECT} Source/mean.c)


add_library(stm32cubemx_navi)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx_navi)

# Add linked libraries
target_link_libraries(${NAVI_PROJECT} PUBLIC
    stm32cubemx_navi
)

target_sources(${NAVI_PROJECT} PUBLIC
        Source/mean.c
        ${SHARED_SRC_DIR}/app/app_stateMachine.c
        ${SHARED_SRC_DIR}/hw/hw_dcMotor.c
        ${SHARED_SRC_DIR}/hw/hw_rotaryEncoder.c
        ${SHARED_SRC_DIR}/io/io_adc.c
        ${SHARED_SRC_DIR}/io/io_pwm.c
)

target_include_directories(${NAVI_PROJECT} PUBLIC
        ${SHARED_SRC_DIR}/app
        ${SHARED_SRC_DIR}/io
        ${SHARED_SRC_DIR}/hw
        Source
)