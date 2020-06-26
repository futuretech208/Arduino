//Code written by Sumukh Anil Purohit
//Licenced under GNU GPL v2
//
//Arduino Based telescope single axis tracker

#define PIN_RA_STEP  2         //define step pin
#define PIN_RA_DIRECTION  3
#define PIN_16X  4               //8X speed only when switch is pushed
#define PIN_2X  5
#define PIN_RA_MINUS  6       // change the direction
#define STEP_INTERVAL_SIDEREAL  24389.74     // u second    this is configured for my telescope and gear ratios
#define STEP_INTERVAL_2X_SIDEREAL  12194.87  // u seconds   this is configured for my telescope and gear ratios
#define STEP_INTERVAL_16X_SIDEREAL  1524.36   // u seconds  this is configured for my telescope and gear ratios

uint32_t last_pulse;

void setup()
{
  pinMode(PIN_RA_STEP, OUTPUT);
  pinMode(PIN_RA_DIRECTION, OUTPUT);
  pinMode(PIN_16X, INPUT);
  pinMode(PIN_2X, INPUT);
  pinMode(PIN_RA_MINUS, INPUT);
  digitalWrite(PIN_RA_DIRECTION, LOW);                  // for changing the direction in Northen or southern hemisphere
  digitalWrite(PIN_16X, LOW);
  digitalWrite(PIN_2X, LOW);
  digitalWrite(PIN_RA_MINUS, LOW);
  Serial.begin(115200);
  last_pulse = micros();
}

void loop()
{
  setRightAscensionDirection();
  checkSpeed();
}

void setRightAscensionDirection()
{
  if (digitalRead(PIN_RA_MINUS) == HIGH)
  {
    digitalWrite(PIN_RA_DIRECTION, HIGH);                 // counterclockwise
  }
  else
  {
    digitalWrite(PIN_RA_DIRECTION, LOW);                // clockwise
  }
}

void checkSpeed()
{
  if ((digitalRead(PIN_16X) == HIGH))
  {
    if (micros() - last_pulse >= STEP_INTERVAL_16X_SIDEREAL)
    {
      last_pulse += STEP_INTERVAL_16X_SIDEREAL;
      digitalWrite(PIN_RA_STEP, LOW);
      digitalWrite(PIN_RA_STEP, HIGH);
    }
  }

  if ((digitalRead(PIN_2X) == HIGH))
  {
    if (micros() - last_pulse >= STEP_INTERVAL_2X_SIDEREAL)
    {
      last_pulse += STEP_INTERVAL_2X_SIDEREAL;
      digitalWrite(PIN_RA_STEP, LOW);
      digitalWrite(PIN_RA_STEP, HIGH);
    }
  }
  else
  {
        if (micros() - last_pulse >= STEP_INTERVAL_SIDEREAL)
    {
      last_pulse += STEP_INTERVAL_SIDEREAL;
      digitalWrite(PIN_RA_STEP, LOW);
      digitalWrite(PIN_RA_STEP, HIGH);
    }
  }
}

