/*
 Test pseudo-MIDI commands from a 3x2 key matrix.

 Turns on a LED when at least one key is pressed.

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
const int keyPins[] = {8, 9, 10};  // organ keys C, C#, D
const int rankPins[] = {69, 68};   // drive wires for rank 0 (top) and 1 (bottom)
const int ledPin = LED_BUILTIN;  // LED pin
/*
  Note for reference, the analog -> digital pin mappings for the Mega are:
  A0 = 54
  A1 = 55
  A2 = 56
  A3 = 57
  A4 = 58
  A5 = 59
  A6 = 60
  A7 = 61
  A8 = 62
  A9 = 63
  A10 = 64
  A11 = 65
  A12 = 66
  A13 = 67
  A14 = 68
  A15 = 69
*/

// define how large some things are (matrix size)
const int numKeys = 3;
const int numRanks = 2;
const int midiNotes[numRanks][numKeys] = {
  {9, 10, 11},
  {9, 10, 11},
};
const int midiChannels[numRanks] = {0, 1};

// State of all keys in a 2D multidimensional array
bool keyStates[numRanks][numKeys] = {
  {false,  false,  false},
  {false,  false,  false},
};

// Reading all pins for a particular loop
bool keyReads[] = {false, false, false};
int activeKeys = 0;


// simple function to compare current state with new read
// if different, state and LED are updated
// We're using input_pullup so the switch is active when LOW!
// Note: Internally changing `state` requires call-by-reference
void pinChange(bool value, bool &state, int pin) {
  // Compare them - `state` records inverse of switch read
  if (value == state) { // Equal means a change

    // Update values in state record
    state = not value;

    // If there is a state change, send a MIDI event
    // Serial.write();
    // Serial.write();
    // Serial.write();
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
  // Start the Serial interface
  // Serial.begin(31280)  // Check baud

  // initialize LED
  pinMode(ledPin, OUTPUT);

  // drive pins per rank
  for(int d=0; d<numRanks; d++){
      pinMode(d, OUTPUT);
      digitalWrite(d, HIGH);
  }
  // initialize the organ keys' pins as inputs
  for(int k=0; k<numKeys; k++){
      pinMode(k, INPUT_PULLUP);
  }
}


void loop() {
  // read the state of the key value:
  for(int rank = 0; rank < numRanks; rank++){
      digitalWrite(rankPins[rank], LOW);
      rankPoll(keyPins, keyStates[rank]);
      digitalWrite(rankPins[rank], HIGH);
   }

  // light an LED if any key is pressed
  activeKeys = 0;
  for(int rank=0; rank<numRanks; rank++){
      // shortcut loop if any key press found
      if(activeKeys > 0){
          break;
      }
      for(int key=0; key<numKeys; key++){
          // if key pressed, turn on LED, record, and stop looking
          if (keyStates[rank][key] == true) {
            digitalWrite(ledPin, HIGH);
            activeKeys++;
            break;
          }
      }
  }

  // LED off if no keys pressed
  if(activeKeys == 0){
      digitalWrite(ledPin, LOW);
  }
}
