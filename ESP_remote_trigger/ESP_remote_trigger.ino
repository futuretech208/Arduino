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

String shots;
String shutter;
String delayBetween;
int tv, d, s;
ESP8266WebServer server(80);

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
  for (int i = 20; i >= 1; i--)
  {
    digitalWrite(D4, HIGH);
    delay(500);
    digitalWrite(D4, LOW);
    delay(500);
  }
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
void setParam()
{
  String shutter = server.arg("TV");
  String delayBetween = server.arg("setDelay");
  String shots = server.arg("shots");
  response();
}

void Intervalometer()
{
  //    focusPin = LOW;
  //  shutterPin = LOW;
  shutter = server.arg("TV");
  delayBetween = server.arg("setDelay");
  shots = server.arg("shots");
  int tv = shutter.toInt();
  int d = delayBetween.toInt();
  int s = shots.toInt();
  Serial.println(tv);
  Serial.println(d);
  Serial.println(s);
  for (int i = s; i >= 1; i--)
  {
    //        digitalWrite(focusPin, HIGH);
    //        delay(100);
    //        digitalWrite(shutterPin, HIGH);
    digitalWrite (D4, LOW);
    delay(tv * 1000);
    //        digitalWrite(shutterPin, LOW);
    //        digitalWrite(focusPin, LOW);
    digitalWrite (D4, HIGH);
    delay(d * 1000);
  }
  response();
}

const String HtmlHtml = "<html><head>"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlHtmlClose = "</html>";
const String HtmlTitle = "<h1>Wireless shutter trigger</h1><br/>\n";
const String HtmlButtons =
  "<a href=\"capture\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Capture</button></a>"
  "<a href=\"burst\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Burst mode</button></a><br/><br/>"
  "<a href=\"bulbOn\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Start Bulb Mode</button></a>"
  "<a href=\"bulbOff\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Stop</button></a><br/><br/>";
const String HtmlTv = "<h2>Intervalometer</h2><br/>\n"
                      "<form action=\"/setParam\">Tv in s: <input type=\"text\" name=\"TV\" ><br><br>"// <input type=\"submit\" value=\"Set Tv\"><br/><br/>"
                      "Delay in s: <input type=\"text\" name=\"setDelay\" ><br><br>"// <input type=\"submit\" value=\"Set Delay\"><br/><br/>"
                      "No. of shots: <input type=\"text\" name=\"shots\" ><br><br> <input type=\"submit\" value=\"Set parameters\">"
                      " </form>"
                      "<a href=\"startInt\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Start</button></a>"
                      "<a href=\"bulbOff\"><button style=\"display: inline-block; width: 50%; margin: 5px auto; text-align: center;\">Stop</button></a><br/><br/>";

void response()
{
  String htmlRes = HtmlHtml + HtmlTitle;
  htmlRes += HtmlButtons;
  htmlRes += HtmlTv;
  String shots = server.arg("shots");
  String shutter = server.arg("TV");
  String delayBetween = server.arg("setDelay");
  htmlRes += "Tv: " + String(shutter) + "<br/> Delay: " + String(delayBetween) + "<br/> Shots: " + String(shots) + "<br/>" ;
  htmlRes += HtmlHtmlClose;
  server.send(200, "text/html", htmlRes);
}

void setup()
{
  WiFi.setOutputPower(20.5);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  WiFi.softAP(ssid, password);
  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: \n");
  Serial.println(apip);
  server.on("/", handleRoot);
  server.on("/capture", capture);
  server.on("/burst", burstMode);
  server.on("/bulbOn", bulbOn);
  server.on("/bulbOff", bulbOff);
  server.on("/setParam", setParam);
  server.on("/startInt", Intervalometer);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(D1, OUTPUT);
  digitalWrite(D1, focusPin);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, shutterPin);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
}
void loop()
{
  server.handleClient();
  
}
