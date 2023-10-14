# Multiplexing-MFRC522-NFC-Modules: Readme & Developers Blog
NFC reading from 4x MFRC522 Modules on the same SPI bus from an ESP32




- A general description of the project:
  - _An ESP32 that multiplexes 4x of my custom desighned MFRC522 NFC modules, and uses RGB LEDs to indicate status_
  - _See my other repo, [Custom MFRC522 Board Design](https://github.com/palcan/Custom-MFRC522-Board-Design), for details on the custom desighned MFRC522 NFC module_

- The project status:
  - _The project is finished, and intended to be reused, in some form, in a new project for multiplexing the frontend of one of the modules to 8x antennas. This project that can be found in a new repo some time in the future. The code in this repo will not be further updated._
     
- The requirements on the development environment for integration:
  - _I've used plain Aurdiono IDE (version 1.8.13) for the code. The code should work fine with any ESP32 board that has HW SPI. The custom MFRC522 module I made can be interchanged with the commonlys sold RC522 and RC522 MINI modules. The LEDs are the common WS2812B type, that can be interchanged with any other LED strips based on the WS2812B, such as neopixels_
  
- A guide to installation and use:
  - _The Arduino IDE downloads and use is extensively documented by Arduino.cc._
  - _The entries in the code where the red, blue and green tag ID's are set must be changed to the IDs of your own tags that you will be using_
  - _The specs for the ESP32 devboard I've used here can be found at https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/hw-reference/esp32c3/user-guide-devkitc-02.html._
  - _As stated earlier, the NFC modules can be bought all over the internet, and the pinout is typically printed on the modules_
  - _The library in use can be found through normal library search in the Arduino IDE (if you don't know how, there's info on Arduino.cc that explains that much better than me). The l\ink to the library Github page is https://github.com/miguelbalboa/rfid/tree/master._

- A list of technology used and any links to further information related to this technology:
  - _Much of the technology used (what we can call technology in this context) is readily available in uncounted sources a quick google search away. Keywords are_
    - _SPI_
    - _ESP32_
    - _Arduino IDE_
    - _Neopixel_
  - _For a deeper understanding of NFC there's multiple primers out there that can be helpful, I used the educational material available from NXP and STMicro quite a bit, and read through a few of the examples on connecting ESP32 and Arduino to RC522 (Lots of videos on Youtube on this)_

- Known bugs and any bug fixes:
  - _The first version I made of the custom board had a header H1 and a WS2812B RGB LED on as well. I've later removed those. You'll se the unpopulated footprints of them on the modules used in this project, while they are not present in the schematic shown._
  - _It is a bit unfortunate to place a through hole header where I did, as it protrudes up on the surface where the tag should be placed. If there will be a later revision of the PCB, I might move it all the way to the board edge, or rearrange the component placement enough to find board space for a SMD header_
  - _If there will be a later revision of the PCB, I'd print the signal names on the silkscreen next to the header pins. It was a bit unnecessarily annoying to have to check my notepad all the time to see which pin was which signal_
    
- FAQ section with all previously asked questions:
  - _Nobody has asked anything yet...!_
 
- Copyright and licensing information:
  - _See the license file in this repo_

  
## Table of Contents
[Introduction](#Introduction)  
[Design](#Design)  
[Test](#Testing-multiplexing-of-the-MFRC522-NFC-reader)  
[Code explanation](#Code-explanation)  

## Introduction
I've just recently dived into designing my own NFC frontend + antenna, and got it to work with some code on an ESP32 microcontroller. I thought it was the next logical step to test multiplexing multiple of these through the SPI bus, but didn't want to clutter the original repo where the design of the custom module was detailed, and I felt solving multiplexing multiple modules was worth it's own standalone solution space. I already had a few of the custom modules lying around from my original design order (see my other repo, [Custom MFRC522 Board Design](https://github.com/palcan/Custom-MFRC522-Board-Design), for all the details on the custom designed MFRC522 NFC module) and an extra ESP32 development board and some discrete WS2812B LEDs breakout PCBs, so all that was needed was to cobble it together on a breadboard. 

I re-used the colorcoded tags from the original project and altered the code to use external LEDs intstead of the builtin LED on the development board, and it worked almost perfectly. I had to add in the reset of module after a successful read, as otherwise I experienced some unexplained hangups (the module stopped responding, and halted the whole program... Probably some error with unfinished reading of a tag that were stuck waiting for data that never came. Or something else. Who knows.) 

[Back to Table of Contents](#Table-of-Contents)  

## Design
The breadborded "beauty". I ran out of wires at my desk, so it's not very consistent in wire lengths and coloring scheme. But I added a wiring list below that details which pins are connected where. 

<img src="Images/Multiple_RC522_ESP32_breadboard.jpg" width=75% height=75%>

*Breadboarded ESP32 with 4x LEDs and 4x MFRC522 PCBs*

<img src="Images/Wiring_list_Multiple_RC522.png" width=100% height=100%>

*Wiring together the ESP32 and 4x MFRC522 PCBs. LEDs are unfortunately not shown here*

[Back to Table of Contents](#Table-of-Contents)  

## Testing multiplexing of the MFRC522 NFC reader

The following images and gif show in sequence that the different tags all are recognized independently of which reader is being used, and that there's no problem reading multiple tags when present. The color of the LEDs is set to match the color coding on the tags, and that matches without error. 

<img src="Images/Multiple_RC522_ESP32_breadboard_BLUE_test.jpg" width=75% height=75%>

*Testing the recognition of the blue tag*

<img src="Images/Multiple_RC522_ESP32_breadboard_RED_test.jpg" width=75% height=75%>

*Testing the recognition of the red tag*

<img src="Images/Multiple_RC522_ESP32_breadboard_GREEN_test.jpg" width=75% height=75%>

*Testing the recognition of the green tag*

<img src="Images/Multiple_RC522_ESP32_Recognizing_tags_gif.gif" width=50% height=50%>

*Testing the recognition of all the three tags simultanously -gif-*

[Back to Table of Contents](#Table-of-Contents)  

## Code explanation

Please see the reposirory folder "Code" for the the full code where the snippets and blocks below are copied out from. I have in general re-used some of the description from the single module repo (see [Custom MFRC522 Board Design](https://github.com/palcan/Custom-MFRC522-Board-Design)) and added on with extra description for the sections where it is changed.  

The first block is basically just including the necessary external libraries used in the program. Note that the neopixel library is necessary to include to control the WS2812B LEDs. Alternatively the FastLED library would've worked as well. The SPI library lets you use the built in SPI bus for this ESP32 dev board, and the MFRC522.h library gives the abstraction layer functions you use to talk to the MFRC chip on the NFC module. 

```
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
```

The reset pin for the MFRC522 chip is not part of the SPI bus, but is necessay to define before initializing the MFRC522 instance, so must be set at this section. The same goes for the SS pins (although those _are_ part of the SPI bus). It is defined separately from the SPI bus to allow for defining multiple SS pins when you have more than one device on the SPI bus. We are also defining the number of readers here - it gives simplifies scaling up later, as the loop can be set to loop over fewer reader modules with just changing this value (or more, just make sure enough SS pins are defined as well). Finally, we need to choose which pin we're driving the LED dataline from, and how many LEDs there are in the string, as the WS2812B LEDs are serially wired together.  

```
#define RST_PIN         7       // Configurable, not part of ESP32 SPI hardware pins
#define SS_1_PIN        0       // Configurable, not part of ESP32 SPI hardware pins
#define SS_2_PIN        1       // Configurable, not part of ESP32 SPI hardware pins
#define SS_3_PIN        2       // Configurable, not part of ESP32 SPI hardware pins
#define SS_4_PIN        3       // Configurable, not part of ESP32 SPI hardware pins
#define NR_OF_READERS   4       
#define LED_PIN         19
#define LED_COUNT       4
```


```
byte ssPins[] = {SS_4_PIN, SS_3_PIN, SS_2_PIN, SS_1_PIN};           // Array of Slave Select pins for later use in cycling through with for loop
MFRC522 mfrc522[NR_OF_READERS];                                     // Create a MFRC522 instance for each of the readers we have
```



```
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);  // Instatiate the strip of WS2812B LEDs we are hooking up
```



```
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
```

[Back to Table of Contents](#Table-of-Contents)  
