/**
 * @file HTTPLogger.cpp
 * @author Simon Schimik
 * @version 3.0
 */

#include "Arduino.h"
#include "config.h"
#include "Logger.h"

#include <HTTPClient.h>


class HTTPLogger : public Logger
{
    private:
        WiFiClient wifiClient;
        HTTPClient httpClient;

    public:
        HTTPLogger(){};


        /**
         * Publishes the current sensor values 
         * 
         * @exception WiFiNotConnectedException Thrown if no WiFi connection available
         * @exception LoggerException Thrown if HTTP-POST returned an invalid response code
         * @param sensorData the sensor values to be published
         */
    void log(const std::map<const char*, double>* sensorData)
    {
        String payload = "{";   
        for(auto const& iter : *sensorData)
        {
            payload += "\"" + String(iter.first) + "\"" + ":\"" + String(iter.second) + "\",";
        }
        payload += "\"mac\":\"" + String(WiFi.macAddress()) + "\"}";
        payload = payload.substring(0, payload.length()-1);
        payload += "}";

        httpClient.begin(HTTPSERVER);
        httpClient.addHeader("Content-Type", "application/json");
        int httpResponseCode = httpClient.POST(payload);

        if(httpResponseCode != 200){
            throw LoggerException(httpClient.errorToString(httpResponseCode).c_str(), httpResponseCode);
        }

        httpClient.end();
    }
};
