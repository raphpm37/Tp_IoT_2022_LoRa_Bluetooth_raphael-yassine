#include <Wire.h>
#include "SHT2x.h"

SHT2x SHT2x;

void setup(){
  Wire.begin(21, 22);
  SHT2x.begin();
  Serial.begin(115200);
}

void loop(){
  uint32_t start = micros();
  Serial.print("Humidity(%RH): ");
  Serial.print(SHT2x.GetHumidity(), 1);
  Serial.print("\tTemperature(C): ");
  Serial.print(SHT2x.GetTemperature(), 1);

  uint32_t stop = micros();
  Serial.print("\tReadTime: ");
  Serial.print(stop - start);
  delay(1000);
}