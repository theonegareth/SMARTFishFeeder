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
const int servoStartDelay = 1; // Time in seconds after which servo starts moving
const int servoStopDelay = 5;  // Time in seconds after which servo stops moving

// Ultrasonic sensor pins for three sensors
const int triggerPin1 = 2;
const int echoPin1 = 3;
const int triggerPin2 = 4; // New pins for the second sensor
const int echoPin2 = 5;
const int triggerPin3 = 6; // New pins for the third sensor
const int echoPin3 = 7;

// Constants for tank dimensions
const int tankDepth = 20; // Depth of the tank in centimeters
const int maxDistance = 400; // Maximum distance before displaying "OVERLOAD"

// Variables for calculating fullness percentage
long duration1, duration2, duration3;
float distance1, distance2, distance3, averageDistance;
int fullnessPercentage;

// LCD configuration
const int lcdAddress = 0x27;  // Change this address to match your module
const int lcdColumns = 16;
const int lcdRows = 2;
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
  rtc.adjust(DateTime(2024, 1, 1, 5, 59, 55));

  // Set the trigger and echo pins as output and input respectively for all sensors
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(triggerPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}

void loop() {
  DateTime now = rtc.now();
  // Measure distance for each sensor
  distance1 = measureDistance(triggerPin1, echoPin1);
  distance2 = measureDistance(triggerPin2, echoPin2);
  distance3 = measureDistance(triggerPin3, echoPin3);

  // Calculate the average distance
  averageDistance = (distance1 + distance2 + distance3) / 3.0;

  // Check if the average distance exceeds the maximum distance
  if (averageDistance > maxDistance) {
    // Display "OVERLOAD"
    Serial.println("OVERLOAD");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fullness: N/A");
    lcd.setCursor(0, 1);
    lcd.print("Status: OVERLOAD");
  } else {
    // Calculate the percentage of fullness
    fullnessPercentage = map(averageDistance, 3, tankDepth, 100, 0);
    fullnessPercentage = constrain(fullnessPercentage, 0, 100);

    // Print the fullness percentage and average distance to the serial monitor
    Serial.print("Fullness: ");
    Serial.print(fullnessPercentage);
    Serial.println("%");
    Serial.print("Distance1: ");
    Serial.print(distance1);
    Serial.println(" cm");
    Serial.print("Distance2: ");
    Serial.print(distance2);
    Serial.println(" cm");
    Serial.print("Distance3: ");
    Serial.print(distance3);
    Serial.println(" cm");
    Serial.print("averageDistance: ");
    Serial.print(averageDistance);
    Serial.println(" cm");

    // Display the percentage and status on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fullness: ");
    lcd.print(fullnessPercentage);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Status: OK");
  }

  // Check if it's morning feeding time and not already triggered
  if (now.hour() == morningFeedingHour && now.minute() == morningFeedingMinute) {
    handleFeeding(now);
  }

  // Check if it's evening feeding time and not already triggered
  if (now.hour() == eveningFeedingHour && now.minute() == eveningFeedingMinute) {
    handleFeeding(now);
  }
  // Display time on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Current Time: ");
  lcd.setCursor(0, 1);
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
  // Delay before the next reading
  delay(200);
}

void handleFeeding(DateTime now) {
  if (now.second() == servoStartDelay) {
    servo.write(1);  // Move servo a bit clockwise
  } else if (now.second() == servoStartDelay + 1) {
    servo.write(90);  // Stop servo movement
  } else if (now.second() == servoStopDelay) {
    servo.write(359);  // Move servo a bit counterclockwise
  } else if (now.second() == servoStopDelay + 1) {
    servo.write(90);  // Stop servo movement
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

