/***************************************************
    Syp's main code (Teensy, SPI)
    Last Updated: March 16, 2022
 ****************************************************/
 
// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Plug in the display to the Teensy's digital pins
#define SCLK_PIN 13 // CLK = SPI Clock (SCK)
#define MOSI_PIN 11 // DIN = Data Input (MOSI = SPI Master out Slave in)
#define DC_PIN 9    // DC
#define CS_PIN 10   // CS = Chip Select
#define RST_PIN 15  // RST = Reset

const int BUTTON = 2;    // Pushbutton

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1351.h>
#include <SPI.h>

// external animations
//extern uint8_t* nike[6];
//extern uint8_t* wink[48];
//extern uint8_t* cake[5];
//extern uint8_t* smilee[10];
//extern uint8_t* face[1];

// Option 1: use any pins but a little slower
// Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);

// Option 2: must use the hardware SPI pins -- we'll come back to this later <<THIS IS WHAT WE HAVE RIGHT NOW>>
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);
GFXcanvas16 display(128, 128);

float p = 3.1415926;
int eyeWidth = 45;  // 30
int eyeHeight = 40; // 30
int eyeRound = 9;   // 20
int xPos = 10;
int yPos = 45;
unsigned long petTime = 0;
unsigned long blinkTime = 0;
int blinkInterval = 5000;
int sensorValue = 0;

void setup(void)
{
    Serial.begin(9600);
    Serial.println("starting setup!");
    tft.begin();
    delay(500);
    tft.fillScreen(BLACK);
    glitch();
    Serial.println("setup finished!");
}

int buttonState = 0; 

void loop()
{
  blink();
  wink();

//tft.fillScreen(CYAN);
} 

// Extra variables
double iconDimension = 128;

void showThis(uint8_t** animation) {
    unsigned int animationSize = sizeof(*animation) / sizeof(*animation[0]);
  
    // loop through the frames
    for (unsigned int i = 0; i < animationSize; i++)
    {
        display.fillScreen(BLACK);
        display.drawBitmap(0, 0, animation[i], iconDimension, iconDimension, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
        delay(10);
    }

    // reverse loop
    for (unsigned int i = animationSize; i > 0; i--)
    {
        display.fillScreen(BLACK);
        display.drawBitmap(0, 0, animation[i-1], iconDimension, iconDimension, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
        delay(10);
    }
    delay(500);
}


void blink()
{
    for (int i = 0; i < eyeHeight / 2; i += 5)
    {

        display.fillScreen(BLACK);
        display.fillRoundRect(xPos, yPos + i, eyeWidth, eyeHeight - i * 2, eyeRound, CYAN);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + i, eyeWidth, eyeHeight - i * 2, eyeRound, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }

    for (int i = 0; i >= -eyeHeight / 2; i -= 5)
    {

        display.fillScreen(BLACK);
        display.fillRoundRect(xPos, yPos + i + eyeHeight / 2, eyeWidth, -i * 2, eyeRound, CYAN);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + i + eyeHeight / 2, eyeWidth, -i * 2, eyeRound, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }
    delay(500);
}

void smile()
{

    display.fillScreen(BLACK);
    display.fillRoundRect(xPos, yPos, eyeWidth, eyeHeight, eyeRound, CYAN);
    display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos, eyeWidth, eyeHeight, eyeRound, CYAN);

    for (int i = 0; i < eyeHeight / 1.1; i += 10)
    {
        display.fillRoundRect(xPos, yPos - i + eyeHeight, eyeWidth, eyeHeight, eyeRound * 2, BLACK);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos - i + eyeHeight, eyeWidth, eyeHeight, eyeRound * 2, BLACK);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }

    delay(1000);
    glitch();
    // drop down eyes after smile, replaced with glitch
    //   for (int i = eyeHeight / 1.2; i >= 0; i -= 7) {
    //     display.fillRoundRect(xPos, yPos, eyeWidth, eyeHeight, eyeRound, CYAN);
    //     display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos, eyeWidth, eyeHeight, eyeRound, CYAN);
    //
    //     display.fillRoundRect(xPos, yPos - i + eyeHeight, eyeWidth, eyeHeight, eyeRound * 2, BLACK);
    //     display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos - i + eyeHeight, eyeWidth, eyeHeight, eyeRound * 2, BLACK);
    //     tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    //
    //   }
}

// wink either left or right eye
void wink()
{
    
    for (int i = 0; i < eyeHeight / 2; i += 5)
    {

        display.fillScreen(BLACK);
        display.fillRoundRect(xPos, yPos + i, eyeWidth, eyeHeight - i * 2, eyeRound, CYAN);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + i, eyeWidth, eyeHeight - i * 2, eyeRound, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }

    for (int i = 0; i >= -eyeHeight / 2; i -= 5)
    {

        display.fillScreen(BLACK);
        display.fillRoundRect(xPos, yPos + i + eyeHeight / 2, eyeWidth, -i * 2, eyeRound, CYAN);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + i + eyeHeight / 2, eyeWidth, -i * 2, eyeRound, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }
    delay(500);
}



