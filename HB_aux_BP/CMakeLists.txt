cmake_minimum_required(VERSION 3.22)

# Set the project name
set(AUX_PROJECT HB_aux_BP)

# Include toolchain file

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
#project(${CMAKE_PROJECT_NAME})
#file(GLOB_RECURSE AUX_SRC Core/*.c)
#file(GLOB_RECURSE AUX_MID Middlewares/*.c)

# Create an executable object type
add_executable(${AUX_PROJECT}
        ../shared/app/app_lineFollowing.h)
#
#message(${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc)

add_library(stm32cubemx_aux)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx_aux)

# Add linked libraries
target_link_libraries(${AUX_PROJECT} PUBLIC
    stm32cubemx_aux
)

target_sources(${AUX_PROJECT} PUBLIC
        ${SHARED_SRC_DIR}/app
)

target_include_directories(${AUX_PROJECT} PUBLIC
        ${SHARED_SRC_DIR}/app
)