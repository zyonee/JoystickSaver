// Joystick Saver
// For the Arduino Pro Micro
//
// Original QuickShot Joystick to Keyboard Converter
// by Ido Gendel, 2014. Share and enjoy!

#include <Keyboard.h>

#define INPUTS (6)          // How many inputs from DB9 port
#define MOVES (20)          // How many moves for the auto movement

#define kLeftPin (16)       // DB9 LEFT pin
#define kRightPin (10)      // DB9 RIGHT pin
#define kUpPin (15)         // DB9 UP pin
#define kDownPin (14)       // DB9 DOWN pin
#define kFire1Pin (A0)      // DB9 FIRE1 pin
#define kFire2Pin (A1)      // DB9 FIRE2 pin

#define kModePin (9)        // Auto movement mode selector button
#define kSpeedDownPin (A2)  // Auto movement speed decrease button
#define kSpeedUpPin (A3)    // Auto movement speed increase button

#define kLEDLeftPin (8)     // Left LED pin
#define kLEDRightPin (7)    // Right LED pin
#define kLEDUpPin (6)       // Up LED pin
#define kLEDDownPin (5)     // Down LED pin
#define kLEDFire1Pin (4)    // Fire 1 pin
#define kLEDFire2Pin (3)    // Fire 2 pin

#define kKeyLeft (216)      // Cursor left keycode
#define kKeyRight (215)     // Cursor Right keycode
#define kKeyUp (218)        // Cursor Up keycode
#define kKeyDown (217)      // CursorDown keycode
#define kKeyRCtrl (132)     // Right Ctrl keycode
#define kKeyRAlt (134)      // Right Alt keycode

byte currentMode;           // Current auto movement mode
                            // 0=off, 1=left/right, 2=up/down, 3=clockwise, 4=counter clockwise

int currentSpeed = 0;       // Speed for auto movement

byte speedFactor = 1;       // Speed factor for auto movement. As clockwise and counter clockwise
                            // modes use twice as many moves as left/right and up/down, we need to
                            // divide the delay time to properly match the speed.

boolean leftLEDActive = 0;  // Is left LED active (for some reason, digitalRead() won't do the job.
boolean rightLEDActive = 0; // Is right LED active (for some reason, digitalRead() won't do the job.
boolean upLEDActive = 0;    // Is up LED active (for some reason, digitalRead() won't do the job.
boolean downLEDActive = 0;  // Is down LED active (for some reason, digitalRead() won't do the job.


struct tJSInput {
  byte joyPin;
  byte ledPin;
  boolean state;
  byte key;
  char desc[10];
} JSInput[INPUTS] = {{kLeftPin, kLEDLeftPin, 0, kKeyLeft, "left"}, 
                     {kRightPin, kLEDRightPin, 0, kKeyRight, "right"}, 
                     {kUpPin, kLEDUpPin, 0, kKeyUp, "up"}, 
                     {kDownPin, kLEDDownPin, 0, kKeyDown, "down"}, 
                     {kFire1Pin, kLEDFire1Pin, 0, kKeyRCtrl, "fire 1"}, 
                     {kFire2Pin, kLEDFire2Pin, 0, kKeyRAlt, "fire 2"}};

struct tAutoMoves {
  byte moveMode;
  boolean left;
  boolean right;
  boolean up;
  boolean down;
                   // Left/Right (0)
} AutoMoves[MOVES] = {{1, 1, 0, 0, 0}, 
                      {1, 0, 1, 0, 0}, 
                   // Up/Down (1)
                      {2, 0, 0, 1, 0}, 
                      {2, 0, 0, 0, 1}, 
                   // Clockwise (2)
                      {3, 1, 0, 0, 0}, 
                      {3, 1, 0, 1, 0}, 
                      {3, 0, 0, 1, 0}, 
                      {3, 0, 1, 1, 0}, 
                      {3, 0, 1, 0, 0}, 
                      {3, 0 ,1, 0, 1}, 
                      {3, 0, 0, 0, 1}, 
                      {3, 1, 0, 0, 1}, 
                   // Counter clockwise (3)
                      {4, 1, 0, 0, 0}, 
                      {4, 1, 0, 0, 1}, 
                      {4, 0, 0, 0, 1}, 
                      {4, 0, 1, 0, 1}, 
                      {4, 0, 1, 0, 0}, 
                      {4, 0, 1, 1, 0}, 
                      {4, 0, 0, 1, 0}, 
                      {4, 1, 0, 1, 0}}; 

