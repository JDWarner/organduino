/*
 Test a six-key matrix with 3 keys in two ranks.

 Turns on a LED when ANY two keys are pressed simultaneously.

 The circuit:
 * LED built in to pin 13, connected to ground
 * Key matrix: C, C#, and D from two keyboards
   * C into digital pin 8
   * C# into digital pin 9
   * D into digital pin 10
   * Internal pull-up resistors activated for all inputs
 * Keyboard 0 - pin 69 (analog 15)
 * Keyboard 1 - pin 68 (analog 14)
 * Six diodes allowing facing away from the inputs (allowing
   current to be sunk, and hence inputs pulled down,
   but open circuit otherwise)

 created 2016 by @JDWarner
*/

// constants won't change. They're used here to
// set pin numbers:
const int keyPin0 = 8;           // pin from organ key C
const int keyPin1 = 9;           // pin from organ key C#
const int keyPin2 = 10;          // pin from organ key D
const int ledPin = LED_BUILTIN;  // LED pin
const int drivePin0 = 69;        // Drive pin for rank 0
const int drivePin1 = 68;        // Drive pin for rank 1

// define how large some things are (matrix size)
const int numKeys = 3;
const int numRanks = 2;

// State of all keys in a 2D multidimensional array
bool keyStates[numRanks][numKeys] = {
  {false,  false,  false},
  {false,  false,  false},
};

// Reading of all keys for a particular loop
bool keyReads[] = {false, false, false};
int keyPins[] = {8, 9, 10};
int rankPins[] = {69, 68};
int activeKeys = 0;

// simple function to compare current state with new read
// if different, state and LED are updated
// We're using input_pullup so the switch is active when LOW!
// Note: Internally changing `state` requires call-by-reference
void pinChange(bool value, bool &state, int pin) {
  // Compare them - `state` records inverse of switch read
  if (value == state) { // Equal means a change

    // Update values as needed, if needed
    state = not value;

    // Do something here - MIDI event, or turn on LED with
    // digitalWrite(ledPin, state);
  }
}


// function to handle the loop over a given rank, updating states
void rankPoll(int keyPins[numKeys], bool (&states)[numKeys]) {
  for(int key=0; key<numKeys; key++){
      // states[key] = not digitalRead(keyPins[key]);
      pinChange(digitalRead(keyPins[key]), states[key], keyPins[key]);
  }
}


void setup() {
  // initialize the LED pin and drivers as outputs
  pinMode(ledPin, OUTPUT);
  pinMode(drivePin0, OUTPUT);
  pinMode(drivePin1, OUTPUT);

  // start driver pins HIGH (off)
  digitalWrite(drivePin0, HIGH);
  digitalWrite(drivePin1, HIGH);

  // initialize the organ keys' pins as inputs
  pinMode(keyPin0, INPUT_PULLUP);
  pinMode(keyPin1, INPUT_PULLUP);
  pinMode(keyPin2, INPUT_PULLUP);
}


void loop() {
  // read the state of the key value:
  for(int rank = 0; rank < numRanks; rank++){
      digitalWrite(rankPins[rank], LOW);
      rankPoll(keyPins, keyStates[rank]);
      digitalWrite(rankPins[rank], HIGH);
   }

  // sum up total active keys
  activeKeys = 0;
  for(int rank=0; rank<numRanks; rank++){
      for(int key=0; key<numKeys; key++){
          if (keyStates[rank][key] == true) {
            activeKeys++;
          }
      }
  }

  // Activate LED if correct # of keys pressed
  if(activeKeys == 2){
      digitalWrite(ledPin, HIGH);
  } else {
      digitalWrite(ledPin, LOW);
  }
}
