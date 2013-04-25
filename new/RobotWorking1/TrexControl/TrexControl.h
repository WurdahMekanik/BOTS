/*
  TrexControl.h is a Library for controlling a Trex DMC01
  motor controller through the serial port at the default
  baud rate of 19200.
 */
 
 #ifdef TrexControl_h
 #define TrexControl_h

class TrexControl
{
/*
      Set up to work with Serial1 on teensy 
      note- replace all Serial1 with Serial
      in TrexControl.cpp to work for most arduinos 
*/
   public:
      /*Motor Control Functions*/
      void goStraight(int speed);
      void fullForward();
      void fullStop();
      void slowDown();
      void left();
      void right();
      void reverse();
      void rotate();
      
      
      //you can also send motor commands and setting using this
      //with the command constants below
      void sendMotorCommand(unsigned char motorCommand, unsigned char value);
    
      //constants for motor1 commands
      const unsigned char m1Forward = 0xC6;
      const unsigned char m1Reverse = 0xCE;
      const unsigned char m1Brake = 0xC7;
    
      //constants for motor2 commands
      const unsigned char m2Forward = 0xC5;
      const unsigned char m2Reverse = 0xCD;
      const unsigned char m2Brake = 0xCF;
    
    
      //use this function and the commands below to get information
      //from the trex. send a byte, returns a byte
      unsigned char getData(unsigned char dataCommand);
      
      //get data commands
      const unsigned char getMode = 0x82;
      const unsigned char getM1Current = 0x8D;
      const unsigned char getM2Current = 0x8E;
      const unsigned char getTotalCurrent = 0x8F;
      const unsigned char getStatusByte = 0x84;
};

#endif
      


