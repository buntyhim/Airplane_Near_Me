#include "FlightService.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

FlightService::FlightService() {}

bool FlightService::fetchFlights() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Error: WiFi not connected");
        return false;
    }

    HTTPClient http;

    // Calculate bounding box for 50km radius
    float lat_delta = 50.0 / 111.0;
    float lon_delta = 50.0 / (111.0 * cos(MY_LAT * M_PI / 180.0));

    float lamin = MY_LAT - lat_delta;
    float lamax = MY_LAT + lat_delta;
    float lomin = MY_LON - lon_delta;
    float lomax = MY_LON + lon_delta;

    // Correct URL for OpenSky /states/all with bounding box
    String url = "https://opensky-network.org/api/states/all?lamin=" + String(lamin, 4) +
                  "&lomin=" + String(lomin, 4) +
                  "&lamax=" + String(lamax, 4) +
                  "&lomax=" + String(lomax, 4);

    Serial.println("Requesting: " + url);
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
        Serial.printf("HTTP Code: %d\n", httpCode);
        if (httpCode == 200) {
            String payload = http.getString();
            DynamicJsonDocument doc(16384);
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                nearbyPlanes.clear();
                JsonArray states = doc["states"];

                for (JsonArray state : states) {
                    Aircraft a;
                    // OpenSky /states/all returns a 2D array.
                    // We access values by index.
                    if (state.size() >= 10) {
                        a.icao24 = state[0].as<String>();
                        a.callsign = state[1].as<String>();
                        a.barHeight = state[7].as<float>();
                        a.speed = state[9].as<float>();
                        a.source = state[2].as<String>();
                        a.destination = "N/A";

                        // Calculate distance from my location (MY_LAT, MY_LON)
                        float lat1 = MY_LAT;
                        float lon1 = MY_LON;
                        float lat2 = state[6].as<float>();
                        float lon2 = state[5].as<float>();

                        // Haversine Formula
                        float dLat = (lat2 - lat1) * M_PI / 180.0;
                        float dLon = (lon2 - lon1) * M_PI / 180.0;
                        float rLat1 = lat1 * M_PI / 180.0;
                        float rLat2 = lat2 * M_PI / 180.0;

                        float a_haver = sin(dLat/2) * sin(dLat/2) +
                                       cos(rLat1) * cos(rLat2) *
                                       sin(dLon/2) * sin(dLon/2);
                        float c = 2 * atan2(sqrt(a_haver), sqrt(1 - a_haver));
                        a.distance = 6371000 * c / 1000.0; // Distance in km

                        nearbyPlanes.push_back(a);
                    }
                }
                http.end();
                return true;
            } else {
                Serial.print("JSON Parse Error: ");
                Serial.println(error.c_str());
            }
        } else {
            Serial.printf("Server returned error code: %d\n", httpCode);
        }
    } else {
        Serial.printf("Connection failed: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    return false;
}
