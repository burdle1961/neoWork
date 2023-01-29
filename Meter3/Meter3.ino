#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

uint16_t pattern[] = {
  0, 0, 20, 9,
  0,10, 20, 9,
  0,20, 20, 9,
  0,30, 20, 9,  
  0,40, 20, 9,
  0,50, 20, 50,
  21,50,9,50,
  31,50,9,50, 
  41,50,9,50, 
  51,50,9,50, 
  61,50,9,50, 
  71,50,9,50, 
  81,50,9,50, 
  91,50,9,50, 
  101,50,9,50, 
  111,50,9,50, 
  121,50,9,50, 
  131,50,9,50, 
  141,50,9,50, 
  151,50,9,50
};

TFT_eSPI tft = TFT_eSPI();

void setup() {

  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(0);
  Serial.begin(57600); // For debug
  tft.fillScreen(TFT_BLACK);


  for (int i = 0 ; i <= 20 ; i++) {
      tft.drawRect(pattern[i*4]+20, 120-pattern[i*4+1]-pattern[i*4+3], pattern[i*4+2], pattern[i*4+3], TFT_DARKGREY);
  }
  delay(2000);
  
  for (int i = 0 ; i <= 20 ; i++) {
    if (i < 5) 
      tft.fillRect(pattern[i*4]+20, 120-pattern[i*4+1]-pattern[i*4+3], pattern[i*4+2], pattern[i*4+3], TFT_GREEN);
    else if (i < 6)
      tft.fillRect(pattern[i*4]+20, 120-pattern[i*4+1]-pattern[i*4+3], pattern[i*4+2], pattern[i*4+3], TFT_BLUE);
    else if (i < 10) 
      tft.fillRect(pattern[i*4]+20, 120-pattern[i*4+1]-pattern[i*4+3], pattern[i*4+2], pattern[i*4+3], TFT_WHITE);
    else if (i < 15)
      tft.fillRect(pattern[i*4]+20, 120-pattern[i*4+1]-pattern[i*4+3], pattern[i*4+2], pattern[i*4+3], TFT_ORANGE);
    else
      tft.fillRect(pattern[i*4]+20, 120-pattern[i*4+1]-pattern[i*4+3], pattern[i*4+2], pattern[i*4+3], TFT_RED);
    
    delay(200);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
