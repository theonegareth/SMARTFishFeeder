#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

// Initialize the RTC library
RTC_DS3231 rtc;

// Initialize the LCD on address 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the I2C communication
  Wire.begin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize the RTC module
  if (!rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  // This line sets the RTC with an explicit date & time
  // For example, to set January 2, 2024, at 15:30:00 you would call:
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  // Get the current time
  DateTime now = rtc.now();

  // Clear the LCD and set the cursor to the beginning
  lcd.clear();
  lcd.setCursor(0, 0);

  // Print the current time to the LCD
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  // Wait for a second
  delay(1000);
}
