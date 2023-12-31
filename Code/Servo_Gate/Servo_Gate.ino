#include <Servo.h>

Servo myServo;
int servoPin = A0;  // Change to the pin you are using

void setup() {
  myServo.attach(servoPin);
  myServo.write(90);
}

void loop() {
  myServo.write(1);
  delay(200);
  myServo.write(90);
  delay(2000);
  myServo.write(359);
  delay(200);
  myServo.write(90);
  delay(2000);
}
