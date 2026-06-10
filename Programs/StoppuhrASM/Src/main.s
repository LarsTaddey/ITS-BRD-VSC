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
Zeit					DCB 	"00:00:00", 0
ResetZeit				DCB 	"00:00:00", 0

Zeitstempel 			DCD 	0
LetzterStempel			DCD		0
Zeitspanne 				DCD		0
StoppuhrZeit			DCD 	0

Z_INIT					EQU 	0x0
Z_RUNNING				EQU 	0x1
Z_HOLD					EQU 	0x2

Zustand 				DCB 	0

B_S5					EQU 	0xDF
B_S6 					EQU 	0xBF
B_S7					EQU 	0x7F

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
				ldr 	R1,=TIM2_PSC   					; Set pre scaler such that 1 timer tick represents 10 us
				mov 	R0,#(90*10-1) 
				strh	R0,[R1]
				ldr 	R1,=TIM2_ERG   					; Restart timer	
				mov		R0,#0x01
				strh	R0,[R1]							; Set UG Bit
				MOV 	R0, #24
				bl  	lcdSetFont		

				mov 	R5, Z_INIT
Super_Loop												; Schleife für die Stoppuhr
				bl		UpdateClk						
				bl 		ReadButtons				
				bl 		UpdateZustand
				bl 		RunZustand

				b 		Super_Loop

;-----------------------------------------
; Aktualisiert den Zustand der Stoppuhr im Speicher
;-----------------------------------------
UpdateZustand	PROC									; Aktualisiert den Zustand der Uhr (0 = INIT ; 1 = RUNNING ; 2 = HOLD)
				push 	{R0,LR}							; Benutzt Register R5 um Zustand zu setzen
				ldr 	R0, =Zustand
				strb 	R5, [R0]
				pop		{R0,LR}
				bx 		lr
				ENDP

;-----------------------------------------
; Überprüfe den aktuellen Zustand und führe diesen aus
;-----------------------------------------
RunZustand		PROC									
				push 	{R0,R1,R2,LR}

				ldr 	R0, =Zustand
				ldrb 	R1, [R0]
if_init													
				cmp		R1, Z_INIT						; Überprüfe, ob der Zustand INIT ist
				bne 	endif_init						; Wenn nicht, dann prüfe den nächsten Zustand
then_init 		
				bl 		INIT							; Wenn ja, dann führe INIT aus
endif_init

if_running
				cmp 	R1, Z_RUNNING					; Überprüfe, ob der Zustand RUNNING ist
				bne 	endif_running					; Wenn nicht, dann prüfe den nächsten Zustand
then_running
				bl   	RUNNING							; Wenn ja, dann führe RUNNING aus
endif_running

if_hold 
				cmp 	R1, Z_HOLD						; Überprüfe, ob der Zustand HOLD ist
				bne 	endif_hold						; Wenn nicht, dann gehe raus aus dem Unterprogramm und fange von vorne an
then_hold
				bl 		HOLD							; Wenn ja, dann führe HOLD aus
endif_hold
				pop 	{R0,R1,R2,LR}
				bx 		lr
				ENDP

;******************************************
; Zustand: INIT 
;	Setzt die Uhr zurück auf "00:00:00" und schaltet die LEDs D8 und D9 aus
;	Wechselt beim drücken von Taste S7 zu RUNNING
;******************************************
INIT 			PROC
				push 	{R0,R1,LR}
				bl 		ClearLEDs						; Schalte alle LEDs aus
				bl 		ResetClk						; Setze die Anzeige auf dem Display zurück
				mov 	R0, #10							; X-Koordinate für das Display
				mov 	R1, #6							; Y-Koordinate für das Display
				bl 		lcdGotoXY						; Setze den Cursor im Display auf die übergebenen Werte
				ldr 	R0, =Zeit						
				bl 		lcdPrintS						; Zeige den übergebenen String in R0 auf dem Bildschirm

if_init_s7
				cmp 	R4, B_S7						; Überprüfe, ob die Taste S7 gedrückt wurde
				bne 	endif_init_s7					; Wenn nicht, dann gehe raus aus INIT
then_init_s7 	
				mov 	R5, Z_RUNNING					; Wenn ja, dann setze den Zustand auf RUNNING (= 1)
				bl 		UpdateZustand					
endif_init_s7
				pop 	{R0,R1,LR}
				bx		lr
				ENDP

;******************************************
; Zustand: RUNNING
;	Zeigt die, seit dem Start vergangene, Zeit auf dem Display im Format "mm:ss:nn" an
;	Wechselt beim drücken von Taste S5 zu INIT und bei Taste S6 zu HOLD
;******************************************
RUNNING			PROC
				push 	{R0,R1,LR}
if_runing_s5
				cmp 	R4, B_S5						; Überprüfe, ob die Taste S5 gedrückt wurde
				bne 	endif_running_s5				; Wenn nicht, dann gehe weiter
