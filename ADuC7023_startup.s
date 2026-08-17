;;/***********************************************************************/
;;/*  This file is part of the CA ARM C Compiler package                 */
;;/*  Copyright KEIL ELEKTRONIK GmbH 2002-2007                           */
;;/***********************************************************************/
;;/*                                                                     */
;;/*  STARTUP.S:  Startup file for ADuC7023                */
;;/*                                                                     */
;;/***********************************************************************/
;#include <ADuC7023.inc>

;;/* 
;;/*** <<< Use Configuration Wizard in Context Menu >>> *** 



;;/*
; *  The STARTUP.S code is executed after CPU Reset. This file may be 
 ;*  translated with the following SET symbols. In uVision these SET 
 ;*  symbols are entered under Options - ASM - Set.
 ;*
 ;*  RAM_INTVEC: when set the startup code copies exception vectors 
 ;*  from on-chip Flash to on-chip RAM and remaps RAM to address 0.



; Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

        Mode_USR  EQU      0x10
        Mode_FIQ  EQU      0x11
        Mode_IRQ  EQU      0x12
        Mode_SVC  EQU      0x13
        Mode_ABT  EQU      0x17
        Mode_UND  EQU      0x1B
        Mode_SYS  EQU      0x1F

        I_Bit     EQU      0x80    ;;/* when I bit is set, IRQ is disabled */
        F_Bit     EQU      0x40    ;;/* when F bit is set, FIQ is disabled */


;/*
// <h> Stack Configuration (Stack Sizes in Bytes)
//   <o0> Undefined Mode      <0x0-0xFFFFFFFF:4>
//   <o1> Supervisor Mode     <0x0-0xFFFFFFFF:4>
//   <o2> Abort Mode          <0x0-0xFFFFFFFF:4>
//   <o3> Fast Interrupt Mode <0x0-0xFFFFFFFF:4>
//   <o4> Interrupt Mode      <0x0-0xFFFFFFFF:4>
//   <o5> User/System Mode    <0x0-0xFFFFFFFF:4>
// </h>
*/
        UND_Stack_Size  EQU     0x00000004
        SVC_Stack_Size  EQU     0x00000004
        ABT_Stack_Size  EQU     0x00000004
        FIQ_Stack_Size  EQU     0x00000004
        IRQ_Stack_Size  EQU     0x00000080
        USR_Stack_Size  EQU     0x00000400

AREA   STACK, DATA, READWRITE, ALIGN=2
        DS   (USR_Stack_Size+3)&~3  ; Stack for User/System Mode 
        DS   (SVC_Stack_Size+3)&~3  ; Stack for Supervisor Mode
        DS   (IRQ_Stack_Size+3)&~3  ; Stack for Interrupt Mode
        DS   (FIQ_Stack_Size+3)&~3  ; Stack for Fast Interrupt Mode 
        DS   (ABT_Stack_Size+3)&~3  ; Stack for Abort Mode
        DS   (UND_Stack_Size+3)&~3  ; Stack for Undefined Mode
Top_Stack:

$IF (RAM_INTVEC)
// Exception Vector Area in RAM
AREA   VECTORS, DATA, AT 0x00010000
                DS      64
$ENDIF        
        

// Startup Code must be linked at address which it expects to run.
                          
AREA   STARTUPCODE, CODE, AT 0x00080000
       PUBLIC  __startup

       EXTERN  CODE32 (?C?INIT)

__startup       PROC    CODE32

// Pre-defined interrupt handlers that may be directly 
// overwritten by C interrupt functions
EXTERN CODE32 (Undef_Handler?A)
EXTERN CODE32 (SWI_Handler?A)
EXTERN CODE32 (PAbt_Handler?A)
EXTERN CODE32 (DAbt_Handler?A)
EXTERN CODE32 (IRQ_Handler?A)
EXTERN CODE32 (FIQ_Handler?A)

// Exception Vectors
// Mapped to Address 0.
// Absolute addressing mode must be used.

Vectors:        LDR     PC,Reset_Addr         
                LDR     PC,Undef_Addr
                LDR     PC,SWI_Addr
                LDR     PC,PAbt_Addr
                LDR     PC,DAbt_Addr
                NOP ;/* Reserved Vector */
                LDR     PC,IRQ_Addr
                LDR     PC,FIQ_Addr

Reset_Addr:     DD      Reset_Handler
Undef_Addr:     DD      Undef_Handler?A
SWI_Addr:       DD      SWI_Handler?A
PAbt_Addr:      DD      PAbt_Handler?A
DAbt_Addr:      DD      DAbt_Handler?A
IRQ_Addr:       DD      IRQ_Handler?A
FIQ_Addr:       DD      FIQ_Handler?A


// Reset Handler

Reset_Handler:  


// Copy Exception Vectors to Internal RAM and Remap Memory
//  (when Interrupt Vectors are in RAM)

$IF (RAM_INTVEC)
                ADR     R8, Vectors         ; Source
                LDR     R9, =0x00010000    ; Destination
                LDMIA   R8!, {R0-R7}        ; Load Vectors 
                STMIA   R9!, {R0-R7}        ; Store Vectors 
                LDMIA   R8!, {R0-R7}        ; Load Handler Addresses 
                STMIA   R9!, {R0-R7}        ; Store Handler Addresses 
                LDR     R0, =MMR_BASE
                MOV     R1, #1     
                STR     R1, [R0,#PREMAP_OFFSET]          
                STR     R1, [R0,#REMAP_OFFSET]          
$ENDIF


// Setup Stack for each mode
                LDR     R0, =Top_Stack

// Enter Undefined Instruction Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_UND|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

// Enter Abort Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_ABT|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

// Enter FIQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_FIQ|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

// Enter IRQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

// Enter Supervisor Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

// Enter User Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_USR
                MOV     SP, R0

// Enter the C code
                LDR     R0,=?C?INIT
                TST     R0,#1       ; Bit-0 set: main is Thumb
                LDREQ   LR,=exit?A  ; ARM Mode
                LDRNE   LR,=exit?T  ; Thumb Mode
                BX      R0
                ENDP

PUBLIC exit?A
exit?A          PROC    CODE32
                B       exit?A
                ENDP

PUBLIC exit?T
exit?T          PROC    CODE16
                B       exit?T
                ENDP

                END





