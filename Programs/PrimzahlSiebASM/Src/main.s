;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Silke Behn	
;* Version            : V1.0
;* Date               : 01.06.2021
;* Description        : This is a simple main.
;					  :
;					  : Replace this main with yours.
;
;*******************************************************************************
    EXTERN initITSboard
    EXTERN lcdPrintS            ;Display ausgabe
    EXTERN GUI_init
;	EXTERN TP_Init

;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	
	AREA MyData, DATA, align = 2
	
	    GLOBAL text
DEFAULT_BRIGHTNESS DCW  800
	
text	DCB	"Hallo liebes TI-Labor (asm-project)",0


;********************************************
; Code section, aligned on 8-byte boundery
;********************************************

	AREA |.text|, CODE, READONLY, ALIGN = 3				

;--------------------------------------------
; main subroutine
;--------------------------------------------
	EXPORT main [CODE]
	
main	PROC				

        BL initITSboard
		ldr r1, =DEFAULT_BRIGHTNESS
		ldrh r0, [r1]
		bl GUI_init
		mov r0, #0x00
;		bl TP_Init
		
		LDR	r0,=text
        BL  lcdPrintS

; Labels benötigter Speicherbereiche setzen.
; Benötigte Labels: 	
	; PrimZahlSieb: Speicherbereich für die Werte, ob eine Zahl eine Primzahl ist. Repräsentiert durch die Adresse relativ zu PrimZahlSieb (Zahl 2 = PrimZahlSieb + 2)
	; Primzahlen: Speicherbereich für die gefundenen Primzahlen als tatsächliche Zahlen
	; Sieb: Zeigt auf den Beginn der Funktion, um Primzahlen mit dem Verfahren nach Erathosthenes zu finden
	; Abspeichern: Schreibt die gefunden Primzahlen in den Speicher ab Abspeichern

		; Für den Speicherbereich mit dem Label PrimZahlSieb müssen so viele Bytes gesetzt werden, wie das Limit (1000 Bytes)
		; Für den Speicherbereich mit dem Label Abspeichern müssen so viele Bytes, mit Primzahlen gefüllt werden, wie Primzahlen gefunden worden

		; Sieb: 
			; Soll erst alle benötigten Bytes ab PrimZahlSieb auf 1 s setzen. 1 repräsentiert ein true und 0 ein false. Benötigte Bytes = Anzahl gefundener Primzahlen
			; Wenn das Quadrat der gefundenen Primzahl größer als das Limit ist, ist das Sieben fertig und es kann zu Abspeichern gesprungen werden
			; Die kleinste gefundene Zahl soll direkt als Primzahl gespeichert werden
			; Danach wird das Quadrat der momentanen Zahl (mit 1) auf 0 gesetzt
			; Dann werden alle weiteren vielfachen der Zahl auf 0 gesetzt (Adresse von PrimZahlSieb + Offset soll 0 sein)
			; Es wird die nächste potentielle Primzahl gesucht (das nächste Byte das 1 enthält)
			; Dabei werden bereits auf 0 gesetzte Stellen nicht mehr betrachtet
			; Dann wird zu Sieb gesprungen bis die größte Primzahl erreicht ist und alle Vielfachen gestrichen wurden
		
		; Für Sieb benötigt man einige Register.
			; r0 enthält Basisadresse unseres Siebs
			; r1 enthält aktuelles Limit
			; r2 enthält aktuelle Primzahl
			; r3 enthält das quadrat einer Primzahl und das vielfachen
			; r4 zur berechnung temporärer Adressen oder Primzahlen
			; r5 als Zählregister
			; r6 zur markierung von vielfachen und zur Initialisierung des Siebs

		; Abspeichern:
				
			; Soll den gesiebten Speicherbereich analysieren und gefundene Primzahlen in eigenem Speicherbereich schreiben
			; Dabei ist der Zähler der tatsächliche Wert der gesuchten Primzahl, da an der Stelle die 1 im Byte gefunden wurde
			; Das passiert solange, bis der Zähler einmal durch das ganze Sieb gegangen ist
			; In den neuen Speicherbereich werden dann die gefundenen Primzahlen in den Speicher hineingeschrieben
			; Das Ergebnis ist ein Speicherbereich mit allen gefunden Primzahlen aus dem Bereich 2 bis n (hier n = 1000)
			; Wenn das Sieb fertig durchlaufen ist, dann ist das Programm fertig

		; Abspeichern braucht weniger Register
			; r0 enthält Basisadresse des Siebs
			; r1 enthält aktuelles Limit
			; r4 enthält Adresse des neuen Speicherbereichs
			; r5 als Zählregister


forever	b	forever		; nowhere to retun if main ends		
		ENDP
	
		ALIGN
       
		END