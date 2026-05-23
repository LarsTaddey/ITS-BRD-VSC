;************************************************
;* Beginn der globalen Daten *
;************************************************
                   AREA MyData, DATA, align = 2

LIMIT 			EQU		31									; Die gesetzte Obergrenze des Bereichs (hier 1000)
PrimZahlSieb 	FILL 	LIMIT + 1							; Der Speicherbereich, der das Sieb repräsentiert 0 = false, 1 = true
Primzahlen		FILL 	(LIMIT / 2) * 4						; Der Speicherbereich, der die gefundenen Primzahlen enthält. 
															; Länge wird approximiert, um etwas Speicher zu sparen

;***********************************************
;* Beginn des Programms *
;************************************************
    AREA |.text|, CODE, READONLY, ALIGN = 3
; ----- S t a r t des Hauptprogramms -----
        		EXPORT main
        		EXTERN initITSboard
main    		PROC

;------------------------------------------------
; Teilfunktion Sieb
;------------------------------------------------
Sieb		
				ldr		R0, =PrimZahlSieb					; Startadresse des Siebs wird in R0 geladen
				ldr 	R1, =LIMIT							; Der Wert von Limit wird in R1 geladen

;------------------------------------------------			
; for-Schleife: Initialisieren
;------------------------------------------------	
for_init
				mov 	R5, #2								; Setze den Zähler R5 auf 2
				mov 	R6, #1								; Setze R6, zum setzen der Bytes im Sieb, auf 1
until_init
				cmp 	R5, R1								; Schleifenabbruch, wenn R5 größer als R1 (LIMIT) ist
				bgt 	enddo_init							; Sonst springe in die Schleife
do_init
				strb	R6, [R0,R5]							; Setze das Byte an der Stelle ab R0 + R5, im Speicher, auf 1
step_init
				add 	R5, #1								; Erhöhe den Zähler um 1
				b 		until_init							; Springe zum Schleifenanfang
enddo_init

;------------------------------------------------			
; Äußere for-Schleife: Finde potentielle Primzahlen 
;------------------------------------------------	
for_sieben
				mov 	R5, #2								; Setze den Zähler wieder auf 2
				mov 	R6, #0								; Setze R6 auf 0 zum markieren
until_sieben
				mul 	R3, R5, R5							; Berechne das Quadrat aus der momentanen Stelle (Zähler) und schreibe in R3
				cmp		R3, R1								; Vergleiche, ob R3 größer als R1
				bgt 	enddo_sieben						; Wenn R3 größer R1, ist man fertig. Sonst gehe in die Schleife
do_sieben
				ldrb 	R2, [R0, R5]						; Lade den Wert aus dem Byte von R0 + R5
				b 		if_sieben							; Springe in den If-Case

step_sieben							
				b 		NaechstePrimzahl					; Springe zu NaechstePrimzahl
enddo_sieben
				b 		Abspeichern							; Sieb ist fertig, springe zu Abspeichern

; Wenn gefundener Wert gleich 1 ist, dann markiere Vielfache, 
; sonst finde das nächste nichtgestrichene Byte
;------------------------------------------------	

if_sieben
				cmp 	R2, #1								; Vergleiche, ob R2 gleich 1 ist, also R2 eine gültige Primzahl
				beq 	then_sieben							; Wenn R2 == 1, dann fang mit dem markieren der Vielfachen an,
				b 		else_sieben							; sonst finde die nächste potentielle Primzahl
then_sieben	
				b		for_markieren						; Springe zu for_markieren

else_sieben		b 		endif_sieben						; Springe zu endif_sieben

endif_sieben	b 		NaechstePrimzahl					; Springe zu NaechstePrimzahl

;------------------------------------------------			
; Innere for-Schleife: Markiere Vielfache
;------------------------------------------------	
for_markieren

until_markieren
				cmp 	R3, R1								; Vergleiche, ob R3 größer R1 ist
				bgt 	enddo_markieren						; Wenn R3 > R1, dann ist das Markieren dieser Vielfache abgeschlossen,
															; sonst gehe in die Schleife
do_markieren
				strb	R6, [R0, R3]						; Speicher den Wert 0 im Byte an der Stelle R0 + R3. Also markiere Vielfache (setze auf 0)
step_markieren
				add 	R3, R5								; Addiere zu dem Vielfachen die eigentliche Zahl
				b 		until_markieren						; Springe zum Schleifenkopf
enddo_markieren
				b 		NaechstePrimzahl					; Wenn R3 > R1, dann finde die nächste potentielle Primzahl

;------------------------------------------------			
; Unterfunktion NaechstePrimzahl
; Erhöht den Zähler um 1
;------------------------------------------------	
NaechstePrimzahl
				add 	R5, #1								; Erhöhe den Zähler um 1, damit die nächste potentielle Primzahl gefunden werden kann
				b 		until_sieben						; Springe zum Schleifenkopf von sieben




;---Ignorieren ist nicht Teil der Aufgabe 5---------------------------------------------------------------------------------------------------------------------------------------------------------------

;------------------------------------------------			
; Teilfunktion Abspeichern
;------------------------------------------------
Abspeichern

forever         
				; Existiert nur für den Breakpoint zum debuggen
				mov 	R0, #0        						
				b forever             
                ENDP
                END