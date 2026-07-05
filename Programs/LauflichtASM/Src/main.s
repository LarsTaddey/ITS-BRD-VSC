;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Franz Korf  
;* Version            : V1.0
;* Date               : 16.05.2022
;* Modified by        : Thomas Lehmann, 2024-07-12
;* Description        : This is the frame for the last assignment.
;                     : Einfaches Lauflicht.
;
;*******************************************************************************
    EXTERN initITSboard
    EXTERN lcdPrintS            ;Display ausgabe
    EXTERN GUI_init
    EXTERN TP_Init
    EXTERN delay
        
; Define address of selected GPIO and Timer registers
PERIPH_BASE         equ 0x40000000                 ;Peripheral base address
AHB1PERIPH_BASE     equ (PERIPH_BASE + 0x00020000)
APB1PERIPH_BASE     equ PERIPH_BASE

GPIOD_BASE          equ (AHB1PERIPH_BASE + 0x0C00)
GPIOE_BASE          equ (AHB1PERIPH_BASE + 0x1000)
GPIOF_BASE          equ (AHB1PERIPH_BASE + 0x1400)
TIM2_BASE           equ (APB1PERIPH_BASE + 0x0000)

GPIO_F_PIN          equ (GPIOF_BASE + 0x10)

GPIO_D_PIN          equ (GPIOD_BASE + 0x10)
GPIO_D_SET          equ (GPIOD_BASE + 0x18)
GPIO_D_CLR          equ (GPIOD_BASE + 0x1A) 
    
GPIO_E_PIN          equ (GPIOE_BASE + 0x10)
GPIO_E_SET          equ (GPIOE_BASE + 0x18)
GPIO_E_CLR          equ (GPIOE_BASE + 0x1A)     



;********************************************
; Data section, aligned on 4-byte boundery
;********************************************   
    AREA MyData, DATA, align = 2
TestPattern DCW     0x8000, 0x7000, 0x5000

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************
    AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; main subroutine
;--------------------------------------------

        
; Unterprogramm Lauftlicht
;
; Einfaches Lauflicht, das ein Bitmuster zyklisch ueber die 
; LEDs D23 bis D8 schiebt. Das LED Muster wird nach rechts 
; geschoben. Die Frequenz betraegt 2 Hz.
;
; IN R0  Die unteren 16 Bits von R0 speichern das Muster, mit
;        dem die LEDs initialisiert werden.
; IN R1  Anzahl Schritte, die das Lauflicht laufen soll.
;--------------------------------------------       
;

DelayTime   EQU     500

Lauflicht   PROC
        push    {R4-R8,LR}

for_lauflicht
        mov     R4, R0
        mov     R5, R1

until_lauflicht
        cmp     R5, #0
        beq     enddo_lauflicht

do_lauflicht
        mov     R0, R4
        bl      SetLEDs
        ldr     R0, =DelayTime
        bl      delay  

        mov     R1, R4                      ; Bitmaske in R1 abspeichern

        lsr    R4, R4, #1                   ; logical right shift -> 0x8000 => 0x4000 => 0x2000 ...
        and    R1, R1, #1                   ; das niedrigwertigste bit in R1 setzen
        lsl     R1, R1, #15                 ; das Bit in R1 um 15 stellen nach links verschieben
        orr     R4, R4, R1                  ; das verschobene Bit mit der momentanen Bitmaske verodern
                                            ; so sichert man das bit, dass rausfallen würde und behält dieses (0x0001 => 0x8000)
      
step_lauflicht
        subs     R5, #1
        b       until_lauflicht            

enddo_lauflicht
        bl      ClearAllLEDs
        pop     {R4-R8,LR}
        bx lr
        ENDP


; Unterprogramm SetLEDs
;
; Eine simple Funktion, welche die entsprechenden LEDs D23 bis D8 setzt.
;
; IN R0  Die unteren 16 Bits von R0 speichern das Muster, mit
;        dem die LEDs initialisiert werden.
;--------------------------------------------       
SetLEDs PROC
        push    {R4-R6,LR}
        
        mov     R4, R0

        bl      ClearAllLEDs
            
        lsr     R5, R4, #8

        ldr     R6, =GPIO_E_SET
        strh    R5, [R6]     
        
        ldr     R6, =GPIO_D_SET
        strh    R4, [R6]

        pop     {R4-R6,LR}
        bx lr
        ENDP

; Unterprogramm ClearAllLEDs
;
; Eine simple Funktion, welche alle LEDs ausschaltet.
;
;--------------------------------------------       
ClearAllLEDs    PROC
        push    {R4,LR}
        mov     R0, #0xFF
        ldr     R1, =GPIO_E_CLR
        str     R0, [R1]
        ldr     R1, =GPIO_D_CLR
        str     R0, [R1]
        pop     {R4,LR}
        bx lr
        ENDP

;--------------------------------------------
; main subroutine
;--------------------------------------------
    EXPORT main [CODE]
        
InterTestDelay  EQU     4000
    
main    PROC
        BL initITSboard
        LDR     R7, =TestPattern
        MOV     R8, #0                  ; Laufindex Testpattern
forever 
        CMP     R8, #3
        MOVGE   R8, #0
        
        ; Test Lauflicht
        LDRH    R0, [R7,R8,LSL #1]
        MOV     R1, #20
        BL      Lauflicht
        
        LDR     R0, =InterTestDelay
        BL      delay

        ADD     R8, #1
        BAL     forever     ; nowhere to retun if main ends     
        ENDP
    
        ALIGN
        END
