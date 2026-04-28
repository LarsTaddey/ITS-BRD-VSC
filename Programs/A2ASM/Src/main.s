;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Martin Becke    
;* Version            : V1.0
;* Date               : 01.06.2021
;* Description        : This is a simple main to demonstrate data transfer
;                     : and manipulation.
;                     : 
;
;*******************************************************************************
    EXTERN initITSboard ; Helper to organize the setup of the board

    EXPORT main         ; we need this for the linker - In this context it set the entry point,too

ConstByteA  EQU 0xaffe
    
;* We need some data to work on
    AREA DATA, DATA, align=2    
VariableA   DCW 0xbeef
VariableB   DCW 0x1234
VariableC   DCW 0xaffe

;* We need minimal memory setup of InRootSection placed in Code Section 
    AREA  |.text|, CODE, READONLY, ALIGN = 3    
    ALIGN   
main
    BL initITSboard             ; needed by the board to setup
;* swap memory - Is there another, at least optimized approach?
    ldr     R0,=VariableA   ; Anw01
    ldrb    R2,[R0]         ; Anw02
    ldrb    R3,[R0,#1]      ; Anw03
    lsl     R2, #8          ; Anw04
    orr     R2, R3          ; Anw05
    strh    R2,[R0]         ; Anw06 
    
;* const in var
    mov     R5,#ConstByteA  ; Anw07
    strh    R5,[R0]         ; Anw08
    
;* swap memory - 0xaffe to AF FE in memory. lower Byte to higher adress and higher Byte to lower adress.
    ldr     R0, =VariableC  ; load adress of VariableC to R2 = 0x20000010
    ldrb    R2, [R0]        ; load lowest Byte from memory at adress R0 to R2. R2 = 0xfe
    ldrb    R3, [R0, #1]    ; load the next byte with offset of 1 byte from meemory at adress R0 to R3. R3 = 0xaf
    lsl     R2, #8          ; logical shif left of 8 bit, so 0xfe becomes the higher-value byte. R2 = 0xfe00
    orr     R2, R3          ; bitwise or operation to swap the bits and write result to R2. R2 = 0xfeaf
    strh    R2, [R0]        ; store halfword of R2 to memory at adress R0. Memory should look like af fe

;* Change value from x1234 to x4321
    ldr     R1,=VariableB   ; Anw09
    ldrh    R6,[R1]         ; Anw0A load halfword from memory at adress R1 - first 2 bytes
    rev16   R6, R6          ; Anw0B reverse order of the first 2 bytes of R6
    strh    R6, [R1]        ; Anw0C store halfword in memory at adress R1
    b .                     ; Anw0E
    
    ALIGN
    END