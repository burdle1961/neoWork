/*
 Display all the fast rendering fonts in a sprite

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

// Specify sprite 160 x 128 pixels (needs 40Kbytes of RAM for 16 bit colour)
#define IWIDTH  230
#define IHEIGHT 100

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

  tft.fillScreen(TFT_BLUE);

  //img.setColorDepth(8); // Optionally set depth to 8 to halve RAM use
  img.createSprite(IWIDTH, IHEIGHT);
  fill.createSprite(IWIDTH, IHEIGHT);
  fill.fillSprite(TFT_BLACK);
  img.setTextSize(2);
}

char  string[8];
int i = 0;

void loop() {

  sprintf(string, "%4.1f", (float)i/10);

//  img.fillSprite(TFT_BLACK);
  img.drawString(string, 0, 0, 7);
//  img.pushSprite(i%20, i%100);
  img.pushSprite(5,10);
  delay(1000);
  img.drawRect(0,0, IWIDTH, IHEIGHT, TFT_BLUE);
  img.pushSprite(5,10);
  
  i += 1;
}
