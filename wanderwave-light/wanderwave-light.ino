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


//------------------------------------------------------------------
// Define our ledControl class
//------------------------------------------------------------------

class ledControl {

    // Private vars only used by the class
    private:
    unsigned long ledLastSwitch;
    
    unsigned long OnTime;
    unsigned long ledOffTime;
    unsigned long flashCount;
    unsigned long totalOnTime;
    unsigned long totalOffTime;
    unsigned long offTime;
    unsigned long curTime;

    // Public vars to be shared with the rest of the code
    public:
    unsigned int ledPin;
    char note;
    unsigned int ledState;
    unsigned int notePressed;
    

    // Constructor - creates an ledControl
    ledControl(int _pin, char _note, long _ledOnTime, long _flashCount ) {
      ledPin = _pin;
      pinMode(ledPin, OUTPUT);
      note = _note;
      ledState = LOW;
      notePressed = 0;
      ledLastSwitch = 0;
      OnTime = _ledOnTime;
      flashCount = _flashCount;
      curTime = 0;

      // Calculate the offTime
      totalOnTime = onTime * flashCount;
      totalOffTime = 1000 - totalOnTime;
      offTime = totalOffTime / flashCount;

    }

    // Function for switching on and off the light for this channel
    void flashSwitch() {
      curTime = millis();
      
      if (( ledState == LOW ) && ( curTime - ledLastSwitch >= offTime )) {
        ledState = HIGH;
        ledLastSwitch = curTime;
        digitalWrite(ledPin, ledState);
      } else if (( ledState == HIGH ) && ( curTime - ledLastSwitch >= onTime )) {
        ledState = LOW;
        ledLastSwitch = curTime;
        digitalWrite(ledPin, ledState);
      }
    }

    // Function to return the LED note
    void getNote() {
        return(note);
    }
    
}; // End definition of ledControl class

//------------------------------------------------------------------
// Configure each LED channel here
//------------------------------------------------------------------

// input vars are (pin, note, onTime, flashCount)
ledControl led1(10,'0',onTime, flashCount);
ledControl led2(11,'2',onTime, flashCount);
ledControl led3(12,'4',onTime, flashCount);
ledControl led4(13,'5',onTime, flashCount);

//------------------------------------------------------------------
// Leave below as is please!
//------------------------------------------------------------------

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity) {
    Serial.print(String(inNote));
    Serial.print("\n");
    if ( inNote == led1.note ) {
        led1.notePressed = 1; 
        Serial.print("led1 key has been pressed\n");
    } else if ( inNote == led2.note ) { 
        led2.notePressed = 1; 
        Serial.print("led2 key has been pressed\n");
    } else if ( inNote == led3.note ) { 
        led3.notePressed = 1; 
        Serial.print("led3 key has been pressed\n");
    } else if ( inNote == led4.note ) { 
        led4.notePressed = 1; 
        Serial.print("led4 key has been pressed\n");
    } 
} // End handleNoteOn

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity) {
  if ( inNote == led1.note ) {
    led1.notePressed = 0;
    led1.ledState = LOW;
    digitalWrite(led1.ledPin, led1.ledState);
  } else if ( inNote == led2.note ) { 
    led2.notePressed = 0;
    led2.ledState = LOW;
    digitalWrite(led2.ledPin, led2.ledState);
  } else if ( inNote == led3.note ) { 
    led3.notePressed = 0;
    led3.ledState = LOW;
    digitalWrite(led3.ledPin, led3.ledState);
  } else if ( inNote == led4.note ) { 
    led4.notePressed = 0;
    led4.ledState = LOW;
    digitalWrite(led4.ledPin, led4.ledState);
  }  
} // End handleNoteOff

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
    
} // End setup

void loop() {

    //------------------------------------------------------------------
  // Begin MIDI control
  //------------------------------------------------------------------

  MIDI.read();

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
  if ( led4.notePressed == 1 ) {
    led4.flashSwitch();
  }
}

