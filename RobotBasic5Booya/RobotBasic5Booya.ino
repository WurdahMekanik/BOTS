// constants for motor1 commands
const unsigned char mLeftForward =  0xC6;
const unsigned char mLeftReverse =  0xC5;
const unsigned char mLeftBrake   =  0xC7;
// constants for motor2 commands
const unsigned char mRightForward =  0xCE;
const unsigned char mRightReverse =  0xCD;
const unsigned char mRightBrake   =  0xCF;
// sensor pins
int analogLeft = A5;
int analogRight = A6;
int analogLeft1 = A3;
int analogRight1 =A4;
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

int black = 30;
int white = 80;
int blackblack = 59;

int rotateSpeed = 60;

int led = 7;

// initialization
void setup() {
  initial();
}

// loopy
void loop() {
  if(!started){
    tiltMask = digitalRead(tilt);
    if (tiltMask){
    digitalWrite(led, HIGH);
  }
    if (!tiltMask){
    digitalWrite(led, LOW);
  }
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
  else{
    analogLeft  = A1;
    analogRight = A2;
    straightLineFast();
    
    if (grapMask){
      grapple();
    }
  }
  
  Serial.print("s1 = " );                       
  Serial.print(sensorLeft);      
  Serial.print("   s2 = " );                       
  Serial.println(sensorRight);
  
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
  pinMode(tilt, INPUT);
  pinMode(hookservo, OUTPUT);
  pinMode(led, OUTPUT);
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
  sensorLeft = analogRead(analogLeft);
  sensorRight = analogRead(analogRight);
  
  sensorDiff = sensorLeft - sensorRight;
  if (sensorDiff > 120){
    Serial1.write(mLeftForward);
    Serial1.write(127);
    Serial1.write(mRightForward);
    Serial1.write(100);
  }
  sensorLeft = analogRead(analogLeft);
  sensorRight = analogRead(analogRight);
  
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

void straightLineFast(){
   sensorLeft = analogRead(analogLeft);
   sensorRight = analogRead(analogRight);
 
   if((sensorLeft + sensorRight) > white){
     forward(127);
     delay(5);
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
   
   
   
   if((sensorLeft + sensorRight) < blackblack){
     if(sensorLeft > sensorRight){
          forward(80);
          delay(20);
          rotateLeft(100);
          delay(20);
          count = 0;
     } 
     if((sensorLeft + sensorRight) < blackblack){
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
     if((sensorLeft + sensorRight) < blackblack){
       forward(80);
       delay(20);
       rotateRight(100);
       delay(20);
     }
     count = 0;
   }
   
   sensorLeft = analogRead(analogLeft);
   sensorRight = analogRead(analogRight);
       
   if(sensorLeft < black){
        brakemRight(rotateSpeed);
        delay(20);
        //sensorLeft = analogRead(analogLeft);
        //sensorRight = analogRead(analogRight);
        count = 0;
        if(sensorLeft < black){
          forward(60);
          delay(20);
        rotateRight(rotateSpeed);
        delay(20);
        //sensorLeft = analogRead(analogLeft);
        //sensorRight = analogRead(analogRight);
        count = 0;
   }
   }
   sensorLeft = analogRead(analogLeft);
   sensorRight = analogRead(analogRight);
   if(sensorRight < black){
        brakemLeft(rotateSpeed);
        delay(20);
        //sensorLeft = analogRead(analogLeft);
        //sensorRight = analogRead(analogRight);
        count = 0;
        if(sensorRight < black){
          forward(80);
          delay(20);
          rotateLeft(rotateSpeed);
          delay(20);
         //sensorLeft = analogRead(analogLeft);
         //sensorRight = analogRead(analogRight);
         count = 0;
       }
   
     }
   }
