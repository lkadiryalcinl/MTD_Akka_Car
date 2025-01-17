#include <SoftwareSerial.h>
SoftwareSerial BTSerial(0, 1);

#define rightEcho 8
#define rightTrig 9

#define frontEcho 10
#define frontTrig 11

#define leftEcho 12
#define leftTrig 13

#define LDRPin A0
#define LDRLed 22

#define leftMotorBackward 7
#define leftMotorForward 4
#define rightMotorForward 13
#define rightMotorBackward 2
#define leftMotorEn 6
#define rightMotorEn 5

int motorSpeed = 1;
int rightDistance = 0;
int leftDistance = 0;
int frontDistance = 25;
int frontMinDistance = 30;
int leftRightMinDistance = 20;
boolean isMotorStarted = false;
boolean isMotorStopedTunel = false;

void setup() {

  Serial.begin(38400);
  BTSerial.begin(9600);

  pinMode(LDRLed, OUTPUT);

  // HC SR04 kurulumu
  setupDistanceSensors(rightTrig, rightEcho);
  setupDistanceSensors(frontTrig, frontEcho);
  setupDistanceSensors(leftTrig, leftEcho);

  // Motor pin ayarlamaları
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);
  pinMode(leftMotorEn, OUTPUT);
  pinMode(rightMotorEn, OUTPUT);

  Serial.println("ARDUINO STARTED");
}

void loop() {

  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
  if (BTSerial.available()) {
    char data = BTSerial.read();
    Serial.println(data);
    // if (data == 'a' || data == 'A') {
    //   isMotorStarted = !isMotorStarted;
    // }
    
    switch (data) {
      case 'U':
        forward();
        return;
      case 'D':
        backward();
        return;
      case 'L':
        left();
        return;
      case 'R':
        right();
        return;
      default:
        stopMotor();
        return;
    }
    
  }

  // if (isMotorStarted) {
  //   readDataFromLDR();
  //   rightDistance = readDataFromDistanceSensor(rightTrig, rightEcho, "Right");
  //   frontDistance = readDataFromDistanceSensor(frontTrig, frontEcho, "Front");
  //   leftDistance = readDataFromDistanceSensor(leftTrig, leftEcho, "Left");
  //   if (frontDistance > frontMinDistance) {
  //     if (leftDistance < leftRightMinDistance || rightDistance < leftRightMinDistance) {
  //       // Sağ tarafa yakın sola dön
  //       if (rightDistance < leftRightMinDistance) {
  //         left();
  //         delay(50);
  //       }
  //       // Sol taraf yakın sağa dön
  //       else if (leftDistance < leftRightMinDistance) {
  //         right();
  //         delay(60);
  //       }
  //     }
  //     forward();
  //     delay(100);
  //     return;
  //   } else if (frontDistance < frontMinDistance) {
  //     backward();
  //     delay(400);
  //     if (rightDistance < leftDistance) {
  //       left();
  //       delay(400);
  //     } else if (leftDistance < rightDistance) {
  //       right();
  //       delay(700);
  //     }
  //     return;
  //   }
  // } else {
  //   isMotorStopedTunel = false;
  //   stopMotor();
  // }
}

void forward() {
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(leftMotorBackward, LOW);

  analogWrite(rightMotorEn, 240);
  analogWrite(leftMotorEn, 240);
}

void backward() {
  digitalWrite(rightMotorBackward, HIGH);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorForward, LOW);

  analogWrite(rightMotorEn, 240);
  analogWrite(leftMotorEn, 240);
}

void right() {
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
 
  analogWrite(rightMotorEn, 0);
  analogWrite(leftMotorEn, 180);
}

void left() {
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);

  analogWrite(rightMotorEn, 20);
  analogWrite(leftMotorEn, 0);
}

void stopMotor() {
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(leftMotorForward, LOW);
  analogWrite(rightMotorEn, 0);
  analogWrite(leftMotorEn, 0);
}

void setupDistanceSensors(int trigPin, int echoPin) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

int readDataFromDistanceSensor(int trigPin, int echoPin, String sensorName) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long time = pulseIn(echoPin, HIGH);
  long distance = (time / 2) * 0.0343;

  /*Serial.print(sensorName);
  Serial.print(distance);
  Serial.println(" cm");*/
  return distance;
}

void readDataFromLDR() {
  int LDRValue = analogRead(LDRPin);
  Serial.println(LDRValue);
  if (LDRValue < 400 && !isMotorStopedTunel) {
    digitalWrite(LDRLed, HIGH);
    isMotorStopedTunel = true;
    stopMotor();
    delay(5000);
  } else {
    digitalWrite(LDRLed, LOW);
  }
}
