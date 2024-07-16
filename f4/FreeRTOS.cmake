# FreeRTOS

set(FREERTOS_PORT ${RTOS_PORT})
set(FREERTOS_HEAP 4)

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE
    ${MAIN_PATH}/Config)
target_compile_definitions(freertos_config INTERFACE
    ${MCU_MODEL_U}
    projCOVERAGE_TEST=0)
target_compile_options(freertos_config INTERFACE ${MCU_PARAM} ${COMPILE_FLAG} --verbose)
target_link_options(freertos_config INTERFACE ${LINK_FLAG} -T${MCU_LINKER_SCRIPT})

add_subdirectory(${RTOS_PATH}/FreeRTOS/Source)