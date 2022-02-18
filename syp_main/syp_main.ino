/*************************************************** 
 *  Test code for Syp
 *  Last Updated: February 18, 2022
 ****************************************************/

// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 

// Plug in the display to the Arduino's digital pins
#define SCLK_PIN 2  // CLK = SPI Clock
#define MOSI_PIN 3  // DIN = Data Input (MOSI = SPI Master out Slave in)
#define DC_PIN   4  // DC
#define CS_PIN   5  // CS = Chip Select
#define RST_PIN  6  // RST = Reset

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

// Imports
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// Setup
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);  

// Variables
float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print("Syp is loading...");
  tft.begin();

  Serial.println("init");

  uint16_t time = millis();
  tft.fillRect(0, 0, 128, 128, BLACK);
  time = millis() - time;
  
  Serial.println(time, DEC);
  delay(500);
  
  tft.invert(true);
  delay(100);
  tft.invert(false);
  delay(100);

  tft.fillScreen(BLACK);
  testdrawtext("hi, my name is syp :)", CYAN);
  delay(500);


}

void loop() {
}

/*** --------------- Syp's Functions Go Here --------------- ***/

// 
void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0,0);
  tft.setTextColor(color);
  tft.print(text);
}
