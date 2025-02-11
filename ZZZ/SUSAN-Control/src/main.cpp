#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  
#define SCREEN_ADDRESS 0x3C  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float gain_dB = -60.0;
float min_dB = -60.0; // -infinity threshold
float max_dB = 0.0;

// Dial settings
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)
#define RADIUS 30
#define X_SHIFT_CONSTANT -30

void drawGainDial(float gain) {
    display.clearDisplay();
    
    // pot outline
    display.drawCircle(CENTER_X + X_SHIFT_CONSTANT, CENTER_Y, RADIUS, SSD1306_WHITE);
    float angle = map(gain, min_dB, max_dB, 360, 0);
    float rad = angle * PI / 180.0;

    // pot line position
    int dotX = CENTER_X + X_SHIFT_CONSTANT + (int)(cos(rad) * (RADIUS - 3));
    int dotY = CENTER_Y - (int)(sin(rad) * (RADIUS - 3));

    // pot line
    display.fillCircle(dotX, dotY, 3, SSD1306_WHITE);

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(CENTER_X + X_SHIFT_CONSTANT - 20, CENTER_Y - 5);

    if (gain <= min_dB) {
        display.print("-∞ dB");
    } else {
        display.print(gain, 1);
        display.print("dB");
    }

    display.setTextSize(2); 
    display.setCursor(CENTER_X + RADIUS + X_SHIFT_CONSTANT + 10, CENTER_Y - 8);
    display.print("gain");

    display.display();
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 not bon"));
        while (true);
    }

    Serial.println(F("SSD1306 bon"));
    drawGainDial(gain_dB);
}

void loop() {
    // Simulate gain changing
    static float direction = 1.0;
    gain_dB += direction * 1.1; // Increase/decrease gain

    if (gain_dB >= max_dB || gain_dB <= min_dB) {
        direction *= -1;  // Reverse direction at limits
    }

    drawGainDial(gain_dB);
    delay(10);
}