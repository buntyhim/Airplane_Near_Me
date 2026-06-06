#include "DisplayManager.h"

DisplayManager::DisplayManager() : scrollOffset(0), m_isScreenOn(true), m_needsRedraw(true), m_lastConnState(WiFiManager::IDLE_CONN), m_lastRemaining(-1) {}

void DisplayManager::begin() {
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BLACK);
}

void DisplayManager::drawStatus(WiFiManager::State connState, unsigned long nextUpdateTime) {
    long remaining = (long)(nextUpdateTime - millis()) / 1000;
    if (remaining < 0) remaining = 0;

    // Only redraw the status area if something actually changed
    if (connState != m_lastConnState || remaining != m_lastRemaining) {
        // Clear the top status bar
        M5.Lcd.fillRect(0, 0, 240, 20, BLACK);

        // WiFi Status (Left)
        M5.Lcd.setCursor(10, 0);
        if (connState == WiFiManager::CONNECTING) {
            M5.Lcd.setTextColor(WHITE);
            M5.Lcd.print("Connecting...");
        } else if (connState == WiFiManager::CONNECTED_CONN) {
            M5.Lcd.setTextColor(GREEN);
            M5.Lcd.print("Connected");
        } else {
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Disconnected");
        }

        // Countdown (Middle)
        M5.Lcd.setCursor(130, 0);
        M5.Lcd.setTextColor(YELLOW);
        M5.Lcd.printf("Upd: %ds", remaining);

        // Battery (Right)
        int battery = M5.Power.getBatteryLevel();
        M5.Lcd.setCursor(180, 0);
        M5.Lcd.setTextColor(YELLOW);
        M5.Lcd.printf("Bat: %d%%", battery);

        m_lastConnState = connState;
        m_lastRemaining = remaining;
    }
}

void DisplayManager::update(FlightService& flightService, WiFiManager::State connState, unsigned long nextUpdateTime) {
    if (!m_isScreenOn) return;

    drawStatus(connState, nextUpdateTime);

    M5.Lcd.setCursor(0, 30);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("------------------------------------");
    M5.Lcd.println("Callsign    | Alt   | Spd  | Dist");
    M5.Lcd.println("------------------------------------");

    if (m_needsRedraw) {
        M5.Lcd.fillRect(0, 75, 240, 120, BLACK);

        const auto& planes = flightService.getNearbyPlanes();
        for (int i = 0; i < MAX_VISIBLE_LINES; i++) {
            int idx = scrollOffset + i;
            if (idx < (int)planes.size()) {
                M5.Lcd.setCursor(0, 75 + (i * 12));
                M5.Lcd.printf("%-10s | %-7d | %-5d | %-5.1f\n",
                    planes[idx].callsign.c_str(),
                    (int)(planes[idx].barHeight / 10.0) * 10,
                    (int)planes[idx].speed,
                    planes[idx].distance);
            }
        }
        m_needsRedraw = false;
    }
}

void DisplayManager::toggleScreen() {
    if (!m_isScreenOn) {
        m_isScreenOn = true;
        M5.Lcd.fillScreen(BLACK);
    } else {
        m_isScreenOn = false;
        M5.Lcd.clear();
    }
}

void DisplayManager::scroll(int delta, const std::vector<Aircraft>& planes) {
    if (!m_isScreenOn) return;
    scrollOffset += delta;

    // Bounds checking
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > (int)planes.size() - MAX_VISIBLE_LINES) {
        scrollOffset = (int)planes.size() - MAX_VISIBLE_LINES;
        if (planes.size() < MAX_VISIBLE_LINES) scrollOffset = 0;
    }

    m_needsRedraw = true;
}

void DisplayManager::setNeedsRedraw(bool needs) {
    m_needsRedraw = needs;
}

bool DisplayManager::isScreenOn() const {
    return m_isScreenOn;
}
