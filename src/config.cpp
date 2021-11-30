#include "config.h"
#include <sstream>

ConfigClass::ConfigClass() {
    parameters = {
        {"seuil", &_threshold},
        {"nuit", &_night},
        {"jour", &_day},
        {"tempo1", &_delay1},
        {"tempo2", &_delay2},
        #ifdef SALLE_PRINTEMPS
        {"tempo3", &_delay3},
        #endif

    };
}


bool ConfigClass::begin()
{
    preferences.begin("config", false);
    for(const auto p: parameters) {
        if(preferences.isKey(p.first.c_str()))
            *p.second = preferences.getFloat(p.first.c_str());
        else
            return false;
    }
    return true;
}

bool ConfigClass::define(char *name, float value)
{
    if(parameters.count(name) == 0)
        return false;
    preferences.putFloat(name, value);
    *parameters[name] = value;
    return true;
}

std::string ConfigClass::dump()
{
    std::stringstream result;
    for(const auto p: parameters) {
        result << p.first << ":\t";
        if(preferences.isKey(p.first.c_str()))
            result << preferences.getFloat(p.first.c_str());
        else
             result << "?";
        result << std::endl;
    }
    return result.str();
}

float ConfigClass::night() { return _night; }
float ConfigClass::day() { return _day; }
uint16_t ConfigClass::threshold() { return _threshold; }
float ConfigClass::delay1() { return _delay1; }
float ConfigClass::delay2() { return _delay2; }
#ifdef SALLE_PRINTEMPS
float ConfigClass::delay3() { return _delay3; }
#endif

ConfigClass Config;