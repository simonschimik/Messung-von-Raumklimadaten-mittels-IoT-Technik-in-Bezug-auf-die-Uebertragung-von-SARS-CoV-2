#pragma once
/**
 * @file config.h
 * @author Simon Schimik
 * @version 1.0
 */

// Arduino Core-libraries
#include "Arduino.h"

// C++-Standard-libraries
#include <map>

// TFT-Libraries
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

// SDS011-Library
#include <SdsDustSensor.h>

// MH-Z19C-Library
#include "MHZ19.h"     

// BME280-Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Connectivity
#include <WiFi.h>

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

// MQTTLogger
#define AIOSERVER "0.0.0.0"
#define AIOSERVERPORT 1883
#define AIOUSERNAME ""
#define AIOKEY ""
#define LOOPDELAY 15000

// HTTPLogger
#define HTTPSERVER ""

/** Defines limit for the WiFi-reconnect-count until ESP reset.*/
#define WIFI_CONNECT_LIMIT 30

/** Defines limit for the MQTT-reconnect-count until ESP reset.*/
#define MQTT_CONNECT_LIMIT 5

/**
 * Defines a threshold for SDS011-sensor values variation during preheating.
 * 
 * SDS011-Preheating will only succeed once two sensor readings taken after another do not vary by more than this amount.
 */
#define SDS_PREHEAT_THRESHOLD 0.5

/**
 * Defines a threshold for MHZ-19C-sensor values variation during preheating.
 * 
 * MHZ-19C-Preheating will only succeed once two sensor readings taken after another do not vary by more than this amount.
 */
#define MH_PREHEAT_THRESHOLD 10