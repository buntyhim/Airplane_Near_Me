#ifndef MODELS_H
#define MODELS_H

#include <Arduino.h>
#include <String.h>
#include <vector>

struct Aircraft {
    String icao24;
    String callsign;
    float distance;
    String source;
    String destination;
};

#endif // MODELS_H
