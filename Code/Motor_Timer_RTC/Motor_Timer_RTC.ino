#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool triggeredMorning = false;
bool triggeredEvening = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight

  // Attaches the servo on analog pin A0 to the servo object
  // Use the digital pin number equivalent for A0
  servo.attach(A0);  // For Uno, Nano, etc., you can also use myServo.attach(14);

  // Set the RTC to a specific date and time (only once, then comment it out)
  rtc.adjust(DateTime(2024, 1, 1, 5, 59, 55));

}

void loop() {
  DateTime now = rtc.now();

  // Check if it's 6:00 AM and not already triggered
  if (now.hour() == 6 && now.minute() == 0 && !triggeredMorning) {
    // Turn on the servo for 30 seconds
    servo.write(90);
    delay(30000); // 30 seconds
    servo.write(0);
    triggeredMorning = true;
  }

  // Reset the morning trigger after 6:00 AM
  if (now.hour() == 6 && now.minute() > 0) {
    triggeredMorning = false;
  }
  
  // Check if it's 6:00 PM and not already triggered
  if (now.hour() == 18 && now.minute() == 0 && !triggeredEvening) {
    // Turn on the servo for 30 seconds
    servo.write(90);
    delay(30000); // 30 seconds
    servo.write(0);
    triggeredEvening = true;
  }

  // Reset the evening trigger after 6:00 PM
  if (now.hour() == 18 && now.minute() > 0) {
    triggeredEvening = false;
  }

  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second());
  Serial.println();

  delay(1000); // Wait for 1 second
}