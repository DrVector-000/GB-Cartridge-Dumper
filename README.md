# Game Boy Cartridge Dumper

Il progetto nasce in un contesto più ampio di conservazione della tecnologia software e hardware dei primi anni dell'informatica applicata a dispositivi di gioco.

E' composto da Hardware e software studiati per il backup di cartucce di gioco della console portatile Game Boy e Game Boy Color.

La componente Hardware utilizza la scheda Arduino Nano (ATMega328P) come controllore e degli Shift Register (74HC595) per estenderne gli I/O; mentre la parte software è formata dal firmware caricato sull'Arduino ed un client Windows in C# per .Net Framework.


![alt text](https://github.com/DrVector-000/GB-Cartridge-Dumper/blob/main/Images/GB%20Cartridge%20Dumper%20001.JPG?raw=true)

## Funzionalità implementate e previste
- [x] Lettura Informazioni dalla cartuccia.
- [x] Backup ROM su file.
- [x] Backup RAM salvataggio del gioco su file.
	- [x] Memory Bank Controller MBC5. Primi test passati con successo.
	- [ ] Altri MBC
- [ ] Backup RAM GB Camera su file.
- [X] Restore RAM.
	- [x] Memory Bank Controller MBC5. Primi test passati con successo.
	- [ ] Altri MBC
- [ ] Restore ROM su Flash Carts.

## Compatibilità
[Lista compatibilità](https://github.com/DrVector-000/GB-Cartridge-Dumper/blob/main/Docs/Compatibility%20List.txt)

## Licenza
[MIT](https://github.com/DrVector-000/GB-Cartridge-Dumper/blob/main/LICENSE.txt)
