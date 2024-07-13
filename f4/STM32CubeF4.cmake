# HAL
file(GLOB_RECURSE HAL_SRC ${HAL_PATH}/Src/*.c)
file(GLOB_RECURSE HAL_SRC_EXCEPTION ${HAL_PATH}/Src/*template.c)
list(REMOVE_ITEM HAL_SRC ${HAL_SRC_EXCEPTION})

set(HAL_INC ${HAL_PATH}/Inc ${HAL_PATH}/Inc/Legacy)

set(HAL_CFG_PATH ${MAIN_PATH}/Config)

# CMSIS
set(CMSIS_SRC ${CMSIS_PATH}/Source/Templates/gcc/startup_${MCU_MODEL_L}.s)
set(CMSIS_INC ${CMSIS_PATH}/Include)

# lib
add_library(stm32cubef4 INTERFACE ${HAL_SRC} ${CMSIS_SRC})
target_include_directories(stm32cubef4 INTERFACE ${HAL_INC} ${CMSIS_INC} ${HAL_CFG_PATH})
target_compile_definitions(stm32cubef4 INTERFACE ${MCU_FAMILY})
target_compile_options(stm32cubef4 INTERFACE ${MCU_PARAM} --verbose)