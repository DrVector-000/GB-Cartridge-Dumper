/*  
  GBCartDumperSR.ino - Game Boy Cartridge Dumper
  Copyright (C) 2021 DrVector
  
  Helper funzioni di gestione bus dati e control pins della cartuccia Game Boy
*/

//******************************************************************************************************************//
//* Lettura di un byte all'indirizzo selezionato dal bus dati della cartuccia
//******************************************************************************************************************//
byte readByte(unsigned int address);