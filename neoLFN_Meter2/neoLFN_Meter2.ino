/*
  Example animated analogue meters using a ILI9341 TFT LCD screen

  Needs Font 2 (also Font 4 if using large scale label)

  Make sure all the display driver and pin connections are correct by
  editing the User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"

// Do not include "" around the array name!
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define NEEDLE  80
#define METER_X 120
#define METER_Y 120

// Callback function to provide the pixel color at x,y
uint16_t pixelColor(uint16_t x, uint16_t y) { return tft.readPixel(x, y); }

#define TFT_GREY 0x5AEB

#define LOOP_PERIOD 200 // Display updates every 200 ms

float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update

int old_analog =  -999; // Value last displayed
int old_digital = -999; // Value last displayed

int value[6] = {0, 0, 0, 0, 0, 0};
int old_value[6] = { -1, -1, -1, -1, -1, -1};
int d = 0;

void setup(void) {
  tft.init();
  tft.setRotation(0);
  Serial.begin(57600); // For debug
  tft.fillScreen(TFT_BLACK);

//  tft.setCallback(pixelColor);  // Switch on color callback for anti-aliased fonts

  tft.loadFont(AA_FONT_LARGE);

  tft.drawRect(10, 10, 228, 119, TFT_GREEN);
//  tft.drawRect(9, 9, 230, 132, TFT_GREEN);
  tft.drawRect(9, 9, 228, 119, TFT_GREEN);
//  tft.fillRect(90, 95, 60, 30, TFT_GREEN); 

  draw_tick();
}


void draw_tick() {
uint16_t x0, y0, x1, y1;
int leng;
float pi2deg = 0.0174532915;

    for (int i = 10 ; i <= 170 ; i += 8) {

      if ((i-10) % 40 == 0) leng = 8;
      else                  leng = 4;

      x0 = METER_X + (NEEDLE+leng) * sin((i-90) * pi2deg);
      y0 = METER_Y + (NEEDLE+leng) * cos((i+90) * pi2deg);

      x1 = METER_X + (NEEDLE+1) * sin((i-90) * pi2deg);
      y1 = METER_Y + (NEEDLE+1) * cos((i+90) * pi2deg);

      tft.drawLine(x0, y0, x1, y1, TFT_YELLOW);
    }
}

void loop() {
uint16_t x0, y0, x1, y1, x2, y2, x3, y3;
uint16_t needle; // length of needle
// x0, y0 : center of needle
// x1, y1 : end of needle
// x2, y2 : left vertex of needle
// x3, y3 : right vertex of needle
float pi2deg = 0.0174532915;
needle = NEEDLE;
x0 = METER_X;
y0 = METER_Y;

    for (int i = 10 ; i <= 170 ; i++) {

      float sx = sin((i-90) * pi2deg);
      float sy = cos((i+90) * pi2deg);

      x1 = x0 + needle * sx;
      y1 = y0 + needle * sy;

      x2 = x0 + (needle/4) * sin((i-10-90)*pi2deg);
      y2 = y0 + (needle/4) * cos((i-10+90)*pi2deg);

      x3 = x0 + (needle/4) * sin((i+10-90)*pi2deg);
      y3 = y0 + (needle/4) * cos((i+10+90)*pi2deg);

      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_MAROON);
      tft.fillTriangle(x0, y0, x1, y1, x3, y3, TFT_PURPLE);
      tft.drawLine(x0, y0, x1, y1, TFT_RED);
      tft.fillCircle(x0, y0-1, (needle/10-1), TFT_BLACK); 
      tft.drawCircle(x0, y0-1, (needle/10-1), TFT_YELLOW); 
//      tft.fillRect(90, 95, 60, 30, TFT_GREEN); 

      delay(100);
      tft.drawLine(x0, y0, x1, y1, TFT_BLACK);
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_BLACK);
      tft.fillTriangle(x0, y0, x1, y1, x3, y3, TFT_BLACK);
      tft.fillCircle(x0, y0-1, (needle/10-1), TFT_BLACK); 
      tft.drawCircle(x0, y0-1, (needle/10-1), TFT_YELLOW); 

//      tft.drawLine(x0, y0, x1, y1, TFT_RED);
//      delay(500);
//      tft.drawLine(x0, y0, x1, y1, TFT_BLACK);
    }

    // Magenta zone limits
//    if (i >= 25 && i < 50) {
//      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_MAROON);
//      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_MAROON);
//    }


}


// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
void analogMeter()
{
  // Meter outline
  tft.fillRect(0, 0, 239, 126, TFT_GREY);
  tft.fillRect(5, 3, 230, 119, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);  // Text colour

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (100 + tl) + 120;
    uint16_t y0 = sy * (100 + tl) + 140;
    uint16_t x1 = sx * 100 + 120;
    uint16_t y1 = sy * 100 + 140;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (100 + tl) + 120;
    int y2 = sy2 * (100 + tl) + 140;
    int x3 = sx2 * 100 + 120;
    int y3 = sy2 * 100 + 140;

    // Green zone limits
    if (i >= -25 && i < -24) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_DARKGREEN);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_DARKGREEN);
    }

    // Yellow zone limits
    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREENYELLOW);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREENYELLOW);
    }

    // Magenta zone limits
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_MAROON);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_MAROON);
    }

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords incase tick lenght changed
    x0 = sx * (100 + tl) + 120;
    y0 = sy * (100 + tl) + 140;
    x1 = sx * 100 + 120;
    y1 = sy * 100 + 140;

    // Draw tick
    tft.drawLine(x0, y0, x1, y1, TFT_BLACK);

    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (100 + tl + 10) + 120;
      y0 = sy * (100 + tl + 10) + 140;
      switch (i / 25) {
        case -2: tft.drawCentreString("0", x0, y0 - 12, 2); break;
        case -1: tft.drawCentreString("5", x0, y0 - 9, 2); break;
        case 0: tft.drawCentreString("10", x0, y0 - 6, 2); break;
        case 1: tft.drawCentreString("15", x0, y0 - 9, 2); break;
        case 2: tft.drawCentreString("20", x0, y0 - 12, 2); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * 100 + 120;
    y0 = sy * 100 + 140;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, y0, x1, y1, TFT_BLACK);
  }

  tft.drawString("km/h", 5 + 230 - 64, 119 - 25, 4); // Units at bottom right
  tft.drawCentreString("0.0", 120, 70, 4); // Comment out to avoid font 4
  tft.drawRect(5, 3, 230, 119, TFT_BLACK); // Draw bezel line

  plotNeedle(0, 0); // Put meter needle at 0
}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void plotNeedle(int value, byte ms_delay)
{
  char buf[8]; dtostrf((value+50)/5-10, 4, 0, buf);

//  tft.fillRect(105, 60, 50, 50, TFT_WHITE);
//  tft.setTextColor(TFT_BLACK, TFT_WHITE);
//  tft.drawCentreString(buf, 112, 60, 6);

  if (value < -10) value = -10; // Limit value to emulate needle end stops
  if (value > 110) value = 110;

  // Move the needle util new value reached
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++;
    else old_analog--;

    if (ms_delay == 0) old_analog = value; // Update immediately id delay is 0

    float sdeg = map(old_analog, -10, 110, -150, -30); // Map value to angle
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft.drawLine(120 + 20 * ltx - 2, 140 - 20, osx - 1, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx + 2, 140 - 20, osx + 1, osy, TFT_WHITE);

    // Re-plot text under needle
    tft.fillRect(100, 60, 50, 50, TFT_WHITE);
    tft.setTextColor(TFT_BLACK);
    tft.drawCentreString(buf, 112, 60, 6); // // Comment out to avoid font 4

    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 98 + 120;
    osy = sy * 98 + 140;

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(120 + 20 * ltx - 2, 140 - 20, osx - 1, osy, TFT_RED);
    tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, TFT_RED);
    tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, TFT_RED);
    tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, TFT_RED);
    tft.drawLine(120 + 20 * ltx + 2, 140 - 20, osx + 1, osy, TFT_RED);

    // Slow needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
}

// #########################################################################
//  Draw a linear meter on the screen
// #########################################################################
void plotLinear(char *label, int x, int y)
{
  int w = 36;
  tft.drawRect(x, y, w, 155, TFT_GREY);
  tft.fillRect(x + 2, y + 19, w - 3, 155 - 38, TFT_WHITE);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawCentreString(label, x + w / 2, y + 2, 2);

  for (int i = 0; i < 110; i += 10)
  {
    tft.drawFastHLine(x + 20, y + 27 + i, 6, TFT_BLACK);
  }

  for (int i = 0; i < 110; i += 50)
  {
    tft.drawFastHLine(x + 20, y + 27 + i, 9, TFT_BLACK);
  }

  tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 - 5, TFT_RED);
  tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 + 5, TFT_RED);

  tft.drawCentreString("---", x + w / 2, y + 155 - 18, 2);
}

// #########################################################################
//  Adjust 6 linear meter pointer positions
// #########################################################################
void plotPointer(void)
{
  int dy = 187;
  byte pw = 16;

  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // Move the 6 pointers one pixel towards new value
  for (int i = 0; i < 6; i++)
  {
    char buf[8]; dtostrf(value[i], 4, 0, buf);
    tft.drawRightString(buf, i * 40 + 36 - 5, 187 - 27 + 155 - 18, 2);

    int dx = 3 + 40 * i;
    if (value[i] < 0) value[i] = 0; // Limit value to emulate needle end stops
    if (value[i] > 100) value[i] = 100;

    while (!(value[i] == old_value[i])) {
      dy = 187 + 100 - old_value[i];
      if (old_value[i] > value[i])
      {
        tft.drawLine(dx, dy - 5, dx + pw, dy, TFT_WHITE);
        old_value[i]--;
        tft.drawLine(dx, dy + 6, dx + pw, dy + 1, TFT_RED);
      }
      else
      {
        tft.drawLine(dx, dy + 5, dx + pw, dy, TFT_WHITE);
        old_value[i]++;
        tft.drawLine(dx, dy - 6, dx + pw, dy - 1, TFT_RED);
      }
    }
  }
}
