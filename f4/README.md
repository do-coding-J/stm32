## Cmake 사용하여 STM32F4 HAL 빌드하기 

### Step 1 : 빌드 환경 만들기
1. [STM32CubeF4](https://github.com/STMicroelectronics/STM32CubeF4) 레포지토리 클론하기  
2. [arm.toolchain](arm.toolchain) 크로스 컴파일러 설정하기
3. [CMakeLists.txt](CMakeLists.txt)[1:7] 프로젝트 정의 및 크로스 컴파일 추가

### Step 2 : STM32CubeF4 라이브러리로 빌드
1. [CMakeLists.txt](CMakeLists.txt)[9:20] MCU 타겟 및 경로 설정
2. [STM32CubeF4.cmake](STM32CubeF4.cmake)[1:14] HAL 및 CMSIS 라이브러리로 빌드

### Step 3 : 