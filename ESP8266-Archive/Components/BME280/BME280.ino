#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;
char buffer[50];

void setup() {
  Serial.begin(115200);
  bme.begin(0x76);
}
void loop() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  sprintf(buffer, "Temperature: %-5.2f\nHumidity: %-5.2f\nPressure: %-5.2f\n\n\n\n\n", temperature, humidity, pressure);
  Serial.println(buffer);
  delay(1000);
}
