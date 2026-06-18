;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Franz Korf	
;* Version            : V1.0
;* Date               : 11.05.2022
;* Description        : Rahmen zur Loesung von GTP Woche 7-9 (Stoppuhr).
;
;*******************************************************************************

; Define address of selected GPIO and Timer registers
PERIPH_BASE     	equ	0x40000000                 				;Peripheral base address
AHB1PERIPH_BASE 	equ	(PERIPH_BASE + 0x00020000)
APB1PERIPH_BASE     equ PERIPH_BASE

GPIOD_BASE			equ	(AHB1PERIPH_BASE + 0x0C00)
GPIOF_BASE			equ	(AHB1PERIPH_BASE + 0x1400)
TIM2_BASE           equ (APB1PERIPH_BASE + 0x0000)
	
GPIO_F_PIN        	equ	(GPIOF_BASE + 0x10)

GPIO_D_PIN			equ	(GPIOD_BASE + 0x10)
GPIO_D_SET			equ (GPIOD_BASE + 0x18)
GPIO_D_CLR			equ	(GPIOD_BASE + 0x1A)
	
TIMER				equ (TIM2_BASE + 0x24)   					; CNT : current time stamp (32 bit),  resolution
TIM2_PSC			equ (TIM2_BASE + 0x28)   					; Prescaler  resolution
TIM2_ERG			equ (TIM2_BASE + 0x14)   					; 16 Bit register, Bit 0 : 1 Restart Timer


    EXTERN initITSboard
    EXTERN GUI_init
	EXTERN TP_Init
	EXTERN initTimer
	EXTERN lcdSetFont
	EXTERN lcdGotoXY      		; TFT goto x y function
	EXTERN lcdPrintS			; TFT output function	
    EXTERN lcdPrintC            ; TFT output one character		
	EXTERN Delay				; Delay (ms) function


;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	AREA MyData, DATA, align = 2

DEFAULT_BRIGHTNESS		DCW     800
MY_TEXT					DCB		"Hold down different buttons from S0 to S7 and watch D8 to D15.", 0

ZeitString				DCB 	"00:00:00", 0
ZeitAltString			DCB 	"00:00:00", 0
ResetZeitString			DCB 	"00:00:00", 0

IsInitialized			DCB 	0

LetzterStempel			DCD		0
DeltaZeit 				DCD		0
StoppuhrZeit			DCD 	0

Z_INIT					EQU 	0x0
Z_RUNNING				EQU 	0x1
Z_HOLD					EQU 	0x2

Zustand 				DCB 	0

B_S5					EQU 	5
B_S6 					EQU 	6
B_S7					EQU 	7

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************
	AREA |.text|, CODE, READONLY, ALIGN = 3


;--------------------------------------------
; main subroutine
;--------------------------------------------
	EXPORT main [CODE]
	
main		PROC

				; Initialisierung der HW
				bl		initITSboard
				ldr   	r1, =DEFAULT_BRIGHTNESS
				ldrh 	r0, [r1]
				bl   	GUI_init
				bl  	initTimer
				ldr 	R1,=TIM2_PSC   						; Set pre scaler such that 1 timer tick represents 10 us
				mov 	R0,#(90*10-1) 
				strh	R0,[R1]
				ldr 	R1,=TIM2_ERG   						; Restart timer	
				mov		R0,#0x01
				strh	R0,[R1]								; Set UG Bit
				MOV 	R0, #24
				bl  	lcdSetFont		

				bl 		UpdateClk
Super_Loop													; Schleife für die Stoppuhr				
				bl		UpdateClk							; Aktualisiere die Zeitstempel

				bl 		ReadButtons							
								
				ldr 	R1, =Zustand
				bl 		RunZustand

				b 		Super_Loop

;-----------------------------------------
; Aktualisiert den Zustand der Stoppuhr im Speicher
; Parameter: Register R0 mit Zahl von 0-2
;-----------------------------------------
UpdateZustand	PROC									
				push 	{R4,LR}							
				ldr 	R1, =Zustand
				strb 	R0, [R1]
				pop		{R4,LR}
				bx 		lr
				ENDP

;-----------------------------------------
; Überprüfe den aktuellen Zustand und führe diesen aus
; Param: R0 Zustand 0,1 oder 2
;-----------------------------------------
RunZustand		PROC					
				push 	{R4-R6,LR}

				mov 	R4, R1
				mov 	R5, R0

if_init								
				ldrb 	R6, [R4]					
				cmp		R6, #Z_INIT							; Überprüfe, ob der Zustand INIT ist
				bne 	endif_init							; Wenn nicht, dann prüfe den nächsten Zustand
