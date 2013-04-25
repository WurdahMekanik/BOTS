#include "RobotFrame.h"

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
 rotateLeft(90);
 delay(700);
 rotateRight(90);
}
