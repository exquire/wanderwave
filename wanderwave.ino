// Flappy Hoses program. Also know as wanderwave 

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

//------------------------------------------------------------------
// LED configuration
//------------------------------------------------------------------

// Set the number of times for the lights to flash per second
// And the duration (microseconds) that each flash on should be.
// 1 second = 1,000,000 microseconds. Minimum duration is 64 microseconds
// For exampe, onTime = 2000 would be 2 miliseconds
const long onTime = 2000; 
unsigned int flashCount = 20;

// Calculate our flash offTime
    // First work out duration of on per second
    unsigned long totalOnTime = onTime * flashCount;
    unsigned long totalOffTime = 1000000 - totalOnTime;
    unsigned long offTime = totalOffTime / flashCount;

//------------------------------------------------------------------
// Define our ledControl class
//------------------------------------------------------------------

class ledControl {

    // Class member vars
    // Initialised at startup
    int ledPin;
    char note;
    long ledOnTime;
    long ledOffTime;
    // Maintain current state
    int ledState;
    //int notePressed;
    unsigned long ledLastSwitch;

    // Constructor - creates an ledControl
    public:
    ledControl(int _pin, char _note, long _ledOnTime, long _ledOffTime) {
      ledPin = _pin;
      pinMode(ledPin, OUTPUT);
      note = _note;
      ledState = LOW;
      notePressed = 0;
      ledLastSwitch = 0;
      ledOnTime = _ledOnTime;
      ledOffTime = _ledOffTime;
    }

    void flashSwitch() {
      unsigned long curTime = mozziMicros();
      
      if (( ledState == LOW ) && ( curTime - ledLastSwitch >= ledOffTime )) {
        ledState = HIGH;
        ledLastSwitch = curTime;
        digitalWrite(ledPin, ledState);
      } else if (( ledState == HIGH ) && ( curTime - ledLastSwitch >= ledOnTime )) {
        ledState = LOW;
        ledLastSwitch = curTime;
        digitalWrite(ledPin, ledState);
      }
    }
};

//------------------------------------------------------------------
// Configure each LED channel here
//------------------------------------------------------------------

// input vars are pin, note, onTime, offTime
ledControl led1(10,'NOTE_C2',onTime, offTime);
ledControl led2(11,'NOTE_D2',onTime, offTime);
ledControl led3(12,'NOTE_E2',onTime, offTime);
ledControl led4(13,'NOTE_F2',onTime, offTime);

//------------------------------------------------------------------
// Leave below as is please!
//------------------------------------------------------------------

int notesOn = 0; // Keep track of how many notes are current on

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity) {
    if ( inNote == led1.note ) {// Turn on LED1
        led1.notePressed = 1;
    } else if ( inNote == led2.note ) { // Turn on LED2
        led2.notePressed = 1;
    } else if ( inNote == led3.note ) { // Turn on LED3
        led3.notePressed = 1;
    } else if ( inNote == led4.note ) { // Turn on LED4
        led4.notePressed = 1;
    } else { // Else control frequency generated
    int frequency = sNotePitches[inNote];
    aSin.setFreq(frequency); // Sets the frequency to be dependent on the note we press.
    //Serial.print(String(inNote) + "\n");  
    notesOn++; // Add 1 to our track of how many keys are pressed
    //digitalWrite(led1Pin, HIGH); // Turn on the light
    }
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity) {
  if ( inNote == led1.ledNote ) { // Turn off LED1
    led1.notePressed = 0;
    led1.ledState = LOW;
  } else if ( inNote == led2.note ) { // Turn off LED2
    led2.notePressed = 0;
    led2.ledState = LOW;
  } else if ( inNote == led3.note ) { // Turn off LED3
    led3.notePressed = 0;
    led3.ledState = LOW;
  } else if ( inNote == led4.note ) { // Turn off LED4
    led4.notePressed = 0;
    led4.ledState = LOW;
  } else {
  //Serial.print(String(inNote) + "\n");
  notesOn-- ; // Decrease our track of how many keys are pressed by one
  if ( notesOn == 0 ) {
  int frequency = 0; // Set the frequency to cheating off
  aSin.setFreq(frequency);  
  //digitalWrite(led1Pin, LOW); // Turn off the light
    }
  }
}

void setup() {
//    pinMode(led1Pin, OUTPUT); // set the digital pin as output for LED1
//    pinMode(led2Pin, OUTPUT); // set the digital pin as output for LED2
//    pinMode(led3Pin, OUTPUT); // set the digital pin as output for LED3
//    pinMode(led4Pin, OUTPUT); // set the digital pin as output for LED4

    startMozzi(CONTROL_RATE); // set a control rate of 64
    int frequency = 0; // Initialises a variable to 0, we'll change it with keyboard presses

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin();

    //Serial.begin(9600);
    Serial.print("Starting output of midi signals...\n");

    

   
}

void updateControl() {
      // This part is apparently needed by Mozzi
    }

int updateAudio() {
  return aSin.next(); // return an int signal for sinewaving centered around 0
}




void loop() {
  // put your main code here, to run repeatedly:

  //------------------------------------------------------------------
  // Begin LED control
  //------------------------------------------------------------------

  if ( led1.notePressed == 1 ) {
    led1.flashSwitch();
  } 
  if ( led2.notePressed == 1 ) {
    led2.flashSwitch();
  } 
  if ( led3.notePressed == 1 ) {
    led3.flashSwitch();
  } 
  if ( led4.notePRessed == 1 ) {
    led4.flashSwitch();
  }

  //------------------------------------------------------------------
  // Begin sinewave control
  //------------------------------------------------------------------

  audioHook(); // Runs our sinewave generation

  //------------------------------------------------------------------
  // Begin MIDI control
  //------------------------------------------------------------------

  MIDI.read();

}
