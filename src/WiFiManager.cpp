#include "WiFiManager.h"
#include "credential.h"

WiFiManager::WiFiManager() : currentState(IDLE_CONN) {}

void WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    currentState = IDLE_CONN;
}

void WiFiManager::connect() {
    if (WiFi.status() == WL_CONNECTED) {
        currentState = CONNECTED_CONN;
        return;
    }

    currentState = CONNECTING;
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        currentState = CONNECTED_CONN;
    } else {
        currentState = IDLE_CONN;
    }
}
