// constants for motor1 commands
const unsigned char mLeftForward =  0xC6;
const unsigned char mLeftReverse =  0xC5;
const unsigned char mLeftBrake   =  0xC7;
// constants for motor2 commands
const unsigned char mRightForward =  0xCE;
const unsigned char mRightReverse =  0xCD;
const unsigned char mRightBrake   =  0xCF;
// sensor pins
int analogLeft   = A1;
int analogRight  = A2;
int analogLeft1  = A3;
int analogRight1 = A4;
// grappling & tilt stuff
const int tilt      =  5;
const int grap      =  4;
const int hookservo =  3;
const char winch    =  0xF1;
// sensor values
int sensorLeft  =  0;
int sensorRight =  0;
int sensorDiff  =  0;
// masks & counts
int count        =  0;
boolean started  =  0;
boolean tiltMask =  0;
boolean shot     =  0;
boolean grapMask =  0;


// used for default straightLineFollow
int black = 100;
int white = 400;
int blackblack = 200;

// used for flipside line following
int white0 = 250;
int black0 = 80;
int blackblack0 = 150;

// assorted other stuff
int speed       =  70;
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
    lineTracking();
  }
  if(!grapMask){
    if(!tiltMask){
      straightLineFast();
      if (grapMask){
        grapple();
      }
    }
  }
  /*** TESTING ***/
  Serial.print("s1 = " );                       
  Serial.print(sensorLeft);      
  Serial.print("   s2 = " );                       
  Serial.println(sensorRight);
  Serial.print("tilt = ");
  Serial.print("tilt");
}


/*******************/
/**** FUNCTIONS ****/
/*******************/

// both motors set to forward at full 127 speed
void forward(int speed){
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

// left motor set to dSpeed value
// negative values -> reverse
// positive values -> forward
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

// right motor set to dSpeed value
// negative values -> reverse
// positive values -> forward
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

// both motors set to dSpeed value
// negative values -> reverse
// positive values -> forward
void setmLeftmRight(int dSpeedmLeft, int dSpeedmRight){
  setmLeft(dSpeedmLeft);
  setmRight(dSpeedmRight);
  setmLeft(dSpeedmLeft);
  setmRight(dSpeedmRight);
}
 
// left motor set to value of speed (0-127)
// right motor set to reverse value of speed
void rotateRight(int speed){
  setmLeft(speed);
  setmRight(0 - speed);
}

// right motor set to value of speed (0-127)
// left motor set to reverse value of speed
void rotateLeft(int speed){
  setmRight(speed);
  setmLeft(0 - speed);
}

// initialize hardware & software serial communication
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

// request version number of TREX and firmware
unsigned char helloTrex(){
  Serial1.write(0x81);
}

void lineTracking(){
  sensorLeft = analogRead(analogLeft1);
  sensorRight = analogRead(analogRight1);
  if((sensorLeft + sensorRight) > white0){
    forward(55);
    delay(10);
    count++; 
  }  
  sensorLeft = analogRead(analogLeft1);
  sensorRight = analogRead(analogRight1);  
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
  sensorLeft = analogRead(analogLeft1);
  sensorRight = analogRead(analogRight1);
  if(sensorLeft < black0){
    rotateRight(60);
    delay(20);
    count = 0;
    if(sensorLeft < black0){
      forward(60);
      delay(20);
      rotateRight(60);
      delay(20);
      count = 0;
    }
  }
  sensorLeft = analogRead(analogLeft1);
  sensorRight = analogRead(analogRight1);
  if(sensorRight < black0){
    rotateLeft(60);
    delay(20);
    count = 0;
    if(sensorRight < black0){
      forward(80);
      delay(20);
      rotateLeft(60);
      delay(20);
      count = 0;
    }
  }
}
 
void grapple(){
  if (!shot){
    Serial1.write(mLeftReverse);
    Serial1.write(127);
    Serial1.write(mRightReverse);   
    Serial1.write(127);
    delay(250);
    analogWrite(hookservo, 255);
    Serial1.write(mLeftReverse);
    Serial1.write(0);
    Serial1.write(mRightReverse);   
    Serial1.write(0);
    delay(300);
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
  if((sensorLeft + sensorRight) > white0){
    forward(127);
    delay(5);
    count++;
  }  
  sensorLeft = analogRead(analogLeft);
  sensorRight = analogRead(analogRight);   
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
    count = 0;
    if(sensorLeft < black0){
      forward(60);
      delay(20);
      rotateRight(rotateSpeed);
      delay(20);
      count = 0;
    }
  }
  sensorLeft = analogRead(analogLeft);
  sensorRight = analogRead(analogRight);
  if(sensorRight < black0){
    brakemLeft(rotateSpeed);
    delay(20);
    count = 0;
    if(sensorRight < black0){
      forward(80);
      delay(20);
      rotateLeft(rotateSpeed);
      delay(20);
      count = 0;
    } 
  }
}
