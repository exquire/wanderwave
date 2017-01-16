// Flappy Hoses program, built peice by peice

// Include libraries
// Mozzi Libs
#include <MozziGuts.h> // This disables millis(), so need to uses mozziMicros() instead
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

//Midi Libs
#include <MIDI.h>
#include "pitches.h"
MIDI_CREATE_DEFAULT_INSTANCE();

// Set required const at the beginning
const int ledPin = 13; // The number of the pin for the LED

Oscil <2048, AUDIO_RATE> aSin(SIN2048_DATA); // Set our Oscil up for sinewaving
#define CONTROL_RATE 64 // Set our control rate (update frequency) for sinewaving. Must be power of 2 number

// Time in microseconds for the LED to be on and off. 
// Keep in mind 1 microsecond is 0.001 milliseconds, and 1000 milliseconds = 1 second
// My default is 500 microseconds for onTime and 39,500 for offTime
// These defaults give us the same as a video camera would do with 1/2000th shutter at 25p
const long onTime = 500; // Time in microseconds that the LED should be on
const long offTime = 1000000000; // Time in microseconds that the LED should be off


// Set required vars at the beginning
int ledState = LOW; // ledState used to control the LED.
unsigned long curTime = 0;
unsigned long lastSwitch = 0; // Used to store the last time the LED was status changed


void setup() {
  // put your setup code here, to run once:
    pinMode(ledPin, OUTPUT); // set the digital pin as output

    startMozzi(CONTROL_RATE); // set a control rate of 64
    int frequency = 25; // Initialises a variable to 25, we'll change it with keyboard presses

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin();

    //Serial.begin(9600);
    Serial.print("Starting output of midi signals...\n");

    

}

void updateControl() {
      // We'll put changing controls for sinewaving in here, apparently.
    }

int updateAudio() {
  return aSin.next(); // return an int signal for sinewaving centered around 0
}

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{
    int frequency = sNotePitches[inNote];
    aSin.setFreq(frequency); // Sets the frequency to be dependent on the note we press.
    Serial.print(String(inNote) + "\n");  
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
  Serial.print(String(inNote) + "\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  //------------------------------------------------------------------
  // Begin LED control
  //------------------------------------------------------------------

  
  // Here is my re-write to change the style to having a different time for on and off
  // We first need to determine if the LED is currently on or off and then decide if we need to switch
  curTime = mozziMicros(); // Set a time var for us to work with, keeps things a little cleaner

  
//  if (ledState == LOW) { // if led is off
//    if (curTime - lastSwitch >= offTime) { // time the light should be off
//      lastSwitch = curTime; // Reset the previous switch counter to right now
//      digitalWrite(ledPin, HIGH); // Turn on the light
//      ledState = HIGH; // Set the var to what we now have
//      
//    }
//  } 
//    if (ledState == HIGH) { // if led is on
//    if (curTime - lastSwitch >= onTime) { // time the light should be on
//      lastSwitch = curTime; // Reset the previous switch counter to right now
//      digitalWrite(ledPin, LOW);  // Turn off the light
//      ledState = LOW; // Set the var to what we now have
      
//    }
//  }

  //------------------------------------------------------------------
  // End LED control
  //------------------------------------------------------------------

  //------------------------------------------------------------------
  // Begin sinewave control
  //------------------------------------------------------------------

  audioHook(); // Runs our sinewave generation

  //------------------------------------------------------------------
  // End sinewave control
  //------------------------------------------------------------------

  //------------------------------------------------------------------
  // Begin MIDI control
  //------------------------------------------------------------------

  MIDI.read();

  //------------------------------------------------------------------
  // End MIDI control
  //------------------------------------------------------------------
}