then_running_s5 
				mov 	R5, Z_INIT						; Wenn ja, dann setze den Zustand auf INIT (= 0) 
				bl 		UpdateZustand					
	
endif_running_s5

if_running_s6 	
				cmp 	R4, B_S6						; Überprüfe, ob die Taste S6 gedrückt wurde
				bne 	endif_running_s6				; Wenn nicht, dann mach weiter mit RUNNING
then_running_s6
				mov 	R5, Z_HOLD						; Wenn ja, dann setze den Zustand auf HOLD (= 2)
				bl 		UpdateZustand					; Aktualisiere den Zustand
endif_running_s6	
				mov 	R1, 0x1							; Um die erste LED D8 anzuschalten
				bl 		SetLEDs							; Setze die LEDs mit dem übergebenen Parameter R1
				pop 	{R0,R1,LR}
				bx 		lr
				ENDP

;******************************************
; Zustand: HOLD
;	Dieser Zustand zeigt die gestoppte Zeit zum Zeitpunkt der Betätigung von S6 an
;	Die Uhr läuft im Hintergrund aber weiter, sodass durch Drücken von S7 wieder die aktuelle Zeit angezeigt wieder
;   Wechselt beim Drücken von S5 zu INIT und bei S7 in RUNNING
;******************************************
HOLD 			PROC
				push 	{R0,R1,LR}
if_hold_s5
				cmp 	R4, B_S5						; Überprüfe, ob die Taste S5 gedrückt wurde
				bne 	endif_hold_s5					; Wenn nicht, dann mach weiter
then_hold_s5
				mov 	R5, Z_INIT						; Wenn ja, dann setze den Zustand auf INIT
				bl 		UpdateZustand					
endif_hold_s5

if_hold_s7
				cmp 	R4, B_S7						; Überprüfe, ob die Taste S7 gedrückt wurde
				bne 	endif_hold_s7					; Wenn nicht, dann gehe raus aus HOLD
then_hold_s7
				mov 	R5, Z_RUNNING					; Wenn ja, dann setze den Zustand auf RUNNING
				bl 		UpdateZustand
endif_hold_s7
				mov 	R1, 0x3							; Schalte die LEDs D8 und D9 an
				bl 		SetLEDs
				pop 	{R0,R1,LR}
				bx 		lr

;---------------------------------------
; Setzt die Anzeige auf "00:00:00" zurück
;---------------------------------------
ResetClk 		PROC
				push 	{R0,R1,R2,LR}
				ldr 	R0, =Zeit
				ldr 	R1, =ResetZeit
while_reset
				ldrb 	R2, [R1], #1
				cmp 	R2, #0
				beq 	endwhile_reset
do_reset
				strb 	R2, [R0], #1
				b 		while_reset
endwhile_reset			
				pop  	{R0,R1,R2,LR}
				bx 		lr
				ENDP

;---------------------------------------
; Liest die Tasten am Board aus
;---------------------------------------
ReadButtons 	PROC
				push	{R0,LR}					
				ldr		R0,=GPIO_F_PIN
				ldrh	R4,[R0]
				and		R4,#0xFF   												; set bit 31 to 8 of R0 to 0 ; bit 7 to 0 do not change
				pop 	{R0,LR}
				bx		lr
				ENDP

;---------------------------------------
; Setzt die LEDs anhand des übergebenen Parameters
; Param: R1
;---------------------------------------
SetLEDs			PROC
				push 	{R0,R1,LR}				
				bl 		ClearLEDs
				ldr 	R0, =GPIO_D_SET
				strb 	R1, [R0]
				pop 	{R0,R1,LR}				
				bx		lr
				ENDP

;---------------------------------------
; Schaltet alle LEDs aus auf PIN_D
;---------------------------------------
ClearLEDs		PROC
				push 	{R0,R1,LR}
				ldr 	R0, =GPIO_D_CLR
				mov 	R1, 0xFF
				strb 	R1, [R0]
				pop 	{R0,R1,LR}
				bx 		lr
				ENDP

;---------------------------------------
; Aktualisiert die Zeitanzeige auf dem Display
;---------------------------------------
DisplayTime 	PROC
				push 	{LR}
				pop 	{LR}
				bx 		lr
				ENDP

;---------------------------------------
; Lies den Zeitgeber aus und aktualisiert die Variable,
; die die Zeitspanne der Stoppuhr speichert
;---------------------------------------
CheckTimer 		PROC
				push 	{LR}
				ldr 	R0, =TIMER
				ldr		R1, [R0]
				pop 	{LR}
				bx 		lr
				ENDP

;---------------------------------------
; Speichert aktuellen Zeitstempel und berechnet Zeitspanne
;---------------------------------------
UpdateClk 		PROC
				push 	{LR}
				pop		{LR}
				bx		lr
				ENDP

			ENDP
			ALIGN
			END
