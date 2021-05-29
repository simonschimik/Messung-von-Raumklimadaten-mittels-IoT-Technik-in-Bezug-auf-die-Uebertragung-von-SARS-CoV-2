/**
 * @file MQTTLogger.cpp
 * @author Simon Schimik
 * @version 2.0
 */

#include "DataLoggingHandler.cpp"
#include "Arduino.h"
#include "config.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

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
 * Thrown either if connection to broker or publishing to a feed failed
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
    
    /** Pointer to a std::map storing MQTT-feeds */
    std::map<String, Adafruit_MQTT_Publish>* feeds;

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
     * Initiales MQTTLogger
     * 
     * Initiales mqttClient and feeds
     * Copies the passed sensorData map and keeps the keys, but changes the values to be Adafruit_MQTT_Publish's 
     * The general idea behind this is that the MQTT-feeds are creating using the keys of sensorData and that they can be accessed using the same keys
     * @param sensorData std::map used to initialise the std::map feeds and the Adafruit_MQTT_Publish's
     */
    MQTTLogger(const std::map<const char*, double>* sensorData){
      
      mqttClient = new Adafruit_MQTT_Client(&wifiClient, AIOSERVER, AIOSERVERPORT, AIOUSERNAME, AIOKEY);
      feeds = new std::map<String, Adafruit_MQTT_Publish>;

      for(auto const& iter : *sensorData)
      {
        feeds->insert({iter.first, Adafruit_MQTT_Publish(mqttClient, (String(AIOUSERNAME) + "/feeds/" + String(iter.first)).c_str() )});
      }
    }

    /**
     * Publishes the current sensor values 
     * 
     * @exception WiFiNotConnectedException Thrown if no WiFi connection available
     * @exception MQTTConnectionFailed Thrown if connecting to broker failed or publish to a feed failed
     * @param sensorData the sensor values to be published
     */
    void log(const std::map<const char*, double>* sensorData)
    {
      connectMQTT();
      for(auto const& iter : *sensorData)
      {
        if(!feeds->at(iter.first).publish(iter.second)) throw MQTTConnectionFailedException("Publishing to a feed failed!");
      }
    }
};
