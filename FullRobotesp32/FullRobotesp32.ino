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
#define UNITS Freedom

enum Units {
  Freedom,
  Metric
};

class Ultrasonic {
  private:
    int echoPin, trigPin;
    Units units;
  
  public:
    Ultrasonic(int echoPin, int trigPin, Units units) {
      this->echoPin = echoPin;
      this->trigPin = trigPin;
      this->units = units;
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
    }
  
    double getDist() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      pinMode(echoPin, INPUT);

      switch(units) {
        case Metric:
          return (pulseIn(echoPin, HIGH)/2.0) / 29.1;
          break;
        case Freedom:
          return (pulseIn(echoPin, HIGH)/2.0) / 74.0;
          break;
        default:
          break;
      }
    }
};

class Robot {
  private:
    Servo left, right;
    Units units;

  public:
    Ultrasonic ultra = Ultrasonic(ECHO, TRIG, units);
    
    Robot(int leftPin, int rightPin, Units units) {
      left.attach(leftPin);
      right.attach(rightPin);
      this->units = units;
    }
  
    void drive(int leftSpeed, int rightSpeed, int t) {
      left.write(leftSpeed);
      right.write(rightSpeed);
      delay(t);
      left.write(90);
      right.write(90);
    }
};

Robot robot = Robot(LSERVO, RSERVO, UNITS);

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  if(touchRead(TOUCH)<THRESH){
    robot.drive(105, 65, 500);
    Serial.println(robot.ultra.getDist());
  }
}
