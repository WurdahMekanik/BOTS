HardwareSerial Uart = HardwareSerial();
//pins for reading IR sensors
const int analogLeft = A0;
const int analogRight = A1;
//constants for motor control
const char m1Forward = 0xC6;
const char m2Forward = 0xCE;
const char m1Reverse = 0xC5;
const char m2Reverse = 0xCD;
//store sensor values
int sensorLeft = 0;
int sensorRight = 0;
int sensorDiff = 0;
int m1Speed = 65;
int m2Speed = 65;
char m1Direction = m1Forward;
char m2Direction = m2Forward;
//limits used for determining how sharp to turn
const int s1Limit1 = 80;
const int s2Limit1 = -80;
const int s1Limit2 = 220;
const int s2Limit2 = -220;
const int s1Limit3 = 320;
const int s2Limit3 = -320;
const int s1Limit4 = 420;
const int s2Limit4 = -420;
//Hook & winch
const int grap = 4;
const int hookservo = 3;
const char winch = 0xF1;
int grapmask = 0;

void setup(){
  analogReadAveraging(20);
  pinMode(grap, INPUT);
  pinMode(hookservo, OUTPUT);
  Uart.begin(19200);
  Serial.begin(19200);
  delay(5);
  Uart.write(0x81);
}
void loop(){ 
  if (digitalRead(grap) == 1) {
    Serial.println("MASK!");
    grapmask=1;
  }
  while(grapmask==0){
    traceur();
  }
  while(grapmask==1){
    grappy();
  }
  //TESTING
  Serial.print("m1 = " );                       
  Serial.print(m1Speed);      
  Serial.print("   m2 = " );                       
  Serial.print(m2Speed);      
  Serial.print("    Difference: ");
  Serial.println(sensorDiff);
}
void traceur(){
  sensorLeft = analogRead(analogLeft);
  sensorRight = analogRead(analogRight);
  sensorDiff = sensorLeft - sensorRight;  

  if(sensorDiff > s1Limit4){
    //stop, look, listen...
    //then rotate in place a bit
    m1Direction = m1Reverse;
    m1Speed = 45;
    m2Direction = m2Forward;
    m2Speed = 45;
    motor_set();
    delay(800);
  }
  else if(sensorDiff < s2Limit4){
    //stop, look, listen...
    //then rotate in place a bit
    m1Direction = m1Forward;
    m1Speed = 45;
    m2Direction = m2Reverse;
    m2Speed = 45;
    motor_set();
    delay(800);
  }
  else if(sensorDiff > s1Limit3){
    m1Direction = m1Reverse;
    m1Speed = 40;
    m2Direction = m2Forward;
    m2Speed = 40;
    motor_set();
    delay(700);
  }
  else if(sensorDiff < s2Limit3){
    m1Direction = m1Forward;
    m1Speed = 40;
    m2Direction = m2Reverse;
    m2Speed = 40;
    motor_set();
    delay(700);
  }
  else if(sensorDiff > s1Limit2){
    m1Direction = m1Reverse;
    m1Speed = 35;
    m2Direction = m2Forward;
    m2Speed = 35;
    motor_set();
    delay(600);
  }
  else if(sensorDiff < s2Limit2){
    m1Direction = m1Forward;
    m1Speed = 35;
    m2Direction = m2Reverse;
    m2Speed = 35;
    motor_set();
    delay(600);
  }
  else if(sensorDiff > s1Limit1){
    m1Direction = m1Reverse;
    m1Speed = 30;
    m2Direction = m2Forward;
    m2Speed = 30;
    motor_set();
    delay(500);
  }
  else if(sensorDiff < s2Limit1){
    m1Direction = m1Forward;
    m1Speed = 30;
    m2Direction = m2Reverse;
    m2Speed = 30;
    motor_set();
    delay(500);
  }
  else{
    m1Direction = m1Forward;
    m2Direction = m2Forward;
    m1Speed = 45;
    m2Speed = 45;
    motor_set();
  } 
}

void grappy(){
  Uart.write(m1Reverse);
  Uart.write(50);
  Uart.write(m2Reverse);   
  Uart.write(50);
    Serial.println("HERE IT COMES!");
  // wait for it...
  delay(800);
  analogWrite(hookservo, 200);
  Uart.write(winch);
  Uart.write(127);
  // There will be Slack! (change the delay to let winch acquire Slack)
  delay(500);
  analogWrite(hookservo, 0);
  Uart.write(m1Forward);
  Uart.write(45);
  Uart.write(m2Forward);
  Uart.write(45);
}
void motor_set(){
  // send motor commands
  Uart.write(m1Direction);
  Uart.write(m1Speed);
  Uart.write(m2Direction);   
  Uart.write(m2Speed);
}

