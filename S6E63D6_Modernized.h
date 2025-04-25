#ifndef _S6E63D6_MODERNIZED_H_
#define _S6E63D6_MODERNIZED_H_

#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Arduino.h>

// Define display size
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240

class S6E63D6 : public Adafruit_GFX {
public:
    S6E63D6(uint8_t i2cAddress);
    void begin();
    void reset();
    void writeCommand(uint8_t command);
    void writeData(uint16_t data);
    void clearScreen(uint16_t color);
    void setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

private:
    uint8_t _i2cAddress;
    uint16_t _portState; // Keeps track of the port state of PCF8575
    void updatePort();
    void setControlPin(uint8_t pin, bool value);
    void write8(uint8_t data);
};

#endif // _S6E63D6_MODERNIZED_H_