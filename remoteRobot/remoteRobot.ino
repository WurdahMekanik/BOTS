int speedPot  = A0;
int turnPot     = A1;
int dSpeed     = 0;
int turn          = 0;
int rightSpeed = 0;
int leftSpeed  = 0;

// constants for motor1 commands
const unsigned char leftForward  = 0xC6;
const unsigned char leftReverse = 0xC5;
const unsigned char leftBrake     = 0xC7;
// constants for motor2 commands
const unsigned char rightForward  = 0xCE;
const unsigned char rightReverse = 0xCD;
const unsigned char rightBrake     = 0xCF;

void setup(){
  Serial.begin(19200);
  delay(5);
  Serial1.begin(19200);
  delay(5);
  //while(!Serial);
}

void loop(){
  dSpeed = map(analogRead(speedPot),0,1023,-120,120);
  turn = map(analogRead(turnPot),0,1023,-127,127);
  
  setVelocity();
  
  Serial.print("Right Speed: ");
  Serial.println(rightSpeed);
  Serial.print("Left Speed: ");
  Serial.println(leftSpeed);
  Serial.print("Turn: ");
  Serial.println(turn);
  delay(100);
}

/*************************/
/***** FUNCTIONS *****/
/*************************/

void turning(){
  if (dSpeed > 0){
    if (turn >= 0){
      rightSpeed = (dSpeed - turn);
      leftSpeed = dSpeed;
    }
    else if (turn < 0){
      rightSpeed = dSpeed;
      leftSpeed = (dSpeed + turn);
    }
  }
  else if (dSpeed <= 0){
    if (turn >= 0){
      rightSpeed = (dSpeed + turn);
      leftSpeed = dSpeed;
    }
    else if (turn < 0){
      rightSpeed = dSpeed;
      leftSpeed = (dSpeed - turn);
    }
  }
}
// Left motor control
void setLeft(int leftSpeed){
  if(leftSpeed >= 0){
    Serial1.write(leftForward);
    Serial1.write(leftSpeed);
  }
  if(leftSpeed < 0){
    leftSpeed = abs(leftSpeed);
    Serial1.write(leftReverse);
    Serial1.write(leftSpeed);
  }
  delay(5);
}

// Right motor control
void setRight(int rightSpeed){
  if(rightSpeed >= 0){
    Serial1.write(rightForward);
    Serial1.write(rightSpeed);
  }
  if(rightSpeed < 0){
    rightSpeed = abs(rightSpeed);
    Serial1.write(rightReverse);
    Serial1.write(rightSpeed);
  }
  delay(5);
}

void setVelocity(){
  turning();
  delay(5);
  setRight(rightSpeed);
  setLeft(leftSpeed);
}
