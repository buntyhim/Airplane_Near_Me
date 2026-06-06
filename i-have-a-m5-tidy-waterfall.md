# Context
The goal is to transform the current "mock" flight display project into a functional, modular application. The system will connect to "Halbach Wifi", fetch real-time flight data from the OpenSky Network API within a 50km radius, and present it in a clean UI. The UI will feature a vertical scrolling list of flights and a 30-second countdown timer for data refreshes.

# Architecture
The code will be modularized into the following components:
- `Models.h`: Common data structures.
- `WiFiManager`: Handles connection to "Halbach Wifi" and connection state.
- `FlightService`: Handles HTTP requests to OpenSky API and JSON parsing.
- `DisplayManager`: Manages the LCD screen, button scrolling, and countdown timer.
- `main.cpp`: Orchestrates the system.

# Implementation Steps
1. **Define Data Models**: Create `src/Models.h` to define `Aircraft` and `FlightData` structures.
2. **Create WiFiManager**: 
    - Create `src/WiFiManager.h/cpp`.
    - Implement connection logic using `WiFi.h`.
    - Use the "Halbach Wifi" credentials from `src/credential.h`.
3. **Create FlightService**:
    - Create `src/FlightService.h/cpp`.
    - Implement `fetchFlights()` using `HTTPClient.h` and `ArduinoJson.h`.
    - Implement OpenSky API endpoint: `https://opensky-network.org/api/place.json?lat={lat}&lon={lon}&radius=50000`.
    - Parse the JSON response to populate the `Aircraft` list.
4. **Create DisplayManager**:
    - Create `src/DisplayManager.h/cpp`.
    - Implement a vertical scrolling list for `Aircraft` data.
    - Implement a 30-second countdown timer.
    - Ensure the screen refreshes on button press for scrolling.
    - Ensure data refreshes only when the timer hits zero.
5. **Refactor main.cpp**:
    - Remove mock data and existing WiFi logic.
    - Initialize and run the new modules.

# Critical Files
- `src/Models.h` (New)
- `src/WiFiManager.h` (New)
- `src/WiFiManager.cpp` (New)
- `src/FlightService.h` (New)
- `src/FlightService.cpp` (New)
- `src/DisplayManager.h` (New)
- `src/DisplayManager.cpp` (New)
- `src/main.cpp` (Update)

# Verification
- Verify WiFi connection to "Halbach Wifi".
- Verify successful fetch from OpenSky API (check JSON parsing).
- Verify 30-second countdown and automatic data refresh.
- Verify button-based scrolling and UI responsiveness.
