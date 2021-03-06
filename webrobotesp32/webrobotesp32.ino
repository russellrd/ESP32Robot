// Libraries
#include <ESP32_Servo.h> // Install ESP32Servo library
#include <WiFi.h>
#include <AsyncTCP.h> // Install https://github.com/me-no-dev/AsyncTCP
#include <ESPAsyncWebServer.h> // Install https://github.com/me-no-dev/ESPAsyncWebServer

// I/O
#define TOUCH 15
#define LED 13
#define ECHO 5
#define TRIG 2
#define THRESH 10
#define LSERVO 32
#define RSERVO 33

const char* ssid = "SSID";
const char* password = "PASSWORD";

int pos = 0;
long duration;
double cm, inches;

// Drive servos
Servo left, right;

AsyncWebServer server(80);

// Code for website
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js"></script>
    <title>Robot Control Panel</title>
</head>
<body>
<h1><b>R</b>obot <b>C</b>ontrol <b>P</b>anel</h1>
<div class="grid-container">
    <div class="empty-item"></div>
    <a href="/forward"><div class="grid-item">^</div></a>
    <div class="empty-item"></div>
    <a href="/left"><div class="grid-item"><</div></a>
    <a href="/backward"><div class="grid-item">v</div></a>
    <a href="/right"><div class="grid-item">></div></a>
</div>
</body>
<style>
h1 {
    background-color: black;
    color: white;
    text-align: center;
    margin: 0;
}
.grid-container {
    display: grid;
    grid-template-columns: auto auto auto;
}
.grid-item {
    background-color: lightseagreen;
    color: white;
    border: 0px;
    padding: 20px;
    font-size: 30px;
    text-align: center;
}
.empty-item {
    background-color: rgba(255, 255, 255, 0.8);
    padding: 20px;
    font-size: 30px;
    text-align: center;
}
</style>
</html>
)rawliteral";

// Drive for 0.8 seconds with a left and right speed
void drive(int leftSpeed, int rightSpeed) {
  left.write(leftSpeed);
  right.write(rightSpeed);
  delay(800);
  left.write(90);
  right.write(90);
}

void setup() {
  Serial.begin(115200);

  // Setup I/O
  left.attach(LSERVO);
  right.attach(RSERVO);
  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(LED, LOW);

  // Setup wifi
  WiFi.begin(ssid, password);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
    Serial.print (".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Serve html page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Serve and move forward
  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    drive(105, 65);
  });

  // Serve and move backward
  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    drive(70, 110);
  });

  // Serve and move right
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    drive(110, 110);
  });

  // Serve and move left
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    drive(70, 70);
  });
  
  server.begin();
}

void loop() {
  // Turn on led if frame is touched
  if(touchRead(TOUCH)<THRESH){
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
