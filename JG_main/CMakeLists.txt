cmake_minimum_required(VERSION 3.22)

# Set the project name
set(MAIN_PROJECT JG_main)

# Include toolchain file

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
#project(${CMAKE_PROJECT_NAME})
#file(GLOB_RECURSE AUX_SRC Core/*.c)
#file(GLOB_RECURSE AUX_MID Middlewares/*.c)

# Create an executable object type
add_executable(${MAIN_PROJECT})
#
#message(${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc)

add_library(stm32cubemx_main)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx_main)

# Add linked libraries
target_link_libraries(${MAIN_PROJECT} PUBLIC
        stm32cubemx_main
)

target_sources(${MAIN_PROJECT} PUBLIC
        ${SHARED_SRC_DIR}/app/app_stateMachine.c
        ${SHARED_SRC_DIR}/app/app_drivetrain.c
        ${SHARED_SRC_DIR}/app/app_lineFollowing.c
        ${SHARED_SRC_DIR}/app/app_navi.c
        ${SHARED_SRC_DIR}/app/app_sweeper.c
        ${SHARED_SRC_DIR}/app/app_lift.c
        ${SHARED_SRC_DIR}/app/app_claw.c
        ${SHARED_SRC_DIR}/hw/hw_dcMotor.c
        ${SHARED_SRC_DIR}/hw/hw_reflectance.c
        ${SHARED_SRC_DIR}/hw/hw_rotaryEncoder.c
        ${SHARED_SRC_DIR}/hw/hw_servo.c
        ${SHARED_SRC_DIR}/io/io_adc.c
        ${SHARED_SRC_DIR}/io/io_pwm.c
)

target_include_directories(${MAIN_PROJECT} PUBLIC
        ${SHARED_SRC_DIR}/app
        ${SHARED_SRC_DIR}/io
        ${SHARED_SRC_DIR}/hw
)