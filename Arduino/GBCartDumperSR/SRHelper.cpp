/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Helper funzioni Shift Register 74HC595
*/

#include <Arduino.h>
#include "Const.h"

void addressWrite(unsigned int value) {
  byte b1 = 0;
  byte b2 = 0;

  b1 = value & 0xFF;
  b2 = value >> 8;
  // Serial.println("b1=" + String(b1));
  // Serial.println("b2=" + String(b2));

  digitalWrite(SN_RCLK_PIN, LOW);
  shiftOut(SN_SER_PIN, SN_SRCLK_PIN, MSBFIRST, b2);
  shiftOut(SN_SER_PIN, SN_SRCLK_PIN, MSBFIRST, b1);
  digitalWrite(SN_RCLK_PIN, HIGH);
}