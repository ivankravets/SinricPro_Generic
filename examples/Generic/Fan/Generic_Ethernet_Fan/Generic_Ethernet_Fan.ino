/****************************************************************************************************************************
  Generic_Ethernet_Fan.ino
  For Generic boards, running W5x00 or ENC28J60 Ethernet shield

  Based on and modified from SinricPro libarary (https://github.com/sinricpro/)
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/SinricPro_Generic
  Licensed under MIT license
  Version: 2.4.0

  Copyright (c) 2019 Sinric. All rights reserved.
  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)

  This file is part of the Sinric Pro (https://github.com/sinricpro/)

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.4.0   K Hoang      21/05/2020 Initial porting to support SAMD21, SAMD51 nRF52 boards, such as AdaFruit Itsy-Bitsy,
                                  Feather, Gemma, Trinket, Hallowing Metro M0/M4, NRF52840 Feather, Itsy-Bitsy, STM32, etc.
 *****************************************************************************************************************************/

// STM32 Boards supported: Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32F1, STM32F3, STM32F4, STM32H7, STM32L0, etc.
// SAM DUE
// Teensy 4.1, 4.0, 3.6, 3.5, 3.2/3.1, 3.0


#if defined(ESP8266) || defined(ESP32)
#error This code is not intended to run on the ESP32/ESP8266 boards ! Please check your Tools->Board setting.
#endif

// Uncomment the following line to enable serial debug output
#define ENABLE_DEBUG    true

#if ENABLE_DEBUG
  #define DEBUG_PORT            Serial
  #define NODEBUG_WEBSOCKETS
  #define NDEBUG
#endif

#define LOGWARN(x)     if(ENABLE_DEBUG) { Serial.print("[SINRIC_PRO] "); Serial.println(x); }
#define LOGWARN1(x,y)  if(ENABLE_DEBUG) { Serial.print("[SINRIC_PRO] "); Serial.print(x);\
                                          Serial.print(" "); Serial.println(y); }
                                          

// Use true  for ENC28J60 and UIPEthernet library (https://github.com/UIPEthernet/UIPEthernet)
// Use false for W5x00 and Ethernetx library      (https://www.arduino.cc/en/Reference/Ethernet)

#define USE_UIP_ETHERNET   true
//#define USE_UIP_ETHERNET   false

#if USE_UIP_ETHERNET
  #define WEBSOCKETS_NETWORK_TYPE     NETWORK_ENC28J60
#endif

//#define USE_CUSTOM_ETHERNET     true

// Note: To rename ESP628266 Ethernet lib files to Ethernet_ESP8266.h and Ethernet_ESP8266.cpp
// In order to USE_ETHERNET_ESP8266
#if ( !defined(USE_UIP_ETHERNET) || !USE_UIP_ETHERNET )

  // To override the default CS/SS pin. Don't use unless you know exactly which pin to use
  //#define USE_THIS_SS_PIN   27//22  //21  //5 //4 //2 //15
  
  // Only one if the following to be true
  #define USE_ETHERNET2         false //true
  #define USE_ETHERNET3         false //true
  #define USE_ETHERNET_LARGE    false //true
  #define USE_ETHERNET_ESP8266  false //true
  
  #if ( USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE || USE_ETHERNET_ESP8266 )
    #ifdef USE_CUSTOM_ETHERNET
      #undef USE_CUSTOM_ETHERNET
    #endif
    #define USE_CUSTOM_ETHERNET   true
  #endif
  
  #ifdef WEBSOCKETS_NETWORK_TYPE
    #undef WEBSOCKETS_NETWORK_TYPE
  #endif
  #define WEBSOCKETS_NETWORK_TYPE     NETWORK_W5100
  
  #if USE_ETHERNET3
    #include "Ethernet3.h"
    #warning Use Ethernet3 lib
  #elif USE_ETHERNET2
    #include "Ethernet2.h"
    #warning Use Ethernet2 lib
  #elif USE_ETHERNET_LARGE
    #include "EthernetLarge.h"
    #warning Use EthernetLarge lib
  #elif USE_ETHERNET_ESP8266
    #include "Ethernet_ESP8266.h"
    #warning Use Ethernet_ESP8266 lib
  #elif USE_CUSTOM_ETHERNET
    #include "Ethernet_XYZ.h"
    #warning Use Custom Ethernet library from EthernetWrapper. You must include a library here or error.
  #else
    #define USE_ETHERNET          true
    #include "Ethernet.h"
    #warning Use Ethernet lib
  #endif
  
  // Ethernet_Shield_W5200, EtherCard, EtherSia not supported
  // Select just 1 of the following #include if uncomment #define USE_CUSTOM_ETHERNET
  // Otherwise, standard Ethernet library will be used for W5x00

