#include <BH1750FVI.h>
#include <Wire.h>

//Settings General
float nightThreshold = 0.0;
float dayThreshold = 50.0;
int doorTimeOut = 180;
int doorDelay = 1200;
int closeDelayCounter = 0;

//Settings L298N
const uint8_t pwmLeft = 14;      // ENA - Enable and PWM
const uint8_t leftForward = 12;  // IN1 - Forward Drive
const uint8_t leftReverse = 13;  // IN2 - Reverse Drive

// Settings Light sensor
uint8_t ADDRESSPIN = 13;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

// Create the Lightsensor instance
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);

//Working variables
int luxHistSize = 3;
float luxHist[] = {100, 100, 100}; //Array of lux values
boolean isNight = false;
int doorTimeCount = 0;
bool motorIsRunning = false;

void OpenDoor() {
  Serial.println("Opening door");
  if (motorIsRunning == true) {
    StopMotor();
  }
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  analogWrite(pwmLeft, 255);
  doorTimeCount = doorTimeOut;
  motorIsRunning = true;
}

void CloseDoor() {
  Serial.println("Closing door");
  if (motorIsRunning == true) {
    StopMotor();
  }
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, HIGH);
  analogWrite(pwmLeft, 255);
  doorTimeCount = doorTimeOut;
  motorIsRunning = true;
}

void StopMotor() {
  motorIsRunning = false;
  analogWrite(pwmLeft, 0);
  doorTimeCount = 0;
  delay(2000);
}

void SetHist(float value) {
  //Setting lux series
  luxHist[2] = luxHist[1];
  luxHist[1] = luxHist[0];
  luxHist[0] = value;
}

float GetAverageLux() {
  float luxSum = 0;

  for (int f=0; f <= luxHistSize-1; f++){
      luxSum+=luxHist[f];
      delay(1000);
    }
    return luxSum/luxHistSize;
}

void setup() {
  Serial.begin(9600);
  LightSensor.begin();
  analogWriteFreq(10);

  pinMode(pwmLeft, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftReverse, OUTPUT);

  Serial.println("Determining startup mode");
  for (int f = 0; f <= 3; f++) {
    SetHist(LightSensor.GetLightIntensity());
    delay(1000);
  }
  float avgLux = GetAverageLux();
  if (avgLux <= nightThreshold){
      isNight = true;
      Serial.println("It was night");
      CloseDoor();
    }else{
      isNight = false;
      Serial.println("It was day");
      OpenDoor();
    }
}

void loop() {
  float lux = LightSensor.GetLightIntensity();
  Serial.print(lux);
  Serial.println(" lx");

  //Handle Timeout counters
  if (motorIsRunning) {
    doorTimeCount--;
    if (doorTimeCount <= 0)
      StopMotor();
  }

  if(closeDelayCounter > 0){
    closeDelayCounter--;
    if (closeDelayCounter <= 0){
        CloseDoor();
      }
    }

  if (isNight == false && lux < nightThreshold && luxHist[2] < nightThreshold && luxHist[1] < nightThreshold && luxHist[0] < nightThreshold) {
    Serial.print("IT IS NOW NIGHT!");
    isNight = true;
    closeDelayCounter = doorDelay;
  }

  if (isNight == true && lux > dayThreshold && luxHist[2] > dayThreshold && luxHist[1] > dayThreshold && luxHist[0] > dayThreshold) {
    Serial.print("GOODMORNING!");
    isNight = false;
    OpenDoor();
  }

  SetHist(lux);
  delay(1000);
}
