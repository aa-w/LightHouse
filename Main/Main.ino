/*
    _   _    _____  __ __      _____   ___  ___  _    _____   __
   /_\ | |  | __\ \/ / \ \    / / _ \ / _ \|   \| |  | __\ \ / /
  / _ \| |__| _| >  <   \ \/\/ / (_) | (_) | |) | |__| _| \ V /
  /_/ \_\____|___/_/\_\   \_/\_/ \___/ \___/|___/|____|___| |_|

*/

// CONSTANTS //
#include <FastLED.h>

//Arduino Pinout

#define LEDPIN 10
#define LDRPIN 6
#define LDRREAD A4

#define TIMERVALUE 30
#define SLEEPTIMERVALUE 50

//LED Set Up

#define TURNONTIME 3000
#define TURNOFFTIME 3000
#define UPDATETIME 50

#define NUM_LEDS    5
int Brightness = 100;
int LedBrightness = 0;
int PreLedBrightness = 0;
int Reading = 0;

#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
//Constants

CRGB leds[NUM_LEDS];


byte State = 0;
void setup()
{
  //Debug
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, LEDPIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.setBrightness(BRIGHTNESS);

  //LDR Setup
  pinMode(6, OUTPUT);
}

void loop()
{
  CheckLDR();
  if (Reading < 450 && State == 0)
  {
    LedBrightness = (Brightness - (Reading / 10));

    State = 3;
  }
  else if (Reading > 450 && State == 1)
  {
    LedBrightness = (Brightness - (Reading / 10));
    State = 4;
  }
  Serial.print("LedBrightness: ");
  Serial.println(LedBrightness);

  switch (State)
  {
    case 0: //keep lights off
      {

        FastLED.show();
        delay(10);
        break;
      }
    case 1: //keep lights on
      {

        FastLED.show();
        delay(10);
        break;
      }
    case 3: //turn lights on slowly
      {
        TurnLightsOn();
        break;
      }
    case 4: //turn lights off slowly
      {
        TurnLightsOff();
        break;
      }
  }

  //Serial.println(CheckLDR());
  //boot up
  //Serial.println("Starting...");
  //look for time from gps
  //compare to ldr readings
  //if it's night enable lights
  //animate lights
  //keep checking the time for shut off time
}

void TurnLightsOn()
{
  Serial.println("TurnLightsOn");
  unsigned long OnTimer = millis() + TURNONTIME;
  unsigned long UpdateTimer = millis() + UPDATETIME;
  float BrightnessValue = (LedBrightness / (TURNONTIME / UPDATETIME));
  Serial.print("BrightnessValue ");
  Serial.println(BrightnessValue);
  float BrightnessAdder = 0;
  while (OnTimer > millis())
  {
    if (millis() > UpdateTimer)
    {
      BrightnessAdder = BrightnessAdder + BrightnessValue;
      FastLED.setBrightness(BrightnessAdder);

      for (byte i = 0; i != NUM_LEDS; i++)
      {
        leds[i] = CRGB::Orange;
      }
      Serial.print("TurnLightsOn BrightnessAdder: ");
      Serial.println(BrightnessAdder);
      FastLED.show();

      UpdateTimer = millis() + UPDATETIME;
    }
  }
}

void TurnLightsOff()
{
  Serial.println("TurnLightsOff");
  unsigned long OffTimer = millis() + TURNOFFTIME;
  unsigned long UpdateTimer = millis() + UPDATETIME;
  float BrightnessValue = (LedBrightness / (TURNOFFTIME / UPDATETIME));
  float BrightnessAdder = LedBrightness;
  while (OffTimer > millis())
  {
    if (millis() > UpdateTimer)
    {
      BrightnessAdder = BrightnessAdder - BrightnessValue;
      FastLED.setBrightness(BrightnessAdder);

      for (byte i = 0; i != NUM_LEDS; i++)
      {
        leds[i] = 0xFF9900;
      }
      FastLED.show();
      Serial.print("TurnLightsOff BrightnessAdder: ");
      Serial.println(BrightnessAdder);

      UpdateTimer = millis() + UPDATETIME;
    }
  }
}

unsigned long DebugTimer = 0;

void CheckLDR()
{
  digitalWrite(6, HIGH);
  delay(5);
  Reading = analogRead(A4);

  Serial.print("LedBrightness: ");
  Serial.println(LedBrightness);
  digitalWrite(6, LOW);
  if (millis() > DebugTimer)
  {
    Serial.print("Reading: ");
    Serial.println(Reading);
    DebugTimer = millis() + 500;
  }
  //Serial.print("LDR ");
  //Serial.println(Reading);
}
