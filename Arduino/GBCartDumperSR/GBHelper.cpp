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
byte readByte(unsigned int address, bool ram) {
  // Imposta indirizzo
  addressWrite(address);
  
  // Abilita lettura
  if (ram) {
    digitalWrite(GB_MREQ_PIN, LOW);
  }
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
  if (ram) {
    digitalWrite(GB_MREQ_PIN, HIGH);
  }
  digitalWrite(GB_RD_PIN, HIGH);

  // Serial.println("readByte=" + (String)bval);
  return bval;
}

//******************************************************************************************************************//
//* Scrittura di un byte all'indirizzo selezionato nel bus dati della cartuccia
//******************************************************************************************************************//
void writeByte(unsigned int address, byte data) {
  // Imposta indirizzo
  addressWrite(address);

  // Abilita scrittura
  digitalWrite(GB_WR_PIN, LOW);

  // Imposta D1/D9 in OUTPUT
  for (int i = 2; i <= 9; i++) {
    pinMode(i, OUTPUT);
  }
  
  // Scrittura pins D2/D9 (Bus Dati)
  for (int z = 9; z >= 2; z--) {
    if (bitRead(data, z - 2) == HIGH) {
      digitalWrite(z, HIGH);
    }
    else {
      digitalWrite(z, LOW);
    }
  }
  
  // Disabilita scrittura
  digitalWrite(GB_WR_PIN, HIGH);

  // Azzera bus dati e ripristina in INPUT
  for (int i = 2; i <= 9; i++) {
    digitalWrite(i, LOW);
    pinMode(i, INPUT);
  }
}