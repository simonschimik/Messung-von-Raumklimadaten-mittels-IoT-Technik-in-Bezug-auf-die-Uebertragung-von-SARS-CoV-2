/**
 * @file MQTTLogger.cpp
 * @author Simon Schimik
 * @version 1.0
 */

#include "DataLoggingHandler.cpp"
#include "Arduino.h"
#include "config.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <exception>

/**
 * Exception thrown if no WiFi connection is available
 * 
 * Inherits from std::runtime_error, thrown if no WiFi connection available during critical tasks
 */
struct WifiNotConnectedException : public std::runtime_error
{
    WifiNotConnectedException(const char* msg) : 
      std::runtime_error(msg) {}
};

/**
 * Exception thrown if problems with the MQTT connection occur
 * 
 * Thrown if connection to broker failed
 */
struct MQTTConnectionFailedException : public std::runtime_error
{
    MQTTConnectionFailedException(const char* msg) :
      std::runtime_error(msg) {}
};

/**
 *  MQTTLogger class implementing DataLoggingHandler interface
 */
class MQTTLogger : public DataLoggingHandler
{
  private:
    WiFiClient wifiClient;
    Adafruit_MQTT_Client* mqttClient;

    /**
     * Connects to MQTT-Broker, if not already connected
     * 
     * @exception WiFiNotConnectedException Thrown if no WiFi connection available
     * @exception MQTTConnectionFailed Thrown if connecting to broker failed
     */
    void connectMQTT()
    {
      if(WiFi.status() != WL_CONNECTED) throw WifiNotConnectedException("WiFi not connected!");
      if(mqttClient->connected()) return;
      
      Serial.print("Connecting to MQTT… ");
      if(mqttClient->connect() != 0) throw MQTTConnectionFailedException("Couldn't connect to MQTT-Broker"); // connect will return 0 for connected
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
     * @exception MQTTConnectionFailed Thrown if connecting to broker failed
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
