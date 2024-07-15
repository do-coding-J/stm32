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

### END