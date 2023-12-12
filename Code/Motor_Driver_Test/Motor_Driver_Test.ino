  // Define the pins connected to the L298N motor driver
  const int enablePin = 9;  // PWM pin for speed control
  const int in1Pin = 3;     // Input 1
  const int in2Pin = 2;     // Input 2

  void setup() {
    // Set the motor control pins as outputs
    pinMode(enablePin, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
  }

  void loop() {
    // Spin the fan in one direction (clockwise)
    spinClockwise();
    
    // Stop the fan

    
    // Spin the fan in the opposite direction (counter-clockwise)
    
  }

  void spinClockwise() {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    analogWrite(enablePin, 200); // Adjust PWM value for speed control (0-255)
  }

  void spinCounterClockwise() {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    analogWrite(enablePin, 200); // Adjust PWM value for speed control (0-255)
  }

  void stopFan() {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    analogWrite(enablePin, 0); // Stop the motor by setting PWM to 0
  }
