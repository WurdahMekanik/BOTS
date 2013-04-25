/*Working on a frame work for controlling the Trex DMC01 motor
controller with Teensy 3.0*/

#include <Arduino.h>

/*initializes Serial1 of Teensy 3.0*/
void initSerial();

/*trex DMC01 will return firmware version and print to serial monitor */
unsigned char helloTrex();

/*Motor Control Functions*/
void forward(int speed);
void fullForward();
void rotateLeft(int speed);
void rotateRight(int speed);
void reverse(int speed);
void fullReverse();
void setM1(int dSpeed);
void setM2(int dSpeed);
void setM1M2(int dSpeedM1, int dSpeedM2);

//Need to Define
//void stop();

/*Data Retrieval Function*/
unsigned char sendDataCommand(unsigned char dataCommand);

/*constants for dataCommand*/
const unsigned char getMode = 0x82;
const unsigned char getM1Current = 0x8D;
const unsigned char getM2Current = 0x8E;
const unsigned char getTotalCurrent = 0x8F;
const unsigned char getStatusByte = 0x84;  

/*Used by Motor Control Functions to send motorCommands*/
void sendMotorCommand(unsigned char motorCommand, unsigned char value);

//constants for motor1 commands
const unsigned char m1Forward = 0xC6;
const unsigned char m1Reverse = 0xCE;
const unsigned char m1Brake = 0xC7;

//constants for motor1 commands
const unsigned char m2Forward = 0xC5;
const unsigned char m2Reverse = 0xCD;
const unsigned char m2Brake = 0xCF;

//store motorspeeds to keep track of changes
//Motor Speeds range from 0 to 127
unsigned char m1Speed;
unsigned char m2Speed;


/************************************************************
*************************************************************
             FUNCTION DEFINITIONS*/
  
/*request Signature from Trex
  should return TREX and firmware version number 
  */
unsigned char helloTrex(){
  Serial.print(sendDataCommand(0x81));
  while(Serial1.available()){
     Serial.print(Serial1.read());
     Serial.println();
  }
}
  

/* sendDataCommand(unsigned char dataCommand)
   sends  a serial dataCommand  and returns a unsigned char
   recieved from the Trex DMC01 motor controller
   */
unsigned char sendDataCommand(unsigned char dataCommand){
   Serial1.write(dataCommand);
   while(!Serial1.available());
   Serial.println(Serial1.read());
   return Serial1.read();
}


/* sendMotorCommand(unsigned char dataCommand)
   sends  a serial dataCommand  and returns a unsigned char
   recieved from the Trex DMC01 motor controller 
   */
void sendMotorCommand(unsigned char motorCommand, unsigned char value){
  Serial1.write(motorCommand);
  Serial1.write(value);
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

/*
  both motors set to forward at speed
  input 0-127
*/
void forward(int speed){
  sendMotorCommand(m1Forward, speed);
  sendMotorCommand(m2Forward, speed);
}

/*
  both motors set to forward at full 127 speed
*/
void fullForward(){
  sendMotorCommand(m1Forward, 127);
  sendMotorCommand(m2Forward, 127);
}

/*
  both motors set to reverse at speed
  input 0-127
*/
void reverse(int speed){
  sendMotorCommand(m1Reverse, speed);
  sendMotorCommand(m2Reverse, speed);
}

/*
  both motors set to forward at full 127 speed
*/
void fullReverse(){
  sendMotorCommand(m1Reverse, 127);
  sendMotorCommand(m2Reverse, 127);
}

/* 
  motor2 set to reverse and motor1 forward at speed
  input 0-127
*/
void rotateRight(int speed){
  sendMotorCommand(m1Forward, speed);
  sendMotorCommand(m2Reverse, speed);
}

/*
  motor2 set to reverse and motor1 forward at speed
  input 0-127
*/
void rotateLeft(int speed){
  sendMotorCommand(m2Forward, speed);
  sendMotorCommand(m1Reverse, speed);
}

/*
  motor1 set to speed forward or reverse
  input -127 to 127 dSpeed, direction/speed
  negative numbers are reverse speed
  positive are forward speed
*/
void setM1(int dSpeed){
  if(dSpeed >= 0)
    sendMotorCommand(m1Forward, dSpeed);
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    sendMotorCommand(m1Reverse, dSpeed);
  }
}

/*
  motor2 set to speed forward or reverse
  input -127 to 127 dSpeed, direction/speed
  negative numbers are reverse speed
  positive are forward speed
*/
void setM2(int dSpeed){
  if(dSpeed >= 0)
    sendMotorCommand(m2Forward, dSpeed);
  if(dSpeed < 0){
    dSpeed = abs(dSpeed);
    sendMotorCommand(m2Reverse, dSpeed);
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
