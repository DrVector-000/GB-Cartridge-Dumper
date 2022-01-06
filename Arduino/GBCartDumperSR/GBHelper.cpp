/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Helper funzioni di gestione bus dati e control pins della cartuccia Game Boy
*/

#include <Arduino.h>
#include "Const.h"
#include "SRHelper.h"

//******************************************************************************************************************//
//* Abilita/Disabilita lettura
//******************************************************************************************************************//
void enableRead(bool enable) {
  if (enable) {
    // Abilita lettura
    digitalWrite(GB_RD_PIN, LOW);
      
    // Imposta D1/D9 in INPUT
    for (int i = 2; i <= 9; i++) {
      pinMode(i, INPUT);
    }
  }
  else {
    // Disabilita lettura
    digitalWrite(GB_RD_PIN, HIGH);
  }
}

//******************************************************************************************************************//
//* Abilita/Disabilita CS
//******************************************************************************************************************//
void enableCS(bool enable) {
  if (enable) {
    digitalWrite(GB_MREQ_PIN, LOW);
  }
  else {
    digitalWrite(GB_MREQ_PIN, HIGH);
  }
}

//******************************************************************************************************************//
//* Lettura di un byte all'indirizzo selezionato dal bus dati della cartuccia
//******************************************************************************************************************//
byte readByte(unsigned int address, bool ram) {
  // Imposta indirizzo
  addressWrite(address);

  // Lettura pins D2/D9 (Bus Dati)
  byte bval = 0;
  for (int y = 0; y < 8; y++) {
    bitWrite(bval, y, digitalRead(y + 2));
  }

  // Serial.println("readByte=" + (String)bval);
  return bval;
}

//******************************************************************************************************************//
//* Abilita/Disabilita scrittura
//******************************************************************************************************************//
void enableWrite(bool enable) {
  if (enable) {
    // Abilita scrittura
    digitalWrite(GB_WR_PIN, LOW);

    // Imposta D1/D9 in OUTPUT
    for (int i = 2; i <= 9; i++) {
      pinMode(i, OUTPUT);
    }
  }
  else {
    // Disabilita scrittura
    digitalWrite(GB_WR_PIN, HIGH);

    // Azzera bus dati e ripristina in INPUT
    for (int i = 2; i <= 9; i++) {
      digitalWrite(i, LOW);
      pinMode(i, INPUT);
    }
  }
}

//******************************************************************************************************************//
//* Scrittura di un byte all'indirizzo selezionato nel bus dati della cartuccia
//******************************************************************************************************************//
void writeByte(unsigned int address, byte data) {
  // Imposta indirizzo
  addressWrite(address);

  // Abilita scrittura
  enableWrite(true);
  
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
  enableWrite(false);
}