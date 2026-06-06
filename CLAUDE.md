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
- **Application Entry**: `src/main.cpp` currently contains a "Hello World" example with a smooth 2-second slide animation.
- **Reference/Legacy Logic**: The `backup/main_backup.cpp` contains a more complex WiFi connection state machine (`SELECTING`, `TYPING`, `CONNECTING`, `FINISHED`) and a custom keyboard UI.
- **Core UI Component**: `src/keyboard.h` defines `M5Keyboard`, a Bruce-style layout for LCD interaction. It handles:
    - Cursor navigation via `BtnB` (horizontal) and `BtnPWR` (vertical).
    - Character entry and "Hold to Finish" via `BtnA`.
    - Smooth flickering-free redraws for UI elements.
- **Configuration**:
    - `platformio.ini`: Main configuration for the `m5stick-cplus2` environment and dependency management (`ArduinoJson`, `M5StickCPlus2`).
    - `src/credential.h`: Contains hardcoded WiFi credentials and backend (Ollama) URL.
- **File Layout**:
    - `src/`: Primary application source files.
    - `lib/`: Project-specific private libraries.
    - `include/`: Project-wide header files.

## Development Tips
- **Keyboard Navigation**:
    - `BtnB`: Horizontal movement.
    - `BtnPWR`: Vertical movement.
    - `BtnA`: Selection/Entry & "Hold to Finish".
- **Library Management**: `lib_ldf_mode = chain` is used in `platformio.ini` to optimize library dependency resolution.
