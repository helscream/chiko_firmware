#ifndef __CHIKO_LED__
#define __CHIKO_LED__

#include "chiko_definations.h"
#include <Arduino.h>           // Core Arduino functionality

void initilize_LED();
void setLEDState(LEDStates currentLEDState);
LEDStates getLEDState();

#endif