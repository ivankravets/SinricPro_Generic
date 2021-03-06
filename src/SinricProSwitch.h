/****************************************************************************************************************************
  SinricProSwitch.h - Sinric Pro Library for boards

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

#ifndef _SINRICSWITCH_H_
#define _SINRICSWITCH_H_

#include "SinricProDevice.h"

/**
   @class SinricProSwitch
   @brief Device suporting basic on / off command
 **/
class SinricProSwitch :  public SinricProDevice 
{
  public:
    SinricProSwitch(const char* deviceId, unsigned long eventWaitTime = 100);
};

SinricProSwitch::SinricProSwitch(const char* deviceId, unsigned long eventWaitTime) : SinricProDevice(deviceId, eventWaitTime) {}


#endif    //_SINRICSWITCH_H_
