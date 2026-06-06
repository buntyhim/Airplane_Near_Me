#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <M5StickCPlus2.h>
#include "Models.h"
#include "FlightService.h"
#include "WiFiManager.h"

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void update(FlightService& flightService, WiFiManager::State connState, unsigned long nextUpdateTime);
    void toggleScreen();
    void scroll(int delta, const std::vector<Aircraft>& planes);
    void drawStatus(WiFiManager::State connState, unsigned long nextUpdateTime);
    bool isScreenOn() const;
    void setNeedsRedraw(bool needs);

private:
    int scrollOffset;
    const int MAX_VISIBLE_LINES = 6;
    bool m_isScreenOn;
    bool m_needsRedraw;
    WiFiManager::State m_lastConnState;
    long m_lastRemaining;
};

#endif // DISPLAY_MANAGER_H
