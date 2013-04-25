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
  Use sendMotorCommand() to control motors 
  or use Data Retrieval Command
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
   
      unsigned char getData(unsigned char dataCommand);
      void sendMotorCommand(unsigned char motorCommand, unsigned char value);
    
      //constants for motor1 commands
      const unsigned char m1Forward = 0xC6;
      const unsigned char m1Reverse = 0xCE;
      const unsigned char m1Brake = 0xC7;
    
      //constants for motor2 commands
      const unsigned char m2Forward = 0xC5;
      const unsigned char m2Reverse = 0xCD;
      const unsigned char m2Brake = 0xCF;
    
      //get data commands
      const unsigned char getMode = 0x82;
      const unsigned char getM1Current = 0x8D;
      const unsigned char getM2Current = 0x8E;
      const unsigned char getTotalCurrent = 0x8F;
      const unsigned char getStatusByte = 0x84;
};

#endif
      


