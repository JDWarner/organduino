#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <midi_Message.h>
#include <MIDI.h>
/*
 Test serial-MIDI commands from a 61-key keyboard.

 The circuit:
 * Single OrganDuino 2.0 PCB shield, populated with diodes
 * Solder jumper on back for rank 0 bridged
 * 61-key organ manual soldered in with buss soldered to a Drive Pin thru-hole

 created 2016 by @JDWarner
*/

#define keyON 127
#define keyOFF 0
#define no false

// Initialize MIDI
MIDI_CREATE_DEFAULT_INSTANCE();

// constants won't change. They're used here to
// set pin numbers for first 2 octaves
const int keyPins[] = {28, 26, 24, 22, 13, 12, 11, 10, 9, 8, 7, 6, 14, 15, 16, 17, 18, 19, 20, 21, 27, 25, 23, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30,};
const int rankPins[] = {5, 4, }; // drive wire(s) for ranks are: {5, 4, 3, 2, 67, and 69 (pedals)}
const int pedalPin = 69;
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
const int numKeys = 61;    // full organ manual, also length of keyPins and midiKeyNotes
const int numPedals = 32;  // full AGO 32-key pedalboard
const int numManuals = 2;  // number of manuals

const unsigned char midiKeyNotes[numKeys] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, };
const unsigned char midiChannels[numManuals] = {1, 2, };  // Five manuals plus pedals (pedals always rank 16)
const unsigned char pedalChannel = 16;

// State of all keys in a 2D multidimensional array
bool keyStates[numManuals][numKeys] = {
  {no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, },
  {no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, },
};

// State of all pedals in a 1D array
bool pedalStates[numPedals] = {no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, };


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
void keyChange(bool value, bool &state, int key, int channel) {
  // Compare them - `state` records inverse of switch read
  if (value == state) { // Equal means a change

    // Update values in state record
    state = not value;

    // If there is a state change, send a MIDI event
    MIDI.sendNoteOn(midiKeyNotes[key], stateMIDIvalue(state), channel);
  }
}


// function to handle the loop over a given rank, updating states
void keyPoll(const int keyPins[], bool (&states)[numKeys], int arraySize, int rank) {
  for(int key=0; key<arraySize; key++){
      // run through all switches, reading their values
      // pass results to keyChange which will dispatch a MIDI event if needed
      keyChange(digitalRead(keyPins[key]), states[key], key, midiChannels[rank]);
  }
}

// function to loop over pedals
void pedalPoll(const int pollPin, bool (&states)[numPedals], int arraySize, int pedalMidiChannel) {
  for(int key=0; key<arraySize; key++){
      // run through all switches, reading their values
      // pass results to keyChange which will dispatch a MIDI event if needed
      keyChange(digitalRead(pollPin), states[key], key, pedalMidiChannel);
  }
}


void setup() {
  // Start the Serial interface
  MIDI.begin(1);
  Serial.begin(115200);

  // drive pins per rank
  for(int d=0; d<numManuals; d++){
      pinMode(rankPins[d], OUTPUT);
      digitalWrite(rankPins[d], HIGH);
  }
  // initialize the organ keys' pins as inputs
  for(int k=0; k<numKeys; k++){
      pinMode(keyPins[k], INPUT_PULLUP);
  }
}


void loop() {
  // read manuals
  for(int manual=0; manual<numManuals; manual++){
      // pull the drive pin for this manual to GND
      digitalWrite(rankPins[manual], LOW);

      // poll all keys, sending MIDI events if warranted
      keyPoll(keyPins, keyStates[manual], numKeys, manual);

      // return the drive pin to +5V (off)
      digitalWrite(rankPins[manual], HIGH);
  }

  // read the pedals
  digitalWrite(pedalPin, LOW);
  pedalPoll(pedalPin, pedalStates, numPedals, pedalChannel);
  digitalWrite(pedalPin, HIGH);
}
