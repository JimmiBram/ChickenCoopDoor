//CHICKENCOOP CONTROLLER

//INCLUDES
#include <BH1750FVI.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//GLOBAL SETTINGS
boolean isNight = false;
bool motorIsRunning = false;
String doorState = "unknown";  //States: opening, closing, closed, open, unknown

//GLOBAL VARIABLES
static const unsigned long REFRESH_INTERVAL = 1000; // ms
static unsigned long lastRefreshTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogWriteFreq(10);
  SetupWifi();
  SetupDoor();
  SetupLight();
}

void loop() {
  //Main loop
  bool wholeSecond = false;

  //One second tasks
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
      lastRefreshTime += REFRESH_INTERVAL;
      wholeSecond = true;
  }

  //Hourly tasks
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
      lastRefreshTime += REFRESH_INTERVAL;
      wholeSecond = true;
  }

  HandleWifi(wholeSecond);
  HandleLight(wholeSecond);
  HandleDoor(wholeSecond);
}
