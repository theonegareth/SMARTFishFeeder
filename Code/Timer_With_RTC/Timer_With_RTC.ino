#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

RTC_DS3231 rtc;
Servo servo;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  // Set the initial servo position
  servo.attach(9);
  servo.write(0);
}

void loop() {
  DateTime now = rtc.now();
  
  // Check if it's 6:00
  if (now.hour() == 6 && now.minute() == 0 && now.second() == 0) {
    // Turn on the servo for 30 seconds
    servo.write(90);
    delay(30000); // 30 seconds
    servo.write(0);
  }
  
  // Check if it's 18:00
  if (now.hour() == 18 && now.minute() == 0 && now.second() == 0) {
    // Turn on the servo for 30 seconds
    servo.write(90);
    delay(30000); // 30 seconds
    servo.write(0);
  }
  
  delay(1000); // Wait for 1 second
}