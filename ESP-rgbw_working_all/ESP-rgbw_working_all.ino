#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "Heisenberg";
const char *password = "gAte2TheWorld@MolEl";

const int redLedPin = 14;    // PWM pin for the red LED
const int greenLedPin = 12;  // PWM pin for the green LED
const int blueLedPin = 13;   // PWM pin for the blue LED
const int whiteLedPin = 15;  // PWM pin for the white LED

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String htmlContent = "<!DOCTYPE html>"
                         "<html>"
                         "<head>"
                         "<script>"
                         "function updateLed(led, value) {"
                         "  var xhr = new XMLHttpRequest();"
                         "  xhr.open('GET', '/setBrightness?led=' + led + '&value=' + value, true);"
                         "  xhr.send();"
                         "}"
                         "</script>"
                         "</head>"
                         "<body>"
                         "<h2>LED Control</h2>"
                         "<p>Red: <input type='range' id='red' min='0' max='1023' onchange='updateLed(\"red\", this.value)'></p>"
                         "<p>Green: <input type='range' id='green' min='0' max='1023' onchange='updateLed(\"green\", this.value)'></p>"
                         "<p>Blue: <input type='range' id='blue' min='0' max='1023' onchange='updateLed(\"blue\", this.value)'></p>"
                         "<p>White: <input type='range' id='white' min='0' max='1023' onchange='updateLed(\"white\", this.value)'></p>"
                         "</body>"
                         "</html>";

    request->send(200, "text/html", htmlContent);
  });

  // Set brightness endpoint
  server.on("/setBrightness", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("led") && request->hasParam("value")) {
      String led = request->getParam("led")->value();
      int brightness = request->getParam("value")->value().toInt();

      if (led == "red") {
        analogWrite(redLedPin, brightness);
      } else if (led == "green") {
        analogWrite(greenLedPin, brightness);
      } else if (led == "blue") {
        analogWrite(blueLedPin, brightness);
      } else if (led == "white") {
        analogWrite(whiteLedPin, brightness);
      }

      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  server.begin();

  // Initialize PWM pins
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(whiteLedPin, OUTPUT);

  analogWrite(redLedPin, 0);
  analogWrite(greenLedPin, 0);
  analogWrite(blueLedPin, 0);
  analogWrite(whiteLedPin, 0);
}

void loop() {
  // Your code here (if needed)
}
