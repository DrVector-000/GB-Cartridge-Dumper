/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Gestione campi di intestazione della cartuccia Game Boy
*/

#include <Arduino.h>
#include "GBHelper.h"

//******************************************************************************************************************//
//* Lettura titolo del gioco
//******************************************************************************************************************//
String readCartTitle() {
  char title[11];
  for (int i = 0; i < 11; i++) {
    unsigned int addr = 0x0134 + i;
    enableRead(true);
    title[i] = (char)readByte(addr, false);
    enableRead(false);
  }
  title[11] = '\0';

  return title;
}

//******************************************************************************************************************//
//* Lettura codice del gioco
//******************************************************************************************************************//
String readCartCode() {
  char code[4];
  for (int i = 0; i < 4; i++) {
    unsigned int addr = 0x013F + i;
    enableRead(true);
    code[i] = (char)readByte(addr, false);
    enableRead(false);
  }
  code[4] = '\0';

  return code;
}

//******************************************************************************************************************//
//* Lettura tipo cartuccia
//******************************************************************************************************************//
int readCartType() {
  enableRead(true);
  int cartType = readByte(0x0147, false);
  enableRead(false);

  return cartType;
}

//******************************************************************************************************************//
//* Lettura dimensione ROM
//******************************************************************************************************************//
int readROMSize() {
  enableRead(true);
  int romSize = readByte(0x0148, false);
  enableRead(false);

  return romSize;
}

//******************************************************************************************************************//
//* Lettura dimensione RAM
//******************************************************************************************************************//
int readRAMSize() {
  enableRead(true);
  int ramSize = readByte(0x0149, false);
  enableRead(false);

  return ramSize;
}

//******************************************************************************************************************//
//* Lettura supporto per Game Boy Color
//******************************************************************************************************************//
int readCGBSupportCode() {
  enableRead(true);
  int cgbCode = readByte(0x0143, false);
  enableRead(false);

  return cgbCode;
}

//******************************************************************************************************************//
//* Lettura codice produttore
//******************************************************************************************************************//
String readMakerCode() {
  char code[2];
  for (int i = 0; i < 2; i++) {
    unsigned int addr = 0x0144 + i;
    enableRead(true);
    code[i] = (char)readByte(addr, false);
    enableRead(false);
  }
  code[2] = '\0';

  return code;
}

//******************************************************************************************************************//
//* Lettura supporto per Super Game Boy
//******************************************************************************************************************//
int readSGBSupportCode() {
  enableRead(true);
  int sgbCode = readByte(0x0146, false);
  enableRead(false);

  return sgbCode;
}

//******************************************************************************************************************//
//* Lettura codice destinazione
//******************************************************************************************************************//
int readDestinationCode() {
  enableRead(true);
  int destCode = readByte(0x014A, false);
  enableRead(false);

  return destCode;
}

//******************************************************************************************************************//
//* Lettura vecchio codice produttore
//******************************************************************************************************************//
int readOldMakerCode() {
  enableRead(true);
  int makCode = readByte(0x014B, false);
  enableRead(false);

  return makCode;
}

//******************************************************************************************************************//
//* Lettura versione ROM
//******************************************************************************************************************//
int readROMVersion() {
  enableRead(true);
  int romVer = readByte(0x014C, false);
  enableRead(false);

  return romVer;
}
