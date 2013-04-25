
#include <SoftwareSerial2.h>

SoftwareSerial2 mySerial(0, 1);

void setup()  
{
  Serial.begin(19200);
  mySerial.begin(19200);
  delay(5);
  //Serial.println("Goodnight moon!");
  char value = 0x81;
  //mySerial.write(value);
  Serial.write(0x81);

}

void loop()                     // run over and over again
{

  if (mySerial.available()) {
      Serial.print((char)mySerial.read());
  }
  if (Serial.available()) {
      mySerial.print((char)Serial.read());
  }
}
