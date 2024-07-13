## Cmake 사용하여 STM32F4 HAL 빌드하기 

### Step 1 : 빌드 환경 만들기
1. [STM32CubeF4](https://github.com/STMicroelectronics/STM32CubeF4) 레포지토리 클론하기  
2. [arm.toolchain](arm.toolchain) 크로스 컴파일러 설정하기
3. [CMakeLists.txt](CMakeLists.txt)[1:7] 프로젝트 정의 및 크로스 컴파일 추가

### Step 2 : STM32CubeF4 라이브러리로 빌드
1. [CMakeLists.txt](CMakeLists.txt)[9:20] MCU 타겟 및 경로 설정
2. [STM32CubeF4.cmake](STM32CubeF4.cmake)[1:14] HAL 및 CMSIS 라이브러리로 빌드

### Step 3 : executable 확인
1. [CMakeLists.txt](CMakeLists.txt)[22:26] 테스트로 소스 추가 및 라이브러리 추가
2. [main.c](Core/Src/main.c)[1] #include "stm32f4xx_hal.h"를 통해 확인하기

### step 4 : 테스트 빌드
```
In file included from ~/ws/stm32/f4/Core/Src/main.c:1:
~/ws/stm32/f4/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h:29:10: fatal error: stm32f4xx_hal_conf.h: No such file or directory
   29 | #include "stm32f4xx_hal_conf.h"
      |          ^~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
```
stm32f4xx_hal_conf.h 없음 확인

### step 5 : stm32f4xx_hal_conf.h 작성
1. [stm32f4xx_hal_conf.h](Core/Config/stm32f4xx_hal_config.h) 생성
2. [Template 가져오기](STM32CubeF4/Projects/STM32F446ZE-Nucleo/Templates/Inc/stm32f4xx_hal_conf.h)
3. [stm32f4xx_hal_conf.h](Core/Config/stm32f4xx_hal_config.h)에서 사용 할 모듈 수정 하기[34:78]
4. [stm32f4xx_hal_conf.h](Core/Config/stm32f4xx_hal_config.h) 그 외 설정 값은 기본으로 놔둠
5. #include <stdint.h> 추가

### step 6 : 다시 빌드
```
/home/jj/ws/stm32/f4/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h:174:2: error: #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"
  174 | #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"
      |  ^~~~~
```
Build 중 Device 선택 안됨 FLAG 설정 필요


### step 7 : 빌드 플래그 설정
