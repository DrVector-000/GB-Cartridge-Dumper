/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  Utilizza 74HC595 shift registers
*/

#include <Arduino.h>
#include "Const.h"
#include "SRHelper.h"
#include "GBHelper.h"
#include "GBHeader.h"

//******************************************************************************************************************//
//* Variabili globali
//******************************************************************************************************************//
bool serialEcho = false;

//******************************************************************************************************************//
//* Setup
//******************************************************************************************************************//
void setup() {
  // Inizializza Ouput Pins per SN74HC595
  pinMode(SN_SRCLK_PIN, OUTPUT);
  pinMode(SN_SER_PIN, OUTPUT);
  pinMode(SN_RCLK_PIN, OUTPUT);

  // Inizializza Ouput Pins connettore GB
  pinMode(GB_RD_PIN, OUTPUT);
  pinMode(GB_WR_PIN, OUTPUT);
  pinMode(GB_MREQ_PIN, OUTPUT); 

  // Inizializza in lettura
  digitalWrite(GB_RD_PIN, LOW); 
  digitalWrite(GB_WR_PIN, HIGH);
  digitalWrite(GB_MREQ_PIN, HIGH);

  // Imposta pin D2/D9 in INPUT
  for (int i = 2; i <= 9; i++) {
    pinMode(i, INPUT);
  }

  // Azzera lo shift register
  addressWrite(0x0000);

  Serial.begin(115200);
  Serial.println("GAME BOY CARTRIDGE DUMPER V.1.00");
  Serial.println("");
}

//******************************************************************************************************************//
//* Ciclo principale
//******************************************************************************************************************//
void loop() {
  // Lettura porta seriale
  String s = ReadSerialComand();

  // Parsing dei comandi
  ParseComands(s);
}

//******************************************************************************************************************//
//* Comandi
//******************************************************************************************************************//
// Parsing dei comandi
void ParseComands(String s) {
  String params[10];

  s.toUpperCase();
  if (s != "") {
    if (serialEcho) {
      Serial.println(s);
    }

    String comand = GetComand(s);
    // Serial.println("COMAND: " + comand);
    //**********************************************
    // ECHO
    //**********************************************
    if (comand == "ECHO") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "1") {
        serialEcho = true;
      }
      else if (params[0] == "0") {
        serialEcho = false;
      }
      else if (params[0] == "?") {
        Serial.println("+ECHO=" + String(serialEcho));
      }
    }
    //**********************************************
    // VERSION
    //**********************************************
    if (comand == "VERSION") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        Serial.println("+VERSION=0.011b");
      }
    }
    //**********************************************
    // READBYTE
    //**********************************************
    if (comand == "READBYTE") {
      GetComandParams(s, params);
      //Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        byte b = readByte(params[0].toInt());
        Serial.println("+READBYTE=" + (String)b);
      }
    }
    //**********************************************
    // GETTITLE
    //**********************************************
    if (comand == "GETTITLE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        String s = readCartTitle();
        Serial.println("+GETTITLE=" + s);
      }
    }
    //**********************************************
    // GETCODE
    //**********************************************
    if (comand == "GETCODE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        String s = readCartCode();
        Serial.println("+GETCODE=" + s);
      }
    }
    //**********************************************
    // GETTYPE
    //**********************************************
    if (comand == "GETTYPE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readCartType();
        Serial.println("+GETTYPE=" + (String)i);
      }
    }
    //**********************************************
    // GETROMSIZE
    //**********************************************
    if (comand == "GETROMSIZE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readROMSize();
        Serial.println("+GETROMSIZE=" + (String)i);
      }
    }
    //**********************************************
    // GETRAMSIZE
    //**********************************************
    if (comand == "GETRAMSIZE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readRAMSize();
        Serial.println("+GETRAMSIZE=" + (String)i);
      }
    }
    //**********************************************
    // GETROMBANKS
    //**********************************************
    if (comand == "GETROMBANKS") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = GetROMBanks();
        Serial.println("+GETROMBANKS=" + (String)i);
      }
    }
    //**********************************************
    // DUMPROMBANK
    //**********************************************
    if (comand == "DUMPROMBANK") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        dumpROMBank(params[0].toInt());
        Serial.println("+++");
      }
    }
    //**********************************************
    // DUMPRAM
    //**********************************************
    if (comand == "DUMPRAM") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        dumpRAM();
        Serial.println("+++");
      }
    }
  }
}

// Ritorna la stringa di comando
String GetComand(String s) {
  String comand = "";

  int i = s.indexOf('=');
  return s.substring(0, i);
}

// Ritorna lista parametri
void GetComandParams(String s, String(&params)[10]) {
  int index = s.indexOf('=');
  String par = s;
  par.remove(0, index + 1);
  
  for (int i = 0; i < 10; i++) {
    index = par.indexOf(',');
    if (index == -1) {
      int l = par.length();
      if (l > 0) {
        params[i] = par;
      }
      return;
    }
    else {
      params[i] = par.substring(0, index);
    }
  }
}

//******************************************************************************************************************//
//* Lettura comandi su porta seriale
//******************************************************************************************************************//
// Variabili globali
char receivedChars[64] = "";
int rcIndex = 0;

