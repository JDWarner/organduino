/*
  key

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a key attached to pin 2.

 This is 


 The circuit:
 * LED built in to pin 13, connected to ground
 * Organ key attached to pin 2 from GND, through a 10k resistor
 * Pin 2 internal pull-up resistor activated

 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 modified 03 May 2016
 by @JDWarner
 */

// constants won't change. They're used here to
// set pin numbers:
const int keyPin = 2;          // pin connected to organ key
const int ledPin =  LED_BUILTIN;  // the number of the LED pin
bool keyState = false;    // the key state, starts off
bool keyRead = false;     // input from digital pin


// simple function to compare current state with new read
// if different, state and LED are updated
// We're using input_pullup so the switch is active when LOW!
void pinchange(bool value, bool state, int pin) {
  // Compare them - state records inverse of switch read
  if (value == state) { // Equal means a change
    // Update values as needed, if needed
    value = not state;
    digitalWrite(ledPin, value);
  }
}

void setup() {
  // initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  // initialize the organ key's pin as an input
  pinMode(keyPin, INPUT_PULLUP);
}

void loop() {
  // read the state of the key value:
  keyRead = digitalRead(keyPin);

  // act on it
  pinchange(keyRead, keyState, keyPin);
}
