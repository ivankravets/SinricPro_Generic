/****************************************************************************************************************************
  Generic_WiFiNINA_MotionSensor.ino
  For Generic boards, running WiFiNINA

  Based on and modified from SinricPro libarary (https://github.com/sinricpro/)
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/SinricPro_Generic
  Licensed under MIT license
  Version: 2.4.0

  Copyright (c) 2019 Sinric. All rights reserved.
  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)

  This file is part of the Sinric Pro (https://github.com/sinricpro/)

  Example for SinricPro Motionsensor device:
   - Setup motionsensor device
   - Support onPowerState to turn on / turn off motion sensor
   - Checks a motion sensor connected to MOTIONSENSOR_PIN and send event if state changed

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
  #define DEBUG_PORT Serial
  #define NODEBUG_WEBSOCKETS
  #define NDEBUG
#endif

#define WEBSOCKETS_NETWORK_TYPE   NETWORK_WIFININA

#include "SinricPro_Generic.h"
#include "SinricProMotionsensor.h"

#define WIFI_SSID         "YOUR-WIFI-SSID"
#define WIFI_PASS         "YOUR-WIFI-PASSWORD"

#define APP_KEY           "YOUR-APP-KEY"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "YOUR-APP-SECRET"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"

#define MOTIONSENSOR_ID   "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"

#define BAUD_RATE         115200              // Change baudrate to your need

#define MOTIONSENSOR_PIN  5                   // PIN where motionsensor is connected to
// LOW  = motion is not detected
// HIGH = motion is detected

bool myPowerState = true;                     // assume device is turned on
bool lastMotionState = false;
unsigned long lastChange = 0;

/**
   @brief Checks motionsensor connected to MOTIONSENSOR_PIN

   If motion sensor state has changed, send event to SinricPro Server
   state from digitalRead():
        HIGH = motion detected
        LOW  = motion not detected
*/
void handleMotionsensor() 
{
  // if device switched off...do nothing
  if (!myPowerState) 
    return;                            // if device switched off...do nothing

  unsigned long actualMillis = millis();

  // debounce motionsensor state transitions (same as debouncing a pushbutton)
  if (actualMillis - lastChange < 250) 
    return;

  bool actualMotionState = digitalRead(MOTIONSENSOR_PIN);   // read actual state of motion sensor

  if (actualMotionState != lastMotionState) 
  {         
    // if state has changed
    Serial.println("Motion " + String(actualMotionState ? "detected" : "not detected"));
    
    lastMotionState = actualMotionState;              // update last known state
    lastChange = actualMillis;                        // update debounce time
    SinricProMotionsensor &myMotionsensor = SinricPro[MOTIONSENSOR_ID]; // get motion sensor device
    myMotionsensor.sendMotionEvent(actualMotionState);
  }
}

/**
   @brief Callback for setPowerState request

   @param deviceId      String containing deviceId (useful if this callback used by multiple devices)
   @param[in] state     bool true=turn on device / false=turn off device
   @param[out] state    bool true=device turned on / false=device turned off
   @return true         request handled properly
   @return false        request can't be handled because some kind of error happened
*/
bool onPowerState(const String &deviceId, bool &state) 
{
  Serial.println("Device " + deviceId + " turned " + String(state ? "on" : "off"));
  
  myPowerState = state;
  return true; // request handled properly
}


// setup function for WiFi connection
void setupWiFi()
{
  Serial.print("\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.print("[WiFi]: IP-Address is ");
  Serial.println(WiFi.localIP());
}

// setup function for SinricPro
void setupSinricPro() 
{
  // add device to SinricPro
  SinricProMotionsensor& myMotionsensor = SinricPro[MOTIONSENSOR_ID];

  // set callback function to device
  myMotionsensor.onPowerState(onPowerState);

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

  Serial.println("\nStarting Generic_WiFiNINA_MotionSensor");

  setupWiFi();
  setupSinricPro();
}

void loop() 
{
  handleMotionsensor();
  SinricPro.handle();
}