then_init 						
				mov 	R0, R5
				bl 		INIT								; Wenn ja, dann führe INIT aus
endif_init

if_running			
				ldrb 	R6, [R4]			
				cmp 	R6, #Z_RUNNING						; Überprüfe, ob der Zustand RUNNING ist
				bne 	endif_running						; Wenn nicht, dann prüfe den nächsten Zustand
then_running		
				mov 	R0, R5		
				bl   	RUNNING								; Wenn ja, dann führe RUNNING aus
endif_running

if_hold 			
				ldrb 	R6, [R4]			
				cmp 	R6, #Z_HOLD							; Überprüfe, ob der Zustand HOLD ist
				bne 	endif_hold							; Wenn nicht, dann gehe raus aus dem Unterprogramm und fange von vorne an
then_hold			
				mov 	R0, R5	
				bl 		HOLD								; Wenn ja, dann führe HOLD aus
endif_hold
				pop 	{R4-R6,LR}
				bx 		lr
				ENDP

;******************************************
; Zustand: INIT 
;	Setzt die Uhr zurück auf "00:00:00" und schaltet die LEDs D8 und D9 aus
;	Wechselt beim drücken von Taste S7 zu RUNNING
; Param: R0 - Hexwert der gedrückten Knöpfe
;******************************************
INIT 			PROC
				push 	{R4-R6,LR}

				ldr 	R4, =IsInitialized
				mov 	R5, R0				
if_initialized												; Führe die Initialisierung nur aus, wenn das nach Start oder Wechsel noch nicht geschehen ist
				ldrb 	R2, [R4]
				cmp 	R2, #0
				bne 	endif_initialized				
then_initialized
				bl 		ClearLEDs							; Schalte alle LEDs aus

				bl 		ResetClk							; Setze die Anzeige auf dem Display zurück
				mov 	R0, #10								; X-Koordinate für das Display
				mov 	R1, #6								; Y-Koordinate für das Display
				bl 		lcdGotoXY							; Setze den Cursor im Display auf die übergebenen Werte

				ldr 	R0, =ZeitString						
				bl 		lcdPrintS							; Zeige den übergebenen String in R0 auf dem Bildschirm

				mov 	R0, #1								
				strb 	R0, [R4]							; Setze IsInitialized auf 1
endif_initialized

if_init_s7
				mov 	R0, #B_S7
				mov 	R1, R5
				bl 		IsButtonPressed

				cmp 	R0, #1								; Überprüfe, ob die Taste S7 gedrückt wurde	
				bne 	endif_init_s7										
then_init_s7 					
				mov 	R1, #0
				strb 	R1, [R4]							; Setze IsInitialized auf 0
				
				ldr 	R0, =TIM2_ERG						
				mov 	R1, #1
				strb 	R1, [R0]							; Setze den Timer zurück

				mov 	R0, #Z_RUNNING						; Wenn ja, dann setze den Zustand auf RUNNING (= 1)
				bl 		UpdateZustand					
endif_init_s7
				pop 	{R4-R6,LR}
				bx		lr
				ENDP

;******************************************
; Zustand: RUNNING
;	Zeigt die, seit dem Start vergangene, Zeit auf dem Display im Format "mm:ss:nn" an
;	Wechselt beim drücken von Taste S5 zu INIT und bei Taste S6 zu HOLD
; Param: R0 - Hexwert der gedrückten Knöpfe
;******************************************
RUNNING			PROC
				push 	{R4-R6,LR}

				mov 	R4, R0
if_running_s6 	
				mov 	R0, #B_S6
				mov 	R1, R4
				bl 		IsButtonPressed

				cmp 	R0, #1								; Überprüfe, ob die Taste S6 gedrückt wurde
				bne 	endif_running_s6					; Wenn nicht, dann mach weiter mit RUNNING
then_running_s6
				mov 	R0, #Z_HOLD							; Wenn ja, dann setze den Zustand auf HOLD (= 2)
				bl 		UpdateZustand						; Aktualisiere den Zustand
endif_running_s6	

if_runing_s5
				mov 	R0, #B_S5
				mov 	R1, R4
				bl 		IsButtonPressed

				cmp 	R0, #1								; Überprüfe, ob die Taste S5 gedrückt wurde
				bne 	endif_running_s5					; Wenn nicht, dann gehe weiter
then_running_s5 
				mov 	R0, #Z_INIT							; Wenn ja, dann setze den Zustand auf INIT (= 0) 
				bl 		UpdateZustand					
	
