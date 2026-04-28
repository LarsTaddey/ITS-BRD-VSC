# Praktikumsaufgabe Woche 2
1. [Dokumentation Anw01 bis Anw06]   
  - In Anw01 wird die VariableA (nicht der Inhalt!) in Register R0 geladen. Dabei verändert sich erstmal nur R0. R2 bleibt 
    0x4, und R3 0x8. R0 enthält 0x2000000c.

  - In Anw02 wird das Byte an der Adresse R0 aus dem Speicher in R2 geladen. R2 enthät dann das erste Byte aus &VariableA 
    0xbeef, also 0xef. R0 und R3 bleiben unverändert.

  - In Anw03 wird das nächste Byte mit offset +1 aus dem Speicher an der Adresse R0 in R3 geladen. Das nächste Byte an der    
    Adresse ist 0xbe, also R3 enhält 0xbe. R0 und R2 bleiben unverändert

  - In Anw04 wird der Inhalt von R2 um 8 Bit nach links verschoben und wird zum High-Byte. R2 sieht dann so aus: 0xef00.
    R0 und R3 bleiben unverändert

  - In Anw05 wird der Inhalt von R2 mit R3 durch die bitwise or operation verknüpft und das Resultat in R2 geschrieben.
    Bitwise or von R2 (0xef00) und R3 (0x00be) sieht so aus: 0xefbe. In R2 steht dann 0xefbe. R0 und R3 bleiben unverändert.
  
  - In Anw06 werden die unteren 16-Bit von R2 and Adresse R0 gespeichert. Im Speicher an der Adresse R0 (=VariableA) steht
    dann be ef und nicht mehr ef be. R0, R2 und R3 bleiben unverändert. Es wird lediglich der Inhalt des ersten und zweiten
    Bytes getauscht. 