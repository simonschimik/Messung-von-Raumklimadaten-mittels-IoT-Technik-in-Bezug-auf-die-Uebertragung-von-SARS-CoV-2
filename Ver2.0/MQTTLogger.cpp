#include "DataLoggingHandler.cpp"
#include "Arduino.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

class MQTTLogger : public DataLoggingHandler
{
  private:
    const char *aioServer;
    const int  aioServerport;
    const char *aioUsername;
    const char *aioKey;
    
    WiFiClient wifiClient;
    Adafruit_MQTT_Client *mqttClient;
    Adafruit_MQTT_Publish *pm25Feed;
    Adafruit_MQTT_Publish *pm10Feed;
    Adafruit_MQTT_Publish *tempFeed;
    Adafruit_MQTT_Publish *humFeed;
    Adafruit_MQTT_Publish *pressFeed;
    Adafruit_MQTT_Publish *CO2Feed;

    void connectMQTT()
    {
      
      if (mqttClient->connected()) return;
      
      Serial.print("Connecting to MQTT… ");
      while ((mqttClient->connect()) != 0) { // connect will return 0 for connected
        Serial.println("Retrying MQTT connection in 5 seconds…");
        mqttClient->disconnect();
        delay(5000); // wait 5 seconds
      }
      Serial.println("MQTT Connected!");
    }
    
  public:
    MQTTLogger(const char *aioServer, int  aioServerport, const char *aioUsername, const char *aioKey) :  aioServer(aioServer), aioServerport(aioServerport), aioUsername(aioUsername), aioKey(aioKey) {}

    void init()
    {
      mqttClient = new Adafruit_MQTT_Client(&wifiClient, aioServer, aioServerport, aioUsername, aioKey);
      pm25Feed = new Adafruit_MQTT_Publish(mqttClient, "XXXX/feeds/pm25"); // TODO
      pm10Feed = new Adafruit_MQTT_Publish(mqttClient, "XXXX/feeds/pm10"); // TODO
      tempFeed = new Adafruit_MQTT_Publish(mqttClient, "XXXX/feeds/temperature"); // TODO
      humFeed = new Adafruit_MQTT_Publish(mqttClient, "XXXX/feeds/humidity"); // TODO
      pressFeed = new Adafruit_MQTT_Publish(mqttClient, "XXXX/feeds/pressure"); // TODO
      CO2Feed = new Adafruit_MQTT_Publish(mqttClient, "XXXX/feeds/co2"); // TODO
      connectMQTT();
    }

    void log(double pm25, double pm10, double temperature, double humidity, double pressure, int CO2)
    {
      connectMQTT();
      pm25Feed->publish(pm25);
      pm10Feed->publish(pm10);
      tempFeed->publish(temperature);
      humFeed->publish(humidity);
      pressFeed->publish(pressure);
      CO2Feed->publish(CO2);
    }
};
