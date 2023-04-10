#include <NewPing.h>

// Define the trig and echo pins for each sensor
const int trigPin_FR = 35;
const int echoPin_FR = 34;
const int trigPin_FL = 36;
const int echoPin_FL = 37;
const int trigPin_R = 39;
const int echoPin_R = 38;

// Define the maximum distance we want to sense (in centimeters)
const int maxDistance = 200;

// Define the LED and buzzer pins
const int ledPin = 7;
const int buzzerPin = 6;

// Create instances of the NewPing library for each sensor
NewPing sonar_FR(trigPin_FR, echoPin_FR, maxDistance);
NewPing sonar_FL(trigPin_FL, echoPin_FL, maxDistance);
NewPing sonar_R(trigPin_R, echoPin_R, maxDistance);

void setup() {
  Serial.begin(9600);
  
  // Set the LED and buzzer pins as outputs
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Read the distances from each sensor
  int distance_FR = sonar_FR.ping_cm();
  int distance_FL = sonar_FL.ping_cm();
  int distance_R = sonar_R.ping_cm();

  // Print the distance readings in the serial monitor
  Serial.print("Front Right distance: ");
  Serial.print(distance_FR);
  Serial.print(" cm");

  Serial.print(" Front Left distance: ");
  Serial.print(distance_FL);
  Serial.print(" cm");

  Serial.print(" Rear distance: ");
  Serial.print(distance_R);
  Serial.println(" cm");

  // Check if any sensor detects an object less than 5 cm away
  if (distance_FR < 5 || distance_FL < 5 || distance_R < 5) {
    // Turn on the LED and make the buzzer beep continuously
    digitalWrite(ledPin, HIGH);
    //tone(buzzerPin, 890);
  } else {
    // Turn off the LED and stop the buzzer
    digitalWrite(ledPin, LOW);
    //noTone(buzzerPin);
  }

  delay(100);  // Wait for a short period before reading the sensors again
}
