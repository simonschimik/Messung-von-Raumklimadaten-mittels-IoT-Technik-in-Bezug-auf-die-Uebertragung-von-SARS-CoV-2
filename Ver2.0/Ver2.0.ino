/**
 * @file Ver2.0.ino
 * @author Simon Schimik
 * @version 2.0
 */
 
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

// ElegantOTA-Libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "DataLoggingHandler.cpp"
#include "MQTTLogger.cpp"

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

// ElegantOTA-Definitions
const char* ssid = "XXXX";
const char* password = "XXXX";
AsyncWebServer server(80); 

// Sensor-Definitions
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SdsDustSensor sds(SDS_RX, SDS_TX);
MHZ19 myMHZ19;                                            
HardwareSerial mhSerial(1); // Use UART channel 1  
Adafruit_BME280 bme;
DataLoggingHandler* logger;


/**
 * Initialises OTA-Server
 * 
 * Connects to WiFi with specified credentials, initialises an asynchronous webserver and starts ElegentOTA functionality
 */
void initElegentOTA()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server started");
}

/**
 * Draws centered text on display
 * 
 * @param text: The text to be printed
 * @param x: Width of the screen in which the text is supposed to be centered (f.e. <b> x = tft.width() </b> centers the text on the entire display, <b> x = tft.width()  / 2 </b>  only on the left half of the display)
 * @param y: Y position of the centered text on the display
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
 * @param pm10: The pm10 value
 * @return Returns a 16-bit hexadecimal representation of the corrosponding color
 */
long getPm10Color(double pm10)
{
  if(pm10 <= 50){
    return ST7735_CYAN;
  }else if(pm10 <= 100){
    return ST7735_GREEN;
  }else if(pm10 <= 250){
    return ST7735_YELLOW;
  }else if(pm10 <= 350){
    return ST7735_ORANGE;
  }else if(pm10 <= 430){
    return ST7735_RED;
  }else{
    return ST7735_PURPLE;
  }
}

/**
 * Returns a color based on the parameter pm25
 * 
 * Returns a color for better visualisation on the display of the pm2.5 value
 * @param pm25: The pm2.5 value
 * @return Returns a 16-bit hexadecimal representation of the corrosponding color
 */
long getPm25Color(double pm25)
{
  if(pm25 <= 30){
    return ST7735_CYAN;
  }else if(pm25 <= 60){
    return ST7735_GREEN;
  }else if(pm25 <= 90){
    return ST7735_YELLOW;
  }else if(pm25 <= 120){
    return ST7735_ORANGE;
  }else if(pm25 <= 250){
    return ST7735_RED;
  }else{
    return ST7735_PURPLE;
  }
}

/**
 * Returns a color based on the parameter co2
 * 
 * Returns a color for better visualisation on the display of the co2 value
 * @param co2: The co2 value
 * @return Returns a 16-bit hexadecimal representation of the corrosponding color
 */
long getCO2Color(int co2)
{
  if(co2 <= 650){
    return ST7735_CYAN;
  }else if(co2 <= 950){
    return ST7735_GREEN;
  }else if(co2 <= 1250){
    return ST7735_YELLOW;
  }else if(co2 <= 1500){
    return ST7735_ORANGE;
  }else if(co2 <= 1850){
    return ST7735_RED;
  }else{
    return ST7735_PURPLE;
  }
}

/**
 * Prints the UI and sensor values
 * 
 * @param pm25: The pm2.5 value to print
 * @param pm10: The pm10 value to print
 * @param temperature: The temperature value to print
 * @param humidity: The humidity value to print
 * @param pressure: The pressure value to print
 *  @param CO2: The CO2 value to print
 * 
 */
void printDisplay(const double pm25, const double pm10, const double temperature, const double humidity, const double pressure, const int CO2)
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
  drawCenteredText("PM10", tft.width()/2+2, tft.height()*2/3+2);
  drawCenteredText("PM2.5", tft.width()*3/2+2, tft.height()*2/3+2);
  drawCenteredText("um_g/m^3", tft.width() / 2, tft.height()*2/3+43);
  drawCenteredText("um_g/m^3", tft.width() * 3/2, tft.height()*2/3+43);

  // Draw sensor values
  tft.setTextSize(1);
  tft.setTextColor(ST7735_BLUE);
  drawCenteredText(String(temperature) + " C", tft.width(), 15);
  drawCenteredText(String(humidity) + " %", tft.width(), 35);
  drawCenteredText(String(pressure) + " hPa", tft.width(), 55);

  tft.setTextSize(2);
  tft.setTextColor(getCO2Color(CO2));
  drawCenteredText(String(CO2), tft.width(), tft.height()*5/12 + 17);
  tft.setTextColor(getPm10Color(pm10));
  drawCenteredText(String((int)pm10), tft.width() / 2, tft.height()*2/3+17);
  tft.setTextColor(getPm25Color(pm25));
  drawCenteredText(String((int)pm25), tft.width() * 3/2, tft.height()*2/3+17);
}

/**
 * Reads sensor values
 * 
 * Reads sensors and assigns the retrieved values to the passed pointers
 * @param pm25: Pointer to a double variable for pm25 values
 * @param pm10: Pointer to a double variable for pm10 values
 * @param temperature: Pointer to a double variable for temperature values
 * @param humidity: Pointer to a double variable for humidity values
 * @param pressure: Pointer to a double variable for pressure values
 * @param CO2: Pointer to an integer variable for CO2 values
 */
void readSensors(double* const pm25, double* const pm10, double* const temperature, double* const humidity, double* const pressure, int* const CO2)
{
  PmResult sds_results = sds.queryPm();
  if (sds_results.isOk()){
    *pm25 = sds_results.pm25;
    *pm10 = sds_results.pm10;
  } else {
    *pm25 = -1;
    *pm10 = -1;
  }

  *CO2 = myMHZ19.getCO2();

  *temperature = bme.readTemperature();
  *humidity = bme.readHumidity();
  *pressure = bme.readPressure() / 100.0;
}

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
  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode
  sds.wakeup();

  // Init MH-Z19C
  mhSerial.begin(9600, SERIAL_8N1, 32, 33);                               
  myMHZ19.begin(mhSerial);                                
  myMHZ19.autoCalibration(false); 

  // Init BME280
  bme.begin(0x76);

  initElegentOTA();
  logger =  new MQTTLogger( "io.adafruit.com", 1883, "XXXX",  "XXXX");
  logger->init();
}

void loop() 
{
  delay(15000); // TODO

  double pm25, pm10, temperature, humidity, pressure;
  int CO2;
  
 readSensors(&pm25, &pm10, &temperature, &humidity, &pressure, &CO2);
 printDisplay(pm25, pm10, temperature, humidity, pressure, CO2);
 logger->log(pm25, pm10, temperature, humidity, pressure, CO2);
  //AsyncElegantOTA.loop();
}
