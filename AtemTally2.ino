

/*****************
 * Example: ATEM Monitor
 * Connects to the Atem Switcher and outputs changes to Preview and Program on the Serial monitor (at 9600 baud)
 *
 * - kasper
 */
/*****************
 * TO MAKE THIS EXAMPLE WORK:
 * - You must have an Arduino with Ethernet Shield (or compatible such as "Arduino Ethernet", http://arduino.cc/en/Main/ArduinoBoardEthernet)
 * - You must have an Atem Switcher connected to the same network as the Arduino - and you should have it working with the desktop software
 * - You must make specific set ups in the below lines where the comment "// SETUP" is found!
 */



// Including libraries: 
#include <SPI.h>
#include <Ethernet.h>
#include <Streaming.h>
#include <MemoryFree.h>
//#include <SkaarhojPgmspace.h>

//#include <MemoryFree.h>


// MAC address and IP address for this *particular* Arduino / Ethernet Shield!
// The MAC address is printed on a label on the shield or on the back of your device
// The IP address should be an available address you choose on your subnet where the switcher is also present
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x10, 0x74, 0x01 };      // <= SETUP!  MAC address of the Arduino
 
IPAddress clientIp(192, 168, 0, 35);        // <= SETUP!  IP address of the Arduino
IPAddress switcherIp(192, 168, 0, 30);     // <= SETUP!  IP address of the ATEM Switcher


// Include ATEM library and make an instance:
// Connect to an ATEM switcher on this address and using this local port:
// The port number is chosen randomly among high numbers.
#include <ATEMbase.h>
#include <ATEMstd.h>
ATEMstd AtemSwitcher;

  int CamSet = 1;
  int CamStartNum = (CamSet*4)+1;

  int Prev1Pin = 6;
  int Prog1Pin = 5;
  
  int Prev2Pin = 3;
  int Prog2Pin = 2;

  int Prev3Pin = 17;
  int Prog3Pin = 16;
  
  int Prev4Pin = 15;
  int Prog4Pin = 14;


  int PrevPins[] = {Prev1Pin, Prev2Pin, Prev3Pin, Prev4Pin};
  int ProgPins[] = {Prog1Pin, Prog2Pin, Prog3Pin, Prog4Pin};
  
void setup() { 
  // Setup Pins
  
  pinMode(Prev1Pin, OUTPUT);  // Preview 1
  pinMode(Prog1Pin, OUTPUT);  // Program 1
  
  pinMode(Prev2Pin, OUTPUT);  // Preview 2
  pinMode(Prog2Pin, OUTPUT);  // Program 2

  pinMode(Prev3Pin, OUTPUT);  // Preview 3
  pinMode(Prog3Pin, OUTPUT);  // Program 3

  pinMode(Prev4Pin, OUTPUT);  // Preview 4
  pinMode(Prog4Pin, OUTPUT);  // Program 4



  randomSeed(analogRead(5));  // For random port selection
  
  // Start the Ethernet, Serial (debugging) and UDP:
  Ethernet.begin(mac,clientIp);
  Serial.begin(115200);
  Serial << F("\n- - - - - - - -\nSerial Started\n");  

  // Initialize a connection to the switcher:
  AtemSwitcher.begin(switcherIp);
  AtemSwitcher.serialOutput(0x80);
  AtemSwitcher.connect();

  // Shows free memory:  
  Serial << F("freeMemory()=") << freeMemory() << "\n";
}

void loop() {
  // Check for packets, respond to them etc. Keeping the connection alive!
  // VERY important that this function is called all the time - otherwise connection might be lost because packets from the switcher is
  // overlooked and not responded to.
    AtemSwitcher.runLoop();
    
 /*   
    digitalWrite(Prev1Pin, AtemSwitcher.getPreviewTally(CamStartNum));
    digitalWrite(Prog1Pin, AtemSwitcher.getProgramTally(CamStartNum));
  
    digitalWrite(Prev2Pin, AtemSwitcher.getPreviewTally(1+CamStartNum));
    digitalWrite(Prog2Pin, AtemSwitcher.getProgramTally(1+CamStartNum));
  
    digitalWrite(Prev3Pin, AtemSwitcher.getPreviewTally(2+CamStartNum));
    digitalWrite(Prog3Pin, AtemSwitcher.getProgramTally(2+CamStartNum));

    digitalWrite(Prev4Pin, AtemSwitcher.getPreviewTally(3+CamStartNum));
    digitalWrite(Prog4Pin, AtemSwitcher.getProgramTally(3+CamStartNum));
*/

for(int i = 0; i<4; i++){

    if( (AtemSwitcher.getPreviewTally(CamStartNum + i)) && (AtemSwitcher.getProgramTally(CamStartNum + i)) ) {
        digitalWrite(ProgPins[i], HIGH);
        digitalWrite(PrevPins[i], LOW);
    }
    
    if( (AtemSwitcher.getPreviewTally(CamStartNum + i)) && (!AtemSwitcher.getProgramTally(CamStartNum + i)) ) {
        digitalWrite(ProgPins[i], LOW);
        digitalWrite(PrevPins[i], HIGH);
    }
    
    if( (!AtemSwitcher.getPreviewTally(CamStartNum + i)) && (AtemSwitcher.getProgramTally(CamStartNum + i)) ) {
        digitalWrite(ProgPins[i], HIGH);
        digitalWrite(PrevPins[i], LOW);
    }
    
    if( (!AtemSwitcher.getPireviewTally(CamStartNum + i)) && (!AtemSwitcher.getProgramTally(CamStartNum + i)) ) {
        digitalWrite(ProgPins[i], LOW);
        digitalWrite(PrevPins[i], LOW);
    }

}

/*for(int i = CamStartNum; i < CamStartNum + 4; i++){

  if(AtemSwitcher.getPreviewTally(i)){
      Serial.print("Preview ");
      Serial.print(i);
      Serial.println(" On");

  }
  if(AtemSwitcher.getProgramTally(i)){
      Serial.print("Program ");
      Serial.print(i);
      Serial.println(" On");
  }
}*/

}

