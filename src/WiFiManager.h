#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager {
public:
    enum State { IDLE_CONN, CONNECTING, CONNECTED_CONN };

    WiFiManager();
    void begin();
    void connect();
    State getState() const { return currentState; }
    bool isConnected() const { return currentState == CONNECTED_CONN; }
    String getSSID() const { return WiFi.SSID(); }

private:
    State currentState;
};

#endif // WIFI_MANAGER_H
