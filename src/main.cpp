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

    Serial.println("Connecting to WiFi...");
    wifiMgr.connect();

    // Block until connected before entering the main loop
    while (!wifiMgr.isConnected()) {
        Serial.println("Waiting for connection...");
        delay(1000);
    }

    Serial.println("Connected! Fetching initial data...");
    if (flightSvc.fetchFlights()) {
        nextUpdateTime = millis() + 60000; // Set next update to 60s
        displayMgr.setNeedsRedraw(true);
        Serial.println("Initial data fetched successfully.");
    } else {
        Serial.println("Failed to fetch initial data.");
        nextUpdateTime = millis() + 60000; // Still set timer so we don't spam
    }

    Serial.println("Starting loop.");
}

void loop() {
    M5.update();

    // Handle Button A: Toggle Screen
    static bool btnA_handled = false;
    if (M5.BtnA.wasClicked()) {
        if (!btnA_handled) {
            btnA_handled = true;
            Serial.println("Button A Pressed");
            displayMgr.toggleScreen();
        }
    }
    if (!M5.BtnA.wasClicked()) {
        btnA_handled = false;
    }

    // Handle Button B: Scroll
    if (M5.BtnB.wasClicked()) {
        Serial.println("Button B Clicked");
        displayMgr.scroll(1, flightSvc.getNearbyPlanes());
    }
    if (M5.BtnB.pressedFor(400)) {
        Serial.println("Button B Long Pressed");
        displayMgr.scroll(-1, flightSvc.getNearbyPlanes());
    }

    // Update Logic
    if (displayMgr.isScreenOn()) {
        // Fetch data if timer expired
        if (millis() > nextUpdateTime) {
            // We are guaranteed to be connected because we blocked in setup
            Serial.println("Fetching new flight data...");
            if (flightSvc.fetchFlights()) {
                nextUpdateTime = millis() + 60000; // Set next update to 60s
                displayMgr.setNeedsRedraw(true);
                Serial.println("Data updated successfully.");
            } else {
                Serial.println("Failed to fetch data.");
            }
        }

        displayMgr.update(flightSvc, wifiMgr.getState(), nextUpdateTime);
    }
}
