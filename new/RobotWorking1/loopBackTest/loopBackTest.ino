/*
  Serial Event example
 
 When new serial data arrives it adds one waits 300ms abd sends out new data
 
 */
char inChar;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(19200);
  Serial.write('A');
}

void loop() {
  // print the string when a newline arrives:
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    inChar = (char)Serial.read(); 
    // add it to the inputString: 
  }
  delay(300);
  Serial.write(inChar + 1);
}



