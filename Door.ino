int doorTimeCount = 0;
int doorTimeOut = 180;
int doorDelay = 1200;
int closeDelayCounter = 0;

//Settings L298N
const uint8_t pwmLeft = 14;      // ENA - Enable and PWM
const uint8_t leftForward = 12;  // IN1 - Forward Drive
const uint8_t leftReverse = 13;  // IN2 - Reverse Drive


void SetupDoor(){
  
  }


void HandleDoor(bool wholeSecond){
  
  }


void OpenDoor(){
  doorState = "opening";
  Log("Opening door");
  if (motorIsRunning == true) {
    StopMotor();
  }
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  analogWrite(pwmLeft, 255);
  doorTimeCount = doorTimeOut;
  motorIsRunning = true;
  }


void CloseDoor(){
  doorState = "closing";
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
