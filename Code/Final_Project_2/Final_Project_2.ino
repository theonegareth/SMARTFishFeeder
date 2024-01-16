#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Feeding time variables
const int morningFeedingHour = 6;
const int morningFeedingMinute = 0;
const int eveningFeedingHour = 18;
const int eveningFeedingMinute = 0;

// Servo timing variables
const int servoStart = 0; // Time in seconds after which servo starts moving
const int servoStop = 5;  // Time in seconds after which servo stops moving
const int servoDelay = 1; // Time in seconds which servo moves

// Ultrasonic sensor pins for three sensors
const int triggerPin = 6; // New pins for the third sensor
const int echoPin = 7;

// Constants for tank dimensions
const int tankDepth = 20; // Depth of the tank in centimeters
const int maxDistance = 200; // Maximum distance before displaying "OVERLOAD"

// Variables for calculating fullness percentage
long duration;
float distance;
int fullnessPercentage;

// Status message variable
String statusMessage;

// LCD configuration
const int lcdAddress = 0x27;  // Change this address to match your module
const int lcdColumns = 20;
const int lcdRows = 4;
// Initialize the LCD object
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

RTC_DS3231 rtc;
Servo servo;

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
  rtc.adjust(DateTime(2024, 1, 1, 5, 59, 54));

  // Set the trigger and echo pins as output and input respectively for all sensors
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  DateTime now = rtc.now();
  // Measure distance for each sensor
  distance = measureDistance(triggerPin, echoPin);

  // Check if the average distance exceeds the maximum distance
  if (distance > maxDistance) {
    // Display "OVERLOAD"
    statusMessage = "OVERLOAD";
  } else {
    // Calculate the percentage of fullness
    fullnessPercentage = map(distance, 3, tankDepth, 100, 0);
    fullnessPercentage = constrain(fullnessPercentage, 0, 100);
    statusMessage = "OK";
  // Check if it's morning or evening feeding time and not already triggered
  if ((now.hour() == morningFeedingHour && now.minute() == morningFeedingMinute) ||
      (now.hour() == eveningFeedingHour && now.minute() == eveningFeedingMinute)) {
    handleFeeding(now);
  }

  // Check if it's morning feeding time and not already triggered
  if (now.hour() == morningFeedingHour && now.minute() == morningFeedingMinute) {
    handleFeeding(now);
  }

  // Check if it's evening feeding time and not already triggered
  if (now.hour() == eveningFeedingHour && now.minute() == eveningFeedingMinute) {
    handleFeeding(now);
  }
  

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

  // Print the fullness percentage, average distance, and status to the serial monitor
  Serial.print("Fullness: ");
  Serial.print(fullnessPercentage);
  Serial.println("%");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm ");

  // Display time on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.year());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.day());
  lcd.print(" ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());

  // Print status to Serial Monitor
  Serial.print("Status: ");
  Serial.println(statusMessage);

  // Display the status on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  lcd.print(statusMessage);

  // Display the percentage and status on the LCD
  lcd.setCursor(0, 2);
  lcd.print("Fullness: ");
  lcd.print(fullnessPercentage);
  lcd.print("%");
  }
  
  servo.write(90);  // Stop servo movement

  // Delay before the next reading
  delay(100);
}

void handleFeeding(DateTime now) {
  if (now.second() == servoStart) {
    servo.write(30);  // Move servo a bit clockwise
    statusMessage = "Moving";
  } else if (now.second() == servoStart + servoDelay) {
    servo.write(90);  // Stop servo movement
    statusMessage = "Open";
  } else if (now.second() == servoStop) {
    servo.write(330);  // Move servo a bit counterclockwise
    statusMessage = "Moving";
  } else if (now.second() == servoStop + servoDelay) {
    servo.write(90);  // Stop servo movement
    statusMessage = "Closed";
  }
}

float measureDistance(int triggerPin, int echoPin) {
  // Clear the trigger pin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Activate the trigger pulse
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  float distance = duration * 0.034 / 2.0;
  return distance;
}

