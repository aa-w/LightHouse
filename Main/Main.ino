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
#define LDRREAD A4

#define TIMERVALUE 30
#define SLEEPTIMERVALUE 50

//LED Set Up

#define ARRAYSIZE 100
#define NUM_LEDS    5

const int LDRCALIBRATION = 320;
const int LDRSCALE = 400;

float Brightness = 0;
float ReadingCal = 0;
int LedBrightness = 0;
int LedBrightnessArray [ARRAYSIZE];
byte AverageBrightness = 0;
int PrevLedBrightness = 0;

byte ArrayPointer = 0;


int Reading = 0;
unsigned long DebugTimer = 0;

#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

//Constants

CRGB leds[NUM_LEDS];

void setup()
{
  //Debug
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, LEDPIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.setBrightness(BRIGHTNESS);

  //LDR Setup
  pinMode(6, OUTPUT);
  /*
    for (byte i = 80; i != 100; i++)
    {

    float Accel = log(100 - i);
    int LedBrightnessa = (255 - ((i - Accel) * 2.55));

    Serial.print("Accel: ");
    Serial.println(Accel);

    Serial.print("LedBrightnessa: ");
    Serial.println(LedBrightnessa);
    }
  */

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
  
  SerialDebugger();

  switch(State)
  {
    case 0: //Lights On normally
    {
      break;
    }
    case 1: //Start Timer
    {
      break;
    }
    case 2: //Timer Triggered
    {
      break;
    }
    case 3:
    {
      break;
    }
    default:
    {
      Serial.println("Default case run");
      break;
    }
  }
}
//loop normally
//if lights are on start timer
//if lights go off stop timer
//if timer clocks out turn lights off
//if lights now turn off reset timer

byte State = 0;
void UpdateLights()
{
  if (AverageBrightness != PrevLedBrightness)
  {
    FastLED.setBrightness(AverageBrightness);

    for (byte i = 0; i != NUM_LEDS; i++)
    {
      leds[i] = CRGB::Yellow;
    }
    FastLED.show();
    //Serial.println("Update");
  }
  Serial.println("No Update");
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

void CheckLDR()
{
  digitalWrite(6, HIGH);
  delay(5);
  Reading = analogRead(A4);
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
