// Joystick Saver
// For the Arduino Pro Micro
//
// Original QuickShot Joystick to Keyboard Converter
// by Ido Gendel, 2014. Share and enjoy!
//
// Pins:
// 10  LEFT in
// 16  RIGHT in
// 14  UP in
// 15  DOWN in
// A0  FIRE1 in
// A1  FIRE2 in
//
// 8 LED left
// 7 LED right
// 6 LED up
// 5 LED down
// 4 LED fire1
// 3 LED fire2
//
// A1 MODE button
// A2 SPEED+ button
// A3 SPEED- button

#include <Keyboard.h>

#define INPUTS 6

struct tJSInput {
  byte joyPin;
  byte ledPin;
  boolean state;
  byte key;
} JSInput[INPUTS] = {{10, 8, 0, 216}, 
                     {16, 7, 0, 215}, 
                     {14, 6, 0, 218}, 
                     {15, 5, 0, 217}, 
                     {A0, 4, 0, 132}, 
                     {A1, 3, 0, 134}};

void setup() {
  Serial.begin( 9600 );
  pinMode(13, OUTPUT);
  for (int j = 0; j < INPUTS; j++) {
    pinMode(JSInput[j].joyPin, INPUT_PULLUP);
    JSInput[j].state = 
     digitalRead(JSInput[j].joyPin);
     digitalWrite(JSInput[j].ledPin, LOW);
  }  // for
 
 // Time for re-programming in case of trouble
 delay(4000);
 digitalWrite(13, HIGH); // "active" Indication
 Keyboard.begin();
} // setup

void loop() {
  for (int j = 0; j < INPUTS; j++) 
    if (digitalRead(JSInput[j].joyPin) != 
        JSInput[j].state) {
      JSInput[j].state = !JSInput[j].state;
      if (JSInput[j].state)
      {
        Serial.print ("Releasing");
        Keyboard.release(JSInput[j].key);
        digitalWrite(JSInput[j].ledPin, HIGH);
      } else {
        Serial.print ("Pressing");
        Keyboard.press(JSInput[j].key);
        digitalWrite(JSInput[j].ledPin, LOW);
      }
    } // for
  delay(5);
} // loop