String ReadSerialComand(){
  while (Serial.available()) {
    if (rcIndex > 63) rcIndex = 0;
    
    // Legge carattere dalla seriale
    char rc = Serial.read();
    // Carattere di fine comando
    if (rc == '\n' or rc == '\r') {
      receivedChars[rcIndex] = '\0';
      rcIndex = 0;
      return receivedChars;
    }
    else {
      // Nuovo carattere
      receivedChars[rcIndex] = rc;
      rcIndex++;
    }
  }
  
  return "";
}

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
    
    if (bank > 0) {
      selectROMBank(bank);
      addr = 0x4000;
    }
    
    for (int i = 0; i <= 0x3FFF; i++) {  
      byte bval = readByte(addr + i);
      Serial.println(bval, DEC);
    }
    
    // Serial.println("END DUMP BANK " + (String)bank);
}


//******************************************************************************************************************//
//* Dump ROM
//******************************************************************************************************************//
void dumpROM() {
    Serial.println("START DUMP");

    unsigned int addr = 0;
    int romBanks = GetROMBanks();
    
    // Read x number of banks
    for (int y = 1; y < romBanks; y++) {
      selectROMBank(y);

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

// Select the ROM bank by writing the bank number on the data pins
void selectROMBank(int bank) {
  addressWrite(0x2100);

  bankSelect(bank); // Select the bank
  digitalWrite(GB_WR_PIN, LOW); // WR on
  digitalWrite(GB_WR_PIN, HIGH); // WR off
   
  // Reset outputs to LOW and change back to inputs
  for (int i = 2; i <= 9; i++) {
    digitalWrite(i, LOW);
    pinMode(i, INPUT);
  }
}

// Turn on the data lines corresponding to the bank number
void bankSelect(int bank) {
  // Change to outputs
  for (int i = 2; i <= 9; i++) {
    pinMode(i, OUTPUT);
  }
  
  // Read bits in bank variable
  for (int z = 9; z >= 2; z--) {
    if (bitRead(bank, z - 2) == HIGH) {
      digitalWrite(z, HIGH);
    }
    else {
      digitalWrite(z, LOW);
    }
  }
}

//******************************************************************************************************************//
//* Numero blocchi RAM
//******************************************************************************************************************//
int GetRAMBanks() {
  int ramBanks = 0; // Default no RAM banks
  int ramSize = readRAMSize();
  /*
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
  */
  return ramBanks; 
}

//******************************************************************************************************************//
//* Lettura di un byte all'indirizzo selezionato (RAM)
//******************************************************************************************************************//
byte readRAMByte(unsigned int address) {
  // Imposta indirizzo
  addressWrite(address);
  
  // Imposta D1/D9 in INPUT
  for (int i = 2; i <= 9; i++) {
    pinMode(i, INPUT);
  }
  
  digitalWrite(GB_MREQ_PIN, LOW);
  digitalWrite(GB_RD_PIN, LOW);

  // Lettura pins D2/D9
  byte bval = 0;
  for (int y = 0; y < 8; y++) {
    bitWrite(bval, y, digitalRead(y + 2));
  }
  
  digitalWrite(GB_MREQ_PIN, HIGH);
  digitalWrite(GB_RD_PIN, HIGH);

  // Serial.println("readByte=" + (String)bval);
  return bval;
}

void selectRAMBank(int bank) {
  addressWrite(0x4000);

  bankSelect(bank); // Select the bank
  digitalWrite(GB_WR_PIN, LOW); // WR on
  digitalWrite(GB_WR_PIN, HIGH); // WR off
   
  // Reset outputs to LOW and change back to inputs
  for (int i = 2; i <= 9; i++) {
    digitalWrite(i, LOW);
    pinMode(i, INPUT);
  }
}

//******************************************************************************************************************//
//* Dump RAM
//******************************************************************************************************************//
void dumpRAM() {
    Serial.println("START RAM DUMP");

    unsigned int addr = 0x0000;
    int ramSize = readRAMSize();
    
    // Disattiva READ, WRITE e MREQ
    digitalWrite(GB_RD_PIN, HIGH); // RD off
    digitalWrite(GB_WR_PIN, HIGH); // WR off
    digitalWrite(GB_MREQ_PIN, HIGH); // MREQ off

    // Imposta indirizzo 0x0000
    addressWrite(addr);
    delay(1);
  
    // Imposta D0-D7 in OUTPUT
    for (int l = 2; l <= 9; l++) {
      pinMode(l, OUTPUT);
    }

    // Inizializza MBC: 0x0A
    digitalWrite(3, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(GB_WR_PIN, LOW); // WR on
    digitalWrite(GB_WR_PIN, HIGH); // WR off

    // Azzera D0-D7 ed imposta in INPUT
    for (int l = 2; l <= 9; l++) {
      digitalWrite(l, LOW);
      pinMode(l, INPUT);
    }
      
    if (ramSize > 0) {
      // 2 KBytes RAM
      if (ramSize == 1) {

      }
      // 8 KBytes RAM
      else if (ramSize == 2) {
        addr = 0xA000;
        selectRAMBank(0);
        for (; addr <= 0xBFFF; addr++) { 
          byte bval = readRAMByte(addr);
          Serial.println(bval, DEC);
        }
      }
      // RAM banks
      else {

      }
    }
    else {
      Serial.println("NO RAM");
    }

    Serial.println("END RAM DUMP");
}
