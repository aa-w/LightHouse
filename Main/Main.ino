/*
     _   _    _____  __ __      _____   ___  ___  _    _____   __
    /_\ | |  | __\ \/ / \ \    / / _ \ / _ \|   \| |  | __\ \ / /
   / _ \| |__| _| >  <   \ \/\/ / (_) | (_) | |) | |__| _| \ V /
  /_/ \_\____|___/_/\_\   \_/\_/ \___/ \___/|___/|____|___| |_|

*/

// CONSTANTS //
#include <FastLED.h>
#include <math.h>

//Arduino Pinout

#define LEDPIN 10
#define LDRPIN 6
#define LDRREAD A1

#define TIMERVALUE 30

#define TRENDTIMERVALUE 600000 //Ten minutes per average
#define SWITCHOFFTIMERVALUE 18000000 //Switch off after 6 hours

//LED Set Up

#define ARRAYSIZE 100
#define TRENDARRAYSIZE 10
#define NUM_LEDS    5

const int LDRCALIBRATION = 300;
const int LDRSCALE = 400;

float Brightness = 0;
float ReadingCal = 0;
int LedBrightness = 0;
int LedBrightnessArray [ARRAYSIZE];
byte AverageBrightness = 0;
int PrevLedBrightness = 0;
byte LedTrendAverage = 0;

byte ArrayPointer = 0;
byte TrendArrayPointer = 0;

byte MinBrightness = 60;

int LedTrendAverageArray [TRENDARRAYSIZE];
int Reading = 0;

unsigned long DebugTimer = 0;
unsigned long TrendTimer = 0;
bool TimerStarted = false;
unsigned long SwitchOffTimer = 0;

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

//Constants

CRGB leds[NUM_LEDS];

void setup()
{
  //Debug
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, LEDPIN, COLOR_ORDER>(leds, NUM_LEDS);

  //LDR Setup
  pinMode(6, OUTPUT);

}

void loop()
{

  //SimCheckLDR();
  CheckLDR();

  ReadingCal = (Reading - LDRCALIBRATION); //Brightness Percentage
  if (ReadingCal > 0)
  {
    Brightness = ((ReadingCal / LDRSCALE) * 100); //Brightness Percentage
  }
  else
  {
    Brightness = 0;
  }

  if (Brightness < 100)
  {
    LedBrightness = 255 - (Brightness * 2.55); //Brightness Converted to inverted Byte Scale
  }
  else
  {
    LedBrightness = 0;
  }

  AverageBrightness = RollingAverage();

  UpdateLights();

  PrevLedBrightness = AverageBrightness;

  SleepChecker();

  SerialDebugger();
}

void SleepChecker() //Checks for sleeping trends over time
{
  if (millis() > TrendTimer)
  {
    UpdateTrend();
    TrendTimer = millis() + TRENDTIMERVALUE;
    Serial.print("Trend: ");
    Serial.println(LedTrendAverage);
  }

  if ((TimerStarted == false) && (LedTrendAverage > 80))
  {
    SwitchOffTimer = millis() + SWITCHOFFTIMERVALUE;

    Serial.print("TimerStarted: ");
    Serial.println(TimerStarted);

    TimerStarted = true;
  }

  if ((TimerStarted == true) && (millis() > SwitchOffTimer))
  {
    WaitTillWake();
  }
}

void WaitTillWake() //waits for a minimum of 60 percent brightness before waking
{
  Serial.println("SleepTillWake Triggered");

  bool Sleep = true;
  unsigned long WaitTimer = 0;

  FastLED.setBrightness(0);
  for (byte i = 0; i != NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();

  while (Sleep == true)
  {
    if (millis() > WaitTimer)
    {
      CheckLDR();

      ReadingCal = (Reading - LDRCALIBRATION); //Brightness Percentage
      if (ReadingCal > 0)
      {
        Brightness = ((ReadingCal / LDRSCALE) * 100); //Brightness Percentage
        Serial.print("Brightness: ");
        Serial.println(Brightness);
      }
      else
      {
        Brightness = 0;
      }

      if (Brightness > MinBrightness)
      {
        Serial.print("Brightness: ");
        Serial.println(Brightness);
        Serial.println("Sleep Ended");
        Sleep = false;
      }
      WaitTimer = millis() + 10000;
    }
  }

  TimerStarted = false; //reset the timer for the next sequence
  Serial.println("Return to main");
}

void UpdateLights()
{
  if (AverageBrightness != PrevLedBrightness)
  {

    FastLED.setBrightness(AverageBrightness);
    for (byte i = 0; i != NUM_LEDS; i++)
    {
      leds[i] = CRGB::Yellow;
      //      Serial.print(i);
      //      Serial.print(", ");
      //      Serial.print(DitheredBrightness);
      //      Serial.println(", ");
    }
    FastLED.show();
    //Serial.println("Update");
  }
  //Serial.println("No Update");
}

byte RollingAverage()
{
  LedBrightnessArray[ArrayPointer] = LedBrightness;
  float AAverage = 0;
  float AAdder = 0;
  for (byte i = 0; i != ARRAYSIZE; i++)
  {
    AAdder = AAdder + LedBrightnessArray[i];
  }
  AAverage = AAdder / ARRAYSIZE;

  //make array circular
  if (ArrayPointer == (ARRAYSIZE - 1))
  {
    ArrayPointer = 0;
  }
  else
  {
    ArrayPointer++;
  }
  return AAverage;
}

void UpdateTrend()
{
  LedTrendAverageArray[TrendArrayPointer] = LedBrightness;
  float AAverage = 0;
  float AAdder = 0;
  for (byte i = 0; i != TRENDARRAYSIZE; i++)
  {
    //Serial.println(LedTrendAverageArray[i]);
    AAdder = AAdder + LedTrendAverageArray[i];
  }
  AAverage = AAdder / TRENDARRAYSIZE;

  //make array circular
  if (TrendArrayPointer == (TRENDARRAYSIZE - 1))
  {
    TrendArrayPointer = 0;
  }
  else
  {
    TrendArrayPointer++;
  }
  LedTrendAverage = AAverage;
}

void CheckLDR()
{
  digitalWrite(6, HIGH);
  delay(5);
  Reading = analogRead(A1);
  digitalWrite(6, LOW);
}

void SerialDebugger()
{
  if (millis() > DebugTimer)
  {
    Serial.print(" Reading: ");
    Serial.print(Reading);
    Serial.print(" Brightness: ");
    Serial.print(Brightness);
    Serial.print(" LedBrightness: ");
    Serial.print(LedBrightness);
    Serial.print(" ReadingCal: ");
    Serial.println(ReadingCal);

    DebugTimer = millis() + 2000;
  }

}
