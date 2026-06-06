# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Common Development Tasks
- **Build**: `pio run`
- **Upload**: `pio run -t upload`
- **Monitor**: `pio device monitor`
- **Clean**: `pio run -t -C .pio/build/m5stick-cplus2/clean`
- **Build Specific Environment**: `pio run -e m5stick-cplus2`
- **Run Tests**: `pio test` (Note: Test suite is currently empty; refer to `test/` directory)

## Code Architecture & Structure
- **Project Type**: ESP32 Arduino project using PlatformIO and M5StickCPlus2 library.
- **Application Entry**: `src/main.cpp` - Main system orchestration.
- **Modules**:
    - `WiFiManager`: Handles WiFi connection and state tracking.
    - `FlightService`: Interfaces with OpenSky Network API to fetch and process aircraft data.
    - `DisplayManager`: Handles all UI rendering, including the flight list and status bar.
- **Data Models**: `src/Models.h` contains the `Aircraft` struct.
- **Configuration**:
    - `platformio.ini`: PlatformIO configuration.
    - `src/credential.h`: Contains WiFi credentials.
- **File Layout**:
    - `src/`: Primary application source files.
    - `lib/`: Project-specific private libraries.
    - `include/`: Project-wide header files.

## Development Tips
- **Button Interactions**:
    - **BtnA**: Toggles screen and handles WiFi connection.
    - **BtnB**: Click to scroll down, Long press to scroll up.
- **Display Rendering**: Uses a `m_needsRedraw` flag to minimize flicker.
- **Library Management**: `lib_ldf_mode = chain` is used in `platformio.ini` to optimize library dependency resolution.
