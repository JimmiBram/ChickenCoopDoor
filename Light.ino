// Settings Light sensor
uint8_t ADDRESSPIN = 13;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

// Create the Lightsensor instance
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);

//Variables
float luxHist[] = {100, 100, 100}; //Array of lux values
int luxHistSize = 3;

void SetupLight()
{
    LightSensor.begin();
  }

void HandleLight(bool wholeSecond){
  if(wholeSecond){
        SetHist(LightSensor.GetLightIntensity());
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
