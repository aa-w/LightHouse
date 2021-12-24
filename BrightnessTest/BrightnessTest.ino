/*
    _   _    _____  __ __      _____   ___  ___  _    _____   __
   /_\ | |  | __\ \/ / \ \    / / _ \ / _ \|   \| |  | __\ \ / /
  / _ \| |__| _| >  <   \ \/\/ / (_) | (_) | |) | |__| _| \ V /
  /_/ \_\____|___/_/\_\   \_/\_/ \___/ \___/|___/|____|___| |_|

*/

// CONSTANTS //
#include <FastLED.h>

#define LEDPIN 10
#define LDRPIN 6
#define LDRREAD A4
#define NUM_LEDS 5
#define LED_TYPE WS2811
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];
// GLOBALS //
int LedBrightness = 0;
int Brightness = 100;


int Reading = 0;

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LEDPIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  pinMode(6, OUTPUT);
}

void loop()
{
  CheckLDR();
  FastLED.setBrightness(LedBrightness);
  for (byte i = 0; i != NUM_LEDS; i++)
  {
    leds[i] = 0xFF9900;
  }
  FastLED.show();
}

void CheckLDR()
{
  digitalWrite(6, HIGH);
  delay(5);
  Reading = analogRead(A4);
  LedBrightness = ((Brightness - (Reading / 10)) + 20);
  Serial.print("LedBrightness: ");
  Serial.println(LedBrightness);
  digitalWrite(6, LOW);

  //Serial.print("LDR ");
  //Serial.println(Reading);
}
