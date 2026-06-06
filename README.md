# M5Stick Flight Tracker

A modular project for the M5StickCPlus2 that fetches and displays real-time flight data from the OpenSky Network API.

## Features
- **Modular Architecture**: Separate components for WiFi management, flight service, and UI display.
- **Real-time Tracking**: Fetches aircraft within a 50km radius of a specified location.
- **Clean UI**: 
    - Vertical scrolling list of flights.
    - Status bar showing WiFi connection, battery level, and a countdown timer.
    - Smooth, flicker-free screen updates.
- **OpenSky API Integration**: Uses the `/states/all` endpoint with bounding box logic.

## Hardware Requirements
- M5StickCPlus2
- WiFi connection ("Halbach" SSID)

## Build & Run
1.  Ensure you have PlatformIO installed.
2.  Connect your M5Stick to your computer (COM5).
3.  Run the build and upload:
    ```bash
    python -m platformio run -e m5stick-cplus2 -t upload
    ```
4.  To monitor the serial output:
    ```bash
    python -m platformio device monitor -b 115200
    ```

## Project Structure
- `src/Models.h`: Data structures.
- `src/WiFiManager.h/cpp`: WiFi connection handling.
- `src/FlightService.h/cpp`: OpenSky API interaction.
- `src/DisplayManager.h/cpp`: UI rendering and logic.
- `src/main.cpp`: System orchestration.
