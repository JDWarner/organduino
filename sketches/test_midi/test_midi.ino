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

#define keyON 127
#define keyOFF 0

// constants won't change. They're used here to
// set pin numbers for first 2 octaves
const int keyPins[] = {28, 26, 24, 22, 13, 12, 11, 10, 9, 8, 7, 6, 14, 15, 16, 17, 18, 19, 20, 21, 27, 25, 23, 29,};
const int rankPins[] = {5, }; // drive wire(s) for ranks
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
const int numKeys = 24;  // first 2 octaves
const int numRanks = 1;  // only one rank for now

const int midiNotes[numRanks][numKeys] = {
  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,},
  // {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,},
};
const char midiNoteNames[numRanks][numKeys] = {
  {'C2', 'C#2', 'D2', 'D#2', 'E2', 'F2', 'F#2', 'G2', 'G#2', 'A2', 'Bb2', 'B2', 'C3', 'C#3', 'D3', 'D#3', 'E3', 'F3', 'F#3', 'G3', 'G#3', 'A3', 'Bb3', 'B3',},
  // {'C2', 'C#2', 'D2', 'D#2', 'E2', 'F2', 'F#2', 'G2', 'G#2', 'A2', 'Bb2', 'B2', 'C3', 'C#3', 'D3', 'D#3', 'E3', 'F3', 'F#3', 'G3', 'G#3', 'A3', 'Bb3', 'B3',},
};
const int midiChannels[numRanks] = {0x90, }; // 0x91, 0x92, 0x93, 0x94, 0x95};  // Five manuals plus pedals

// State of all keys in a 2D multidimensional array
bool keyStates[numRanks][numKeys] = {
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,},
  // {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,},
};


// Convenience func to convert a boolean to the proper MIDI key velocity
int stateMIDIvalue(bool state) {
  if (state == true) {
    return keyON;
  }
  else {
    return keyOFF;
  }
}


// simple function to compare current state with new read
// if different, state and LED are updated
// We're using input_pullup so the switch is active when LOW!
// Note: Internally changing `state` requires call-by-reference
void pinChange(bool value, bool &state, int key, int channel) {
  // Compare them - `state` records inverse of switch read
  if (value == state) { // Equal means a change

    // Update values in state record
    state = not value;

    // If there is a state change, send a MIDI event
    Serial.write(channel);
    Serial.write(stateMIDIvalue(state));
    Serial.write(midiNoteNames[key]);
  }
}


// function to handle the loop over a given rank, updating states
void rankPoll(const int keyPins[numKeys], bool (&states)[numKeys], int rank) {
  for(int key=0; key<numKeys; key++){
      // states[key] = not digitalRead(keyPins[key]);
      pinChange(digitalRead(keyPins[key]), states[key], keyPins[key], midiChannels[rank]);
  }
}


void setup() {
  // Start the Serial interface
  // Serial.begin(31280)  // Check baud

  Serial.begin(31250);

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
  // read each manual:
  for(int rank = 0; rank < numRanks; rank++){
      // pull the drive pin for this manual to GND
      digitalWrite(rankPins[rank], LOW);

      // poll all keys, sending MIDI events if warranted
      rankPoll(keyPins, keyStates[rank], rank);

      // return the drive pin to +5V (off)
      digitalWrite(rankPins[rank], HIGH);
  }

  // read the pedals
  // digitalWrite(pedalPin, LOW);
  // rankPoll(pedalPins, rank, keyStates[rank]);
  // digitalWrite(pedalPin, HIGH);
}