endif_running_s5

				mov 	R0, #0x1							; Um die erste LED D8 anzuschalten
				bl 		SetLEDs								; Setze die LEDs mit dem übergebenen Parameter R1	
				bl 		DisplayTime

				pop 	{R4-R6,LR}
				bx 		lr
				ENDP

;******************************************
; Zustand: HOLD
;	Dieser Zustand zeigt die gestoppte Zeit zum Zeitpunkt der Betätigung von S6 an
;	Die Uhr läuft im Hintergrund aber weiter, sodass durch Drücken von S7 wieder die aktuelle Zeit angezeigt wieder
;   Wechselt beim Drücken von S5 zu INIT und bei S7 in RUNNING
; Param: R0 - Hexwert der gedrückten Knöpfe
;******************************************
HOLD 			PROC
				push 	{R4,LR}
				
				mov 	R4, R0

if_hold_s7
				mov 	R0, #B_S7
				mov 	R1, R4
				bl 		IsButtonPressed

				cmp 	R0, #1								; Überprüfe, ob die Taste S7 gedrückt wurde
				bne 	endif_hold_s7						; Wenn nicht, dann gehe raus aus HOLD
then_hold_s7
				mov 	R0, #Z_RUNNING						; Wenn ja, dann setze den Zustand auf RUNNING
				bl 		UpdateZustand
endif_hold_s7

if_hold_s5					
				mov 	R0, #B_S5
				mov 	R1, R4
				bl 		IsButtonPressed

				cmp 	R0, #1								; Überprüfe, ob die Taste S5 gedrückt wurde
				bne 	endif_hold_s5						; Wenn nicht, dann mach weiter
then_hold_s5
				mov 	R0, #Z_INIT							; Wenn ja, dann setze den Zustand auf INIT
				bl 		UpdateZustand					
endif_hold_s5
				mov 	R0, #0x3							; Schalte die LEDs D8 und D9 an
				bl 		SetLEDs
				
				pop 	{R4,LR}
				bx 		lr

;---------------------------------------
; Setzt ZeitString, und somit die Anzeige, auf "00:00:00" zurück
;---------------------------------------
ResetClk 		PROC
				push 	{R4,LR}

				ldr 	R0, =ZeitString
				ldr 	R1, =ResetZeitString
while_reset
				ldrb 	R2, [R1], #1
				cmp 	R2, #0
				beq 	endwhile_reset
do_reset
				strb 	R2, [R0], #1
				b 		while_reset
endwhile_reset			

				pop  	{R4,LR}
				bx 		lr
				ENDP

;---------------------------------------
; Liest die Tasten am Board aus
;---------------------------------------
ReadButtons 	PROC
				push	{R4,LR}	

				ldr		R0,=GPIO_F_PIN
				ldrh	R0,[R0]

				and		R0,#0xFF   							; set bit 31 to 8 of R0 to 0 ; bit 7 to 0 do not change

				pop 	{R4,LR}
				bx		lr
				ENDP

;---------------------------------------
; Prüft, anhand einer Bitmaske, ob ein Knopf gedrückt wurde
; Parameter: R0 als Bitmaske für die Bitstelle des Knopfes
; Parameter: R1 die Bits der gedrückten Knöpfe
;---------------------------------------
IsButtonPressed	PROC
				push 	{R4,LR}

				mov 	R2, #1
				lsl 	R3, R2, R0

				ands	R1, R1, R3		
				lsrs 	R1, R1, R0							; Ergebnis ist entweder 0 oder 1	

				eors 	R0, R1, R2							; Ergebnis invertieren mit XOR

				pop		{R4,LR}
				bx 		lr 
				ENDP

;---------------------------------------
; Setzt die LEDs anhand des übergebenen Parameters
; Parameter: R0 mit 1 an der Stelle der LED(s). Von Bit 0-7
;---------------------------------------
SetLEDs			PROC
				push 	{R4,LR}		

				mov 	R4, R0
				bl 		ClearLEDs
				ldr 	R1, =GPIO_D_SET
				str 	R4, [R1]

				pop 	{R4,LR}				
				bx		lr
				ENDP

;---------------------------------------
; Schaltet alle LEDs aus auf PIN_D
;---------------------------------------
ClearLEDs		PROC
				push 	{R4,LR}

				ldr 	R0, =GPIO_D_CLR
				mov 	R1, #0xFF
				strb 	R1, [R0]

				pop 	{R4,LR}
				bx 		lr
				ENDP

