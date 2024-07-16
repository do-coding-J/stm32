## STM32의 IO를 이용한 기능 작성하기

### step 1 : 이전 작업 불러오기
1. 이전 작업은 [f4](../f4/)에 있다. ([README.md](../f4/README.md))  
2. 빌드가 잘 되는지 확인
3. 디버깅 모드로 uwTick이 잘 올라가는지 확인 한다.

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

#### END