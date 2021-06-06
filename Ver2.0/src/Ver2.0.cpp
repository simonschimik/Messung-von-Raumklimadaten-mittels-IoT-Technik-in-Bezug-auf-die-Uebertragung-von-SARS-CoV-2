/**
 * @file Ver2.0.cpp
 * @author Simon Schimik
 * @version 2.0
 */

#include "Arduino.h" 
#include "config.h"
#include "DataLoggingHandler.cpp"
#include "MQTTLogger.cpp"

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

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>


AsyncWebServer server(80); 
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
SdsDustSensor sds(SDS_RX, SDS_TX);
MHZ19 myMHZ19;                                            
HardwareSerial mhSerial(1); // Use UART channel 1  
Adafruit_BME280 bme;
DataLoggingHandler* logger;
int timer;

std::map<const char*, double>* sensorData  = new std::map<const char*, double>{
  {"temperature", 0.0},
  {"humidity", 0.0},
  {"pressure", 0.0},
  {"pm10", 0.0},
  {"pm25", 0.0},
  {"CO2", 0.0}
};

/**
 * Initialises OTA-Server
 * 
 * Initialises an asynchronous webserver and starts ElegentOTA functionality
 */
void initElegentOTA()
{
  Serial.println("Initialising OTA-server!");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello");
  });
  AsyncElegantOTA.begin(&server);
  server.begin();

  Serial.println("OTA-server server started");
}


/**
 * Connects to WiFi
 * 
 * Initialises a WiFi connection with the specified credentials. 
 * Function will not terminate until WiFi connection is successfull.
 */
void connectWifi()
{
  Serial.print("Connecting to WiFi " + String(SSID));
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi-connection successfull!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());
}

/**
 * Draws centered text on display
 * 
 * @param text The text to be printed
 * @param x Width of the screen in which the text is supposed to be centered (f.e. <b> x = tft.width() </b> centers the text on the entire display, <b> x = tft.width()  / 2 </b>  only on the left half of the display)
 * @param y Y position of the centered text on the display
 */
void drawCenteredText(String text, int x, int y)
{
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  tft.setCursor((x - w) / 2, y);
  tft.print(text);
}

/**
 * Returns a color based on the parameter pm10
 * 
 * Returns a color for better visualisation on the display of the pm10 value
 * @param pm10 The pm10 value
 * @return Returns a 16-bit hexadecimal representation of the corrosponding color
 */
long getPm10Color(double pm10)
{
  if(pm10 <= 50) return ST7735_CYAN;
  else if(pm10 <= 100) return ST7735_GREEN;
  else if(pm10 <= 250) return ST7735_YELLOW;
  else if(pm10 <= 350) return ST7735_ORANGE;
  else if(pm10 <= 430) return ST7735_RED;
  else return ST7735_PURPLE;
}

/**
 * Returns a color based on the parameter pm25
 * 
 * Returns a color for better visualisation on the display of the pm2.5 value
 * @param pm25 The pm2.5 value
 * @return Returns a 16-bit hexadecimal representation of the corrosponding color
 */
long getPm25Color(double pm25)
{
  if(pm25 <= 30) return ST7735_CYAN;
  else if(pm25 <= 60) return ST7735_GREEN;
  else if(pm25 <= 90) return ST7735_YELLOW;
  else if(pm25 <= 120) return ST7735_ORANGE;
  else if(pm25 <= 250) return ST7735_RED;
  else return ST7735_PURPLE;
}

/**
 * Returns a color based on the parameter co2
 * 
 * Returns a color for better visualisation on the display of the co2 value
 * @param co2 The co2 value
 * @return Returns a 16-bit hexadecimal representation of the corrosponding color
 */
long getCO2Color(double co2)
{
  if(co2 <= 650) return ST7735_CYAN;
  else if(co2 <= 950) return ST7735_GREEN;
  else if(co2 <= 1250) return ST7735_YELLOW;
  else if(co2 <= 1500) return ST7735_ORANGE;
  else if(co2 <= 1850) return ST7735_RED;
  else return ST7735_PURPLE;
}

/**
 * Prints the regular UI and sensor values
 * 
 * Prints the current sensor values stores in field std::map<const char*, double>* sensorData
 */
