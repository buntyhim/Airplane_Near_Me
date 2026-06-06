#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <M5StickCPlus2.h>

class M5Keyboard {
private:
    static const int COLS = 15;
    static const int ROWS = 4;
    static const int TOP_BAR_HEIGHT = 30;

    // Custom Bruce-style Layout
    const char _layout[ROWS][COLS] = {
      {'q','w','e','r','t','y','u','i','o','p',  '[',']',  '7','8','9'},
      {'a','s','d','f','g','h','j','k','l',';',  '\'','\\','4','5','6'},
      {'z','x','c','v','b','n','m',',','*','/',  '-','+',  '1','2','3'},
      {'!','?','@','#','$','%','^','&','(',')',  '.','<',  ' ','0','='} 
    };

    int _cursorX = 0, _cursorY = 0;
    String _result = "";
    bool _isDone = false;
    unsigned long _lastBlink = 0;
    bool _blinkState = true;

    // Internal helper to draw specific keys (Flicker-free)
    void drawKey(int r, int c, bool selected) {
        int kw = 240 / COLS;
        int kh = (135 - TOP_BAR_HEIGHT) / ROWS;
        int x = c * kw;
        int y = TOP_BAR_HEIGHT + (r * kh);
        
        uint16_t bg = selected ? WHITE : ((c >= 12) ? 0x18C3 : BLACK);
        M5.Lcd.fillRect(x + 1, y + 1, kw - 2, kh - 2, bg);
        if (!selected) M5.Lcd.drawRect(x, y, kw, kh, 0x3186);

        M5.Lcd.setTextColor(selected ? BLACK : WHITE);
        M5.Lcd.setCursor(x + 4, y + 6);
        char k = _layout[r][c];
        if (k == '<') { M5.Lcd.setTextSize(1); M5.Lcd.print("BS"); }
        else if (k == ' ') { M5.Lcd.setTextSize(1); M5.Lcd.print("SP"); }
        else { M5.Lcd.setTextSize(2); M5.Lcd.printf("%c", k); }
    }

    // Internal helper to redraw text area
    void drawTop() {
        M5.Lcd.fillRect(0, 0, 240, TOP_BAR_HEIGHT - 2, WHITE);
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(5, 7);
        M5.Lcd.print(_result + (_blinkState ? "_" : " "));
    }

public:
    // Call this to initialize the keyboard screen
    void begin(String initialText = "") {
        _result = initialText;
        _isDone = false;
        M5.Lcd.fillScreen(BLACK);
        for (int r = 0; r < ROWS; r++) 
            for (int c = 0; c < COLS; c++) 
                drawKey(r, c, (r == _cursorY && c == _cursorX));
        
        drawTop();
    }

    // Call this in the main loop(). Returns true when user holds BtnA to finish.
    bool update() {
        if (_isDone) return true;

        int oldX = _cursorX, oldY = _cursorY;
        bool moved = false, textChanged = false;

        // Blink Timer
        if (millis() - _lastBlink > 500) {
            _blinkState = !_blinkState;
            _lastBlink = millis();
            textChanged = true;
        }

        // Navigation Logic
        if (M5.BtnB.wasClicked()) { // Click -> Right
            _cursorX = (_cursorX + 1) % COLS; 
            moved = true; 
        }
        if (M5.BtnB.pressedFor(400)) { // Hold -> Left
            _cursorX = (_cursorX - 1 + COLS) % COLS; 
            moved = true; 
            delay(100); 
        }
        if (M5.BtnPWR.wasClicked()) { // Power Click -> Down
            _cursorY = (_cursorY + 1) % ROWS; 
            moved = true; 
        }

        // Selection / Finish Logic
        if (M5.BtnA.wasHold()) { 
            _isDone = true; 
            return true; 
        }
        if (M5.BtnA.wasClicked()) {
            char k = _layout[_cursorY][_cursorX];
            if (k == '<') { 
                if (_result.length() > 0) _result.remove(_result.length() - 1); 
            }
            else { _result += k; }
            _blinkState = true; // Force cursor visible when typing
            textChanged = true;
        }

        // Partial screen updates for smoothness
        if (moved) { 
            drawKey(oldY, oldX, false); 
            drawKey(_cursorY, _cursorX, true); 
        }
        if (textChanged) drawTop();
        
        return false;
    }

    String getString() { return _result; }
};

#endif