/**
 * @file config.h
 * @author Simon Schimik
 * @version 1.0
 */

// Arduino Core-libraries
#include "Arduino.h"
#include <Wifi.h>

// C++-Standard-libraries
#include <map>

// TFT-Pins
#define TFT_CS 5 
#define TFT_RST 17                                
#define TFT_DC 16 

// SDS011-Pins
#define SDS_RX 25
#define SDS_TX 26

// MH-Z19C-Pins
#define MH_TX 32
#define MH_RX 33

#define SSID ""
#define PASS ""

#define AIOSERVER "io.adafruit.com"
#define AIOSERVERPORT 1883
#define AIOUSERNAME ""
#define AIOKEY ""
#define LOOPDELAY 15000