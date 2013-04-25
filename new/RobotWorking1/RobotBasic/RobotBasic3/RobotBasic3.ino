//constants for motor1 commands
const unsigned char m1Forward = 0xC6;
const unsigned char m1Reverse = 0xC5;
const unsigned char m1Brake = 0xC7;

//constants for motor1 commands
const unsigned char m2Forward = 0xCE;
const unsigned char m2Reverse = 0xCD;
const unsigned char m2Brake = 0xCF;

//indicates, both sensors over white
const char whitewhite = 0;
//indicates, left sensor over white and right over black
const char whiteblack = 1;
//indicates, left sensor over black and right over white
const char blackwhite = 2;
//indicates, both sensors being over black
const char blackblack = 3;

/*State History Is Stored Here*/

  //the stat before last.
char prePreviousState = whiteblack;
  //keep track of last state
char previousState = whiteblack;
  //current blackwhite State
char currentState = whiteblack;

//used to decide whether we are over white or black
int black = 200;
int white = 900;


//pins for reading ir sensors
const int analogLeft = A0;
const int analogRight = A1;

//store sensor values
int sensorLeft = 0;
int sensorRight = 0;
int sensorDiff = 0;
int m1Speed = 60;
int m2Speed = 60;

void setup() {
  // put your setup code here, to run once:
  initSerial();
  helloTrex();
}

void loop() {
  // put your main code here, to run repeatedly:
 sensorLeft = analogRead(analogLeft);
 sensorRight = analogRead(analogRight);
 
 switch(getCurrentState(sensorLeft, sensorRight)){
   case whiteblack:{
     if(sensorLeft < 700)
       while(sensorLeft < 700)
         brakeM1(40);
     else if(sensorRight > 100)
       while(sensorRight > 100)
         brakeM2(40);
   break;
   }
   case blackwhite:{
     if(sensorLeft > 100)
       while(sensorLeft > 100)
       brakeM1(40);
     else if(sensorRight > 700)
       while(sensorRight < 700)
       brakeM2(40);
     break;
   }
   case whitewhite:{
   break;
   }
   case blackblack:{
     break;
   }
 }
 forward(m1Speed);
 Serial.print("s1 = " );                       
  Serial.print(sensorLeft);      
  Serial.print("   s2 = " );                       
  Serial.print(sensorRight);
}

/*
  both motors set to forward at full 127 speed
*/
void forward(int speed){
  Serial1.write(m1Forward);
  Serial1.write(speed);
  Serial1.write(m2Forward);
  Serial1.write(speed);
}

/*
  both motors set to forward at full 127 speed
*/
void reverse(int speed){
  Serial1.write(m1Reverse);
  Serial1.write(speed);
  Serial1.write(m2Reverse);
  Serial1.write(speed);
}

/*
  both motors set to forward at full 127 speed
*/
void brakeLow(int speed){
  Serial1.write(m1Brake);
  Serial1.write(speed);
  Serial1.write(m2Brake);
  Serial1.write(speed);
}

/*
  both motors set to forward at full 127 speed
*/
void brakeM1(int speed){
  Serial1.write(m1Brake);
  Serial1.write(speed);
}

/*
  both motors set to forward at full 127 speed
*/
void brakeM2(int speed){
  Serial1.write(m2Brake);
  Serial1.write(speed);
}

/*
  motor2 set to speed forward or reverse
  input -127 to 127 dSpeed, direction/speed
  negative numbers are reverse speed
  positive are forward speed
*/
void setM1(int dSpeed){
  if(dSpeed >= 0){
    Serial1.write(m1Forward);
    Serial1.write(dSpeed);
  }
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    Serial1.write(m1Reverse);
    Serial1.write(dSpeed);
  }
}

/*
  motor2 set to speed forward or reverse
  input -127 to 127 dSpeed, direction/speed
  negative numbers are reverse speed
  positive are forward speed
*/
void setM2(int dSpeed){
  if(dSpeed >= 0){
    Serial1.write(m2Forward);
    Serial1.write(dSpeed);
  }
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    Serial1.write(m2Reverse);
    Serial1.write(dSpeed);
  }
}


/*
  motor1 and motor2 set to speed forward or reverse
  input -127 to 127 dSpeed, direction/speed
  negative numbers are reverse speed
  positive are forward speed
*/
void setM1M2(int dSpeedM1, int dSpeedM2){
  setM1(dSpeedM1);
  setM2(dSpeedM2);
}

/* 
  motor2 set to reverse and motor1 forward at speed
  input 0-127
*/
void rotateRight(int speed){
  setM1(speed);
  setM2(0 - speed);
}

/*
  motor2 set to reverse and motor1 forward at speed
  input 0-127
*/
void rotateLeft(int speed){
  setM2(speed);
  setM1(0 - speed);
}

/* initSerial()
   starts up both hardware serial and
   software serial communication */
void initSerial(){
   Serial.begin(19200);
   delay(5);
   Serial1.begin(19200);
   delay(5);
}

/*request Signature from Trex
  should return TREX and firmware version number 
  */
unsigned char helloTrex(){
  Serial1.write(0x81);
  while(Serial1.available()){
     Serial.print(Serial1.read());
     Serial.println();
  }
}

char getCurrentState(int sensorLeft, int sensorRight){
  /*
   *Figure out the current state and return the value
   */
  if((sensorLeft > white) && (sensorRight < black)){
    return whiteblack;
  }
  
  if((sensorRight < black) && (sensorLeft > white)){
    return blackwhite;
  }
  
  if((sensorLeft > white) && (sensorRight > white)){
    return whitewhite;
  }
  
  if((sensorLeft < black) && (sensorRight < black)){
    return blackblack;
  }
}

void cycleSensors(){
  //shift stored states, store currentState
  //read/store the ir sensor values
    sensorLeft = analogRead(analogLeft);
    sensorRight = analogRead(analogRight);
    
    prePreviousState = previousState;
    previousState = currentState;
    currentState = getCurrentState(sensorLeft, sensorRight);
}


