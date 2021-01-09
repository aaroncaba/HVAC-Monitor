#include "ssd1306Display.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

int SsdDisplay::init()
{
    // Initialize the display
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDR))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return -1;
    }

    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE, BLACK);

    return 0;
}

void SsdDisplay::draw_text(byte x_pos, byte y_pos, char *text, byte text_size)
{
    display.setCursor(x_pos, y_pos);
    display.setTextSize(text_size);
    display.print(text);
    display.display();
}
