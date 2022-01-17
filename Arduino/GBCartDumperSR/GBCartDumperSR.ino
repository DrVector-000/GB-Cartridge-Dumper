/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021-2022 DrVector
  Utilizza 74HC595 shift registers
*/

#include <Arduino.h>
#include "Const.h"
#include "SRHelper.h"
#include "GBHelper.h"
#include "GBHeader.h"
#include "GBROM.h"
#include "GBRAM.h"

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

  // Serial.begin(115200);
  Serial.begin(500000);
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
        Serial.println("+VERSION=0.015b");
      }
    }
    //**********************************************
    // READBYTE
    //**********************************************
    if (comand == "READBYTE") {
      GetComandParams(s, params);
      //Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        byte b = readByte(params[0].toInt(), false);
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
    // GETCGBSUPPORT
    //**********************************************
    if (comand == "GETCGBSUPPORT") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readCGBSupportCode();
        Serial.println("+GETCGBSUPPORT=" + (String)i);
      }
    }
    //**********************************************
    // GETMAKERCODE
    //**********************************************
    if (comand == "GETMAKERCODE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        String s = readMakerCode();
        Serial.println("+GETMAKERCODE=" + s);
      }
    }
    //**********************************************
    // GETSGBSUPPORT
    //**********************************************
    if (comand == "GETSGBSUPPORT") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readSGBSupportCode();
        Serial.println("+GETSGBSUPPORT=" + (String)i);
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
    // GETDESTCODE
    //**********************************************
    if (comand == "GETDESTCODE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readDestinationCode();
        Serial.println("+GETDESTCODE=" + (String)i);
      }
    }
    //**********************************************
    // GETOLDMAKERCODE
    //**********************************************
    if (comand == "GETOLDMAKERCODE") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readOldMakerCode();
        Serial.println("+GETOLDMAKERCODE=" + (String)i);
      }
    }
    //**********************************************
    // GETROMVERSION
    //**********************************************
    if (comand == "GETROMVERSION") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = readROMVersion();
        Serial.println("+GETROMVERSION=" + (String)i);
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
    // GETRAMBANKS
    //**********************************************
    if (comand == "GETRAMBANKS") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        int i = GetRAMBanks();
        Serial.println("+GETRAMBANKS=" + (String)i);
      }
    }
    //**********************************************
    // DUMPRAMBANK
    //**********************************************
    if (comand == "DUMPRAMBANK") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        dumpRAMBank(params[0].toInt());
        Serial.println("+++");
      }
    }
    //**********************************************
    // WRiTERAMBANK
    //**********************************************
    if (comand == "WRITERAMBANK") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        writeRAMBank(params[0].toInt());
        //Serial.println("+++");
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
//* Lettura di un byte all'indirizzo selezionato (RAM)
//******************************************************************************************************************//
/*
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
*/

// Turn on the data lines corresponding to the bank number
/*
void bankSelectRAM(int bank) {
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

void selectRAMBank(int bank) {
  addressWrite(0x4000);

  bankSelectRAM(bank); // Select the bank
  digitalWrite(GB_WR_PIN, LOW); // WR on
  digitalWrite(GB_WR_PIN, HIGH); // WR off
   
  // Reset outputs to LOW and change back to inputs
  for (int i = 2; i <= 9; i++) {
    digitalWrite(i, LOW);
    pinMode(i, INPUT);
  }
}
*/

//******************************************************************************************************************//
//* Dump RAM
//******************************************************************************************************************//
/*
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
*/