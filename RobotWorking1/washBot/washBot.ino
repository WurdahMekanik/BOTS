/*Using the Software serial example we 
successfully communicated with the Trex
recieving data and sending commands
*/


/*
  Software serial multple serial test
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
 
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(19200);
  delay(5);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(19200);
  delay(5);
  mySerial.println("Hello, world?");
  mySerial.write(0x81);
}

void loop() // run over and over
{
  
  //write hex command to trex followed by set value
  mySerial.write(0xC6);
  mySerial.write(127);
  
  //write hex command to trex followed by set value
  mySerial.write(0xCE);
  mySerial.write(50);
  
  //motor 1 goes and and forth, hence washbot 
  delay(500);
  mySerial.write(0xC1);
  mySerial.write(127);
  
  
  //Look for serial in, hex to char
  if (mySerial.available())
    Serial.write(mySerial.read() + 0x30);
  if (Serial.available())
    mySerial.write(Serial.read());
  
  //Request motor current from Trex  
  mySerial.write(0x8F);
  delay(200);
}
