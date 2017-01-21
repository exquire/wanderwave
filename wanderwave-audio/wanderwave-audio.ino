// Flappy Hoses audio program

// Include libraries
// Mozzi Libs & setup
#include <MozziGuts.h> // This disables millis(), so need to uses mozziMicros() instead
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
Oscil <2048, AUDIO_RATE> aSin(SIN2048_DATA); // Set our Oscil up for sinewaving
#define CONTROL_RATE 64 // Set our control rate (update frequency) for sinewaving. Must be power of 2 number

//Midi Libs
#include <MIDI.h>
#include "pitches.h"
MIDI_CREATE_DEFAULT_INSTANCE();

int notesOn = 0; // Keep track of how many notes are current on

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity) {
    if ( inNote == '0' ) {
        //
    } else if ( inNote == '2' ) { 
       //
    } else if ( inNote == '4' ) { 
        //
    } else if ( inNote == '5' ) { 
        //
    } else { // Else control frequency generated
    int frequency = sNotePitches[inNote];
    aSin.setFreq(frequency); // Sets the frequency to be dependent on the note we press.
    notesOn++; // Add 1 to our track of how many keys are pressed - Lights don't count
    }
} // End handleNoteOn

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity) {
  if ( inNote == '0' ) {
    //
  } else if ( inNote == '2' ) { 
    //
  } else if ( inNote == '4' ) { 
    //
  } else if ( inNote == '5' ) { 
    //
  } else {
  notesOn-- ; // Decrease our track of how many keys are pressed by one - Lights don't count
  if ( notesOn == 0 ) {
  int frequency = 0; // Set the frequency to 0Hz
  aSin.setFreq(frequency); 
    }
  }
} // End handleNoteOff

void setup() {
    startMozzi(CONTROL_RATE); // set a control rate of 64
    int frequency = 0; // Initialises a variable to 0, we'll change it with keyboard presses

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin();

    // Note: Set your serial baud rate to 31250 as this has to match midi...
    Serial.print("Starting output of midi signals...\n");
    
} // End setup

void updateControl() {
      // This part is apparently needed by Mozzi
    }

int updateAudio() {
  return aSin.next(); // return an int signal for sinewaving centered around 0
}



void loop() {

    //------------------------------------------------------------------
  // Begin MIDI control
  //------------------------------------------------------------------

  MIDI.read();

  //------------------------------------------------------------------
  // Begin sinewave control
  //------------------------------------------------------------------

  audioHook(); 

}
