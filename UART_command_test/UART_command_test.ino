#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

#define TFT_GREY 0x5AEB // New colour

TFT_eSPI tft = TFT_eSPI();  // Invoke library

uint8_t cmd_init[] = {0x66, 0x10, 0x00, 0x76};
uint8_t cmd_parm[] = {0x66, 0x11, 0x00, 0x77};
uint8_t cmd_query[] = {0x66, 0x42, 0x02, 0x00, 0x00, 0xAA};

//struct SPEED {
//  byte header;
//  byte command;
//  byte leng;
//  byte batt_lv;
//  byte temp0;
//  byte pulse{2];
//  byte temp1[3];
//  byte batt_vol{2];
//  byte curr[2];
//  byte checksum;
//};

float rpm;
float kmh;
float battery;


#define RX_LENGTH  128
uint8_t rx_buf[RX_LENGTH];

uint8_t CheckSum(uint8_t *, int);

void setup() {
int cnt;

  tft.init();
  tft.setRotation(2);

  // Fill screen with grey so we can see the effect of printing with and without 
  // a background colour defined
  tft.fillScreen(TFT_GREY);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);
  tft.println("Starting in 1000 msec.");
  
  Serial.begin(9600);
  delay(1000);
  tft.println("Command : initializing");

//  memset(rx_buf, 0, RX_LENGTH);
//  cnt = send_command(cmd_init, 4, rx_buf);
//  display_data(rx_buf, cnt);
//
//  delay(1000);
//  tft.println("Command : read parameters");
//
//  memset(rx_buf, 0, RX_LENGTH);
//  cnt = send_command(cmd_parm, 4, rx_buf);
//  display_data(rx_buf, cnt);
//  
//  int checksum = CheckSum(x, int(x[2]+3));
//
//  if (checksum == check) Serial.println("Matched.");
//
//  Serial.println(checksum, HEX);
//  Serial.println(checksum, BIN);
}

int  send_command(uint8_t *cmd, int cmd_length, uint8_t *buf) {
int recvd;          // number of bytes received.

  // send command
  for (int i = 0 ; i < cmd_length ; i++) Serial.write(cmd[i]);

  recvd = 0;
  while (true) {
    if (Serial.available()){
      buf[recvd] = Serial.read();
      if (recvd > 2 && recvd == buf[2] + 3) break;
      recvd++;
    } 
  }
  return(recvd+1);
}


uint8_t CheckSum(uint8_t  *packet, int leng) {
uint8_t sum = 0;

  for (int i = 0 ; i < leng ; i++) {
      sum += packet[i];
      sum %= 256;  
  }
  return(sum);
}

void display_data(uint8_t *packet, int cnt) {
float pulse;
float volt;

  tft.fillScreen(TFT_GREY);
  tft.setCursor(0, 0, 2);
  for (int i = 0 ; i < cnt ; i++) {
//    Serial.print(packet[i], HEX);
//    Serial.print(" ");
    tft.print(packet[i], HEX);
    tft.print(' ');
  }
  tft.println();
  
  if (packet[1] == 0x42) {
      pulse = packet[5]*256 + packet[6];
      if (pulse <= 0 || pulse > 3500) {
        rpm = 0.0;
                kmh = 0.0;

      } else {
        rpm = 60000/pulse;
        kmh = (12 * 287.267)/pulse ;   // 12 inch wheel. 3600 sec * 3.14 * 2.54cm/inch
      }  
        
      volt = (packet[10]*256+packet[11])/10;

      tft.print("Batt Lv : ");
      tft.println(packet[3]);
      tft.print("rpm : ");
      tft.println(rpm);
      tft.print("speed : ");
      tft.println(kmh);
      tft.print("battery(V) : ");
      tft.println(volt);
  }
} 

void loop() {
int cnt;
  // put your main code here, to run repeatedly:
  tft.println("Command : read speed");

  memset(rx_buf, 0, RX_LENGTH);
  cnt = send_command(cmd_query, 6, rx_buf);
  display_data(rx_buf, cnt);

  delay(2000);
}
