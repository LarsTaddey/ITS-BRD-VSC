# Java Code --- Sieb von Erathosthenes

- In Java brauchen wir zwei Felder. Die Felder kann man als Speicherbereicht betrachen, wenn man das Programm in Assembler übernimmt.
- Nach Aufgabenstellung sollen die einfachen Felder benutzt werden und keine List<>
- Das Feld, für das Sieb, muss vom Typ boolean[] sein, um den Anspruch zu erfüllen, dass ein Feldelement[i] = wahr/falsch enthält (Typ Feldelement: boolean)
- Das Feld, für die tatsächlichen Primzahlen, sollte vom Typ int[] sein, damit die Primzahlen als Integer in das Feld eingetragen werden können (Typ Feldelement: 
  Integer)   

public class PrimzahlSieb
{
    public static void main(String[] args)
    {
        sieb();

    }

    private static final int _NUMBER = 1000;
    private static final boolean[] _PRIMZAHL_SIEB = new boolean[_NUMBER + 1];
    private static final int _START_PRIMZAHL = 2;

    private static void sieb()
    {
        // Alle Feldelemente ab 2 auf 1 (wahr) setzen
        for (int i = _START_PRIMZAHL; i <= _NUMBER; ++i)
        {
            _PRIMZAHL_SIEB[i] = true;
        }

        // Finde die nächste potenzielle Primzahl
       for(int i = _START_PRIMZAHL; i <= _NUMBER; ++i)
       {
           // Wenn das Quadrat groesser als das Limit, dann abbrechen
           if(i * i > _NUMBER)
           {
               break;
           }

           // Wenn die nächste Primzahl gefunden wurde, also true dann markiere das Quadrat und alle vielfachen
           if(_PRIMZAHL_SIEB[i])
           {
               for(int j = i * i; j <= _NUMBER; j += i)
               {
                _PRIMZAHL_SIEB[j] = false;
               }
           }
       }
        // Wenn alle Vielfachen markiert wurden, sind nur noch Primzahlen übrig
        // Jetzt wird die Methode Abspeichern aufgerufen, um die Primzahlen in einem eigenem Feld zu speichern
       abspeichern();
    }

    private static void abspeichern()
    {
        int count = 0;
        // Die Groesse des neuen Feldes bestimmen, anhand der gefundenen Werte, die =true sind
        for (int i = 0; i <= _NUMBER; ++i)
        {
            if (_PRIMZAHL_SIEB[i])
            {
                ++count;
            }
        }
        int[] primZahlen = new int[count];

        // Zähler wieder auf 0 setzen
        count = 0;
        // Gefundene Primzahlen in ein neues Feld schreiben
        // i hat den Wert der gesuchten Primzahl, da an der Stelle ein true gefunden wurde
        for( int i = _START_PRIMZAHL; i <= _NUMBER; ++i)
        {
            if (_PRIMZAHL_SIEB[i])
            {
                primZahlen[count] = i;
                ++count;
            }
        }
    }
}

# Assembler Programm --- Sieb von Erathosthenes

;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	
	AREA MyData, DATA, align = 2

PrimZahlSieb		DCB 0


Sieb									; Teilfunktion, um Primzahlen innerhalb eines Bereichs von 2 bis Limit zu finden

init									; Initialisiere die benötigen Register für Sieb
	; Setze erste Register
		ldr		r0, =PrimZahlSieb			; Lade die Adresse von PrimZahlSieb in r0

		; Setze unser Limit in r1
		mov 	r1, #1000				
	
		mov 	r6, #1						; Setze r6 auf 1
		mov 	r5, #2						; Zähler r5 auf 2 setzen

initLoop								; Schleife um alle Bytes ab Offset 2 (Dezi. 2) auf 1 (wahr) zu setzen

		cmp 	r5, r1						; Vergleiche ob Zähler größer als Limit ist
		bgt     SiebStart					; Wenn bis zum Limit überall 1 gesetzt wurde, fange mit dem Sieben an

		strb 	r6, [r0, r2]				; Speicher 1 an aktuelle Zählstelle im Byte
		add 	r5, r5, #1					; Erhöhe den Zähler um 1 bis Limit
		b 		initLoop					; Solange nicht alle Bytes gesetzt sind springe zum Anfang der Schleife