void setup() {
  Serial.begin(9600);
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

/* void directionsOutput() {
  for (int j = 0; j < 4; j++) {
    pinMode(JSInput[j].joyPin, OUTPUT);
    JSInput[j].state = 
     digitalRead(JSInput[j].joyPin);
     digitalWrite(JSInput[j].ledPin, LOW);
  }  // for
}

void directionsInput() {
  for (int j = 0; j < 4; j++) {
    pinMode(JSInput[j].joyPin, INPUT_PULLUP);
    JSInput[j].state = 
     digitalRead(JSInput[j].joyPin);
     digitalWrite(JSInput[j].ledPin, LOW);
  }  // for
} */

void modeSwitch() {
  delay(250);
  switch (currentMode) {
    case 0:
      currentMode = 1; // Set to left/right
      speedFactor = 1;
      currentSpeed = 500;
      Serial.println("Setting mode 1");
      break;
    case 1:
      currentMode = 2; // Set to up/down
      speedFactor = 1;
      currentSpeed = 500;
      Serial.println("Setting mode 2");
      break;
    case 2:
      currentMode = 3; // Set to clockwise
      speedFactor = 2;
      currentSpeed = 500;
      Serial.println("Setting mode 3");
      break;
    case 3:
      currentMode = 4; // Set to counter clockwise
      speedFactor = 2;
      currentSpeed = 500;
      Serial.println("Setting mode 4");
      break;
    case 4:
      // Disable auto movements and speed setting
      currentMode = 0;
      currentSpeed = 0;
      // Reset all LEDs and cursor keys before exiting the loop
      digitalWrite(kLEDLeftPin, LOW); leftLEDActive = 0;
      Keyboard.release(kKeyLeft);
      digitalWrite(kLEDRightPin, LOW); rightLEDActive = 0;
      Keyboard.release(kKeyRight);
      digitalWrite(kLEDUpPin, LOW); upLEDActive = 0;
      Keyboard.release(kKeyUp);
      digitalWrite(kLEDDownPin, LOW); downLEDActive = 0;
      Keyboard.release(kKeyDown);
      Serial.println("Setting mode 0");
      return;
      break;
    default:
      break;
  }

  do {
//  directionsOutput();  
    // Read each movement. If it matches the current movement mode,
    // press the corresponding keys and light the corresponding LEDs.
    for (int j = 0; j < MOVES; j++) 
      if (AutoMoves[j].moveMode == currentMode)
      {
        if (leftLEDActive && !AutoMoves[j].left)
        {
          Keyboard.release(kKeyLeft);
          digitalWrite(kLEDLeftPin, LOW);
          leftLEDActive = 0;
        }
        if (rightLEDActive && !AutoMoves[j].right) {
          Keyboard.release(kKeyRight);
          digitalWrite(kLEDRightPin, LOW);
          rightLEDActive = 0;
        }
        if (upLEDActive && !AutoMoves[j].up) {
          Keyboard.release(kKeyUp);
          digitalWrite(kLEDUpPin, LOW);
          upLEDActive = 0;
        }
        if (downLEDActive && !AutoMoves[j].down) {
          Keyboard.release(kKeyDown);
          digitalWrite(kLEDDownPin, LOW);
          downLEDActive = 0;
        }
        if (AutoMoves[j].left)
        {
          Keyboard.press(kKeyLeft);
          digitalWrite(kLEDLeftPin, HIGH);
          leftLEDActive = 1;
          senseTimer();
          delay(5);
        }
        if (AutoMoves[j].right)
        {
          Keyboard.press(kKeyRight);
          digitalWrite(kLEDRightPin, HIGH);
          rightLEDActive = 1;
          senseTimer();
          delay(5);
        }
        if (AutoMoves[j].up)
        {
          Keyboard.press(kKeyUp);
          digitalWrite(kLEDUpPin, HIGH);
          upLEDActive = 1;
          senseTimer();
          delay(5);
        }
        if (AutoMoves[j].down)
        {
          Keyboard.press(kKeyDown);
          digitalWrite(kLEDDownPin, HIGH);
          downLEDActive = 1;
          senseTimer();
          delay(5);
        }

      } // for

  } while (!currentMode==0 && digitalRead(kFire1Pin) && digitalRead(kFire2Pin) && digitalRead(kModePin));

//  directionsInput();
  
      // Reset speed and key presses upon auto movement exit
      currentSpeed = 0;
      Keyboard.release(kKeyLeft);
      Keyboard.release(kKeyRight);
      Keyboard.release(kKeyUp);
      Keyboard.release(kKeyDown);
}

void speedUp() {
  switch (currentSpeed) {
    case 500:
      currentSpeed = 400;
      Serial.println("Setting move duration to 400");
      break;
    case 400:
      currentSpeed = 300;
      Serial.println("Setting move duration to 300");
      break;
    case 300:
      currentSpeed = 200;
      Serial.println("Setting move duration to 200");
      break;
    case 200:
      currentSpeed = 100;
      Serial.println("Setting move duration to 100");
      break;
    case 100:
      currentSpeed = 50;
      Serial.println("Setting move duration to 50");
      break;
    case 50:
      Serial.println("Duration already at 50, doing nothing");
    default:
      break;
  }
  delay(175);
}

void speedDown() {
  switch(currentSpeed ) {
    case (50):
      currentSpeed = 100;
      Serial.println("Setting move duration to 100");
      break;
    case 100:
      currentSpeed = 200;
      Serial.println("Setting move duration to 200");
      break;
    case 200:
      currentSpeed = 300;
      Serial.println("Setting move duration to 300");
      break;
    case 300:
      currentSpeed = 400;
      Serial.println("Setting move duration to 400");
      break;
    case 400:
      currentSpeed = 500;
      Serial.println("Setting move duration to 500");
      break;
    case 500:
      Serial.println("Duration already at 500, doing nothing");
    default:
      break;
  }
  delay(175);
}

void senseTimer() {
  unsigned long loopStartTime = millis();
  do {
       // Check for speed or mode change requests
       if (!digitalRead(kSpeedUpPin))
       {
         Serial.println("Speed up pressed");
         speedUp();
       }
       if (!digitalRead(kSpeedDownPin))
       {
         Serial.println("Speed down pressed");
         speedDown();
       }
       if (!digitalRead(kModePin))
       {
         Serial.println ("Mode switch pressed");
         modeSwitch();
       }
     } while(millis() < (loopStartTime + (currentSpeed/speedFactor)));
}

void loop() {
  if (!digitalRead(kModePin))
  {
    Serial.println ("Mode switch pressed");
    modeSwitch();
  }

  // Read joystick ports, press corresponding keys and set corresponding LEDs
  for (int j = 0; j < INPUTS; j++) 
    if (digitalRead(JSInput[j].joyPin) != JSInput[j].state) {
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