#endif    //#if !USE_UIP_ETHERNET

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC      20

byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};

#include "SinricPro_Generic.h"
#include "SinricProFanUS.h"

#define APP_KEY           "YOUR-APP-KEY"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "YOUR-APP-SECRET"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"

#define FAN_ID            "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"

#define BAUD_RATE         115200              // Change baudrate to your need

// we use a struct to store all states and values for our fan
// fanSpeed (1..3)
struct 
{
  bool powerState = false;
  int fanSpeed = 1;
} device_state;

bool onPowerState(const String &deviceId, bool &state) 
{
  Serial.println("Fan turned " + String(state ? "on" : "off"));
  device_state.powerState = state;
  return true; // request handled properly
}

// Fan rangeValue is from 1..3
bool onRangeValue(const String &deviceId, int &rangeValue) 
{
  device_state.fanSpeed = rangeValue;
  
  Serial.println("Fan speed changed to " + String(device_state.fanSpeed));
  
  return true;
}

// Fan rangeValueDelta is from -3..+3
bool onAdjustRangeValue(const String &deviceId, int rangeValueDelta) 
{
  device_state.fanSpeed += rangeValueDelta;
  
  Serial.println("Fan speed changed about " + String(rangeValueDelta) + " to " + String(device_state.fanSpeed));

  rangeValueDelta = device_state.fanSpeed; // return absolute fan speed
  return true;
}

// setup function for setupEthernet connection
void setupEthernet() 
{
  #if USE_ETHERNET
    LOGWARN(F("=========== USE_ETHERNET ==========="));
  #elif USE_ETHERNET2
    LOGWARN(F("=========== USE_ETHERNET2 ==========="));
  #elif USE_ETHERNET3
    LOGWARN(F("=========== USE_ETHERNET3 ==========="));
  #elif USE_ETHERNET_LARGE
    LOGWARN(F("=========== USE_ETHERNET_LARGE ==========="));
  #elif USE_ETHERNET_ESP8266
    LOGWARN(F("=========== USE_ETHERNET_ESP8266 ==========="));
  #else
    LOGWARN(F("========================="));
  #endif
 
  LOGWARN(F("Default SPI pinout:"));
  LOGWARN1(F("MOSI:"), MOSI);
  LOGWARN1(F("MISO:"), MISO);
  LOGWARN1(F("SCK:"),  SCK);
  LOGWARN1(F("SS:"),   SS);
  LOGWARN(F("========================="));
   
  // unknown board, do nothing, use default SS = 10
  #ifndef USE_THIS_SS_PIN
    #define USE_THIS_SS_PIN   10    // For other boards
  #endif
       
  LOGWARN1(F("Use default CS/SS pin : "), USE_THIS_SS_PIN);

  // For other boards, to change if necessary
  #if ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2 )
    // Must use library patch for Ethernet, Ethernet2, EthernetLarge libraries

    Ethernet.init (USE_THIS_SS_PIN);

  #elif USE_ETHERNET3
    // Use  MAX_SOCK_NUM = 4 for 4K, 2 for 8K, 1 for 16K RX/TX buffer
    #ifndef ETHERNET3_MAX_SOCK_NUM
      #define ETHERNET3_MAX_SOCK_NUM      4
    #endif
    
    Ethernet.setCsPin (USE_THIS_SS_PIN);
    Ethernet.init (ETHERNET3_MAX_SOCK_NUM);
                    
  #endif  //( USE_ETHERNET || USE_ETHERNET2 || USE_ETHERNET3 || USE_ETHERNET_LARGE )

    // start the ethernet connection and the server:
  // Use Static IP
  //Ethernet.begin(mac, ip);
  // Use DHCP dynamic IP and random mac
  srand(millis());
  uint16_t index = rand() % NUMBER_OF_MAC;

  Serial.print("Index = ");
  Serial.println(index);

  Ethernet.begin(mac[index]);
  
  Serial.print("Connected!\n[Ethernet]: IP-Address is ");
  Serial.println(Ethernet.localIP());
}


void setupSinricPro() 
{
  SinricProFanUS &myFan = SinricPro[FAN_ID];

  // set callback function to device
  myFan.onPowerState(onPowerState);
  myFan.onRangeValue(onRangeValue);
  myFan.onAdjustRangeValue(onAdjustRangeValue);

  // setup SinricPro
  SinricPro.onConnected([]()
  {
    Serial.println("Connected to SinricPro");
  });

  SinricPro.onDisconnected([]()
  {
    Serial.println("Disconnected from SinricPro");
  });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() 
{
  Serial.begin(BAUD_RATE);
  while (!Serial);

  Serial.println("\nStarting Generic_Ethernet_Fan");

  setupEthernet();
  setupSinricPro();
}

void loop() 
{
  SinricPro.handle();
}
