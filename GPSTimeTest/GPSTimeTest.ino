#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#define RXPin 4
#define TXPin 3
#define GPSBaud 9600

//static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

}

int flip = 0;
void loop()
{
  //ss.read());
  printDateTime(gps.date, gps.time);

  while (ss.available())
  {

      Serial.println(flip);
      flip++;
    gps.encode(ss.read());
  }
  flip = 0;
  delay(1000);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{

  char sz[32];

  sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
  Serial.print(sz);

  sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
  Serial.println(sz);

  gps.encode(ss.read());
}
