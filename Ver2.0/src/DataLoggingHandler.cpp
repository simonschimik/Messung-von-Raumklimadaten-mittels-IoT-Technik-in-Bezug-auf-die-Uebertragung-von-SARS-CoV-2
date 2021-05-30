/**
 * @file DataLoggingHandler.cpp
 * @author Simon Schimik
 * @version 2.0
 */

#pragma once
#include "config.h"

/**
 * Defines Interface DataLoggingHandler 
 * 
 * The abstract class DataLoggingHandler has the pure virutal function log()
 * This means that no object of type DataLoggingHandler can be created
 * If a subclass inherits DataLoggingHandler, it is forced to implement the function log(), and possibly other functions that might be added in the future
 * The general idea behind this is that by defining an abstract class with only pure virutal functions (Java-Interface-equivalent) polymorphism can be utilized
 * This allows for an easy implementation and use of additional DataLoggingHandlers-subclasses, without implying large-scale rewrites where the original DataLoggingHandler was used
 */
class DataLoggingHandler{
    public:
        virtual void log(const std::map<const char*, double>* sensorData) = 0;
};
