// Settings Light sensor
uint8_t ADDRESSPIN = 13;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

// Create the Lightsensor instance
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);

//Variables
float luxHist[] = {100, 100, 100}; //Array of lux values
int luxHistSize = 3;
float currentLux = 0;

void SetupLight()
{
    LightSensor.begin();
    
    Log("Determining startup mode");
    for (int f = 0; f <= 3; f++) {
      SetHist(LightSensor.GetLightIntensity());
      delay(1000);
    }
    float avgLux = GetAverageLux();
    if (avgLux <= nightThreshold){
      isNight = true;
      Log("It is night");
    }else{
      isNight = false;
      Log("It is day");
    }
  }

void HandleLight(bool wholeSecond, bool wholeMinute, bool wholeHour){
  if(wholeSecond){
        currentLux = LightSensor.GetLightIntensity();
        SetHist(currentLux);
    }
  }


float GetLux(){
    return LightSensor.GetLightIntensity();
  }


float GetAverageLux() {
  float luxSum = 0;
  for (int f=0; f <= luxHistSize-1; f++){
      luxSum+=luxHist[f];
    }
    return luxSum/luxHistSize;
}

void SetHist(float value) {
  //Setting lux series
  luxHist[2] = luxHist[1];
  luxHist[1] = luxHist[0];
  luxHist[0] = value;
}


bool IsItDayNow(){
    if(currentLux > dayThreshold && luxHist[2] > dayThreshold && luxHist[1] > dayThreshold && luxHist[0] > dayThreshold)
      {
        return true;
      }else{
        return false;
      }
  }


bool IsItNightNow(){
    if(currentLux < nightThreshold && luxHist[2] < nightThreshold && luxHist[1] < nightThreshold && luxHist[0] < nightThreshold)
      {
        return true;
      }else{
        return false;
      }
  }