void printRegularDisplay()
{
  // Clear screen
  tft.fillScreen(ST7735_BLACK);
  
  // Draw outlines
  tft.drawLine(0, 0, tft.width(), 0, ST7735_WHITE);
  tft.drawLine(0, 0, 0, tft.height(), ST7735_WHITE);
  tft.drawLine(0, tft.height()-1, tft.width()-1, tft.height()-1, ST7735_WHITE);
  tft.drawLine(tft.width()-1, 0, tft.width()-1, tft.height()-1, ST7735_WHITE);

  // Draw sections
  tft.drawLine(0, tft.height()*5/12, tft.width(), tft.height()*5/12, ST7735_WHITE);
  tft.drawLine(0, tft.height()*2/3, tft.width(), tft.height()*2/3, ST7735_WHITE);
  tft.drawLine(tft.width()/2, tft.height()*2/3, tft.width()/2, tft.height(), ST7735_WHITE);

  // Draw headers
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  drawCenteredText("Temperature", tft.width(), 5);
  drawCenteredText("Humidity", tft.width(), 25);
  drawCenteredText("Pressure", tft.width(), 45);
  drawCenteredText("CO2-Concentration", tft.width(), tft.height()*5/12 + 2);
  drawCenteredText("ppm", tft.width(), tft.height()*5/12 + 30);
  drawCenteredText("PM10", tft.width()/2+2, tft.height()*2/3+2);
  drawCenteredText("PM2.5", tft.width()*3/2+2, tft.height()*2/3+2);
  drawCenteredText("um_g/m^3", tft.width() / 2, tft.height()*2/3+43);
  drawCenteredText("um_g/m^3", tft.width() * 3/2, tft.height()*2/3+43);

  // Draw sensor values
  tft.setTextSize(1);
  tft.setTextColor(ST7735_BLUE);
  drawCenteredText(String(sensorData->at("temperature")) + " C", tft.width(), 15);
  drawCenteredText(String(sensorData->at("humidity")) + " %", tft.width(), 35);
  drawCenteredText(String(sensorData->at("pressure")) + " hPa", tft.width(), 55);

  tft.setTextSize(2);
  tft.setTextColor(getCO2Color(sensorData->at("CO2")));
  drawCenteredText(String((int)sensorData->at("CO2")), tft.width(), tft.height()*5/12 + 13);
  tft.setTextColor(getPm10Color(sensorData->at("pm10")));
  drawCenteredText(String(sensorData->at("pm10")), tft.width() / 2, tft.height()*2/3+17);
  tft.setTextColor(getPm25Color(sensorData->at("pm25")));
  drawCenteredText(String(sensorData->at("pm25")), tft.width() * 3/2, tft.height()*2/3+17);
}

/**
 * Prints an error display
 * 
 * Prints every string in the passed array into a new line on the tft
 * @param data std::array of 8 string to be printed into seperate lines
 */
void printErrorDisplay(std::array<String, 8> data)
{
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_RED);
  for(int i = 0; i < 8; i++)
  {
    drawCenteredText(data[i], tft.width(), 5 + i*20);
  }
}

/**
 * Reads sensor values
 * 
 * Reads sensors and assigns the retrieved values to the corresponding pairs in std::map<const char*
 */
void readSensors()
{
  PmResult sds_results = sds.queryPm();
  if (sds_results.isOk()){
    sensorData->at("pm25") = sds_results.pm25;
    sensorData->at("pm10") = sds_results.pm10;
  } else {
    sensorData->at("pm25") = -1;
    sensorData->at("pm10") = -1;
  }
  
  sensorData->at("CO2") = myMHZ19.getCO2();
  sensorData->at("temperature") = bme.readTemperature();
  sensorData->at("humidity") = bme.readHumidity();
  sensorData->at("pressure") = bme.readPressure() / 100.0;
  
}


/**
 * Predefined setup-function
 * 
 * Is called once in the beginning of runtime
 */
void setup() 
{
  Serial.begin(9600);

  // Init TFT
  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  drawCenteredText("init sensors", tft.width(), tft.height()/2);

  // Init SDS
  sds.begin();
  // TODO Error display
  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode
  sds.wakeup();

  // Init MH-Z19C
  mhSerial.begin(9600, SERIAL_8N1, 32, 33);                               
  myMHZ19.begin(mhSerial);                                
  myMHZ19.autoCalibration(false); 

  // Init BME280
  bme.begin(0x76);

  connectWifi();
  initElegentOTA();
  
  logger = new MQTTLogger();
  timer = 0;
}

/**
 * Predefined loop-function
 * 
 * Is called repeatedly during runtime
 */
void loop()
{
  if(timer == LOOPDELAY)
  {
    timer = 0;
    try
    {
      readSensors();
      logger->log(sensorData);
      printRegularDisplay();
    }catch(MQTTConnectionFailedException& e)
    {
      Serial.println("An exception occurred: " + String(e.what()));
      printErrorDisplay({"Couldn't connect", "to MQTT-Broker" ,"IP: " + WiFi.localIP().toString(), "Host: " + String(WiFi.getHostname()), 
                          "WiFi connected: " + String(WiFi.isConnected()), "Retrying in " + String(LOOPDELAY/1000) + "s", AIOSERVER, AIOUSERNAME});
    }catch(WifiNotConnectedException& e) 
    {
      Serial.println("An exception occurred: " + String(e.what()));
      printErrorDisplay({"WiFi connection lost!", "Reconnecting..."});
      connectWifi();
    }
  }

  /**
   * The AsyncElegantOTA.loop() statement must be called repeatedly throughout the loop function. Pausing the loop function directly by delay(LOOPDELAY)
   * would cause unpredictable errors from the OTA-service, hence the AsyncElegantOTA.loop() statement would be called in a very long time period.
   * Therefore, the loop is only paused for 1ms, and instead the counter-var 'timer' is introduced.
   */

  AsyncElegantOTA.loop();
  delay(1); timer++;
}
