#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
const char *ssid = "Heisenberg";
const char *password = "gAte2TheWorld@MolEl";

const int redLedPin = 14;    // PWM pin for the red LED
const int greenLedPin = 12;  // PWM pin for the green LED
const int blueLedPin = 13;   // PWM pin for the blue LED
const int whiteLedPin = 15;  // PWM pin for the white LED

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, payload);

    if (doc.containsKey("led") && doc.containsKey("value")) {
      String led = doc["led"];
      int brightness = doc["value"];

      if (led == "red") {
        analogWrite(redLedPin, brightness);
      } else if (led == "green") {
        analogWrite(greenLedPin, brightness);
      } else if (led == "blue") {
        analogWrite(blueLedPin, brightness);
      } else if (led == "white") {
        analogWrite(whiteLedPin, brightness);
      }

      webSocket.broadcastTXT(payload, length);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // WebSocket handler
  webSocket.begin();
  webSocket.onEvent(handleWebSocketMessage);

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String htmlContent = "<!DOCTYPE html>"
                         "<html>"
                         "<head>"
                         "<script>"
                         "var socket = new WebSocket('ws://' + window.location.hostname + ':81/');"
                         "socket.onmessage = function (event) {"
                         "  var data = JSON.parse(event.data);"
                         "  document.getElementById(data.led).value = data.value;"
                         "  document.getElementById(data.led + '-value').innerText = data.value;"
                         "};"
                         "function updateLed(led, value) {"
                         "  var data = {"
                         "    led: led,"
                         "    value: value"
                         "  };"
                         "  socket.send(JSON.stringify(data));"
                         "}"
                         "</script>"
                         "</head>"
                         "<body>"
                         "<h2>LED Control</h2>"
                         "<p>Red: <input type='range' id='red' min='0' max='1023' step='1' onchange='updateLed(\"red\", this.value)'></p>"
                         "<p>Green: <input type='range' id='green' min='0' max='1023' step='1' onchange='updateLed(\"green\", this.value)'></p>"
                         "<p>Blue: <input type='range' id='blue' min='0' max='1023' step='1' onchange='updateLed(\"blue\", this.value)'></p>"
                         "<p>White: <input type='range' id='white' min='0' max='1023' step='1' onchange='updateLed(\"white\", this.value)'></p>"
                         "<p>Red Value: <span id='red-value'>0</span></p>"
                         "<p>Green Value: <span id='green-value'>0</span></p>"
                         "<p>Blue Value: <span id='blue-value'>0</span></p>"
                         "<p>White Value: <span id='white-value'>0</span></p>"
                         "</body>"
                         "</html>";

    request->send(200, "text/html", htmlContent);
  });

  // Initialize PWM pins
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(whiteLedPin, OUTPUT);

  analogWrite(redLedPin, 0);
  analogWrite(greenLedPin, 0);
  analogWrite(blueLedPin, 0);
  analogWrite(whiteLedPin, 0);
  
  webSocket.begin();
  webSocket.onEvent(handleWebSocketMessage);
  server.begin();
}

void loop() {
  webSocket.loop();
  // Your code here (if needed)
}
