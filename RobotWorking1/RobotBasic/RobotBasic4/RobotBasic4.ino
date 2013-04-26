// constants for motor1 commands
const unsigned char m1Forward =  0xC6;
const unsigned char m1Reverse =  0xC5;
const unsigned char m1Brake   =  0xC7;
// constants for motor2 commands
const unsigned char m2Forward =  0xCE;
const unsigned char m2Reverse =  0xCD;
const unsigned char m2Brake   =  0xCF;
// pins for reading IR sensors
const int analogLeft   =  A3;
const int analogRight  =  A4;
const int analogLeft2  =  A5;
const int analogRight2 =  A6;
// grappling & tilt stuff
const int tilt      =  5;
const int grap      =  4;
const int hookservo =  3;
const char winch    =  0xF1;
// store sensor values
int sensorLeft  =  0;
int sensorRight =  0;
int sensorDiff  =  0;
int speed       =  70;
int m2Speed     =  127;
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
  if (grapMask){
    if (tiltMask){
      lineTracking();
    }
    if (!tiltMask){
      straightLine();
    }
  }
  if (!grapMask){
    grapple();
  }
}

// both motors set to forward at full 127 speed
void forward(int speed){
  Serial1.write(m1Forward);
  Serial1.write(speed);
  Serial1.write(m2Forward);
  Serial1.write(speed);
  Serial1.write(m1Forward);
  Serial1.write(speed);
  Serial1.write(m2Forward);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void reverse(int speed){
  Serial1.write(m1Reverse);
  Serial1.write(speed);
  Serial1.write(m2Reverse);
  Serial1.write(speed);
  Serial1.write(m1Reverse);
  Serial1.write(speed);
  Serial1.write(m2Reverse);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void brakeLow(int speed){
  Serial1.write(m1Brake);
  Serial1.write(speed);
  Serial1.write(m2Brake);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void brakeM1(int speed){
  Serial1.write(m1Brake);
  Serial1.write(speed);
  Serial1.write(m1Brake);
  Serial1.write(speed);
}

// both motors set to forward at full 127 speed
void brakeM2(int speed){
  Serial1.write(m2Brake);
  Serial1.write(speed);
  Serial1.write(m2Brake);
  Serial1.write(speed);
}

// motor2 set to speed forward or reverse
// input -127 to 127 dSpeed, direction/speed
// negative numbers are reverse speed
// positive are forward speed
void setM1(int dSpeed){
  if(dSpeed >= 0){
    Serial1.write(m1Forward);
    Serial1.write(dSpeed);
    Serial1.write(m1Forward);
    Serial1.write(dSpeed);
  }
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    Serial1.write(m1Reverse);
    Serial1.write(dSpeed);
    Serial1.write(m1Reverse);
    Serial1.write(dSpeed);
  }
}

// motor2 set to speed forward or reverse
// input -127 to 127 dSpeed, direction/speed
// negative numbers are reverse speed
// positive are forward speed
void setM2(int dSpeed){
  if(dSpeed >= 0){
    Serial1.write(m2Forward);
    Serial1.write(dSpeed);
    Serial1.write(m2Forward);
    Serial1.write(dSpeed);
  }
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    Serial1.write(m2Reverse);
    Serial1.write(dSpeed);
    Serial1.write(m2Reverse);
    Serial1.write(dSpeed);
  }
}

// motor1 and motor2 set to speed forward or reverse
// input -127 to 127 dSpeed, direction/speed
// negative numbers are reverse speed
// positive are forward speed
void setM1M2(int dSpeedM1, int dSpeedM2){
  setM1(dSpeedM1);
  setM2(dSpeedM2);
  setM1(dSpeedM1);
  setM2(dSpeedM2);
}
 
// motor2 set to reverse and motor1 forward at speed
// input 0-127
void rotateRight(int speed){
  setM1(speed);
  setM2(0 - speed);
  setM1(speed);
  setM2(0 - speed);
}

// motor2 set to reverse and motor1 forward at speed
// input 0-127
void rotateLeft(int speed){
  setM2(speed);
  setM1(0 - speed);
  setM2(speed);
  setM1(0 - speed);
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
  
}

void grapple(){
  if (!shot){
    Serial1.write(m1Reverse);
    Serial1.write(40);
    Serial1.write(m2Reverse);   
    Serial1.write(40);
    Serial.println("HERE IT COMES!");
    // wait for it...
    delay(800);
    analogWrite(hookservo, 255);
    delay(1000);
    shot = 1;
  }
  analogWrite(hookservo, 0);
  Serial1.write(winch);
  Serial1.write(127);
  // There will be Slack! (change the delay to let winch acquire Slack)
  delay(7000);
  Serial1.write(m1Forward);
  Serial1.write(50);
  Serial1.write(m2Forward);
  Serial1.write(50);
}

void testing(){
  Serial.print("m1 = " );                       
  Serial.print(m1Speed);      
  Serial.print("   m2 = " );                       
  Serial.print(m2Speed);      
  Serial.print("    Difference: ");
  Serial.println(sensorDiff);
}
