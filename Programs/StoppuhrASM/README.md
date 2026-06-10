# Stoppuhr

Das Programm soll eine Stoppuhr darstellen, welche in 3 Betriebszustände unterteilt ist
- INIT: Der erste Zustand, in dem die Zeit und LEDs zurückgesetzt werden
    - Mit der Taste S7 wechselt das Programm von INIT zu RUNNING

- RUNNING: Der zweite Zustand, in dem die vergangenge Zeit, seit dem Start, gemessen und auf dem Display ausgegeben wird.
    - Das Format ist dabei "mm:ss:nn" (minuten:sekunden:nanosekunden)
    - In diesem Zustand soll die LED D8 leuchten und mit der Taste S6 wird in den Zustand HOLD oder mit S5 in den Zustand INIT gewechselt

- HOLD: Der dritte Zustand, in dem die Anzeige angehalten und nur die gestoppte Zeit angezeigt wird, sobald S6 während RUNNING gedrückt wurde. 
- Dabei soll die Zeit im Hintergrund weiter laufen, so dass durch Drücken von S7 wieder die Zeit angezeigt wird und in den Zustand Running wechselt
    - Durch Drücken von S5 wird die Uhrzeit zurückgesetzt und die Uhr wechselt in den Zustand INIT
    - Durch Drücken von S7 wechselt die Uhr wieder in RUNNING
    - Während dem Zustand HOLD sollen die LEDs D8 und D9 leuchten

- Dabei soll das Programm in einem Super-Loop laufen
    - Aktualisiere die gestoppte Zeitspanne
    - lese Taster ein
    - Update Zustand der Stoppuhr
    - Wenn im Zustand INIT, dann setze die gestoppte Zeitspanne auf 0
    - Update LEDs in Abhängigkeit vom aktuellen Zustand

# Unterfunktionen

- readButtons:   liest das GPIO_F_PIN Register und speichert den Wert in einem Register und gibt dieses zurück
- setLEDs:      Setzt die LEDs abhängig vom Zustand. Dabei wird eine Maske übergeben um die LEDs erst auszuschalten und dann gezielt anzuschalten
- clearLEDs:    Schaltet alle LEDs auf den D_PINs aus
- displayTime:  Aktualisiert die Zeitanzeige auf dem Display
- checkTimer:   Liest den Zeitgeber aus und aktualisiert die Variable, die die Zeitspanne der Stoppuhr speichert
- updateClk:    Speichert den aktuellen Zeitstempel und berechnet die Zeitspanne, die zwichen zwei Aufrufen der Funktion vergangen ist.
- calcTime:     Rechnet die Zeit aus in Minuten:Sekunden:Millisekunden
- setTimeString: Ersetzt entsprechende Werte im Zeit String
- resetTimer:   Setzt den String Zeit zurück auf "00:00:00"

# INIT

- INIT ist der erste Zustand der Stoppuhr
- Beim Wechsel in INIT, werden alle LEDs ausgeschaltet, die Zeit zurückgesetzt und eventuell in den Zustand RUNNING gewechselt
- Es werden die Unterfunktion clearLEDs und resetTimer benötigt
# Running
- Beim Wechsel in RUNNING, wird die LED D8 angeschaltet und die Zeit gemessen, die seit dem Start vergangen ist. Wenn S6 gedrückt wird, dann wird zu HOLD  
  gewechselt und bei S5 zu INIT
- Es werden die Unterfunktion displayTime, checkTimer, updateClk, calcTime, setTimeString benötigt

# Hold
- In Hold wird die gestoppte Zeit zum Zeitpunkt des Wechsels in Hold angezeigt (RUNNING & S6 -> HOLD)
- Timer läuft weiter
- Wenn S7 in Hold gedrückt wird, dann wird wieder zu Running gewechselt
- Durch das Drücken von S5 wird zu dem Zustand INIT gewechselt
- In diesem Zustand sollen die LEDs D8 und D9 leuchten