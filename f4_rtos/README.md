## RTOS 포팅

### Step 1 : FreeRTOS 다운받기
1. git clone
2. submodule init & update

### Step 2 : [FreeRTOS.cmake](./FreeRTOS.cmake) 작성
1. [FreeRTOS CMakeLists.txt](FreeRTOS/FreeRTOS/Source/CMakeLists.txt)를 보면 몇가지 설정 해야 한다고 적혀있다.
2. FREERTOS_PORT option
3. freertos_config library target
4. [FreeRTOSConfig.h](Core/Config/FreeRTOSConfig.h) 생성하고
5. [CMakeLists.txt](./CMakeLists.txt)에 include 하고 일단 빌드하면 에러 메세지로 친절하게 설명해준다.

### step 3 : [FreeRTOS.cmake](./FreeRTOS.cmake) 수정
1. add_library(freertos_config INTERFACE) 를 작성하고
2. set(FREERTOS_PORT ${RTOS_PORT}) 작성하고, set(RTOS_PORT GCC_ARM_CM4F) [CMakeLists.txt](./CMakeLists.txt)에 작성한다.
3. 추가로 set(FREERTOS_HEAP 4)를 통해 heap4.c를 쓴다고 명시해주고 빌드

### step 4 : SystemCoreClock
1. SystemCoreClock 가 안불러와진다.
2. [FreeRTOSConfig.h](Core/Config/FreeRTOSConfig.h) 에 그냥 명시해줬다.

### step 5 : assembler error
1. Compile Flag와 Link Flag 그리고 Linker Script 추가해줬다.

### step 6 : main.h에 추가
1. #include "FreeRTOS.h"를 해보자

### end