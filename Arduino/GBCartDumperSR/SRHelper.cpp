/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Helper funzioni Shift Register 74HC595
*/

#include <Arduino.h>
#include "Const.h"

void dataOff(){
 bitClear(PORTB, 2);
}

void clockOff(){
 bitClear(PORTB, 4);
}

void clockOn(){
 bitSet(PORTB, 4);
}

void dataOn(){
 bitSet(PORTB, 2);
}

//******************************************************************************************************************//
//* Scrittura Shift Register ottimizzata in velocità
//******************************************************************************************************************//
void shiftOutFast(byte data) {
  // Invia i singoli bit di data byte MSBFIRST
  for (int i = 7; i >= 0; i--)  {
    //--- Turn data on or off based on value of bit
    if (bitRead(data, i) == 1) {
      dataOn();
    }
    else {      
      dataOff();
    }

    //register shifts bits on upstroke of clock pin  
    clockOn();
    //register shifts bits off
    clockOff();
  }
}

void addressWrite(unsigned int value) {
  byte b1 = 0;
  byte b2 = 0;

  b1 = value & 0xFF;
  b2 = value >> 8;
  // Serial.println("b1=" + String(b1));
  // Serial.println("b2=" + String(b2));

  //digitalWrite(SN_RCLK_PIN, LOW);
  bitClear(PORTB, 3);
  
  //shiftOut(SN_SER_PIN, SN_SRCLK_PIN, MSBFIRST, b2);
  shiftOutFast(b2);
  //shiftOut(SN_SER_PIN, SN_SRCLK_PIN, MSBFIRST, b1);
  shiftOutFast(b1);
  
  //digitalWrite(SN_RCLK_PIN, HIGH);
  bitSet(PORTB, 3);
}
