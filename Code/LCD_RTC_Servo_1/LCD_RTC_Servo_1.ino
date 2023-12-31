#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long servoMoveStartTime = 0;
const long servoMoveDuration = 5000;  // Duration to move servo (5 seconds)

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
  servo.write(90); // Start with the servo at neutral position

  // Set the RTC to a specific date and time (only once, then comment it out)
  rtc.adjust(DateTime(2024, 1, 1, 5, 59, 55));
}

void loop() {
  DateTime now = rtc.now();

  // Check if it's 6:00 AM and not already triggered
  if (now.hour() == 6 && now.minute() == 0) {
    if (now.second() == 0) {
      servo.write(1);  // Move servo a bit clockwise
    } else if (now.second() == 1) {
      servo.write(90);  // Stop servo movement
    } else if (now.second() == 5) {
      servo.write(359);  // Move servo a bit counterclockwise
    } else if (now.second() == 6) {
      servo.write(90);  // Stop servo movement
    }
  }

  // Check if it's 6:00 PM and not already triggered
  if (now.hour() == 18 && now.minute() == 0) {
    if (now.second() == 0) {
      servo.write(1);  // Move servo a bit clockwise
    } else if (now.second() == 1) {
      servo.write(90);  // Stop servo movement
    } else if (now.second() == 5) {
      servo.write(359);  // Move servo a bit counterclockwise
    } else if (now.second() == 6) {
      servo.write(90);  // Stop servo movement
    }
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

  servo.write(90);  // Stop servo movement
  
  delay(200); // Delay for 200 milliseconds
}
