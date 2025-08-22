//#include "ArduinoController.h"
#ifndef __CHIKO_bCONTROLLER__
#define __CHIKO_bCONTROLLER__
#include <BLEController.h>
#include <BLEVibrationsCommand.h>   // rumble command type (part of the library)




// ---- App globals ------------------------------------------------------------
BLEController controller;       // main BLE-Gamepad-Client object
bool wasConnected = false;      // previous connection state

// Optional: keep last controls to use outside the read loop
BLEControlsEvent lastControls;


// Optional: simple dead-zone for sticks
static const float DEADZONE = 0.10f;



// ---- Vibration helper -------------------------------------------------------
// Sends a rumble pulse. Intensities are 0..255 (0 = off, 255 = max).
// durationMs is capped internally by the lib around 2550 ms.
void sendVibration(uint8_t leftMain, uint8_t rightMain, uint8_t leftTrigger, uint8_t rightTrigger, uint16_t durationMs, uint8_t cycles = 1);


// --- Callbacks ---------------------------------------------------------------
void onConnectedController(void);
void onDisconnectedController(void);
void printControllerInfo(void);

// --- Utilities ---------------------------------------------------------------
float dz(float v);

// --- MainFunction ---------------------------------------------------------------
void initialize_bController(void (*onConnect)()=NULL,void (*onDisconnect)()=NULL, void (*readController)() = NULL );


#endif