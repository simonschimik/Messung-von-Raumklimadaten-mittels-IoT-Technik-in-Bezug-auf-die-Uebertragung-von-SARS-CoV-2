#include "MHZ19.h"                                        

#define RX_PIN 33                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 32                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      

MHZ19 myMHZ19;                                            
HardwareSerial mySerial(1); // Use UART channel 1  

void setup() {
  Serial.begin(9600);                                     
  mySerial.begin(BAUDRATE, SERIAL_8N1, 32, 33);                               
  myMHZ19.begin(mySerial);                                
  myMHZ19.autoCalibration(false); 
  Serial.println("Preheating sensor!");
  //delay(120000);

}

void loop() {
  int CO2; 
  CO2 = myMHZ19.getCO2();
  Serial.print("CO2 (ppm): ");                      
  Serial.println(CO2);
  
  delay(2000);                                      
}
