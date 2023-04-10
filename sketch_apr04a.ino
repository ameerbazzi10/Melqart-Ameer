#include "CytronMotorDriver.h"

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
const int buzzer = 29;
const int ledPin = 31;
long duration;
int distance;
int safetydistance;
int speed = 40;

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
int readDistance_FR() {
  // Clears the trigPin
  digitalWrite(trigPin_FR, LOW);
  delayMicroseconds(2);
  
  // Sets the TrigPin1on HIGH state for 10 micro seconds
  digitalWrite(trigPin_FR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_FR, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin_FR, HIGH);
  
  // Calculating the distance1
  return duration * 0.034 / 2;
}

int readDistance_FL() {
  // Clears the trigPin
  digitalWrite(trigPin_FL, LOW);
  delayMicroseconds(2);
  
  // Sets the TrigPin1on HIGH state for 10 micro seconds
  digitalWrite(trigPin_FL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_FL, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin_FL, HIGH);
  
  // Calculating the distance1
  return duration * 0.034 / 2;
}

int readDistance_R() {
  // Clears the trigPin
  digitalWrite(trigPin_R, LOW);
  delayMicroseconds(2);
  
  // Sets the TrigPin1on HIGH state for 10 micro seconds
  digitalWrite(trigPin_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_R, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin_R, HIGH);
  
  // Calculating the distance1
  return duration * 0.034 / 2;
}

void loop() {
// read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  motor1.setSpeed(0); // Motor 1 stops.
  motor2.setSpeed(0); // Motor 2 stops.

  
  
  // read distance
  safetydistance = readDistance();

if (safetydistance < 30.5)   // You can change safe distance1 from here changing value Ex. 20 , 40 , 60 , 80 , 100, all in cm
{
  speed = 0;
  


  digitalWrite(buzzer, HIGH);
  tone(buzzer, 990);
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  //delay(200);
  Serial.print("Speed: ");
  Serial.println(speed);
}

else if (safetydistance >= 30.5 && safetydistance <152.4)   // You can change safe distance1 from here changing value Ex. 20 , 40 , 60 , 80 , 100, all in cm
{
  speed = 20;
  Joystick();
  digitalWrite(buzzer, HIGH);
//  tone(buzzer, 400);
  delay(200);
  noTone(buzzer);
  delay(200);
  digitalWrite(ledPin, HIGH);
  Serial.print("Speed: ");
  Serial.println(speed);
}
  
else{
  speed = 40;
  Joystick();
  digitalWrite(buzzer, LOW);
  noTone(buzzer);
  digitalWrite(ledPin, LOW);
  Serial.print("Speed: ");
  Serial.println(speed);
}

 delay(250); //pause to let things settle

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(safetydistance);

}

void Joystick () {
//Left Case
  if (xValue < threshold)
  {
    motor1.setSpeed(speed + 10);  // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(-speed + 10); // Motor 2 runs backward at 50% speed.
    //Serial.print("\n");
    Serial.println("Left");
  }

  //Right Case
  else if (xValue > 1024 - threshold)
  {
    motor1.setSpeed(-speed + 10); // Motor 1 runs backward at 50% speed.
    motor2.setSpeed(speed + 10);  // Motor 2 runs forward at 50% speed.
    //Serial.print("\n");
    Serial.println("Right");
  }

  //Up Case
  else if (yValue < threshold)
  {
    motor1.setSpeed(speed); // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(speed); // Motor 2 runs forward at 50% speed.
    //Serial.print("\n");
    Serial.println("Up");
  }

  //Down Case
  else if (yValue > 1024 - threshold)
  {
    motor1.setSpeed(-speed); // Motor 1 runs backward at 50% speed.
    motor2.setSpeed(-speed); // Motor 2 runs backward at 50% speed.
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

void StopJoystickUP(){
  //Left Case
  if (xValue < threshold)
  {
    motor1.setSpeed(speed + 10);  // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(-speed + 10); // Motor 2 runs backward at 50% speed.
    //Serial.print("\n");
    Serial.println("Left");
  }

  //Right Case
  else if (xValue > 1024 - threshold)
  {
    motor1.setSpeed(-speed + 10); // Motor 1 runs backward at 50% speed.
    motor2.setSpeed(speed + 10);  // Motor 2 runs forward at 50% speed.
    //Serial.print("\n");
    Serial.println("Right");
  }

//  //Up Case
//  else if (yValue < threshold)
//  {
//    motor1.setSpeed(speed); // Motor 1 runs forward at 50% speed.
//    motor2.setSpeed(speed); // Motor 2 runs forward at 50% speed.
//    //Serial.print("\n");
//    Serial.println("Up");
//  }

  //Down Case
  else if (yValue > 1024 - threshold)
  {
    motor1.setSpeed(-speed); // Motor 1 runs backward at 50% speed.
    motor2.setSpeed(-speed); // Motor 2 runs backward at 50% speed.
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
void StopJoystickDOWN(){
  //Left Case
  if (xValue < threshold)
  {
    motor1.setSpeed(speed + 10);  // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(-speed + 10); // Motor 2 runs backward at 50% speed.
    //Serial.print("\n");
    Serial.println("Left");
  }

  //Right Case
  else if (xValue > 1024 - threshold)
  {
    motor1.setSpeed(-speed + 10); // Motor 1 runs backward at 50% speed.
    motor2.setSpeed(speed + 10);  // Motor 2 runs forward at 50% speed.
    //Serial.print("\n");
    Serial.println("Right");
  }

  //Up Case
  else if (yValue < threshold)
  {
    motor1.setSpeed(speed); // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(speed); // Motor 2 runs forward at 50% speed.
    //Serial.print("\n");
    Serial.println("Up");
  }
//
//  //Down Case
//  else if (yValue > 1024 - threshold)
//  {
//    motor1.setSpeed(-speed); // Motor 1 runs backward at 50% speed.
//    motor2.setSpeed(-speed); // Motor 2 runs backward at 50% speed.
//    //Serial.print("\n");
//    Serial.println("Down");
//  }

  //Stop Case
  else
  {
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    //Serial.print("\n");
    //Serial.print("Stop");
  }
}

 
