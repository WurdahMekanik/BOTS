// constants for motor1 commands
const unsigned char mLeftForward =  0xC6;
const unsigned char mLeftReverse =  0xC5;
const unsigned char mLeftBrake   =  0xC7;
// constants for motor2 commands
const unsigned char mRightForward =  0xCE;
const unsigned char mRightReverse =  0xCD;
const unsigned char mRightBrake   =  0xCF;
// sensor pins
int analogLeft = A1;
int analogRight = A2;
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
boolean started  =  false;
boolean tiltMask =  false;
boolean shot     =  false;
boolean grapMask =  false;


//used for default straightLineFollow
int black = 100;
int white = 400;
int blackblack = 200;

//used for flipside line following
int white0 = 1000;
int black0 = 200;
int blackblack0 = 399;

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
    started = true;
  }
  if (digitalRead(grap) == 1) {
    grapMask = true;
  }
  if (tiltMask){
    lineTracking();
  }
  else {
    if(!grapMask){
    straightLineFast();
    }
    if (grapMask){
      grapple();
    }
  }
  
  Serial.print("s1 = " );                       
  Serial.print(sensorLeft);      
  Serial.print("   s2 = " );                       
  Serial.println(sensorRight);
  Serial.print("tilt = ");
  Serial.print("tilt");
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

// brake both motor to set speed
void brakeLow(int speed){
  Serial1.write(mLeftBrake);
  Serial1.write(speed);
  Serial1.write(mRightBrake);
  Serial1.write(speed);
}

// brake left motor to set speed
void brakemLeft(int speed){
  Serial1.write(mLeftBrake);
  Serial1.write(speed);
  Serial1.write(mLeftBrake);
  Serial1.write(speed);
}

// brake right motor to set speed
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
  //analogReadAveraging(5);
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
 sensorLeft = analogRead(analogLeft1);
 sensorRight = analogRead(analogRight1);
 
   if((sensorLeft + sensorRight) > white){
     forward(55);
     delay(10);
     count++;
     //sensorLeft = analogRead(analogLeft);
     //sensorRight = analogRead(analogRight);  
   }  
   
   sensorLeft = analogRead(analogLeft1);
   sensorRight = analogRead(analogRight1);
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
   
   sensorLeft = analogRead(analogLeft1);
   sensorRight = analogRead(analogRight1);
       
   if(sensorLeft < black){
        rotateRight(60);
        delay(20);
        //sensorLeft = analogRead(analogLeft);
        //sensorRight = analogRead(analogRight);
        count = 0;
        if(sensorLeft < black){
          forward(60);
          delay(20);
          rotateRight(60);
          delay(20);
          //sensorLeft = analogRead(analogLeft);
          //sensorRight = analogRead(analogRight);
          count = 0;
        }
   }
   sensorLeft = analogRead(analogLeft1);
   sensorRight = analogRead(analogRight1);
   if(sensorRight < black){
     rotateLeft(60);
     delay(20);
     //sensorLeft = analogRead(analogLeft);
     //sensorRight = analogRead(analogRight);
     count = 0;
     if(sensorRight < black){
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
    Serial1.write(127);
    Serial1.write(mRightReverse);   
    Serial1.write(127);
    delay(350);
    analogWrite(hookservo, 255);
    Serial1.write(mLeftReverse);
    Serial1.write(0);
    Serial1.write(mRightReverse);   
    Serial1.write(0);
    delay(500);
    shot = true;
  }
  analogWrite(hookservo, 0);
  analogWrite(hookservo, 255);
  delay(500);
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
 
   if((sensorLeft + sensorRight) > white0){
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
   
   
   
   if((sensorLeft + sensorRight) < blackblack0){
     if(sensorLeft > sensorRight){
          forward(80);
          delay(20);
          rotateLeft(100);
          delay(20);
          count = 0;
     } 
     if((sensorLeft + sensorRight) < blackblack0){
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
     if((sensorLeft + sensorRight) < blackblack0){
       forward(80);
       delay(20);
       rotateRight(100);
       delay(20);
     }
     count = 0;
   }
   
   sensorLeft = analogRead(analogLeft);
   sensorRight = analogRead(analogRight);
       
   if(sensorLeft < black0){
        brakemRight(rotateSpeed);
        delay(20);
        //sensorLeft = analogRead(analogLeft);
        //sensorRight = analogRead(analogRight);
        count = 0;
        if(sensorLeft < black0){
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
   if(sensorRight < black0){
        brakemLeft(rotateSpeed);
        delay(20);
        //sensorLeft = analogRead(analogLeft);
        //sensorRight = analogRead(analogRight);
        count = 0;
        if(sensorRight < black0){
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
