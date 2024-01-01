#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(9600);
  
  // initialize the LCD
  Serial.println("Initializing LCD...");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  
  Serial.println("LCD initialized!");
}

void loop()
{
  Serial.println("Writing to LCD...");
  lcd.print("Hello, World");
  
  delay(1000);
}
