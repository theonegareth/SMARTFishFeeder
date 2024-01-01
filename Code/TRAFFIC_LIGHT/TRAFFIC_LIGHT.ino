// Define LED pins
int redLedPin = 8;
int yellowLedPin = 9;
int greenLedPin = 10;

void setup() {
  // Set LED pins as outputs
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  // Turn on green light for 1 seconds
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(redLedPin, LOW);
  delay(1000);
  
  // Turn off green light and turn on yellow light for 1 seconds
  digitalWrite(greenLedPin, LOW);
  digitalWrite(yellowLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  delay(1000);

  // Turn off yellow light and turn on red light for 1 seconds
  digitalWrite(greenLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
  delay(1000);

}

