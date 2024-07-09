//#include는 코딩에 필요한 "도구 모음"인 "라이브러리"를 참조하기 위한 명령어입니다!
#include <Adafruit_MPU6050.h> 
#include <Adafruit_Sensor.h>
#include <Wire.h>

//mpu를 사용하겠다고 정의합시다.
Adafruit_MPU6050 mpu; 

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

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
  //--------------------------------------- 설정 끝
}

void loop() {

  /* 항시 값을 읽어들이는 명령어 */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* 읽어온 값을 출력 */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);

  //print와 println의 차이는 엔터를 치냐 안치냐 차이입니다!
}
