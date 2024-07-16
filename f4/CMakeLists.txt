cmake_minimum_required(VERSION 3.20)

# include compiler setting before project
include(arm.toolchain)

project(STM32F4)
enable_language(C CXX ASM)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Dev setting
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL_U STM32F446xx)
set(RTOS_PORT GCC_ARM_CM4F)

# path
set(HAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeF4/Drivers/${MCU_FAMILY}_HAL_Driver)
set(CMSIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeF4/Drivers/CMSIS)
set(MAIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Core)
set(RTOS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS)

string(TOLOWER ${MCU_MODEL_U} MCU_MODEL_L)
set(MCU_PARAM -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp)
set(MCU_LINKER_SCRIPT ${MAIN_PATH}/Config/FLASH.ld)

# lib include
include(STM32CubeF4.cmake)
# include(FreeRTOS.cmake)

# output configuration
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# main source
file(GLOB_RECURSE MAIN_SRC ${MAIN_PATH}/Src/*.c ${MAIN_PATH}/Config/*.c)

add_executable(${PROJECT_NAME} ${MAIN_SRC} ${CMSIS_SRC})
# target_link_libraries(${PROJECT_NAME} PUBLIC stm32cubef4 freertos_config freertos_kernel)
target_link_libraries(${PROJECT_NAME} PUBLIC stm32cubef4)
target_include_directories(${PROJECT_NAME} PUBLIC ${MAIN_PATH}/Inc)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${MCU_MODEL_U})
target_compile_options(${PROJECT_NAME} PUBLIC ${MCU_PARAM} ${COMPILE_FLAG} --verbose)
target_link_options(${PROJECT_NAME} PUBLIC ${MCU_PARAM} ${LINK_FLAG} -T${MCU_LINKER_SCRIPT})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}>
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin)