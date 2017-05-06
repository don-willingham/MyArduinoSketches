/*
 * PIR sensor tester
 * Copied from https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/using-a-pir
 * Reference https://www.arduino.cc/en/Serial/read
 */
#include <Servo.h>
 
int ledPin = 13;                // choose the pin for the LED
int servoPin = 11;              // choose the pin for the servo
Servo servoObj;
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
char strBuffer[256];
int  strLen = 0;

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  servoObj.attach(servoPin);
  Serial.begin(9600);
  
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
  // send data only when you receive data:
  if ((Serial.available() > 0) &&
      (strLen < sizeof(strBuffer))) {
    // read the incoming byte:
    strBuffer[strLen++] = Serial.read();
    if ('\n' == strBuffer[strLen-1]) {
      // If a new line was just read
      if ('%' == strBuffer[strLen-2]) {
        Serial.print("Got ");
        Serial.println(strBuffer);
        // % character before \n
        strBuffer[strLen-2] = '\0';
        String strObj = strBuffer;
        float percent = strObj.toFloat();
        Serial.print("As a float   ");
//        sscanf(strBuffer, "%f", &percent);
        Serial.println(percent);
        if (percent > 100.0) {
          Serial.println("Clipping at 100%");
          percent = 100.0;
        } else if (percent < 0.0) {
          Serial.println("Clipping at 0%");
          percent = 0.0;
        }
        strLen=0;
/*        int zeroTo255 = (int)(percent / 100.0 * 255.0);
        if (zeroTo255 < 0) {
          zeroTo255 = 0; 
        } else if (zeroTo255 > 255) {
          zeroTo255 = 255;
        }*/
        int angle = (int)(percent / 100.0 * 180.0);
        if (angle < 8) {
          angle = 8;
        } else if (angle > 176) {
          angle = 176;
        }
        Serial.print("Servo Out=");
        Serial.println(angle);
        //analogWrite(servoPin, zeroTo255);
        servoObj.write(angle);
      }
    }
  }
}
