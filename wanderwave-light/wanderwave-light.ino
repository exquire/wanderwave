// Flappy Hoses light program

//Midi Libs
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

//------------------------------------------------------------------
// LED configuration
//------------------------------------------------------------------

// Set the number of times for the lights to flash per second
unsigned int flashCount = 20;
// And the duraction (milliseconds) that each flash on should be
// 1 second = 1,000 milliseconds
const long onTime = 20; 

// Automatic calculation of the offTime based on the above
unsigned long totalOnTime = onTime * flashCount;
unsigned long totalOffTime = 1000 - totalOnTime;
unsigned long offTime = totalOffTime / flashCount;


//------------------------------------------------------------------
// Define our ledControl class
//------------------------------------------------------------------

class ledControl {

    // Class that controls the state of the LEDs
    // Doesn't handle timing any more as this needs to be global

    // Private vars to be only used here
    unsigned int ledPin;

    // Public vars to be shared with the rest of the code
    public:
    char note;
    unsigned int notePressed;
    

    // Constructor - creates an ledControl
    ledControl(int _pin, char _note ) {
      ledPin = _pin;
      pinMode(ledPin, OUTPUT);
      note = _note;
      notePressed = 0;
    }

    // Function to switch state as input
    void switchLedState(unsigned int _state) {
      digitalWrite(ledPin,_state);
    }
    
}; // End definition of ledControl class

//------------------------------------------------------------------
// Configure each LED channel here
//------------------------------------------------------------------

// input vars are (pin, note)
ledControl led1(10,'0');
ledControl led2(11,'2');
ledControl led3(12,'4');
ledControl led4(13,'5');

//------------------------------------------------------------------
// Leave below as is please!
//------------------------------------------------------------------

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity) {
    Serial.print(String(inNote));
    Serial.print("\n");
    if ( inNote == led1.note ) {
        led1.notePressed = 1; 
        Serial.write("led1 key pressed\n");
    } else if ( inNote == led2.note ) { 
        led2.notePressed = 1; 
        Serial.write("led2 key pressed\n");
    } else if ( inNote == led3.note ) { 
        led3.notePressed = 1; 
        Serial.write("led3 key pressed\n");
    } else if ( inNote == led4.note ) { 
        led4.notePressed = 1; 
        Serial.write("led4 key pressed\n");
    } 
} // End handleNoteOn

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity) {
  if ( inNote == led1.note ) {
    led1.notePressed = 0;
    led1.switchLedState(LOW);
    Serial.write("led1 key released\n");
  } else if ( inNote == led2.note ) { 
    led2.notePressed = 0;
    led2.switchLedState(LOW);
    Serial.write("led2 key released\n");
  } else if ( inNote == led3.note ) { 
    led3.notePressed = 0;
    led3.switchLedState(LOW);
    Serial.write("led3 key released\n");
  } else if ( inNote == led4.note ) { 
    led4.notePressed = 0;
    led4.switchLedState(LOW);
    Serial.write("led4 key released\n");
  }  
} // End handleNoteOff

// Init the vars for global LED control
unsigned int globalLedState = LOW; // Start with our state as lights off
unsigned long ledLastSwitch = 0;
unsigned long curTime = 0;


// Setup function
void setup() {
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin();

    // Note: Set your serial baud rate to 31250 as this has to match midi...
    Serial.print("Starting output of midi signals...\n");
    Serial.print("led1 note: ");
    Serial.print(led1.note);
    Serial.print("\n led2 note: ");
    Serial.print(led2.note);
    Serial.print("\n led3 note: ");
    Serial.print(led3.note);
    Serial.print("\n led4 note: ");
    Serial.print(led4.note);
    Serial.print("\n");

    // Make sure the lights are off when we start:
    led1.notePressed = 0;
    led2.notePressed = 0;
    led3.notePressed = 0;
    led4.notePressed = 0;
    led1.switchLedState(LOW);
    led2.switchLedState(LOW);
    led3.switchLedState(LOW);
    led4.switchLedState(LOW);
    
} // End setup

// Main loop function
void loop() {

    //------------------------------------------------------------------
  // Begin MIDI control
  //------------------------------------------------------------------

  MIDI.read();

  //------------------------------------------------------------------
  // Begin LED control
  //------------------------------------------------------------------

    curTime = millis();      
    if (( globalLedState == LOW ) && ( curTime - ledLastSwitch >= offTime )) {
      globalLedState = HIGH;
      ledLastSwitch = curTime;
      if ( led1.notePressed = 1 ) {
        led1.switchLedState(HIGH);
      }
      if ( led2.notePressed = 1 ) {
        led2.switchLedState(HIGH);
      }
      if ( led3.notePressed = 1 ) {
        led3.switchLedState(HIGH);
      }
      if ( led4.notePressed = 1 ) {
        led4.switchLedState(HIGH);
      }
    }

    if (( globalLedState == HIGH) && ( curTime - ledLastSwitch >= onTime )) {
      globalLedState = LOW;
      ledLastSwitch = curTime;
      if ( led1.notePressed = 1 ) {
        led1.switchLedState(LOW);
      }
      if ( led2.notePressed = 1 ) {
        led2.switchLedState(LOW);
      }
      if ( led3.notePressed = 1 ) {
        led3.switchLedState(LOW);
      }
      if ( led4.notePressed = 1 ) {
        led4.switchLedState(LOW);
      }
    }
}

