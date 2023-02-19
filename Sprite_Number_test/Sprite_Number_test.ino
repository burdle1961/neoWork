/*
 Display all the fast rendering fonts in a sprite

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/
#include "Free_Fonts.h" // Include the header file attached to this sketch

// Specify sprite 160 x 128 pixels (needs 40Kbytes of RAM for 16 bit colour)
#define IWIDTH  230
#define IHEIGHT 140

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 500

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite fill = TFT_eSprite(&tft);

void setup(void) {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  //img.setColorDepth(8); // Optionally set depth to 8 to halve RAM use
  img.createSprite(IWIDTH, IHEIGHT);
  fill.createSprite(IWIDTH, IHEIGHT);
  fill.fillSprite(TFT_BLACK);
  img.setTextSize(2);
  img.setTextColor(TFT_WHITE, TFT_BLACK);
}

char  string[8];
int i = 0;

void loop() {
  tft.drawLine(10,25, IWIDTH, 25, TFT_BLUE);

  // 10 km/h 보다 느린 경우, 자리수 위치 보정을 위하여 leading 0는 바탕색(black)으로 표시
  if (i < 100) {
      sprintf(string, "%3.1f", (float)i/10);
      img.setTextColor(TFT_BLACK, TFT_BLACK); // leading zero with black
      img.drawString("0", 0, 0, 7);         
      img.setTextColor(TFT_WHITE, TFT_BLACK);
      img.drawString(string, 64, 0, 7);
  } else {
      sprintf(string, "%4.1f", (float)i/10);
      img.setTextColor(TFT_WHITE, TFT_BLACK);
      img.drawString(string, 0, 0, 7);
  }
  img.drawLine(0,100, IWIDTH-10, 100, TFT_BLUE);
  img.setTextColor(TFT_YELLOW, TFT_BLACK);
  img.drawString("km/h", 160, 104, 1);
  img.pushSprite(10,30);
  // Sprite를 이용하여 화면 flicking을 해결
  
  delay(500);

  img.drawRect(0,0, IWIDTH, IHEIGHT, TFT_BLACK);
  img.pushSprite(10,30);
  
  i += 1;
  if (i > 200) i = 0;
}
