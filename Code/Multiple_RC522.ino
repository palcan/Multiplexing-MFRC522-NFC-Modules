 /* NFC Modules Pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      ESP32       
 *             Reader/PCD   C3 DevKit       
 * Signal      Pin          Pin + color of wire          
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          7 
 * SPI SS      SDA(SS)      0, 1, 2, 3          
 * SPI MOSI    MOSI         6 
 * SPI MISO    MISO         5  
 * SPI SCK     SCK          4  
*/


//----------------------------------------------------
// INCLUDE SECTION
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

//----------------------------------------------------
// DEFINE SECTION
#define RST_PIN         7       // Configurable, not part of ESP32 SPI hardware pins
#define SS_1_PIN        0       // Configurable, not part of ESP32 SPI hardware pins
#define SS_2_PIN        1       // Configurable, not part of ESP32 SPI hardware pins
#define SS_3_PIN        2       // Configurable, not part of ESP32 SPI hardware pins
#define SS_4_PIN        3       // Configurable, not part of ESP32 SPI hardware pins
#define NR_OF_READERS   4       
#define LED_PIN         19
#define LED_COUNT       4

 //----------------------------------------------------
 // GLOBALS SECTION
byte ssPins[] = {SS_4_PIN, SS_3_PIN, SS_2_PIN, SS_1_PIN};           // Array of Slave Select pins for later use in cycling through with for loop
MFRC522 mfrc522[NR_OF_READERS];                                     // Create a MFRC522 instance for each of the readers we have
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);  // Instatiate the strip of WS2812B LEDs we are hooking up

byte allowedID[3][4] =                                              // The limited set of recognized unique IDs of the tags I'm using
  {
    {0x90, 0x6A, 0xCF, 0x25},   // ID0 
    {0x11, 0x7C, 0x5D, 0x26},   // ID1    
    {0x90, 0xD3, 0x11, 0x25},   // ID2 
  };   
uint32_t color_matching[3] =                                      // Colors matching the set of recognized unique IDs of the tags I'm using
  {
    strip.Color(  0,   0, 255), // Blue    
    strip.Color(  0,   255, 0), // Green
    strip.Color(  255,   0, 0), // Red
  };

 //----------------------------------------------------
 // SETUP SECTION
void setup() {
  Serial.begin(9600);           // Initialize serial communications with the PC
  while (!Serial);              // Do nothing if no serial port is opened 
  SPI.begin();                  // Initialize the SPI bus
  strip.begin();                // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.clear();                // Turn OFF all pixels ASAP
  strip.setBrightness(50);      // Set BRIGHTNESS to about 1/5 (max = 255)

   
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {    // looking for MFRC522 readers
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
    delay(500);
  }
}

//----------------------------------------------------
// MAIN LOOP 
void loop() {

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {              // CYCLING THROUGH EACH READER TO CHECK IF ANY NEW TAGS ARE PRESENT

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {

      Serial.println();
      Serial.print(F("Reader "));
      Serial.print(reader);
  
      Serial.print(F(": Card UID:"));                                         // Show some details of the PICC (that is: the tag/card)
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
  
      strip.setPixelColor(reader, checkTagUID(mfrc522[reader].uid.uidByte));  // Update the color of the LED next to the current reader according to the tag present
      strip.show();
  
      mfrc522[reader].PICC_HaltA();                                           // Halt PICC
      mfrc522[reader].PCD_StopCrypto1();                                      // Stop encryption on PCD
  
      mfrc522[reader].PCD_Reset();                                            // Test soft reset
      mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    
      strip.clear();                                                          // Set all pixel colors to 'off'
      strip.show();   
    }
  }
}

// *************** ADDITIONAL FUNCTIONS ****************************************************

//------------------------------------------------------------------------------------------
void dump_byte_array(byte * buffer, byte bufferSize) {
  
  for (byte i = 0; i < bufferSize; i++) {
    
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

//------------------------------------------------------------------------------------------
uint32_t checkTagUID(byte * buffer){
  
  uint32_t color = strip.Color(  0,   0,   0, 255);         // std color, true white, as deffault return value
  for (int i = 0; i < 3; i++) {
    if (memcmp(buffer, allowedID[i], 4) == 0){
        color = color_matching[i];
        break;    
    }else{
      Serial.println("No Match");
    }
  }
  return color;
}
