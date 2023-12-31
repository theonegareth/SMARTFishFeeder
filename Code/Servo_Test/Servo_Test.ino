#include <Servo.h>

Servo myServo;  // Create a servo object

void setup() {
  // Attaches the servo on analog pin A0 to the servo object
  // Use the digital pin number equivalent for A0
  myServo.attach(A0);  // For Uno, Nano, etc., you can also use myServo.attach(14);
}

void loop() {
  myServo.write(90);  // Sets the servo position to 90 degrees
  delay(1000);        // Wait for a second
  myServo.write(0);   // Sets the servo position to 0 degrees
  delay(1000);        // Wait for a second
}
