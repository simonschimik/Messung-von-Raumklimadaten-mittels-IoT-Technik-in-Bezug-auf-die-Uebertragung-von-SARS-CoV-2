#pragma once
#include "config.h"

class DataLoggingHandler{
    public:
        virtual void log(const std::map<const char*, double>* sensorData) = 0;
        virtual void init(const std::map<const char*, double>* sensorData) = 0;
};
