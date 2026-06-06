#include <Arduino.h>
#include <M5StickCPlus2.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <math.h>

#include "WiFiManager.h"
#include "FlightService.h"
#include "DisplayManager.h"

// ------------------------------------------------------------------
// Global variables
// ------------------------------------------------------------------
WiFiManager wifiMgr;
FlightService flightSvc;
DisplayManager displayMgr;

unsigned long nextUpdateTime = 60000; // Set to 60 seconds as requested

// ------------------------------------------------------------------
// Helper functions
// ------------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    auto cfg = M5.config();
    M5.begin(cfg);
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BLACK);

    wifiMgr.begin();
    displayMgr.begin();

    Serial.println("System Started");
}

void loop() {
    M5.update();

    // Handle Button A: Toggle Screen & Connect WiFi
    if (M5.BtnA.wasClicked()) {
        Serial.println("Button A Pressed");
        if (WiFi.status() != WL_CONNECTED) {
            wifiMgr.connect();
        }
        displayMgr.toggleScreen();
    }

    // Handle Button B: Scroll
    if (M5.BtnB.wasClicked()) {
        Serial.println("Button B Pressed");
        displayMgr.scroll(1, flightSvc.getNearbyPlanes());
    }

    // Update Logic
    if (displayMgr.isScreenOn()) {
        // Fetch data if timer expired
        if (millis() > nextUpdateTime) {
            if (wifiMgr.isConnected()) {
                Serial.println("Fetching new flight data...");
                if (flightSvc.fetchFlights()) {
                    nextUpdateTime = millis() + 60000; // Set next update to 60s
                    displayMgr.setNeedsRedraw(true);
                    Serial.println("Data updated successfully.");
                } else {
                    Serial.println("Failed to fetch data.");
                }
            } else {
                if (wifiMgr.getState() == WiFiManager::IDLE_CONN) {
                    Serial.println("WiFi disconnected, attempting to connect...");
                    wifiMgr.connect();
                }
                nextUpdateTime = millis() + 5000;
            }
        }

        displayMgr.update(flightSvc, wifiMgr.getState(), nextUpdateTime);
    }
}
