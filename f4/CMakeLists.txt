cmake_minimum_required(VERSION 3.20)

# include compiler setting before project
include(arm.toolchain)

project(STM32F4)
enable_language(C CXX ASM)

# Dev setting
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL_U STM32F446xx)
string(TOLOWER ${MCU_MODEL_U} MCU_MODEL_L)
set(MCU_PARAM -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp)

# path
set(HAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver)
set(CMSIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx)
set(MAIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Core)

# lib include
include(STM32CubeF4.cmake)

# main source
file(GLOB_RECURSE MAIN_SRC ${MAIN_PATH}/Src/*.c)

add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC stm32cubef4)
target_include_directories(${PROJECT_NAME} PUBLIC ${MAIN_PATH}/Inc)
target_compile_options(${PROJECT_NAME} PUBLIC
    --verbose
    ${MCU_PARAM}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    -Wno-binary-constants
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
    )
target_link_options(${PROJECT_NAME} PUBLIC)
    -T ${MCU_LINKER_SCRIPT}
    ${MCU_PARAM}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage)