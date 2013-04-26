// constants for motor1 commands
const unsigned char mLeftForward =  0xC6;
const unsigned char mLeftReverse =  0xC5;
const unsigned char mLeftBrake   =  0xC7;
// constants for motor2 commands
const unsigned char mRightForward =  0xCE;
const unsigned char mRightReverse =  0xCD;
const unsigned char mRightBrake   =  0xCF;
// sensor pins
int analogLeft;
int analogRight;
// grappling & tilt stuff
const int tilt      =  5;
const int grap      =  4;
const int hookservo =  3;
const char winch    =  0xF1;
// sensor values
int sensorLeft  =  0;
int sensorRight =  0;
int sensorDiff  =  0;
int speed       =  70;
// masks & counts
int count        =  0;
boolean started  =  0;
boolean tiltMask =  0;
boolean shot     =  0;
boolean grapMask =  0;

// initialization
void setup() {
  initial();
  if (digitalRead(tilt)){
    tiltMask = 1;
  }
  if (!digitalRead(tilt)){
    tiltMask = 0;
  }
}

// loopy
void loop() {
  if(!started){
    forward(127);
    delay(3);
    started = 1;
  }
  if (digitalRead(grap) == 1) {
    grapMask = 1;
  }
  if (tiltMask){
    analogLeft  = A3;
    analogRight = A4;
    lineTracking();
  }
  if (!tiltMask){
    analogLeft  = A5;
    analogRight = A6;
    straightLine();
    if (grapMask){
      grapple();
    }
  }
}

