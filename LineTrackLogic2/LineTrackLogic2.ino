/* A line following framework that decides what action
 * to take based on the last black/white state of the
 * of the sensors, and if needed the state before last 
 */

//pins for reading ir sensors
const int analogLeft = A0;
const int analogRight = A1;

//store sensor values
int sensorLeft = 0;
int sensorRight = 0;
int sensorDiff = 0;

//store current motor setting
int m1Speed = 50;
int m2Speed = 50;

//constants for motor control
const char m1Forward = 0xC6;
const char m2Forward = 0xC5;
const char m1Reverse = 0xCE;
const char m2Reverse = 0xCD;

//store current motor directions
char m1Direction = m1Forward;
char m2Direction = m2Forward;

/*This is where it gets different than previous attempts 
 *that focused on the difference in value between the sensors
 *
 *Below are used for keeping track of what is happening
 *with the sensors and what just happened
*/
//indicates both sensors over white
const char whitewhite = 0;
//indicates left sensor over white and right over black
const char whiteblack = 1;
//indicates left sensor over black and right over white
const char blackwhite = 2;
//indicates both sensors over black
const char blackblack = 3;

/***State History Is Stored Here***/

  //the state before last
char prePreviousState = whiteblack;
  //the last state
char previousState = whiteblack;
  //the current state
char currentState = whiteblack;

//used to decide whether we are over white or black
int black = 200;
int white = 700;


void setup()  
{
  //set teensy to internal reference voltage
  analogReference(INTERNAL);
  // set the data rate for the Serial port
  // On the teensy 3.0 using teensyduino library
  // the UART1 is refered to as Serial1
  Serial1.begin(19200);
  delay(5);
  //When sent this command the Trex responds with
  //TREX and the version number of the firmware
  Serial1.write(0x81);
}

void loop() // run over and over
{ 
 //read/store the ir sensor values
  sensorLeft = analogRead(analogLeft);
  sensorRight = analogRead(analogRight);
    
  //use getCurrentState to check to see if the sensor 
  //readings indicate the state has changed from what it was
  if(currentState != getCurrentState()){
    //shift stored states, store currentState
    prePreviousState = previousState;
    previousState = currentState;
    currentState = getCurrentState();
  }
  
  /*This nested switch case block uses the currentState, 
   * previousState and prePreviousState to figure out what
   * part of the track we have encountered in order to react
   * accordingly
   */
  switch(currentState){
  
    //current state is left over white, right over black
    case whiteblack:{
      //line following routine to keep left over black
      //we are on the outside edge of line
      break;
    }
    
    //current state is left over black, right over white
    case blackwhite:{
      //line following routine to keep right over black
      //we are on inside of line
      break;
    }
    
    //currently both sensors over black
    case blackblack:{
      //we hit a sharp turn
      //do something depending on what previously happened
      switch(previousState){
        
        case blackwhite:{
          //this means we were on the right edge of the 
          //line(from the bots perspective) and have hit 
          //a sharp right turn
          break;
        }
    
        case whiteblack:{
          //this means we were on the left edge of the 
          //line(from the bots perspective) and have hit 
          //a sharp left turn
          break;
        }
        
        case whitewhite:{
          // WTF!? This should never happen... HALT & CATCH FIRE.
          switch(prePreviousState){
  
              case whiteblack:{
                //we were on the left edge of the line
                //it was a sharp left
                //backup, while turning left until line is found
                break;
              }
    
              case blackwhite:{
               //we were on the right edge of the line
               //it was a sharp right
                break;
              }
           }
          break;
        } 
     }
     break;
   }
    
    //currently both sensors over white
    case whitewhite:{
      //we lost the line, but where was it?
      switch(previousState){
  
        case whiteblack:{
          //we were on the left edge of the line, turn right
        break;
        }
    
        case blackwhite:{
         //we were on the left edge of the line, turn left
        break;
        }
    
        case blackblack:{
         // uh-oh... we lost the line after encountering a
         // very sharp turn, where were we before we lost it?
           switch(prePreviousState){
  
              case whiteblack:{
                //we were on the left edge of the line
                //It was a sharp left
              break;
              }
    
              case blackwhite:{
               //we were on the right edge of the line
               //it was a sharp right
              break;
              }
           }
           break;
        } 
        break;
     }
     break;
   }
  } 
  
  //send motor1 Commands
  Serial1.write(m1Direction);
  Serial1.write(m1Speed);
  //send motor2 Commands  
  Serial1.write(m2Direction);   
  Serial1.write(m2Speed);
  
  //For testing purposes, reports Sensors and the difference
  //for viewing in the serial monitor
  Serial.print("m1 = " );                       
  Serial.print(m1Speed);      
  Serial.print("   m2 = " );                       
  Serial.print(m2Speed);      
  Serial.print("    Difference: ");
  Serial.println(sensorDiff);
}


char getCurrentState(){
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
