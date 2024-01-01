#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x3F for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Initialize the LCD
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
  // Set the cursor to the first position of the first line
  lcd.setCursor(0, 0);
  // Print "Hello, World!"
  lcd.print("Hello, World!");
}

void loop() {
  // Nothing to do here
}
