;************************************************
;* Beginn der globalen Daten *
;************************************************
                   AREA MyData, DATA, align = 2
Base
VariableA          DCW 0x1234
VariableB          DCW 0x4711

VariableC          DCD  0

MeinHalbwortFeld   DCW 0x22 , 0x3e , -52, 78 , 0x27 , 0x45

MeinWortFeld       DCD 0x12345678 , 0x9dca5986
                   DCD -872415232 , 1308622848
                   DCD 0x27000000
                   DCD 0x45000000

MeinTextFeld       DCB "ABab0123",0

                   EXPORT VariableA
                   EXPORT VariableB
                   EXPORT VariableC
                   EXPORT MeinHalbwortFeld
                   EXPORT MeinWortFeld
                   EXPORT MeinTextFeld

;***********************************************
;* Beginn des Programms *
;************************************************
    AREA |.text|, CODE, READONLY, ALIGN = 3
; ----- S t a r t des Hauptprogramms -----
                EXPORT main
                EXTERN initITSboard
main            PROC
                bl    initITSboard                 ; HW Initialisieren

; Laden von Konstanten in Register
; setze die Konstante 0x12 in r0
                mov   r0,#0x12                      ; Anw-01
; setze die Konstante #-128 in r1
                mov   r1,#-128                      ; Anw-02
; lade 32 Bit in r2
                ldr   r2,=0x12345678                ; Anw-03

; Zugriff auf Variable
; lade die Adresse von VariableA in r0
                ldr   r0,=VariableA                 ; Anw-04
; lade 2 Byte aus dem Speicher an Adresse in r0 in r1
                ldrh  r1,[r0]                       ; Anw-05
; lade 4 Byte aus dem Speicher an Adresse in r0 in r2
                ldr   r2,[r0]                       ; Anw-06
; speicher 4 Byte aus r2 im Speicher an der Adresse r0 + (0x4711 - 0x1234)
                str   r2,[r0,#VariableC-VariableA]  ; Anw-07

; Zugriff auf Felder (Speicherzellen)
; lade die Adresse von MeinHalbwortFeld in r0
                ldr   r0,=MeinHalbwortFeld          ; Anw-08
; lade 2 Byte aus dem Speicher an der Adresse in r0 in r1
                ldrh  r1,[r0]                       ; Anw-09
; lade 2 Byte aus dem Speicher an der Adresse in r0 + 2 in r1
                ldrh  r2,[r0,#2]                    ; Anw-10
; setze die Konstante 0xa in r3
                mov   r3,#10                        ; Anw-11
; lade 2 Byte aus dem Speicher an der Adresse r0 + Wert aus r3 in r4
                ldrh  r4,[r0,r3]                    ; Anw-12
; lade 2 Byte aus dem Speicher an der Adresse r0 + 2 in r5 und setze r0 = r0 + 2 	(r0_ursprung + 2)
                ldrh  r5,[r0,#2]!                   ; Anw-13
; lade 2 Byteas dem Speicher an der Adresse r0 + 2 in r6 und setze r0 = r0 + 2   	(r0_ursprung + 4)
                ldrh  r6,[r0,#2]!                   ; Anw-14
; speicher 2 Byte aus r6 an der Adresse r0 + 2  und setzt r0 + 2 					(r0_ursprung + 6)
                strh  r6,[r0,#2]!                   ; Anw-15

; Addition und Subtraktion von unsigned / signed Integer-Werten
; lade die Adresse von MeinWortFeld in r0
                ldr  r0,=MeinWortFeld               ; Anw-16
; lade 4 Byte aus dem Speicher an Adresse r0 in r1
                ldr  r1,[r0]                        ; Anw-17
; lade 4 Byte aus dem Speicher an Adresse r0 + 4 in r2
                ldr  r2,[r0,#4]                     ; Anw-18
; addiere den Inhalt aus r1 mit dem Inhalt aus r2 und schreibe das Ergebnis in r3
                adds r3,r1,r2                       ; Anw-19

; lade 4 Byte aus dem Speicher an der Adresse r0 + 8 in r4
                ldr  r4,[r0,#8]                     ; Anw-20
; lade 4 Byte aus dem Speicher an der Adresse r0 + 12 in r5
                ldr  r5,[r0,#12]                    ; Anw-21
; subtrahiere r5 von r4 und speicher das Ergebnis in r6
                subs r6,r4,r5                       ; Anw-22

; lade 4 Byte aus dem Speicher an der Adresse r0 + 16 in r7
                ldr  r7,[r0,#16]                    ; Anw-23
; lade 4 Byte aus dem Speicher an der Adresse r0 + 20 in r8
                ldr  r8,[r0,#20]                    ; Anw-24
; subtrahiere r8 von r7 und speicher das Ergebnis in r9
                subs r9,r7,r8                       ; Anw-25

; springe zu dem label forever (Endlosschleife)
forever         b   forever                         ; Anw-26
                ENDP
                END