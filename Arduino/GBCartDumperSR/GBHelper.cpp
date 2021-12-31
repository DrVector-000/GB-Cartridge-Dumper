/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Helper funzioni di gestione bus dati e control pins della cartuccia Game Boy
*/

#include <Arduino.h>
#include "Const.h"
#include "SRHelper.h"

//******************************************************************************************************************//
//* Lettura di un byte all'indirizzo selezionato dal bus dati della cartuccia
//******************************************************************************************************************//
byte readByte(unsigned int address) {
  // Imposta indirizzo
  addressWrite(address);
  
  // Abilita lettura
  digitalWrite(GB_RD_PIN, LOW);
    
  // Imposta D1/D9 in INPUT
  for (int i = 2; i <= 9; i++) {
    pinMode(i, INPUT);
  }
  
  // Lettura pins D2/D9 (Bus Dati)
  byte bval = 0;
  for (int y = 0; y < 8; y++) {
    bitWrite(bval, y, digitalRead(y + 2));
  }
  
  // Disabilita lettura
  digitalWrite(GB_RD_PIN, HIGH);

  // Serial.println("readByte=" + (String)bval);
  return bval;
}
