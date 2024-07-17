## Cmake 사용하여 STM32F4 HAL 빌드하기 

> [stm32-cmake](https://github.com/ObKo/stm32-cmake/tree/master) 참고해보면 좋을 듯

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
1. 일단 적용 후 수정 예정
2. [Linker Script Template](STM32CubeF4/Projects/STM32F446ZE-Nucleo/Templates/STM32CubeIDE/STM32F446ZETX_FLASH.ld) 가져오기
3. [arm.toolchain](arm.toolchain) FLAG 정리

### step 8 : 다시 빌드 : LINKER SCRIPT ERROR
```
[2/2 100% :: 0.205] Linking C executable STM32F4
FAILED: STM32F4 
: && /usr/bin/arm-none-eabi-gcc -g -Wl,--start-group -lc -lm -lstdc++ -Wl,--end-group -Wl,--print-memory-usage -T/home/jj/ws/personal/stm32/f4/Core/Config/FLASH.ld -fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections CMakeFiles/STM32F4.dir/Core/Src/main.c.obj -o STM32F4   && :
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld:/home/jj/ws/personal/stm32/f4/Core/Config/FLASH.ld:88: non constant or forward reference address expression for section .ARM.extab
collect2: error: ld returned 1 exit status
```

링킹 에러가 나서 찾아보니

```
.ARM.extab (READONLY) : /* The READONLY keyword is only supported in GCC11 and later, remove it if using GCC10 or earlier. */
```

gcc/arm-none-eabi/10.3.1을 쓰고 있음으로 (READONLY) 제거


### step 9 : 다시 빌드 : RESET_HANDLER
```
[2/2 100% :: 0.459] Linking C executable STM32F4
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: warning: cannot find entry symbol Reset_Handler; defaulting to 0000000008000000
Memory region         Used Size  Region Size  %age Used
             RAM:        1568 B       128 KB      1.20%
             ROM:         168 B       512 KB      0.03%
build finished with warning(s).
```

엔트리 심볼을 찾을 수 없다고 한다.
[CMakeLists.txt](CMakeLists.txt)[28]에 startup script를 추가해준다.


### step 10 : 다시 빌드 : main.c 에러
```
[3/3 100% :: 0.284] Linking C executable STM32F4
FAILED: STM32F4 
: && /usr/bin/arm-none-eabi-gcc -g -Wl,--start-group -lc -lm -lstdc++ -Wl,--end-group -Wl,--print-memory-usage -T/home/jj/ws/personal/stm32/f4/Core/Config/FLASH.ld -fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections CMakeFiles/STM32F4.dir/Core/Src/main.c.obj CMakeFiles/STM32F4.dir/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s.obj -o STM32F4   && :
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: CMakeFiles/STM32F4.dir/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s.obj: in function `Reset_Handler':
/home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s:64: undefined reference to `SystemInit'
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: /home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s:100: undefined reference to `main'
Memory region         Used Size  Region Size  %age Used
             RAM:        1568 B       128 KB      1.20%
             ROM:         832 B       512 KB      0.16%
collect2: error: ld returned 1 exit status
ninja: build stopped: subcommand failed.
build finished with error(s).
```

main.c에서 main, system init 같은 함수가 없어서 생기는 문제이다. "stm32f4xx_hal.h"는 잘 들어간 것 같다.


### step 11 : main.c와 system_stm32f4xx.c 작성
1. [main.c](Core/Src/main.c)에 main 함수 추가 및 HAL_init() 추가  
2. [system_stm32f4xx.c](STM32CubeF4/Projects/STM32F446ZE-Nucleo/Templates/Src/system_stm32f4xx.c)를 [system_stm32f4xx.c](Core/Src/system_stm32f4xx.c)로 복사  


### step 12 : 다시 빌드 : Hal_init 에러
```
FAILED: STM32F4 
: && /usr/bin/arm-none-eabi-gcc -g -Wl,--start-group -lc -lm -lstdc++ -Wl,--end-group -Wl,--print-memory-usage -T/home/jj/ws/personal/stm32/f4/Core/Config/FLASH.ld -fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections CMakeFiles/STM32F4.dir/Core/Src/main.c.obj CMakeFiles/STM32F4.dir/Core/Src/system_stm32f4xx.c.obj CMakeFiles/STM32F4.dir/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s.obj -o STM32F4   && :
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: CMakeFiles/STM32F4.dir/Core/Src/main.c.obj: in function `main':
/home/jj/ws/personal/stm32/f4/Core/Src/main.c:5: undefined reference to `HAL_Init'
```

빌드 인풋을 확인해보니 라이브러리 링킹이 안됨...?
-> [STM32CubeF4.cmake](STM32CubeF4.cmake) stm32cubef4 라이브러리 INTERFACE -> STATIC으로 변경


### step 13 : 다시 빌드 : assert_failed 에러
```
FAILED: STM32F4 
: && /usr/bin/arm-none-eabi-gcc -g -Wl,--start-group -lc -lm -lstdc++ -Wl,--end-group -Wl,--print-memory-usage -T/home/jj/ws/personal/stm32/f4/Core/Config/FLASH.ld -fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections CMakeFiles/STM32F4.dir/Core/Src/main.c.obj CMakeFiles/STM32F4.dir/Core/Src/system_stm32f4xx.c.obj CMakeFiles/STM32F4.dir/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s.obj -o STM32F4  libstm32cubef4.a && :
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: libstm32cubef4.a(stm32f4xx_hal.c.obj): in function `HAL_SetTickFreq':
/home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c:346: undefined reference to `assert_failed'
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: libstm32cubef4.a(stm32f4xx_hal_cortex.c.obj): in function `HAL_NVIC_SetPriorityGrouping':
/home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c:144: undefined reference to `assert_failed'
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: libstm32cubef4.a(stm32f4xx_hal_cortex.c.obj): in function `HAL_NVIC_SetPriority':
/home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c:168: undefined reference to `assert_failed'
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: /home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c:169: undefined reference to `assert_failed'
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: libstm32cubef4.a(stm32f4xx_hal_cortex.c.obj): in function `HAL_NVIC_EnableIRQ':
/home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c:188: undefined reference to `assert_failed'
/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/bin/ld: libstm32cubef4.a(stm32f4xx_hal_cortex.c.obj):/home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c:204: more undefined references to `assert_failed' follow
```

void assert_failed에 대한 함수 만들어줌


### step 14 : 클럭 활성화
1. HAL_init을 실행 해도 클럭을 활성화 하지 않으면 프로그램이 시작 되지 않는다.
2. [main.c](STM32CubeF4/Projects/STM32F446ZE-Nucleo/Templates/Src/main.c)에서 SystemClock_config 참고
3. [main.c](Core/Src/main.c)에 작성

```
[97/98  98% :: 2.718] Linking C static library libstm32cubef4.a
[98/98 100% :: 2.810] Linking C executable STM32F4
Memory region         Used Size  Region Size  %age Used
             RAM:        1584 B       128 KB      1.21%
             ROM:        8056 B       512 KB      1.54%
build finished with warning(s).
```
> 여기서 나오는 warning은 LL 드라이버를 활성화 하지 않아서 발생하는 문제. HAL 드라이버만 사용 할 것으로 무시

### step 15 : OUTPUT 설정
1. 다시 보니 output이 .elf 또는 .bin 파일이 아니라 설정 해주기
2. [arm.toolchain](arm.toolchain)[25] CMAKE_EXECUTABLE_SUFFIX [CMakeLists.txt](CMakeLists.txt)[27]로 이동
3. [CMakeLists.txt](CMakeLists.txt)[36:38]에 output을 .bin으로 바꿔주는 커스텀 커맨드 추가

### step 16 : uploading sh file
1. 업로드 가능한 [upload.sh](upload.sh) 작성

### step 17 : 디버깅해보기
1. [lauch.json](.vscode/launch.json) 파일 작성 후 디버깅 해보기
```
LiveGDB: Reset_Handler () at /home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s:61
LiveGDB: 61	  ldr   sp, =_estack      /* set stack pointer */
LiveGDB: Program stopped, probably due to a reset and/or halt issued by debugger
```
startup script에서 빠져나오질 못함
-> 찾아보니 Systick_Handler의 부재로 인해 스택포인터가 오르지 않은 것으로 확인  
2. [main.c](./Core/Src/main.c)[2:6]에 void SysTick_Handler(void) 추가
3. 확인을 위해 글로벌 변수 uwTick 확인
4. 다시 빌드 후 디버깅

```
LiveGDB: Reset_Handler () at /home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s:61
LiveGDB: 61	  ldr   sp, =_estack      /* set stack pointer */
LiveGDB: Program stopped, probably due to a reset and/or halt issued by debugger
Started live-monitor-gdb session
Note: automatically using hardware breakpoints for read-only addresses.

Program
 received signal SIGINT, Interrupt.
WWDG_IRQHandler () at /home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s:114
114	  b  Infinite_Loop
```
5. WWDG_IRQHandler에러 우선 빌드에서 제외 [stm32f4xx_hal_conf.h](Core/Config/stm32f4xx_hal_conf.h)[72] 주석
6. 다시 빌드
```
LiveGDB: Reset_Handler () at /home/jj/ws/personal/stm32/f4/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s:61
LiveGDB: 61	  ldr   sp, =_estack      /* set stack pointer */
LiveGDB: Program stopped, probably due to a reset and/or halt issued by debugger
```
7. 터지는 부분을 Call Stack으로 찍어보면 [이부분](Core/Config/startup_stm32f446xx.s)[98] __libc_init_array에서 터진다 


### step 18 : main()까지 가는 길 검토하기
1. __libc_init_array에서 터진다 
2. libc 는 c 라이브러리
3. 여러가지 수정 후... [FLAG 중](arm.toolchain) --specs=nano.specs -> --specs=nosys.specs 변경하니 main을 찾을 수 있었다.


---
## STM32의 IO를 이용한 기능 작성하기

### step 1 : 이전 작업 불러오기  
1. 빌드가 잘 되는지 확인
2. 디버깅 모드로 uwTick이 잘 올라가는지 확인 한다.

### GPIO LED 사용

#### step 1 : 사용 할 핀 정하기
1. GPO와 GPI를 사용 해 볼 것이기 때문에 데이터 시트에서 LED와 Btn에 대한 정보를 가져온다.
2. LED는 [Green, Blue, Red] 순서로 [PB0, PB7, PB14]
3. Btn은 B1 PC13 이다.

#### step 2 : 핀 정의
1. [gpio_pin_defs.h](Core/Inc/gpio_pin_defs.h)에 핀 정의 하기
2. [gpio.c](Core/Src/gpio.c)에 GPIO_Init 함수 작성하기

#### step 3 : 빌드 후 확인
1. 이상 없음!


### GPIO Btn(B1) 사용

#### step 1 : 핀 정의
1. LED는 output이고 B1은 input이기 때문에 다른 설정으로 적용한다.
2. [gpio.c](Core/Src/gpio.c)에 poll 방식으로 코드 작성
3. [main.c](Core/Src/main.c)에 if 문으로 동작 방식을 정하고 빌드

### step 2 : 빌드 후 확인
1. 버튼 누르면 LED 동작
2. 버튼 누르면 LED 꺼짐, 원활한 동작을 위해 버튼 동작시 딜레이 추가




---
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



---
## EXTI 사용하기

### step 1 : RTOS 제외
1. RTOS를 빌드에서 제외하고 진행

### step 2 : 버튼에 Exti 적용하기
1. [gpio.c](./Core/Src/gpio.c)의 버튼에 해당하는 부분 핀 모드 변경 및 인터럽트 우선순위 설정
2. [it.c](./Core/Src/it.c)에서 버튼핀(PB13)에 해당하는 부분에 핸들러 연결
3. [main.c](./Core/Src/main.c)에 핸들러 콜백 함수 생성

### step 3 : 빌드 & 테스트
1. 이전에 delay 넣었을 때보다 반응이 좋다.


---
## Timer 사용하기

### TIM Base

#### step 1 : 헤더와 c파일 만들기
1. [tim_defs.h](./Core/Inc/tim_defs.h)를 만들고 설정에 필요한 값을 적어준다.
2. [tim.c](./Core/Src/tim.c)에 설정을 사용할 TIM_Init() 함수를 만든다.
3. TIM6를 사용 할 것이니 RCC CLK를 Enable 해주고
4. Instance는 TIM6로 한다.
5. ARR을 100-1 로 고정해놓는다.
6. PSC는 시스템 클럭 / (타겟 클럭 * ARR) 로 계산한다.
7. 기타 설정은 TIM_HandleTypeDef 가 기술 되어있는 [stm32f4xx_hal_tim.h](STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim.h)[331:380]에 있다
8. Init과 Start 함수까지 포함해준다.

#### step 2 : main.h / main.c
1. [tim_defs.h](./Core/Inc/tim_defs.h)를 [main.h](./Core/Inc/main.h)에 추가해준다.
2. [main.c](/Core/Src/main.c)의 main() 함수에 TIM_Init의 Instance와 Init을 추가해준다.
3. 확인을 위해 TIM6->CNT 가 0이 될 때마다 LED_BLUE를 토글 해줬다.
4. 토클 되는데 10초가 걸린다...? / 버튼 LED가 동작 할 때면 10초가 걸린다?

### TIM Base IT

#### step 1 : 코드 수정
1. [tim.c](Core/Src/tim.c) HAL_TIM_Base_Start 대신 HAL_TIM_Base_Start_IT을 사용한다.
2. [tim.c](Core/Src/tim.c) IT을 사용 할 것이기 때문에 타이머에 해당 되는 인터럽트의 순위를 설정해주고 시작한다.
3. [it.c](Core/Src/it.c)에 타이머에 대항 되는 부분에 HAL_TIM_IRQHandler를 호출해준다.
4. [main.c](Core/Src/main.c)에 인터럽트 콜백을 설정해준다. 타이머가 오버플로우 되어 초기화 될 때 발생하는 인터럽트
5. 클럭이 너무 빨라 정확하지 않았던 LED_BLUE를 인터럽트 콜백 안으로 옮겨준다.

#### step 2 : 결과 확인
1. 0.1 초 간격으로 LED가 잘 반짝인다. 인터럽트를 사용하였기 때문에 이전 코드 때문에 밀리는 현상이 발생하지 않는다.


### TODO : 기타 타이머 추가 하기

---

## RTOS Task 만들기

### step 1 : RTOS 빌드에 추가
1. [CMakeLists.txt](./CMakeLists.txt)에서 RTOS 추가
2. [main.h](./Core/Inc/main.h) 헤더 추가
3. 빌드 하고 확인

