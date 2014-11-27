// QuickShot Joystick to Keyboard Converter
// For the Arduino Leonardo
// by Ido Gendel, 2014. Share and enjoy!
// 5 inputs: Up, Down, Left, Right and Fire
#define INPUTS 5

struct tJSInput {
  byte pin;
  boolean state;
  byte key;
} JSInput[INPUTS] = {{8,  0, 232}, 
                     {9,  0, 226}, 
                     {10, 0, 228}, 
                     {11, 0, 230}, 
                     {12, 0, 229}};

void setup() {
  pinMode(13, OUTPUT);
  for (int j = 0; j < INPUTS; j++) {
    pinMode(JSInput[j].pin, INPUT_PULLUP);
    JSInput[j].state = 
     digitalRead(JSInput[j].pin);
  }  // for
 
 // Time for re-programming in case of trouble
 delay(4000);
 digitalWrite(13, HIGH); // "active" Indication
 Keyboard.begin();
} // setup

void loop() {
  for (int j = 0; j < INPUTS; j++) 
    if (digitalRead(JSInput[j].pin) != 
        JSInput[j].state) {
      JSInput[j].state = !JSInput[j].state;
      if (JSInput[j].state)
        Keyboard.release(JSInput[j].key);
       else Keyboard.press(JSInput[j].key);
    } // for
  delay(5);
} // loop
