/**
 * @file Logger.h
 * @author Simon Schimik
 * @version 1.0
 */

#pragma once
#include "config.h"
#include <exception>


/**
 * Defines Interface Logger .
 * 
 * The abstract class Logger has the pure virutal function log().
 * This means that no object of type Logger can be created.
 * If a subclass inherits Logger, it is forced to implement the function log(), and possibly other functions that might be added in the future.
 * The general idea behind this is that by defining an abstract class with only pure virutal functions (Java-Interface-equivalent) polymorphism can be utilized.
 * This allows for an easy implementation and use of additional Loggers-subclasses, without implying large-scale rewrites where the original Logger was used.
 */
class Logger{
    public:
        virtual void log(const std::map<const char*, double>* sensorData) = 0;
};

/**
 * Exception thrown if any various errors occur during logging
 * 
 * Inherits from std::runtime_error, thrown if a severe error occurs during critical tasks
 */
struct LoggerException : public std::runtime_error
{   
    public:
        int16_t error;
    LoggerException(const char* msg, const uint16_t error) : 
      std::runtime_error(msg), error(error) {}
};

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
