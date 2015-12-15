// Joystick Saver
// For the Arduino Pro Micro
//
// Original QuickShot Joystick to Keyboard Converter
// by Ido Gendel, 2014. Share and enjoy!

#include <Keyboard.h>

#define INPUTS (6)

#define kLeftPin (10)
#define kRightPin (16)
#define kUpPin (14)
#define kDownPin (15)
#define kFire1Pin (A0)
#define kFire2Pin (A1)

#define kModePin (9)
#define kSpeedUpPin (A2)
#define kSpeedDownPin (A3)

#define kLEDLeftPin (8)
#define kLEDRightPin (7)
#define kLEDUpPin (6)
#define kLEDDownPin (5)
#define kLEDFire1Pin (4)
#define kLEDFire2Pin (3)

byte currentMode;
int currentSpeed = 0;

struct tJSInput {
  byte joyPin;
  byte ledPin;
  boolean state;
  byte key;
  char desc[10];
} JSInput[INPUTS] = {{kLeftPin, kLEDLeftPin, 0, 216, "left"}, 
                     {kRightPin, kLEDRightPin, 0, 215, "right"}, 
                     {kUpPin, kLEDUpPin, 0, 218, "up"}, 
                     {kDownPin, kLEDDownPin, 0, 217, "down"}, 
                     {kFire1Pin, kLEDFire1Pin, 0, 132, "fire 1"}, 
                     {kFire2Pin, kLEDFire2Pin, 0, 134, "fire 2"}};

void setup() {
  Serial.begin( 9600 );
  pinMode(13, OUTPUT);
  for (int j = 0; j < INPUTS; j++) {
    pinMode(JSInput[j].joyPin, INPUT_PULLUP);
    JSInput[j].state = 
     digitalRead(JSInput[j].joyPin);
     digitalWrite(JSInput[j].ledPin, LOW);
  }  // for

  pinMode(kModePin, INPUT_PULLUP);
  pinMode(kSpeedUpPin, INPUT_PULLUP);
  pinMode(kSpeedDownPin, INPUT_PULLUP);
 
 // Time for re-programming in case of trouble
 delay(4000);
 digitalWrite(13, HIGH); // "active" Indication
 Keyboard.begin();
} // setup

void modeSwitch( byte inputMode) {
  delay(250);
}

void speedUp() {
  switch ( currentSpeed ) {
    case ( 500 ):
      currentSpeed = 400;
      Serial.println ("Setting move duration to 400");
      break;
    case ( 400 ):
      currentSpeed = 300;
      Serial.println ("Setting move duration to 300");
      break;
    case ( 300 ):
      currentSpeed = 200;
      Serial.println ("Setting move duration to 200");
      break;
    case ( 200 ):
      currentSpeed = 100;
      Serial.println ("Setting move duration to 100");
      break;
    case ( 100 ):
      currentSpeed = 50;
      Serial.println ("Setting move duration to 50");
      break;
    case ( 50 ):
      Serial.println ("Duration already at 50, doing nothing");

    default:
      break;
  }
  delay(175);
}

void speedDown() {
  switch ( currentSpeed ) {
    case ( 50 ):
      currentSpeed = 100;
      Serial.println ("Setting move duration to 100");
      break;
    case ( 100 ):
      currentSpeed = 200;
      Serial.println ("Setting move duration to 200");
      break;
    case ( 200 ):
      currentSpeed = 300;
      Serial.println ("Setting move duration to 300");
      break;
    case ( 300 ):
      currentSpeed = 400;
      Serial.println ("Setting move duration to 400");
      break;
    case ( 400 ):
      currentSpeed = 500;
      Serial.println ("Setting move duration to 500");
      break;
    case ( 500 ):
      Serial.println ("Duration already at 500, doing nothing");

    default:
      break;
  }
  delay(175);
}

void loop() {
  if (!digitalRead(kModePin))
  {
    Serial.println ("Mode switch pressed");
    modeSwitch(currentMode);
  }

  if (!digitalRead(kSpeedUpPin))
  {
    Serial.println ("Speed up pressed");
    speedUp();
  }

  if (!digitalRead(kSpeedDownPin))
  {
    Serial.println ("Speed down pressed");
    speedDown();
  }

for (int j = 0; j < INPUTS; j++) 
    if (digitalRead(JSInput[j].joyPin) != 
        JSInput[j].state) {
      JSInput[j].state = !JSInput[j].state;
      if (JSInput[j].state)
      {
        Serial.print ("Releasing ");
        Serial.println (JSInput[j].desc);
        Keyboard.release(JSInput[j].key);
        digitalWrite(JSInput[j].ledPin, LOW);
      } else {
        Serial.print ("Pressing ");
        Serial.println (JSInput[j].desc);
        Keyboard.press(JSInput[j].key);
        digitalWrite(JSInput[j].ledPin, HIGH);
      }
    } // for
  delay(5);
} // loop