// both motors set to forward at full 127 speed
void forward(int speed){
  Serial1.write(mLeftForward);
  Serial1.write(speed);
  Serial1.write(mRightForward);
  Serial1.write(speed);
  Serial1.write(mLeftForward);
  Serial1.write(speed);
  Serial1.write(mRightForward);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void reverse(int speed){
  Serial1.write(mLeftReverse);
  Serial1.write(speed);
  Serial1.write(mRightReverse);
  Serial1.write(speed);
  Serial1.write(mLeftReverse);
  Serial1.write(speed);
  Serial1.write(mRightReverse);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void brakeLow(int speed){
  Serial1.write(mLeftBrake);
  Serial1.write(speed);
  Serial1.write(mRightBrake);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void brakemLeft(int speed){
  Serial1.write(mLeftBrake);
  Serial1.write(speed);
  Serial1.write(mLeftBrake);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void brakemRight(int speed){
  Serial1.write(mRightBrake);
  Serial1.write(speed);
  Serial1.write(mRightBrake);
  Serial1.write(speed);
}

// motor2 set to speed forward or reverse
// input -127 to 127 dSpeed, direction/speed
// negative numbers are reverse speed
// positive are forward speed
void setmLeft(int dSpeed){
  if(dSpeed >= 0){
    Serial1.write(mLeftForward);
    Serial1.write(dSpeed);
    Serial1.write(mLeftForward);
    Serial1.write(dSpeed);
  }
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    Serial1.write(mLeftReverse);
    Serial1.write(dSpeed);
    Serial1.write(mLeftReverse);
    Serial1.write(dSpeed);
  }
}

// motor2 set to speed forward or reverse
// input -127 to 127 dSpeed, direction/speed
// negative numbers are reverse speed
// positive are forward speed
void setmRight(int dSpeed){
  if(dSpeed >= 0){
    Serial1.write(mRightForward);
    Serial1.write(dSpeed);
    Serial1.write(mRightForward);
    Serial1.write(dSpeed);
  }
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    Serial1.write(mRightReverse);
    Serial1.write(dSpeed);
    Serial1.write(mRightReverse);
    Serial1.write(dSpeed);
  }
}

// motor1 and motor2 set to speed forward or reverse
// input -127 to 127 dSpeed, direction/speed
// negative numbers are reverse speed
// positive are forward speed
void setmLeftmRight(int dSpeedmLeft, int dSpeedmRight){
  setmLeft(dSpeedmLeft);
  setmRight(dSpeedmRight);
  setmLeft(dSpeedmLeft);
  setmRight(dSpeedmRight);
}
 
// motor2 set to reverse and motor1 forward at speed
// input 0-127
void rotateRight(int speed){
  setmLeft(speed);
  setmRight(0 - speed);
  setmLeft(speed);
  setmRight(0 - speed);
}

// motor2 set to reverse and motor1 forward at speed
// input 0-127
void rotateLeft(int speed){
  setmRight(speed);
  setmLeft(0 - speed);
  setmRight(speed);
  setmLeft(0 - speed);
}

// starts up both hardware serial and
// software serial communication
void initial(){
  analogReadAveraging(5);
  pinMode(grap, INPUT);
  pinMode(hookservo, OUTPUT);
  Serial.begin(19200);
  delay(5);
  Serial1.begin(19200);
  delay(5);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

// request Signature from Trex
// should return TREX and firmware version number 
unsigned char helloTrex(){
  Serial1.write(0x81);
}

void lineTracking(){
    // put your main code here, to run repeatedly:
 sensorLeft = analogRead(analogLeft);
 sensorRight = analogRead(analogRight);
 
   if((sensorLeft + sensorRight) > 1700){
     forward(55);
     delay(10);
     count++;
     //sensorLeft = analogRead(analogLeft);
     //sensorRight = analogRead(analogRight);  
   }  
   
   sensorLeft = analogRead(analogLeft);
   sensorRight = analogRead(analogRight);
   /*if((sensorLeft + sensorRight) < 500){
     forward(80);
     delay(20);
     sensorLeft = analogRead(analogLeft);
     sensorRight = analogRead(analogRight);
   }*/
      
   if((sensorLeft + sensorRight) < 399){
     if(sensorLeft > sensorRight){
       forward(80);
       delay(20);
       rotateLeft(100);
       delay(20);
       count = 0;
     } 
     if((sensorLeft + sensorRight) < 399){
       forward(80);
       delay(20);
       rotateLeft(100);
       delay(20);
     } 
     if(sensorLeft < sensorRight){
       forward(80);
       delay(20);
       rotateRight(100);
       delay(20);
     }
     if((sensorLeft + sensorRight) < 399){
       forward(80);
       delay(20);
       rotateRight(100);
       delay(20);
     }
     count = 0;
   }
   
   sensorLeft = analogRead(analogLeft);
   sensorRight = analogRead(analogRight);
       
   if(sensorLeft < 200){
        rotateRight(60);
        delay(20);
        //sensorLeft = analogRead(analogLeft);
        //sensorRight = analogRead(analogRight);
        count = 0;
        if(sensorLeft < 200){
          forward(60);
          delay(20);
          rotateRight(60);
          delay(20);
          //sensorLeft = analogRead(analogLeft);
          //sensorRight = analogRead(analogRight);
          count = 0;
        }
   }
   sensorLeft = analogRead(analogLeft);
   sensorRight = analogRead(analogRight);
   if(sensorRight < 200){
     rotateLeft(60);
     delay(20);
     //sensorLeft = analogRead(analogLeft);
     //sensorRight = analogRead(analogRight);
     count = 0;
     if(sensorRight < 200){
     forward(80);
     delay(20);
     rotateLeft(60);
     delay(20);
     //sensorLeft = analogRead(analogLeft);
     //sensorRight = analogRead(analogRight);
     count = 0;
     }
   }
}
 
void straightLine(){
  sensorDiff = sensorLeft - sensorRight;
  if (sensorDiff > 120){
    Serial1.write(mLeftForward);
    Serial1.write(127);
    Serial1.write(mRightForward);
    Serial1.write(100);
  }
  if (sensorDiff < -120){
    Serial1.write(mLeftForward);
    Serial1.write(100);
    Serial1.write(mRightForward);
    Serial1.write(127);
  }
  else {
    Serial1.write(mLeftForward);
    Serial1.write(127);
    Serial1.write(mRightForward);
    Serial1.write(127);
  }
}

void grapple(){
  if (!shot){
    Serial1.write(mLeftReverse);
    Serial1.write(40);
    Serial1.write(mRightReverse);   
    Serial1.write(40);
    delay(900);
    analogWrite(hookservo, 255);
    delay(500);
    shot = 1;
  }
  analogWrite(hookservo, 0);
  Serial1.write(winch);
  Serial1.write(127);
  // There will be Slack! (change the delay to let winch acquire Slack)
  delay(7000);
  Serial1.write(mLeftForward);
  Serial1.write(40);
  Serial1.write(mRightForward);
  Serial1.write(40);
}
