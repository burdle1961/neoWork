#include  "EEPROM.h"
uint8_t val[3];

void setup() {
  delay(5000);
  Serial.begin(115200);
  // put your setup code here, to run once:
  EEPROM.begin(16);
  val[0] = byte(EEPROM.read(0));
  val[1] = byte(EEPROM.read(1));
  val[2] = byte(EEPROM.read(2));

  Serial.println("Data from EEPROM");
  Serial.print(val[0]);
  Serial.print("  ");
  Serial.print(val[1]);
  Serial.print("  ");
  Serial.print(val[2]);
  Serial.println();
  
}

void loop() {
  // put your main code here, to run repeatedly:

  val[0] = byte(EEPROM.read(0));
  val[1] = byte(EEPROM.read(1));
  val[2] = byte(EEPROM.read(2));

  EEPROM.write(0, val[0] + 1);
  EEPROM.write(1, val[1] + 2);
  EEPROM.write(2, val[2] + 3);
  EEPROM.commit();
  Serial.println("New Value written on EEPROM");
  delay(2000);

}
