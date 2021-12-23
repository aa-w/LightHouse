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
#define NUM_LEDS    5
int Brightness = 100;
int LedBrightness = 0;
int PreLedBrightness = 0;
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
  FastLED.setBrightness(BRIGHTNESS);

  //LDR Setup
  pinMode(6, OUTPUT);
}


int Reading = 0;
int Timer = 0;
unsigned long TurnOnTime = 0;
void loop()
{

  //Serial.print("State: ");
  //Serial.println(State);

  if (millis() > Timer)
  {
    Reading = CheckLDR();
    if (Reading < 450)
    {

      LedBrightness = (Brightness - (Reading / 10));
      Serial.print("LedBrightness: ");
      Serial.println(LedBrightness);

      if (State == 0)
      {
        TurnOnTime = millis();
        State = 3;
      }
      else
      {
        State = 1;
      }
      Timer = millis() + TIMERVALUE;
    }
    else
    {
      if (State == 1)
      {
        State = 4;
      }
      else
      {
        State = 0;
      }

      Timer = millis() + SLEEPTIMERVALUE;
    }
  }

  switch (State)
  {
    case 0: //keep lights off
      {
        leds[0].setRGB( 0, 0, 0);
        leds[1].setRGB( 0, 0, 0);
        leds[2].setRGB( 0, 0, 0);
        leds[3].setRGB( 0, 0, 0);
        leds[4].setRGB( 0, 0, 0);
        FastLED.show();
        delay(10);
      }
    case 1: //keep lights on
      {
        leds[0].setRGB( 201, 160, 46);
        leds[1].setRGB( 201, 160, 46);
        leds[2].setRGB( 201, 160, 46);
        leds[3].setRGB( 201, 160, 46);
        leds[4].setRGB( 201, 160, 46);
        FastLED.show();
        delay(10);
      }
      case 3: //turn lights on slowly
      {
        unsigned long OnBrightnessValue = millis() - TurnOnTime;
        
      }
      case 4: //turn lights off slowly
      {
        
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

int CheckLDR()
{
  int Reading = 0;
  digitalWrite(6, HIGH);
  delay(5);
  Reading = analogRead(A4);
  digitalWrite(6, LOW);
  //Serial.print("LDR ");
  //Serial.println(Reading);
  return Reading;
}
