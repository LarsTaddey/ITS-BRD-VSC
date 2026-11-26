# Bildanzeige

Dieses Programm für das ITS-Board liest RLE8-komprimierte 8-Bit-BMP-Dateien per USB-CDC (S7) oder UART-via-USB (S6) ein.

Ein Druck auf S5 bewirkt:
- Nächstes Bild anfordern, Header einlesen und anzeigen
- Bild anzeigen, wenn der Header korrekt war
- Nächstes Bild anfordern...

Naive Implementierung ohne Pufferung oder Skalierung, zu große Bilder werden abgeschnitten.

Basiert auf JS-Implementierung, sichtbar unter https://l3p3.de/svr/etc/bmprl.html.

## Module

main
-	int main()

bmp_display
-	void drawBmp(void) – Header einlesen und anzeigen, auf S5 warten, Farbtabelle und Bilddaten dekodieren und anzeigen

read
-	uint8_t read_uint8(bool *was_eof)
-	uint16_t read_uint16(bool *was_eof)
-	uint32_t read_uint32(bool *was_eof)

## Autoren

- Len Peschko, HAW Hamburg (2840024)
- Lars Taddey, HAW Hamburg (2880138)
