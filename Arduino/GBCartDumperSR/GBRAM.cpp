/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021-2022 DrVector
  
  Gestione RAM della cartuccia Game Boy
*/

#include <Arduino.h>
#include "GBHeader.h"
#include "SRHelper.h"
#include "GBHelper.h"
#include "Const.h"

//******************************************************************************************************************//
//* Numero blocchi RAM
//******************************************************************************************************************//
int GetRAMBanks() {
  int ramBanks = 0; // Default no RAM banks
  int ramSize = readRAMSize();
  // int cartridgeType = readCartType();

  if (ramSize == 2) { ramBanks = 1; } 
  if (ramSize == 3) { ramBanks = 4; } 
  if (ramSize == 4) { ramBanks = 16; } 
  if (ramSize == 5) { ramBanks = 8; } 

  return ramBanks; 
}

//******************************************************************************************************************//
//* Abilita/Disabilita lettura e scrittura RAM
//******************************************************************************************************************//
void enableRWRAM(bool enable) {
  if (enable) {
    writeByte(0x0000, 0x0A);
  }
  else {
    writeByte(0x0000, 0x00);
  }
}

//******************************************************************************************************************//
//* Dump RAM bank
//******************************************************************************************************************//
void dumpRAMBank(int bank) {
    // Serial.println("START DUMP RAM BANK " + (String)bank);

    unsigned int addr = 0;
    
    // Disattiva READ, WRITE e MREQ
    digitalWrite(GB_RD_PIN, HIGH); // RD off
    digitalWrite(GB_WR_PIN, HIGH); // WR off
    digitalWrite(GB_MREQ_PIN, HIGH); // MREQ off

    // Abilita RW RAM
    enableRWRAM(true);

    // Seleziona il banco
    //if (bank > 0) {
      writeByte(0x4000, bank);
    //}

    // Abilita lettura
    enableWrite(false);
    enableCS(true);
    enableRead(true);

    //addr = 0xA000;
    //for (; addr <= 0xBFFF; addr++) { 
    for (addr = 0xA000; addr < 0xC000; addr++) { 
      byte bval = readByte(addr, true);
      Serial.println(bval, DEC);
    }

    // Disabilita lettura
    enableCS(false);
    enableRead(false);

    // Disabilita RW RAM
    enableRWRAM(false);

    // Serial.println("END DUMP RAM BANK " + (String)bank);
}

//******************************************************************************************************************//
//* Dump RAM MBC2
//******************************************************************************************************************//
void dumpRAMMBC2() {
    // Serial.println("START DUMP RAM BANK " + (String)bank);

    unsigned int addr = 0;
    
    // MBC2 Fix
    readByte(0x0134, false);

    // Disattiva READ, WRITE e MREQ
    digitalWrite(GB_RD_PIN, HIGH); // RD off
    digitalWrite(GB_WR_PIN, HIGH); // WR off
    digitalWrite(GB_MREQ_PIN, HIGH); // MREQ off

    // Abilita RW RAM
    enableRWRAM(true);
    
    // Abilita lettura
    enableWrite(false);
    enableCS(true);
    enableRead(true);

    //addr = 0xA000;
    //for (; addr <= 0xA1FF; addr++) { 
    for (addr = 0xA000; addr < 0xA200; addr++) { 
      /*
      byte bval1 = readByte(addr, true);
      addr++;
      byte bval2 = readByte(addr, true);

      byte bval = (bval1 << 4) + (bval2 & 0xF);
      */
      byte bval = readByte(addr, true);
      Serial.println(bval, DEC);
    }

    // Disabilita lettura
    enableCS(false);
    enableRead(false);

    // Disabilita RW RAM
    enableRWRAM(false);

    // Serial.println("END DUMP RAM BANK " + (String)bank);
}

//******************************************************************************************************************//
//* Write RAM bank
//******************************************************************************************************************//
void writeRAMBank(int bank) {
    // Serial.println("START WRITE RAM BANK " + (String)bank);

    unsigned int addr = 0;
    
    // Disattiva READ, WRITE e MREQ
    digitalWrite(GB_RD_PIN, HIGH); // RD off
    digitalWrite(GB_WR_PIN, HIGH); // WR off
    digitalWrite(GB_MREQ_PIN, HIGH); // MREQ off

    // Abilita RW RAM
    enableRWRAM(true);
    
    // Seleziona il banco
    //if (bank > 0) {
      writeByte(0x4000, bank);
    //}
    
    // Abilita scrittura
    enableCS(true);
    //enableWrite(true);

    addr = 0xA000;
    byte i = 0;
    for (;;) {
      if (Serial.available() > 0) {
        i = Serial.read();

        writeByte(addr, i);
        
        addr++;
        if (addr > 0xBFFF) {
          break;
        }
      }
    }

    // Disabilita scrittura
    enableCS(false);
    enableWrite(false);

    // Disabilita RW RAM
    enableRWRAM(false);

    // Serial.println("END WRITE RAM BANK " + (String)bank);
}