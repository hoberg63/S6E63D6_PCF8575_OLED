#include <Wire.h>
#include <Adafruit_GFX.h>
#include "S6E63D6_Modernized.h"

// Define the I2C address of the PCF8575
#define PCF8575_I2C_ADDRESS 0x20

// Create display object
S6E63D6 display(PCF8575_I2C_ADDRESS);

void setup() {
    // Initialize the display
    display.begin();
    display.clearScreen(0xFFFF); // Fill the screen with white

    // Display a welcome message
    display.setTextColor(0x001F, 0xFFFF); // Blue text with white background
    display.setTextSize(2);
    display.setCursor(10, 10);
    display.print("Welcome!");

    // Draw shapes
    display.drawCircle(160, 120, 50, 0xF800); // Draw a red circle in the center
    display.fillRect(50, 200, 50, 30, 0x07E0); // Draw a filled green rectangle
    display.drawLine(10, 10, 310, 230, 0xF81F); // Draw a magenta diagonal line

    // Display another message
    display.setCursor(10, 50);
    display.setTextColor(0x07E0); // Green text
    display.setTextSize(1);
    display.print("Adafruit GFX + S6E63D6!");
}

void loop() {
    // You can add animations or updates to the display here
}