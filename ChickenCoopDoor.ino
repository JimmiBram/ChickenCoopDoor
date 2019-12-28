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
String currentTime = "00:00";
int currentMinute = 0;
int currentHour = 0;
int currentDay = 0;

float nightThreshold = 0.0;
float dayThreshold = 50.0;
int minimumDayHour = 7;

//GLOBAL VARIABLES
static const unsigned long REFRESH_INTERVAL_SECONDS = 1000; // ms
static const unsigned long REFRESH_INTERVAL_MINUTES = 60000; // ms
static const unsigned long REFRESH_INTERVAL_HOURS = 3600000; // ms
static unsigned long lastRefreshTimeSeconds = 0;
static unsigned long lastRefreshTimeMinutes = 0;
static unsigned long lastRefreshTimeHours = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogWriteFreq(10);
  Log("Chickendoor Node Started");
  SetupWifi();
  SetupDoor();
  SetupLight();
}

void loop() {
  //Main loop
  bool wholeSecond = false;
  bool wholeMinute = false;
  bool wholeHour = false;

  //One second tasks
  if(millis() - lastRefreshTimeSeconds >= REFRESH_INTERVAL_SECONDS)
  {
      lastRefreshTimeSeconds += REFRESH_INTERVAL_SECONDS;
      wholeSecond = true;
  }

  //One minute tasks
  if(millis() - lastRefreshTimeMinutes >= REFRESH_INTERVAL_MINUTES)
  {
      lastRefreshTimeMinutes += REFRESH_INTERVAL_MINUTES;
      wholeMinute = true;
  }

  //One minute tasks
  if(millis() - lastRefreshTimeHours >= REFRESH_INTERVAL_HOURS)
  {
      lastRefreshTimeHours += REFRESH_INTERVAL_HOURS;
      wholeHour = true;
  }

  HandleWifi(wholeSecond, wholeMinute, wholeHour);
  HandleLight(wholeSecond, wholeMinute, wholeHour);
  HandleDoor(wholeSecond, wholeMinute, wholeHour);
}
