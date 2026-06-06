#ifndef FLIGHT_SERVICE_H
#define FLIGHT_SERVICE_H

#include <Arduino.h>
#include <vector>
#include "Models.h"

class FlightService {
public:
    FlightService();
    bool fetchFlights();
    const std::vector<Aircraft>& getNearbyPlanes() const { return nearbyPlanes; }

private:
    std::vector<Aircraft> nearbyPlanes;
    const float MY_LAT = 12.975287;
    const float MY_LON = 77.689208;
    const float RADIUS_KM = 50.0;
};

#endif // FLIGHT_SERVICE_H
