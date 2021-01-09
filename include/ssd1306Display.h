#ifndef SSD1306DISPLAY_H_
#define SSD1306DISPLAY_H_

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define SSD1306_ADDR 0x3C // SSD1306 display I2C address

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
class SsdDisplay
{
public:
    SsdDisplay(){};
    int init();
    void draw_text(byte x_pos, byte y_pos, char *text, byte text_size);
    Adafruit_SSD1306 display;
};

#endif