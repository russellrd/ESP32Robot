// Libraries
#include <ESP32_Servo.h> // Install ESP32Servo library

// I/O
#define TOUCH 15
#define LED 13
#define ECHO 5
#define TRIG 2
#define THRESH 10
#define LSERVO 32
#define RSERVO 33

long duration;
double cm, inches;
boolean start = false;

// Drive servos
Servo left, right;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Setup I/O
  left.attach(LSERVO);
  right.attach(RSERVO);
  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  if(start) {
    // Get distance from ultrasonic sensor
    digitalWrite(LED, HIGH);
    digitalWrite(TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    pinMode(ECHO, INPUT);
    duration = pulseIn(ECHO, HIGH);
    
    cm = (duration/2.0) / 29.1;
    inches = (duration/2.0) / 74.0;

    // Drive forward until distance is less than 5 inches.
    if(inches > 5) {
      left.write(110);
      right.write(65);
    } else {
      // Stop
      left.write(90);
      right.write(90);
      delay(500);
      // Move Back
      left.write(65);
      right.write(110);
      delay(500);
      // Stop
      left.write(90);
      right.write(90);
      delay(500);
      // Turn Right
      left.write(130);
      right.write(130);
      delay(200);
    }
  } else {
     // Start moving when frame is touched
     if(touchRead(TOUCH)<THRESH){
      start = true;
     }
  }
}
