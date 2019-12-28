int motorTimeCount = 0;
int motorTimeOut = 180; //in seconds
int closeDelay = 15; //in minutes
int closeDelayCounter = 0;

String doorState = "unknown";  //States: opening, closing, closed, open, unknown

//Settings L298N
const uint8_t pwmLeft = 14;      // ENA - Enable and PWM
const uint8_t leftForward = 12;  // IN1 - Forward Drive
const uint8_t leftReverse = 13;  // IN2 - Reverse Drive


void SetupDoor(){
  
  }

void HandleDoor(bool wholeSecond, bool wholeMinute, bool wholeHour){

    if(wholeMinute){
        if (closeDelayCounter > 0 && closeDelayCounter < closeDelay)
        {
            closeDelayCounter++;
          }else if (closeDelayCounter > 0 && closeDelayCounter >= closeDelay)
          {
            Log("Countdown ended, closing door.");
            closeDelayCounter = 0;
            CloseDoor();
            }
      }
  
    if(wholeSecond){

      if(motorIsRunning)
      {
        motorTimeCount++;

        if(motorTimeCount >= motorTimeOut)
            {
              StopMotor();
               if(doorState == "opening")
                {
                  doorState = "open";
                  Log("Door is now fully open");
                }else if(doorState == "closing"){
                  doorState = "closed";
                  Log("Door is now fully closed");
                }
              }
        }
 
      if (isNight == false && IsItNightNow() == true) {
          Log("Event triggered - IT IS NOW NIGHT! Countdown started.");
          isNight = true;
          closeDelayCounter = 1;
      }
    
      if (isNight == true && IsItDayNow() == true && currentHour > minimumDayHour) {
        Log("Event triggered - IT IS NOW DAY!");
        isNight = false;
        OpenDoor();
      }
    }
  }

void OpenDoor(){
  doorState = "opening";
  Log("Door is opening");
  if (motorIsRunning == true) {
    StopMotor();
  }
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  analogWrite(pwmLeft, 255);
  motorTimeCount = 0;
  motorIsRunning = true;
  }

void CloseDoor(){
  doorState = "closing";
  Log("Door is closing");
  if (motorIsRunning == true) {
    StopMotor();
  }
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, HIGH);
  analogWrite(pwmLeft, 255);
  motorTimeCount = 0;
  motorIsRunning = true;
  }

void StopDoor(){
    Log("Door stopped");
    StopMotor();
    doorState = "unknown";
  }

void StopMotor() {
  motorIsRunning = false;
  analogWrite(pwmLeft, 0);
  motorTimeCount = 0;
}
