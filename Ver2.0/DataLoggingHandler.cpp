 #pragma once

class DataLoggingHandler{
    public:
        virtual void log(const double pm25, const double pm10, const double temperature, const double humidity, const double pressure, const int CO2) = 0;
        virtual void init() = 0;
};
