;************************************************
;* Beginn der globalen Daten *
;************************************************
    AREA MyData, DATA, align = 2

LIMIT 			EQU		1000								; Die gesetzte Obergrenze des Bereichs (hier 1000)
PrimZahlSieb 	FILL 	LIMIT + 1, 1						; Der Speicherbereich, der das Sieb repräsentiert 0 = false, 1 = true
Primzahlen		FILL 	(LIMIT / 2) * 2						; Der Speicherbereich, der die gefundenen Primzahlen enthält. 
															; Länge wird approximiert, um etwas Speicher zu sparen
															; Für 1000 kann auch direkt 168 als Größe angegeben werden

;***********************************************
;* Beginn des Programms *
;************************************************
    AREA |.text|, CODE, READONLY, ALIGN = 3

; ----- S t a r t des Hauptprogramms -----
        		EXPORT main
main    		PROC

;------------------------------------------------
; Teilfunktion Sieb
;------------------------------------------------
Sieb		
				ldr		R0, =PrimZahlSieb					; Startadresse des Siebs wird in R0 geladen
				ldr 	R1, =LIMIT							; Der Wert von Limit wird in R1 geladen

				mov 	R6, #0								; Setze R6 auf 0 zum markieren
				strb    R6, [R0]							; Setze ersten beiden Bytes im Speicher, der Vollständigkeit, auf 0
				strb 	R6, [R0, #1]						
;------------------------------------------------			
; Äußere for-Schleife: Finde potentielle Primzahlen 
;------------------------------------------------	
for_sieben
				mov 	R5, #2								; Setze den Zähler wieder auf 2
				mov 	R6, #0								
until_sieben
				mul 	R3, R5, R5							; Berechne das Quadrat aus der momentanen Stelle (Zähler) und schreibe in R3
				cmp		R3, R1								; Vergleiche, ob R3 größer als R1
				bgt 	enddo_sieben						; Wenn R3 größer R1, ist man fertig. Sonst gehe in die Schleife
do_sieben
												
						
if_sieben
				ldrb 	R2, [R0, R5]						; Lade den Wert aus dem Byte von R0 + R5
				cmp 	R2, #1								; Vergleiche, ob R2 gleich 1 ist, also R2 eine gültige Primzahl
				beq 	then_sieben							; Wenn R2 == 1, dann fang mit dem markieren der Vielfachen an,
				b 		endif_sieben						; sonst finde die nächste potentielle Primzahl
then_sieben	

for_markieren

until_markieren
				cmp 	R3, R1								; Vergleiche, ob R3 größer R1 ist
				bgt 	enddo_markieren						; Wenn R3 > R1, dann ist das Markieren dieser Vielfache abgeschlossen,
															; sonst gehe in die Schleife
do_markieren
				strb	R6, [R0, R3]						; Speicher den Wert 0 im Byte an der Stelle R0 + R3. Also markiere Vielfache (setze auf 0)
step_markieren
				add 	R3, R5								; Addiere zu dem Vielfachen die eigentliche Zahl
				b 		until_markieren
enddo_markieren									

endif_sieben

step_sieben							
				add 	R5, #1								; Erhöhe den Zähler um 1
				b 		until_sieben						; Springe zum Anfang der äußeren Schleife
enddo_sieben
															; Sieb ist fertig, mache mit Teilfunktion Abspeichern weiter

;------------------------------------------------			
; Teilfunktion Abspeichern
;------------------------------------------------
Abspeichern
				ldr 	R4, =Primzahlen						; Lade die Startadresse von Primzahlen in R4
for_abspeichern
				mov 	R5, #2								; Setze den Zähler wieder auf 2
until_abspeichern
				cmp 	R5, R1								; Vergleiche, ob R5 > R1 ist,
				bgt 	enddo_abspeichern					; Wenn R5 > 1, dann ist Abspeichern fertig und somit das ganze Programm,
															; sonst gehe in die Schleife
do_abspeichern																							

if_abspeichern
				ldrb 	R2, [R0, R5]						; Lade ein Byte ab Stelle R0 + R5 in R2
				cmp 	R2, #1								; Vergleiche, ob R2 == 1									
				bne 	endif_abspeichern					; Wenn R2 == 1, dann mache weiter, sonst pringe zu endif_abspeichern
then_abspeichern
				str 	R5, [R4]							; Speicher den Wert des Zählers, der einer Primzahl entspricht, im Speicher an der Adresse R4
				add 	R4, #2								; Erhöhe die Speicheradresse um 2, damit größre Zahlen dargestellt werden können (2 Bytes)															
endif_abspeichern											
				
step_abspeichern
				add 	R5, #1								; Erhöhe den Zähler um 1
				b 		until_abspeichern					; Springe zum Anfang der Schleife
enddo_abspeichern					

forever         
				; Existiert nur für den Breakpoint zum debuggen
				mov 	R0, #0        						
				b forever             
                ENDP
                END