#include <U8x8lib.h>

int d1=0, d2=0, d3=0;
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15,4,16);

void dispData(){
  char dbuf[16];
  u8x8.clear();
  Serial.println("titre");
  u8x8.drawString(0, 1, "titre");
  sprintf(dbuf, "Data1:%d", d1);
  u8x8.drawString(0, 2, dbuf);
  sprintf(dbuf, "Data2:%d", d2);
  u8x8.drawString(0, 3, dbuf);
  sprintf(dbuf, "Data3:%d", d3);
  u8x8.drawString(0, 4, dbuf);
  delay(6000);
}

void setup(){
  Serial.begin(9600);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void loop(){
  d1++; d2+=2; d3+=4;
  dispData();
}
