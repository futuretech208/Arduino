/*
  Open browser, visit 192.168.4.1
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "Shutter Remote";
const char *password = "password";
int focusPin = LOW;
int shutterPin = LOW;


ESP8266WebServer intwebserver(80);

void handleRoot()
{
  response();
}

void capture()
{
  focusPin = LOW;
  shutterPin = LOW;

  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  delay(100);
  digitalWrite(D2, LOW);
  digitalWrite(D1, LOW);
  delay(100);
  response();
}

void burstMode()
{
  focusPin = LOW;
  shutterPin = LOW;
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  response();
}

void bulbOn()
{
  focusPin = LOW;
  shutterPin = LOW;
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  response();
}

void bulbOff()
{
  focusPin = LOW;
  shutterPin = LOW;
  digitalWrite(D2, LOW);
  digitalWrite(D1, LOW);
  response();
}

const String HtmlHtml = "<html><head>"
                        "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/mini.css/3.0.1/mini-default.min.css\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>ESP8266AstroIntervalometer</title></head>";
const String HtmlHtmlClose = "</body></html>";
const String HtmlTitle = "<body><h1>Wireless shutter trigger</h1><br/>\n";
const String HtmlButtons =
  "<a href=\"capture\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Capture</button></a>"
  "<a href=\"burst\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Burst mode</button></a><br/><br/>"
  "<a href=\"bulbOn\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Start Bulb Mode</button></a>"
  "<a href=\"bulbOff\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Stop</button></a><br/><br/>";
const String HtmlInterval = "<div class=\"container\"><div class=\"row\">"
                            "<form action=\"/astropic\" method=\"post\" target=\"curstatus\">"
                            "<div class=\"row\">Exposure: <input type=\"text\" name=\"exptime\"></div>"
                            "<div class=\"row\">Delay: <input type=\"text\" name=\"waittime\"></div>"
                            "<div class=\"row\">Pics: <input type=\"text\" name=\"reps\"></div>"
                            "<div class=\"row\"><select name=\"action\"><option value=\"Start\">Start</option><option value=\"Stop\">Stop</option></select></div><br>"
                            "<div class=\"row\"><input type=\"submit\" value=\"Submit\"></div></form></div>"
                            "<div class=\"row\"><form action=\"/astropic\" method=\"get\" target=\"curstatus\"><input type=\"submit\" value=\"GetStatus\"></form></div>"
                            "<div class=\"row\"><iframe name=\"curstatus\" id=\"curstatus\"></iframe></div>"; 

void response()
{
  String page = HtmlHtml + HtmlTitle;
  page += HtmlButtons;
  page += HtmlInterval;
  page += HtmlHtmlClose;
  intwebserver.send(200, "text/html", page);
}

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  WiFi.softAP(ssid, password);
  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: \n");
  Serial.println(apip);
  intwebserver.on("/", handleRoot);
  intwebserver.on("/capture", capture);
  intwebserver.on("/burst", burstMode);
  intwebserver.on("/bulbOn", bulbOn);
  intwebserver.on("/bulbOff", bulbOff);
  intwebserver.begin();
  Serial.println("HTTP server started");
  pinMode(D1, OUTPUT);
  digitalWrite(D1, focusPin);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, shutterPin);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
}
void loop()
{
  intwebserver.handleClient();

}