SiebStart								; Start vom Sieben
	
		mov 	r2, #2						; Startzahl auf 2 setzen	
		mov 	r5, #1						; Zähler auf 1 setzen. 1, damit die kleinste zu betrachtende Zahl mit eingeschlossen ist

LoopFindPrime							; Schleife um das nächste Byte zu finden, das eine 1 enthält
	
		mul 	r3, r2, r2			 		; Multipliziere die Zahl mit sich selbst und speicher das Ergebnis in r3

		cmp 	r3, r1					; Vergleiche, ob das Quadrat der aktuellen Primzahl größer als das Limit ist
		bgt 	Abspeichern					; Wenn das Quadrat größer ist, fang mit der Funktion Abspeichern an
		
		add 	r5, r5, #1					; Erhöhe den Zähler um 1
		ldrb 	r4, [r0,r5]					; Lade das Byte aus dem Speicher an der Adresse von Basisadresse des Siebs mit Offset des Zählers

		cmp 	r4, #1						; Vergleiche, ob das gefundene Byte 1 ist
		bne 	increasePrime				; Wenn das gefundene Byte nicht 1 ist, dann erhöhe die zu suchende Primzahl um 1. Hier wurde schon markiert
		
		mov 	r6, #0						; Setze r6 auf 0 zum markieren von Bytes

loopMarkMult							; Schleife um das Quadrat und weitere Vielfache der gefundenen Zahl zu markieren
		
		cmp 	r3, r1						; Vergleiche, ob das Quadrat der aktuellen Zahl größer als das Limit ist
		bgt 	increasePrime				; Wenn das Quadrat größer ist dann erhöhe unsere zu findene Primzahl um 1

		strb 	r6, [r0, r3]				; Markiere das Byte im Speicher Basisadresse plus das Vielfache als Offset
		add 	r3, r3, r2					; Erhöhe das Vielfache um die gefundene Zahl
		b 		loopMarkMult				; Springe zu Anfang der Schleife zurück

increasePrime							; Nebenfunktion, um die zu findene Primzahl um 1 zu erhöhen

		add		r2, r2, #1					; Erhöhe die zu findene Primzahl um 1
		b 		LoopFindPrime				; Springe zur Schleife zurück, um die nächste potenzielle Primzahl zu finden


Abspeichern								; Analysiert Speicherbereich vom Sieb und speichert gefundene Primzahlen als tatsächlichen Wert in neuen Speicherbereich
	
		add 	r4, r0, r1					; Berechne den neuen Speicherbereich r4 anhand der Basisadresse des Siebs und des Limits
		add 	r4, r4, #1					; Offset 1, damit die letzte Stelle vom Sieb nicht überschrieben wird

		mov 	r5, #1						; Zähler r5 auf 1 setzen

LoopFindTruePrimes						; Soll die verbleibenden Bytes mit Wert 1 in dem Sieb finden

		cmp		r5, r1						; Vergleiche, ob Zähler gleich dem Limit ist
		beq 	forever				        ; Wenn Zähler = Limit, dann wurden Primzahlen in neuen Speicherbereich geschrieben und fertig

		add 	r5, r5, #1					; Erhöhe den Zähler um 1
		ldrb 	r2, [r0, r5]				; Lade das Byte aus dem Speicherbereich vom Sieb plus Offset
		cmp 	r2, #1						; Vergleiche, ob der geladene Wert eine 1 ist
		bne 	LoopFindTruePrimes			; Wenn der gefundene Wert keine 1 ist, dann suche weiter bis eine 1 gefunden wurde
	
		strb 	r5, [r4]					; Der Wert des Zählers r5 ist unsere Primzahl, weil an der Stelle eine 1 gefunden wurden
		add 	r4, r4, #1					; Erhöhe die temporäre Adresse um 1
		b 		LoopFindTruePrimes			; Spring zurück und suche die nächste 1

forever	b	forever		; nowhere to retun if main ends	