;---------------------------------------
; Aktualisiert die Zeitanzeige auf dem Display
; Berechnet alle Werte für die Stoppuhr und schreibt diese in den ZeitString
; Es werden nur die Ziffern gesetzt, die unterschiedlich sind
; Der neue ZeitString wir dann in ZeitAltString geschrieben
;---------------------------------------
DisplayTime 	PROC
				push 	{R4-R10,LR}

				ldr 	R0, =StoppuhrZeit
				ldr 	R0, [R0]							
				ldr 	R1, =60000000						; Berechne die Zehnerminuten
				mov 	R2, #0
				bl 		UpdateDigit
											
				ldr 	R1, =6000000						; Berechne die Einerminuten
				mov 	R2, #1
				bl 		UpdateDigit
											
				ldr 	R1, =1000000						; Berechne die Zehnersekunden
				mov 	R2, #3
				bl 		UpdateDigit
						
				ldr 	R1, =100000							; Berechne die Einersekunden
				mov 	R2, #4
				bl 		UpdateDigit
		
				ldr 	R1, =10000							; Berechne die Zehnerstellen
				mov 	R2, #6
				bl 		UpdateDigit
								
				ldr 	R1, =1000							; Berechne die Einerstellen
				mov 	R2, #7
				bl 		UpdateDigit

for_display_time
				mov 	R4, #10								; Die start X-Koordinate für den String auf dem Display
				mov 	R5, #0								; Zähler R5 auf 0 setzen

				ldr 	R6, =ZeitString
				ldr 	R7, =ZeitAltString	
until_display_time				
				cmp 	R5, #7								; Vergleiche, ob der Zähler am Ende des Strings angekommen ist
				bhi 	enddo_display_time
do_display_time

if_zeit_diff
				ldrb 	R8, [R6, R5]						
				ldrb 	R9, [R7, R5]
				cmp 	R8, R9								; Vergleicht, ob in ZeitString und ZeitAltString an einer Stelle eine unterschiedliche Ziffer steht
				beq 	endif_zeit_diff
then_zeit_diff
				mov 	R0, R4
				mov 	R1, #6
				bl 		lcdGotoXY							; Wenn ja, dann wird die Ziffer auf dem Display ausgegeben mit der passenden X-Koordinate
				mov 	R0, R8
				bl 		lcdPrintC							; Zeige den neuen Charakter auf dem Display an
endif_zeit_diff
				strb 	R8, [R7, R5]
step_display_time
 				add 	R4, #1								; Erhöhe den Wert der X-Koordinate um 1
				add 	R5, #1								
				b 		until_display_time
enddo_display_time
				pop 	{R4-R10,LR}
				bx 		lr
				ENDP

;---------------------------------------
; Speichert aktuellen Zeitstempel und berechnet Zeitspanne
;---------------------------------------
UpdateClk 		PROC
				push 	{R4,LR}								
	
				ldr 	R0, =TIMER								; TIMER als Zeitstempel
				ldr 	R1, [R0]								; Zeit in ticks

				ldr 	R2, =LetzterStempel						
				ldr 	R3, [R2]								
	
				sub		R3, R1, R3								; Zeitstempel - LetzterStempel = vergangene Zeit
				
				ldr 	R0, =StoppuhrZeit		
				ldr 	R4, [R0]
				add 	R3, R4, R3								; Addiere vergangene Zeit zur Stoppuhr
				str 	R3, [R0]			

				ldr 	R0, =LetzterStempel
				str 	R1, [R0]								; Aktualisiere LetzterStempel

				pop		{R4,LR}
				bx		lr
				ENDP

;---------------------------------------
; Berechnet aus den übergeben Parametern die entsprechende Stelle und gibt den Restwert zurück
; Parameter: 	R0 für den aktuellen Restwert der Zeit
;				R1 zur Umrechnung in die richtige Stelle
;				R2 als Offset für den ZeitString
;---------------------------------------
UpdateDigit		PROC
				push	{R4-R6,LR}
				ldr 	R3, =ZeitString
				udiv 	R4, R0, R1								; R4 = Zeit / Teiler

				mul 	R5, R1, R4								; R4 * Teiler
				sub 	R5, R0, R5								; R5 = Restwert

				add 	R4, #'0'								; R4 zum ASCII Zeichen 
				strb 	R4, [R3, R2]

				mov 	R0, R5									; R0 gibt Restwert als Rückgabewert

				pop 	{R4-R6,LR}
				bx 		lr
				ENDP
			ENDP
			ALIGN
			END
