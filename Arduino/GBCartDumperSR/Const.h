/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Dichiarazione costanti
*/

#include <Arduino.h>

//******************************************************************************************************************//
//* Pins pilota Shift Register 74HC595
//******************************************************************************************************************//
// Pin SRCLK del SN74HC595
// Shift Register Clock - INPUT
const int SN_SRCLK_PIN = 12;
// Pin SER del SN74HC595
// Serial Data In - INPUT
const int SN_SER_PIN = 10;
// Pin RCLK del SN74HC595
// Storage Register Clock - INPUT
const int SN_RCLK_PIN = 11;

//******************************************************************************************************************//
//* Pins pilota Game Boy Memory Controller (MBC)
//******************************************************************************************************************//
// Pin RD del connettore GB
const int GB_RD_PIN = A1;
// Pin WR del connettore GB
const int GB_WR_PIN = A2;
// Pin MREQ del connettore GB
const int GB_MREQ_PIN = A0;
