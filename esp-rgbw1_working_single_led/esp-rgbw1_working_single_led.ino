#include <ESP8266WiFi.h>

#include <ESPAsyncTCP.h>

#include <ESPAsyncWebServer.h>

// Replace with your network credentials

//WiFi Connection configuration

const char *ssid = "Heisenberg"; // your SSID
const char *password = "gAte2TheWorld@MolEl"; // YOUR wifi PASSWORD
const int output = 14;
String sliderValue = "0";
const char* PARAM_INPUT = "value";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP8266 Web PWM Control Server</title>
  <style>
    html {font-family: Arial; display: inline-block;  margin: 0px auto; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 500px; margin:0px auto; padding-bottom: 25px;}
    .slidecontainer {
        width: 100%;
      }
      .slider {
        -webkit-appearance: none; 
        width: 350px;
        height: 15px;
        background: grey;
        outline: none;
        opacity: 0.5;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }
      .slider:hover {
        opacity: 2;
      }
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 25px;
        height: 25px;
        background: magenta;
        cursor: pointer;
      }
      .slider::-moz-range-thumb {
        width: 25px;
        height: 25px;
        background: #4CAF50;
        cursor: pointer;
      }
  </style>
</head>
<body>
  <h2>ESP8266 Web PWM Control Server</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="1023" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page

String processor(const String& var){

  //Serial.println(var);

  if (var == "SLIDERVALUE"){

    return sliderValue;

  }

  return String();

}

void setup(){

  // Serial port for debugging purposes

  Serial.begin(115200);

  analogWrite(output, sliderValue.toInt());

  // Connect to Wi-Fi

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);

    Serial.println("Connecting to WiFi..");

  }

  // Print ESP Local IP Address

  Serial.println(WiFi.localIP());

  // Route for root / web page

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

    request->send_P(200, "text/html", index_html, processor);

  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {

    String inputMessage;

    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>

    if (request->hasParam(PARAM_INPUT)) {

      inputMessage = request->getParam(PARAM_INPUT)->value();

      sliderValue = inputMessage;

      analogWrite(output, sliderValue.toInt());

    }

    else {

      inputMessage = "No message sent";

    }

    Serial.println(inputMessage);

    request->send(200, "text/plain", "OK");

  });

  

  // Start server

  server.begin();

}

  

void loop() {

  

}
