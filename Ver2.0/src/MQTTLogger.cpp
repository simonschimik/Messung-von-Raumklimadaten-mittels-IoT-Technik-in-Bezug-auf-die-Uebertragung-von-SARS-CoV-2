#include "DataLoggingHandler.cpp"
#include "Arduino.h"
#include "config.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

struct WifiNotConnectedException : public std::runtime_error
{
    WifiNotConnectedException(const char* msg) : 
      std::runtime_error(msg) {}
};

struct MQTTConnectionFailedException : public std::runtime_error
{
    MQTTConnectionFailedException(const char* msg) :
      std::runtime_error(msg) {}
};

class MQTTLogger : public DataLoggingHandler
{
    const char *aioServer;
    const int aioServerport;
    const char *aioUsername;
    const char *aioKey;
    
    WiFiClient wifiClient;
    Adafruit_MQTT_Client* mqttClient;
    std::map<String, Adafruit_MQTT_Publish>* feeds;

    void connectMQTT()
    {
      if(WiFi.status() != WL_CONNECTED) throw WifiNotConnectedException("WiFi not connected!");
      if(mqttClient->connected()) return;
      
      Serial.print("Connecting to MQTT… ");
      if(mqttClient->connect() != 0) { // connect will return 0 for connected
        throw MQTTConnectionFailedException("Couldn't connect to MQTT-Broker");
      }
      Serial.println("MQTT Connected!");

    }
    
  public:
    MQTTLogger(const char *aioServer, int  aioServerport, const char *aioUsername, const char *aioKey) :  aioServer(aioServer), aioServerport(aioServerport), aioUsername(aioUsername), aioKey(aioKey) {}
    
    void init(const std::map<const char*, double>* sensorData)
    {
      feeds = new std::map<String, Adafruit_MQTT_Publish>;
      mqttClient = new Adafruit_MQTT_Client(&wifiClient, aioServer, aioServerport, aioUsername, aioKey);

      for(auto const& iter : *sensorData)
      {
        feeds->insert({iter.first, Adafruit_MQTT_Publish(mqttClient, AIOUSERNAME "/feeds/pm25")});
      }

    }

    void log(const std::map<const char*, double>* sensorData)
    {
      connectMQTT();
    }
};
