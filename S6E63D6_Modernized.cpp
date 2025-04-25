#include "S6E63D6_Modernized.h"
#include <Wire.h>

// Define the control pins on the upper byte of the PCF8575
#define PIN_CS     8  // Bit 8 for CS
#define PIN_RESET  9  // Bit 9 for RESET
#define PIN_RW     10 // Bit 10 for RW
#define PIN_DC     11 // Bit 11 for D/C

S6E63D6::S6E63D6(uint8_t i2cAddress)
    : Adafruit_GFX(DISPLAY_WIDTH, DISPLAY_HEIGHT), _i2cAddress(i2cAddress), _portState(0xFFFF) {}

void S6E63D6::begin() {
    Wire.begin();
    reset();

    // Initialize the display (refer to datasheet for exact commands)
    writeCommand(0x01); // Software reset
    delay(120);
    writeCommand(0x11); // Exit sleep mode
    delay(120);
    writeCommand(0x29); // Display ON
}

void S6E63D6::reset() {
    setControlPin(PIN_RESET, LOW);
    updatePort();
    delay(10);
    setControlPin(PIN_RESET, HIGH);
    updatePort();
    delay(10);
}

void S6E63D6::writeCommand(uint8_t command) {
    setControlPin(PIN_DC, LOW); // Command mode
    updatePort();
    write8(command);
}

void S6E63D6::writeData(uint16_t data) {
    setControlPin(PIN_DC, HIGH); // Data mode
    updatePort();
    write8(data >> 8);           // Send high byte
    write8(data & 0xFF);         // Send low byte
}

void S6E63D6::clearScreen(uint16_t color) {
    setWindow(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
    for (uint32_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        writeData(color);
    }
}

void S6E63D6::setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    writeCommand(0x2A); // Column address set
    writeData(x1);
    writeData(x2);
    writeCommand(0x2B); // Row address set
    writeData(y1);
    writeData(y2);
    writeCommand(0x2C); // Memory write
}

void S6E63D6::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= DISPLAY_WIDTH) || (y < 0) || (y >= DISPLAY_HEIGHT)) {
        return;
    }
    setWindow(x, y, x, y);
    writeData(color);
}

void S6E63D6::write8(uint8_t value) {
    // Write the data byte to the lower byte of the port
    _portState = (_portState & 0xFF00) | value;

    // Pulse RW pin for 6800 write cycle
    setControlPin(PIN_RW, LOW);
    updatePort();
    delayMicroseconds(1); // Small delay for write pulse
    setControlPin(PIN_RW, HIGH);
    updatePort();
}

void S6E63D6::setControlPin(uint8_t pin, bool value) {
    if (value) {
        _portState |= (1 << pin);
    } else {
        _portState &= ~(1 << pin);
    }
}

void S6E63D6::updatePort() {
    Wire.beginTransmission(_i2cAddress);
    Wire.write(_portState & 0xFF);        // Lower byte (data)
    Wire.write((_portState >> 8) & 0xFF); // Upper byte (control)
    Wire.endTransmission();
}