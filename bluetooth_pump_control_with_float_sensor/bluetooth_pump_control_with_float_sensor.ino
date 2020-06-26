//Code written by Sumukh Anil Purohit
//Licenced under GNU GPL v2
//
//Arduino 4 relay bluetooth shield for UNO and MEGA 2560
//the 4 relay bluetooth shield is available for sale on tindie https://www.tindie.com/products/sumukh208/4-relay-bluetooth-shield-for-arduino/
// This code is written for my home pump automation which has a panel with ON switch and OFF switch, thus users have to customize it for their applications.
// the pump panel contains the ON switch with 2 elements which are N/O. and OFF switch with 1 element which is N/C.
//2 relays are engaged simultaneously for ON and 1 relay for OFF. bluetooth module HC 05 or 06 can be used
//Ultrasound sensor for Arduino is used for sensing water level and turning off the pumps correspondingly
//Code is well tested and the unit I hve made is in operation at my home since January 2017.
// the app development is done in MIT app inventor.

#include <SoftwareSerial.h>
SoftwareSerial btserial(2 , 3);  //Rx, Tx Original Rx Tx is 0,1
String voice;
const int levelHigh = 10;
const int subOff = 6;  //IN 3
const int subOn = 7; /// IN2
const int tankOn = 9;  //IN4
const int subOnn = 8; // IN1
const int statusLED = 5;  // red led to show pump is off
boolean flagCheck = true;
void setup()
{

  pinMode(levelHigh, INPUT);
  pinMode(subOff, OUTPUT);
  pinMode(subOnn, OUTPUT);
  pinMode(subOn, OUTPUT);
  pinMode(tankOn, OUTPUT);
  pinMode(statusLED, OUTPUT);
  digitalWrite(subOff, HIGH);
  digitalWrite(subOnn, HIGH);
  digitalWrite(subOn, HIGH);
  digitalWrite(tankOn, HIGH);
  digitalWrite(statusLED, LOW);
  btserial.begin(9600);
}

void loop()
{
  Appconnect();
}
void Appconnect()
{
  if (digitalRead(levelHigh) == LOW)
  {
    while (btserial.available())
    {
      delay(10);
      char c = btserial.read();
      voice += c;

    }
    if (voice.length() > 0)
    {
     btserial.println(voice);
      if (voice == "SUBON")
      {
        digitalWrite(subOn, LOW);
        digitalWrite(subOnn, LOW);
        delay(200);
        digitalWrite(subOn, HIGH);
        digitalWrite(subOnn, HIGH);
      }
      if (voice == "SUBOFF")
      {
        digitalWrite(subOff, LOW);
        delay(200);
        digitalWrite(subOff, HIGH);
      }
      if (voice == "TANKON")
      {
        digitalWrite(tankOn, LOW);
      }
      if (voice == "TANKOFF")
      {
        digitalWrite(tankOn, HIGH);
      }
      voice = "";
    }
  }
  else if (digitalRead(levelHigh == HIGH))
  {
    digitalWrite(tankOn, HIGH);
    digitalWrite(subOff, LOW);
    delay(200);
    digitalWrite(subOff, HIGH);
    digitalWrite(statusLED, HIGH);
    delay(45000);
    digitalWrite(statusLED, LOW);
  }
}
