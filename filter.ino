//#include는 코딩에 필요한 "도구 모음"인 "라이브러리"를 참조하기 위한 명령어입니다!
#include <Adafruit_MPU6050.h> 
#include <Adafruit_Sensor.h>
#include <Wire.h>

// mpu를 사용하겠다고 정의합시다.
Adafruit_MPU6050 mpu; 

// 가중치 상수 설정
const float alpha = 0.2; // 가속도계와 자이로스코프 데이터의 가중치 계수

// 변수들을 저장하기 위한 구조체 선언
typedef struct {
  float x;
  float y;
  float z;
} vector_float_t;

vector_float_t gyro_offset = {0, 0, 0}; // 자이로스코프 오프셋

void setup(void) { //아두이노가 켜질때 같이 실행됩니다. (단 1번)
  Serial.begin(115200); //115200번대의 시리얼 모니터에서 통신합니다. (간단한 설정을 변경해야해요)
  while (!Serial)
    delay(10); //혹~시나 시리얼 통신에 문제 생길 것을 대비한 코드입니다. 완전히 준비되었을 때만 통신을 시작해요

  Serial.println("MPU6050 테스트!"); //새로운 줄에 출력하는 명령어 입니다.

  // MPU 센서 설정을 해봅시다.
  if (!mpu.begin()) { //연결 실패의 경우
    Serial.println("MPU6050과 연결하는데 실패했습니다.");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050를 찾았습니다."); //연결 성공 

  //--------------------------------------- 센서의 상세 설정입니다.
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  Serial.println("");

  // 자이로스코프 오프셋 계산
  uint8_t i;
  for (i = 0; i < 100; i++) { // 초기 100회 측정을 통해 오프셋 계산
    sensors_event_t g;
    mpu.getGyroSensor();
    gyro_offset.x += g.gyro.x;
    gyro_offset.y += g.gyro.y;
    gyro_offset.z += g.gyro.z;
    delay(10);
  }
  gyro_offset.x /= 100.0;
  gyro_offset.y /= 100.0;
  gyro_offset.z /= 100.0;

  Serial.print("자이로스코프 오프셋: ");
  Serial.print(gyro_offset.x);
  Serial.print(", ");
  Serial.print(gyro_offset.y);
  Serial.print(", ");
  Serial.println(gyro_offset.z);

  Serial.println("");
  delay(100);
  //--------------------------------------- 설정 끝
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;

  float gx = g.gyro.x - gyro_offset.x;
  float gy = g.gyro.y - gyro_offset.y;
  float gz = g.gyro.z - gyro_offset.z;

  // 가중치를 적용하여 상보 필터 적용
  float angle_x = alpha * (angle_x + gx * 0.01) + (1 - alpha) * (atan2(-ay, -az) * 180.0 / PI);
  float angle_y = alpha * (angle_y + gy * 0.01) + (1 - alpha) * (atan2(ax, -az) * 180.0 / PI);

  // 시리얼 플로터에 출력하기 위해 CSV 형식으로 출력
  Serial.print(angle_x);
  Serial.print(",");
  Serial.println(angle_y);

  delay(10);
}
