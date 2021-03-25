#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS         15 
#define TFT_RST        0                                
#define TFT_DC         2 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void drawCenteredText(String text, int x, int y){
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  tft.setCursor((x - w) / 2, y);
  tft.print(text);
}

void setup() {
  Serial.begin(9600);

  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST7735_BLACK);

  // Draw outlines
  tft.drawLine(0, 0, tft.width(), 0, ST7735_WHITE);
  tft.drawLine(0, 0, 0, tft.height(), ST7735_WHITE);
  tft.drawLine(0, tft.height()-1, tft.width()-1, tft.height()-1, ST7735_WHITE);
  tft.drawLine(tft.width()-1, 0, tft.width()-1, tft.height()-1, ST7735_WHITE);

  // Draw sections
  tft.drawLine(0, (int)tft.height()*5/12, tft.width(), (int)tft.height()*5/12, ST7735_WHITE);
  tft.drawLine(0, (int)tft.height()*2/3, tft.width(), (int)tft.height()*2/3, ST7735_WHITE);
  tft.drawLine((int)tft.width()/2, (int)tft.height()*2/3, (int)tft.width()/2, tft.height(), ST7735_WHITE);

  // Draw headers
  tft.setTextColor(ST7735_BLUE);
  drawCenteredText("Temperature", tft.width(), 5);
  drawCenteredText("Humidity", tft.width(), 25);
  drawCenteredText("Pressure", tft.width(), 45);

  // Draw sensor values
  tft.setTextColor(ST7735_WHITE);
  drawCenteredText("20.21 C", tft.width(), 15);
  drawCenteredText("25.67 %", tft.width(), 35);
  drawCenteredText("1024 hPa", tft.width(), 55);
  
}

void loop(){

}