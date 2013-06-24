#include "RobotFinalCommands.h"

// initialization
void setup() {
  initial();
  if (digitalRead(tilt)){
    tiltMask = 1;
  }
  if (!digitalRead(tilt)){
    tiltMask = 0;
  }
}

// main program
void loop() {
  if(!started){
    forward(127);
    delay(3);
    started = 1;
  }
  if (digitalRead(grap) == 1) {
    grapMask = 1;
  }
  if (tiltMask){
    analogLeft  = A3;
    analogRight = A4;
    lineTracking();
  }
  if (!tiltMask){
    analogLeft  = A5;
    analogRight = A6;
    straightLine();
    if (grapMask){
      grapple();
    }
  }
}
