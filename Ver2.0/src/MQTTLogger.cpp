/**
 * @file MQTTLogger.cpp
 * @author Simon Schimik
 * @version 1.0
 */

#include "Arduino.h"
#include "config.h"
#include "Logger.h"

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <exception>

/**
 *  MQTTLogger class implementing Logger interface
 */
class MQTTLogger : public Logger
{
  private:
    WiFiClient wifiClient;
    Adafruit_MQTT_Client* mqttClient;

    /**
     * Connects to MQTT-Broker, if not already connected
     * 
     * @exception WiFiNotConnectedException Thrown if no WiFi connection available
     * @exception LoggerException Thrown if connecting to broker failed
     */
    void connectMQTT()
    {
      if(WiFi.status() != WL_CONNECTED) throw WifiNotConnectedException("WiFi not connected!");
      if(mqttClient->connected()) return;
      
      uint8_t reconnectCount = 0;
      Serial.print("Connecting to MQTT… ");
      while(mqttClient->connect() != 0){ // connect will return 0 for connected
        Serial.print(".");
        if(++reconnectCount > MQTT_CONNECT_LIMIT)throw LoggerException("Broker connection fail!", -1); 
      } 
      Serial.println("MQTT Connected!");

    }
    
  public:
    /**
     * Initialises MQTTLogger
     */
    MQTTLogger(){
      mqttClient = new Adafruit_MQTT_Client(&wifiClient, AIOSERVER, AIOSERVERPORT, AIOUSERNAME, AIOKEY);
    }

    /**
     * Publishes the current sensor values 
     * 
     * @exception WiFiNotConnectedException Thrown if no WiFi connection available
     * @exception LoggerException TThrown if connecting to broker failed
     * @param sensorData the sensor values to be published
     */
    void log(const std::map<const char*, double>* sensorData)
    {
      for(auto const& iter : *sensorData)
      {
        
        char* feed = (char*) malloc(strlen(AIOUSERNAME) + strlen("/feeds/") + strlen(iter.first) + 1);
        strcpy(feed, AIOUSERNAME);
        strcat(feed, "/feeds/");
        strcat(feed, iter.first);

        char* payload = (char*) malloc(41); // TODO
        dtostrf(iter.second, 0, 2, payload);

        connectMQTT();
        mqttClient->publish(feed, payload, 0);

        free(feed);
        free(payload);
      }
    }
};
