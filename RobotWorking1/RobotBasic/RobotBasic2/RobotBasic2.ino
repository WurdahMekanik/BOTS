//constants for motor1 commands
const unsigned char m1Forward = 0xC6;
const unsigned char m1Reverse = 0xC5;
const unsigned char m1Brake = 0xC7;

//constants for motor1 commands
const unsigned char m2Forward = 0xCE;
const unsigned char m2Reverse = 0xCD;
const unsigned char m2Brake = 0xCF;

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
 forward(90);
 delay(700);
 reverse(90);
 delay(700);
 brakeLow(30);
 delay(700);
 rotateLeft(90);
 delay(700);
 brakeLow(30);
 delay(700);
 rotateRight(90);
 delay(700);
 
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
