/***************************************************
    Syp's main code (Teensy, SPI)
    Last Updated: March 16, 2022
 ****************************************************/

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Plug in the display to the Teensy's digital pins
#define SCLK_PIN 13 // CLK = SPI Clock (SCK)
#define MOSI_PIN 11 // DIN = Data IInput (MOSI = SPI Mastenpouut (MOSI = SPI Master out Slave in)
#define DC_PIN 9    // DC
#define CS_PIN 10   // CS = Chip Select
#define RST_PIN 15  // RST = Reset

const int BUTTON = 2; // Pushbutton

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1351.h>
#include <SPI.h>

// external animations
extern uint8_t *nike[6];
// extern uint8_t* wink[48];
extern uint8_t *cake[5];
extern uint8_t *smilee[10];
extern uint8_t *face[1];
extern uint8_t *ethan[1];
extern uint8_t *jon[1];
extern uint8_t *sybbure[1];
extern uint8_t *dizzy[40];
extern uint8_t *dizzy_start[29];
extern uint8_t *dizzy_mid[11];
extern uint8_t *dizzy_end[11];
extern uint8_t *rotate[30];
extern uint8_t *sideDropRight[39];
extern uint8_t *sideDropLeft[39];
extern uint8_t *sideDropRightReverse[15];
extern uint8_t *sideDropLeftReverse[15];

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);
GFXcanvas16 display(128, 128);

// Accelerometer
#include <Wire.h>
const int MPU = 0x68; // MPU6050 I2C address

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

  // accelometer
  Wire.begin();                // Initialize comunication
  Wire.beginTransmission(MPU); // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);            // Talk to the register 6B
  Wire.write(0x00);            // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);  // end the transmission

  tft.fillScreen(BLACK);
  glitch();
  Serial.println("setup finished!");
}

int buttonState = 0;

void loop()
{
  // randomly choose a function to run at differenet intervals
  blink();
  delay(2000);
  showThis(rotate, 30);
  delay(2000);
  wink();
  delay(2000);
  circle();
  delay(2000);
  smile();
  delay(2000);

  sideDrop();
  dizzyEyes();
}

// Extra variables
double iconDimension = 128;

void showThis(uint8_t **animation, int size)
{
  unsigned int animationSize = size;

  // loop through the frames
  for (unsigned int i = 0; i < animationSize; i++)
  {
    display.fillScreen(BLACK);
    display.drawBitmap(0, 0, animation[i], iconDimension, iconDimension, CYAN);
    tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    delay(10);
  }
  delay(500);
}

void idleFunctions()
{
  // functions: blink(), smile(), wink(), sleep(), glitch(), pet(), circle(), smallerEyes()
  int function = random(0, 10);
  switch (function)
  {
  case 0:
    blink();
    break;
  case 1:
    smile();
    break;
  case 2:
    wink();
    break;
  case 3:
    sleep();
    break;
  case 4:
    glitch();
    break;
  case 5:
    pet();
    break;
  default:
    break;
  }
}

void sideDrop()
{
  float threshold = 0.1;
  bool dropped = false;
  int side = 0; // 0 = up, 1 = right, 2 = left
  while (1 - abs(readZ()) <= threshold)
  {
    delay(1000);
    if (1 - abs(readZ()) > threshold)
      dropped = true;

    if (!dropped)
    {
      if (readZ() > 0)
      {
        side = 1;
        showThis(sideDropRight, 39);
      }
      else
      {
        side = 2;
        showThis(sideDropLeft, 39);
      }
      dropped = true;
    }
  }
  if (side == 1)
    showThis(sideDropRightReverse, 15);
  else if (side == 2)
    showThis(sideDropLeftReverse, 15);
}

void dizzyEyes()
{
  float one, two, three, four, five;
  float th = 0.5;
  one = readX();
  delay(10);
  two = readX();
  delay(10);
  three = readX();
  delay(10);
  four = readX();
  delay(10);
  five = readX();
  bool dizzy = abs(one - two) > th and abs(two - three) > th and
               abs(three - four) > th and abs(four - five) > th;

  if (dizzy)
    dizzyFunc();
}

void dizzyFunc()
{
  uint8_t **animation = dizzy;
  unsigned int animationSize = 40;
  float one, two, three;
  unsigned int i = 0;
  float th = 0.13;

  // loop through the frames
  //    for (unsigned int i = 0; i < animationSize; i++)
  while (i < animationSize)
  {
    display.fillScreen(BLACK);
    display.drawBitmap(0, 0, animation[i], iconDimension, iconDimension, CYAN);
    tft.drawRGBBitmap(0, 0, display.getBuffer(), 128, 128);
    delay(10);

    if (i == 24)
      one = readX();
    else if (i == 26)
      two = readX();
    else if (i == 28)
      three = readX();
    else if (i == 29 and abs(one - two) > th and abs(two - three) > th)
      i = 16;
    Serial.println(i);
    Serial.println(one);
    Serial.println(two);
    Serial.println(three);

    i++;
  }
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
    eyeW *= .97;
    eyeH *= .97;
    eyeR *= 1.5;
  }

  delay(5000);
}

// send text in the screen
void sendText(String text)
{
  int xPos = SCREEN_WIDTH / 2 - 2 * text.length();

  tft.fillScreen(BLACK);
  tft.setCursor(xPos, SCREEN_HEIGHT / 2);
  tft.setTextColor(CYAN);
  tft.print(text);
  delay(10);
}

// read the x-axis of the accelerometer
float readX()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H), 0x3F
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true);                                   // Read 6 registers total, each axis value is stored in 2 registers
  float AccX = (int16_t)(Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  return AccX;
}

// read the x-axis of the accelerometer
float readY()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3D); // Start with register 0x3D (ACCEL_YOUT_H), 0x3F
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true);                                   // Read 6 registers total, each axis value is stored in 2 registers
  float AccY = (int16_t)(Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  return AccY;
}

// read the x-axis of the accelerometer
float readZ()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3F); // Start with register 0x3F (ACCEL_ZOUT_H), 0x3F
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true);                                   // Read 6 registers total, each axis value is stored in 2 registers
  float AccZ = (int16_t)(Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  return AccZ;
}
