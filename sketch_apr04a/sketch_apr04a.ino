#include "CytronMotorDriver.h"
#include <NewPing.h>

#define VRX_PIN A0 // Arduino pin connected to VRX pin at analog pin A0
#define VRY_PIN A1 // Arduino pin connected to VRY pin at analog pin A1
#define SW_PIN 2   // Arduino pin connected to SW  pin at digital pin 2
// defines pins numbers
const int trigPin_FR= 35;
const int echoPin_FR = 34;
const int trigPin_FL = 36;
const int echoPin_FL = 37;
const int trigPin_R= 39;
const int echoPin_R = 38;

// Define the maximum distance we want to sense (in centimeters)
const int maxDistance = 2000;

const int buzzer = 6;
const int ledPin = 7;
long duration;
int distance;
int safetydistance;

int speedUp = 40;
int speedDown = 40;
int speedSides = 40;

NewPing sonar_FR(trigPin_FR, echoPin_FR, maxDistance);
NewPing sonar_FL(trigPin_FL, echoPin_FL, maxDistance);
NewPing sonar_R(trigPin_R, echoPin_R, maxDistance);

// Configure the motor driver.
CytronMD motor1(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 11, 10); // PWM 2 = Pin 11, DIR 2 = Pin 10.

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

int threshold = 50; // Threshold value for joystick movement detection

bool StopFlag = 0;

const int Stop_ledPin = 11;

void setup() {
  pinMode(trigPin_FR, OUTPUT); // Sets the TrigPin1as an Output
  pinMode(echoPin_FR, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin_FL, OUTPUT); // Sets the TrigPin1as an Output
  pinMode(echoPin_FL, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin_R, OUTPUT); // Sets the TrigPin1as an Output
  pinMode(echoPin_R, INPUT); // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(Stop_ledPin, INPUT_PULLUP);
  Serial.begin(9600); // Starts the serial communication
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

// read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  motor1.setSpeed(0); // Motor 1 stops.
  motor2.setSpeed(0); // Motor 2 stops.

  
  
  // read distance
  safetydistance = readDistance();

  // Check if any sensor detects an object less than 5 cm away
  if ((distance_FR < 30.5 || distance_FL < 30.5) && distance_R >= 30.5 ) {
    // Code for front stop
    speedUp = 0;
    SpeedDown = 40;
    // Turn on the LED and make the buzzer beep continuously
    digitalWrite(ledPin, HIGH);
    //tone(buzzerPin, 890);
  } else if ((distance_FR >= 30.5 && distance_FL >= 30.5) && distance_R < 30.5 ) {
    // Code for rear stop
    speedUp = 40;
    SpeedDown = 0;

    // Turn off the LED and stop the buzzer
    digitalWrite(ledPin, LOW);
    //noTone(buzzerPin);
  } else if((distance_FR < 30.5 || distance_FL < 30.5) && distance_R < 30.5 ){
    // stop Both
    speedUp = 0;
    SpeedDown = 0;
  } else if(((distance_FR >= 30.5 && distance_FR < 152.4) || (distance_FL >= 30.5 && distance_FL < 152.4)) && (distance_R >= 152.4) ) {
    speedUp = 20;
    speedDown = 40;
    // Turn on the LED and make the buzzer beep continuously
    digitalWrite(ledPin, HIGH);
    //tone(buzzerPin, 890);
  } else if(distance_FR >= 152.4 && distance_FL >= 152.4 && distance_R >=30.5 && distance_R < 152.4) ) {
    speedUp = 40;
    speedDown = 20;
    // Turn on the LED and make the buzzer beep continuously
    digitalWrite(ledPin, HIGH);
    //tone(buzzerPin, 890);
  } else if(distance_FR >= 152.4 || distance_FL >= 152.4 || distance_R >= 152.4 ){
    speedUp = 20;
    speedDown = 20;
  } 


    Serial.print("SpeedUp= ");
    Serial.print(speedUp);
    Serial.print("; SpeedDown= ");
    Serial.print(speedDown);
  Joystick ();

 delay(250); //pause to let things settle

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(safetydistance);

}

void Joystick () {
//Left Case
  if (xValue < threshold)
  {
    motor1.setSpeed(speedSides + 10);  // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(-speedSides + 10); // Motor 2 runs backward at 50% speed.
    //Serial.print("\n");
    Serial.println("Left");
  }

  //Right Case
  else if (xValue > 1024 - threshold)
  {
    motor1.setSpeed(-speedSides + 10); // Motor 1 runs backward at 50% speed.
    motor2.setSpeed(speedSides + 10);  // Motor 2 runs forward at 50% speed.
    //Serial.print("\n");
    Serial.println("Right");
  }

  //Up Case
  else if (yValue < threshold)
  {
    motor1.setSpeed(speedUp); // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(speedUp); // Motor 2 runs forward at 50% speed.
    //Serial.print("\n");
    Serial.println("Up");
  }

  //Down Case
  else if (yValue > 1024 - threshold)
  {
    motor1.setSpeed(-speedDown); // Motor 1 runs backward at 50% speed.
    motor2.setSpeed(-speedDown); // Motor 2 runs backward at 50% speed.
    //Serial.print("\n");
    Serial.println("Down");
  }

  //Stop Case
  else
  {
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    //Serial.print("\n");
    //Serial.print("Stop");
  }
}
