/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Helper funzioni di gestione bus dati e control pins della cartuccia Game Boy
*/

#include <Arduino.h>

//******************************************************************************************************************//
//* Lettura di un byte all'indirizzo selezionato dal bus dati della cartuccia
//******************************************************************************************************************//
byte readByte(unsigned int address, bool ram);

//******************************************************************************************************************//
//* Scrittura di un byte all'indirizzo selezionato nel bus dati della cartuccia
//******************************************************************************************************************//
void writeByte(unsigned int address, byte data);
