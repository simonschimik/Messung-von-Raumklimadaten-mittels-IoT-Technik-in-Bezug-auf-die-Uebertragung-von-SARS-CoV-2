#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "SdsDustSensor.h" // Library for novaPM SDS011
#include "MHZ19.h" // Library for MH-Z19C                         
#include <SoftwareSerial.h> 
#include <SPI.h>

// TFT-Pins
#define TFT_CS 15 
#define TFT_RST 0                                
#define TFT_DC 2 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SdsDustSensor sds(SDS_RX, SDS_TX);

void drawCenteredText(String text, int x, int y){
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  tft.setCursor((x - w) / 2, y);
  tft.print(text);
}

long getPm10Color(double pm10){
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

long getPm25Color(double pm25){
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

long getCO2Color(int co2){
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

void setup() {
  Serial.begin(9600);
  // Init MH-Z19C
  mySerial.begin(BAUDRATE);                               
  myMHZ19.begin(mySerial);                               
  myMHZ19.autoCalibration(false); 

  // Init SDS
  sds.begin();
  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode
  sds.wakeup();

  

  // Init TFT
  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  drawCenteredText("init sensors", tft.width(), tft.height()/2);
  
}

void loop(){
  delay(2000);

  // Retrieve sensor values
  PmResult sds_results = sds.queryPm();
  double pm25;
  double pm10;
  if (sds_results.isOk()){
    pm25 = sds_results.pm25;
    pm10 = sds_results.pm10;
  } else {
  }

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
  drawCenteredText("20.21 C", tft.width(), 15);
  drawCenteredText("25.67 %", tft.width(), 35);
  drawCenteredText("1024 hPa", tft.width(), 55);

  tft.setTextSize(2);
  tft.setTextColor(getCO2Color(CO2));
  drawCenteredText(String(CO2), tft.width(), tft.height()*5/12 + 17);
  tft.setTextColor(getPm10Color(pm10));
  drawCenteredText(String((int)pm10), tft.width() / 2, tft.height()*2/3+17);
  tft.setTextColor(getPm10Color(pm25));
  drawCenteredText(String((int)pm25), tft.width() * 3/2, tft.height()*2/3+17);
}