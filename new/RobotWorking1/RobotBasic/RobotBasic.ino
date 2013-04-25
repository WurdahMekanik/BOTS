
void setup() {
  // put your setup code here, to run once:
  initSerial();
//  helloTrex();
mySerial.write(0x81);
}

void loop() {
  // put your main code here, to run repeatedly: 
  //helloTrex();
  if (mySerial.available())
    Serial.print(mySerial.read());
     delay(100);
}
