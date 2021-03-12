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
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css">
  <script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js"></script>
  <title>Robot Control Panel</title>
</head>
<body>
<div class="row">
<a href="/forward"><button class="waves-effect waves-light btn">Forward</button></a>
<a href="/backward"><button class="waves-effect waves-light btn">Backward</button></a>
<a href="/left"><button class="waves-effect waves-light btn">Left</button></a>
<a href="/right"><button class="waves-effect waves-light btn">Right</button></a><br />
</div>
</body>
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
