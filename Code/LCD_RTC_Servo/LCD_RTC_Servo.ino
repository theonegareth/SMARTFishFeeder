#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool triggeredMorning = false;
bool triggeredEvening = false;
unsigned long servoMoveStartTime = 0;
const long servoMoveDuration = 30000;  // Duration to move servo (30 seconds)

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight

  servo.attach(A0);  // Attach the servo on analog pin A0

  // Set the RTC to a specific date and time (only once, then comment it out)
  rtc.adjust(DateTime(2024, 1, 1, 5, 59, 55));
}

void loop() {
  DateTime now = rtc.now();

  // Check if it's 6:00 AM or just after and not already triggered
  if (now.hour() == 6 && now.minute() == 0 && !triggeredMorning) {
    if (now.second() == 0 || (now.second() > 0 && now.second() <= 10)) {
      servo.write(90);  // Move servo
      servoMoveStartTime = millis();  // Record the time when servo moved
      triggeredMorning = true;
      servo.write(0);   // Sets the servo position to 0 degrees
    }
  } else if (now.hour() == 6 && now.minute() > 0 && triggeredMorning) {
    // Reset the morning trigger after 6:00 AM
    triggeredMorning = false;
  }

  // Check if it's 6:00 PM or just after and not already triggered
  if (now.hour() == 18 && now.minute() == 0 && !triggeredEvening) {
    if (now.second() == 0 || (now.second() > 0 && now.second() <= 10)) {
      servo.write(90);  // Move servo
      servoMoveStartTime = millis();  // Record the time when servo moved
      triggeredEvening = true;
      servo.write(0);   // Sets the servo position to 0 degrees
    }
  } else if (now.hour() == 18 && now.minute() > 0 && triggeredEvening) {
    // Reset the evening trigger after 6:00 PM
    triggeredEvening = false;
  }

  // Stop the servo after 30 seconds
  if ((millis() - servoMoveStartTime >= servoMoveDuration) && (triggeredMorning || triggeredEvening)) {
    servo.write(0);  // Reset servo position
  }

  // Display time on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());

  // Print time to Serial Monitor
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
