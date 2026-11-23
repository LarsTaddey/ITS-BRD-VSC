# Bildanzeige

Dieses Programm für das ITS-Board liest BMP-Dateien per USB oder UART ein und zeigt sie auf dem Display an.

## Module

main.c
-	int main()

bmp_display.c
-	int drawBmp()

read.c
-	bool was_eof
-	char read_uint8()
-	uint16 read_uint16()
-	uint32 read_uint32()
-	int32 read_int32()

## Autoren

- Len Peschko, HAW Hamburg (2840024)
- Lars Taddey, HAW Hamburg (2880138)
