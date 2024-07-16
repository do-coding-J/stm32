# HAL
file(GLOB_RECURSE HAL_SRC ${HAL_PATH}/Src/*.c)
file(GLOB_RECURSE HAL_SRC_EXCEPTION ${HAL_PATH}/Src/*template.c)
list(REMOVE_ITEM HAL_SRC ${HAL_SRC_EXCEPTION})

set(HAL_INC ${HAL_PATH}/Inc ${HAL_PATH}/Inc/Legacy)

set(HAL_CFG_PATH ${MAIN_PATH}/Config)

# CMSIS
set(CMSIS_SRC ${MAIN_PATH}/Config/startup_${MCU_MODEL_L}.s)
set(CMSIS_INC ${CMSIS_PATH}/Device/ST/${MCU_FAMILY}/Include ${CMSIS_PATH}/Include)

# lib
add_library(stm32cubef4 STATIC ${HAL_SRC})
target_include_directories(stm32cubef4 PUBLIC ${HAL_INC} ${CMSIS_INC} ${HAL_CFG_PATH})

target_compile_definitions(stm32cubef4 PUBLIC ${MCU_MODEL_U})
target_compile_options(stm32cubef4 PUBLIC ${MCU_PARAM} ${COMPILE_FLAG} --verbose)
target_link_options(stm32cubef4 PUBLIC ${MCU_PARAM} ${LINK_FLAG} -T${MCU_LINKER_SCRIPT})