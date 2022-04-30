/* USB MIDI pedal scanner
 *  
 *  From "Tools", pick the appropriate Teensy board.  The board needs to be 3.6 or 4.1 or similar larger pinout version
 *  
 *  From "Tools", pick USB Type "Serial+MIDI"
 *  
 *  Common rail on all pedal switches is ground
 *  This may not work very well because it relies on pullup resistors in the device, which are generally weak.  Better would be to use a PCB that has external pullup resistors and series protection resistors on it
 *  
 *  Low C1 (MIDI 36) connects to input 0, C#1 to Input 1, etc, through input 31 for high G3.`
 *  
 *  MIT license
 */
#include <MIDI.h>

int MIDIChannel=12; 
const int NoteBase=36;
unsigned long CurrentState=0;
unsigned long PreviousState=0;
int LoopDelay=10;


void setup() {
  Serial.begin(115200);

  Serial.println("Setup...");
  

  for (int i=0;i<=31;i++){
    pinMode(i,INPUT_PULLUP);
  }

}




void loop() {

/////////////////////Scan current state in 
      for (int k=0;k<=31;k++)
      {
          if (digitalRead(k)==0)//inverse logic, 0 is on.  CHANGE to 0 for SJO NC pos sense reader
            bitSet(CurrentState,k);
          else  
            bitClear(CurrentState,k);
      }// for k - 32 bits


//////////////////////Check for change in bits
//  int i=0;
        
      for  (int k=0;k<=31;k++)// 32 bits per long
      {
         if (bitRead(CurrentState,k)!=bitRead(PreviousState,k)){
            Serial.print("CurrentState");
            Serial.println(CurrentState,HEX);  
            if(bitRead(CurrentState,k)==0){
                usbMIDI.sendNoteOn(NoteBase+(k),0x7F,MIDIChannel);
                Serial.print("ON Note: ");Serial.print(NoteBase+(k),DEC);Serial.print(" Channel: ");Serial.println(MIDIChannel,DEC);
            }
            else {
                Serial.print("ON Note: ");Serial.print(NoteBase+(k),DEC);Serial.print(" Channel: ");Serial.println(MIDIChannel,DEC);
                usbMIDI.sendNoteOff(NoteBase+(k),0x7F,MIDIChannel);
            }
        }
      }//k
    PreviousState=CurrentState;

  
  delay(LoopDelay);
  
}// void loop()
