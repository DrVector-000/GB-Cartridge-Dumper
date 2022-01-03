/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021-2022 DrVector
  
  Gestione ROM della cartuccia Game Boy
*/

#include <Arduino.h>
#include "GBHeader.h"
#include "GBHelper.h"
#include "SRHelper.h"
#include "Const.h"

//******************************************************************************************************************//
//* Numero blocchi ROM
//******************************************************************************************************************//
int GetROMBanks() {
  int romBanks = 2; // Default 32K
  int romSize = readROMSize();
  int cartridgeType = readCartType();
  
  if (romSize == 1) { romBanks = 4; } 
  if (romSize == 2) { romBanks = 8; } 
  if (romSize == 3) { romBanks = 16; } 
  if (romSize == 4) { romBanks = 32; } 
  if (romSize == 5 && (cartridgeType == 1 || cartridgeType == 2 || cartridgeType == 3)) { romBanks = 63; } 
  else if (romSize == 5) { romBanks = 64; } 
  if (romSize == 6 && (cartridgeType == 1 || cartridgeType == 2 || cartridgeType == 3)) { romBanks = 125; } 
  else if (romSize == 6) { romBanks = 128; }
  if (romSize == 7) { romBanks = 256; }
  if (romSize == 82) { romBanks = 72; }
  if (romSize == 83) { romBanks = 80; }
  if (romSize == 84) { romBanks = 96; }

  return romBanks; 
}

//******************************************************************************************************************//
//* Dump ROM bank
//******************************************************************************************************************//
void dumpROMBank(int bank) {
    // Serial.println("START DUMP BANK " + (String)bank);

    unsigned int addr = 0;
    
    // Seleziona il banco
    if (bank > 0) {
      writeByte(0x2100, bank);
      addr = 0x4000;
    }
    
    for (int i = 0; i <= 0x3FFF; i++) {  
      byte bval = readByte(addr + i, false);
      Serial.println(bval, DEC);
    }
    
    // Serial.println("END DUMP BANK " + (String)bank);
}

//******************************************************************************************************************//
//* Dump ROM
//******************************************************************************************************************//
/*
void dumpROM() {
    Serial.println("START DUMP");

    unsigned int addr = 0;
    int romBanks = GetROMBanks();
    
    // Read x number of banks
    for (int y = 1; y < romBanks; y++) {
      writeByte(0x2100, y);

      if (y > 1) {
        addr = 0x4000;
      }
    
      for (; addr <= 0x7FFF; addr++) {  
        byte bval = readByte(addr);
        Serial.println(bval, DEC);
      }
    }
    
    Serial.println("END DUMP");
}
*/