void sleep()
{
    int sleepHeight = 6;
    int sleepDepth = -2;
    display.fillRoundRect(xPos, yPos, eyeWidth, eyeHeight, eyeRound, CYAN);
    display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos, eyeWidth, eyeHeight, eyeRound, CYAN);

    for (int i = 0; i < eyeHeight / 1.25; i += 3)
    {
        display.fillRoundRect(xPos, yPos + i - eyeHeight, eyeWidth, eyeHeight, eyeRound * 2, BLACK);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + i - eyeHeight, eyeWidth, eyeHeight, eyeRound * 1.5, BLACK);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }

    while (true)
    {
        for (int i = sleepDepth; i <= sleepHeight; i++)
        {
            display.fillScreen(BLACK);
            display.fillRoundRect(xPos, yPos - i, eyeWidth, eyeHeight, eyeRound, CYAN);
            display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos - i, eyeWidth, eyeHeight, eyeRound, CYAN);
            display.fillRoundRect(xPos, yPos + eyeHeight / 1.25 - eyeHeight - i, eyeWidth, eyeHeight, eyeRound * 1.5, BLACK);
            display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + eyeHeight / 1.25 - eyeHeight - i, eyeWidth, eyeHeight, eyeRound * 1.5, BLACK);
            tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);

            delay(100);
        }
        delay(200);
        for (int i = sleepHeight; i >= sleepDepth; i--)
        {
            display.fillScreen(BLACK);
            display.fillRoundRect(xPos, yPos - i, eyeWidth, eyeHeight, eyeRound, CYAN);
            display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos - i, eyeWidth, eyeHeight, eyeRound, CYAN);
            display.fillRoundRect(xPos, yPos + eyeHeight / 1.25 - eyeHeight - i, eyeWidth, eyeHeight, eyeRound * 1.5, BLACK);
            display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + eyeHeight / 1.25 - eyeHeight - i, eyeWidth, eyeHeight, eyeRound * 1.5, BLACK);

            if (i < sleepHeight)
            {
                display.setCursor(1, 1);
                display.setTextColor(CYAN);
                display.println("ZZZ...");
            }
            tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
            delay(100);
        }
        delay(200);
    }
}

void glitch()
{
    int rectY = 128;
    int rectHeight = 6;

    for (int i = 0; i < 128 / 2; i += 20)
    {
        display.fillScreen(BLACK);
        display.fillRect(0, rectY - rectHeight - i, 128, rectHeight, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }

    tft.fillScreen(BLACK);
    tft.fillRoundRect(xPos, yPos, eyeWidth, eyeHeight, eyeRound, CYAN);
    tft.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos, eyeWidth, eyeHeight, eyeRound, CYAN);
}

void pet()
{

    display.fillScreen(BLACK);
    display.fillRoundRect(xPos, yPos, eyeWidth, eyeHeight, eyeRound, CYAN);
    display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos, eyeWidth, eyeHeight, eyeRound, CYAN);

    for (int i = 0; i < eyeHeight / 1.1; i += 10)
    {
        display.fillRoundRect(xPos, yPos - i + eyeHeight, eyeWidth, eyeHeight, eyeRound * 2, BLACK);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos - i + eyeHeight, eyeWidth, eyeHeight, eyeRound * 2, BLACK);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }
    Serial.println("Mid-pet");

    petTime = millis();
    Serial.println(sensorValue);
    while ((millis() - petTime) < 1000)
    {
        Serial.println(sensorValue);
        sensorValue = analogRead(A5);
        if (sensorValue > 1017)
        {
            petTime = millis();
        }
        delay(10);
    }

    if (sensorValue < 1023)
    {
        glitch();
    }
}

void circle()
{
    // turn the rectangle into a circle through each frame
    int borderRadius = eyeHeight;
    for (int i = 0; i < 100; i++)
    {
        display.fillScreen(BLACK);

        display.fillRoundRect(xPos, yPos, eyeWidth, eyeHeight, borderRadius, CYAN);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos, eyeWidth, eyeHeight, borderRadius, CYAN);
        borderRadius *= 1.001;
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }
}

// make the eyes move around the screen
void theRockEyes()
{
    for (int i = 0; i < 10; i += 1)
    {
        display.fillScreen(BLACK);
        display.fillRoundRect(xPos, yPos - i, eyeWidth, eyeHeight, eyeRound, CYAN);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeWidth), yPos + i, eyeWidth, eyeHeight, eyeRound, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    }
}

// make the eyes smaller
void smallerEyes()
{
    int eyeW = eyeWidth;
    int eyeH = eyeHeight;
    int eyeR = eyeRound;
    for (int i = 0; i < 10; i += 1)
    {
        display.fillScreen(BLACK);
        display.fillRoundRect(xPos, yPos, eyeW, eyeH, eyeR, CYAN);
        display.fillRoundRect(SCREEN_WIDTH - (xPos + eyeW), yPos, eyeW, eyeH, eyeR, CYAN);
        tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
        eyeW *= .99;
        eyeH *= .99;
        eyeR *= 1.5;
    }

    delay(2000);
}

// send text in the screen
void sendText(String text) {
  int xPos = SCREEN_WIDTH/2 - 2*text.length();
  
  tft.fillScreen(BLACK);
  tft.setCursor(xPos, SCREEN_HEIGHT/2);
  tft.setTextColor(CYAN);
  tft.print(text);
  delay(300);
}
