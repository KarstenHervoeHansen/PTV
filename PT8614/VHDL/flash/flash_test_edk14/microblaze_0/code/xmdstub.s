/*----------------------------------------------------------------------
 *
 * Copyright (c) 2005 Xilinx, Inc.  All rights reserved. 
 * Xilinx, Inc. 
 *
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS 
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION. 
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE. 
 *
 * $Header: /devl/xcs/repo/env/Jobs/MDT/sw/Apps/debug/xmdstub/src/xmdstub.s,v 1.4 2005/10/21 18:55:34 rajn Exp $
 * $Id: xmdstub.s,v 1.4 2005/10/21 18:55:34 rajn Exp $
 *
 *	XMD_stub version 1.0                        
 *      MicroBlaze                                  
 *      Author: Göran Bilski                        
 *      Created:  11 Juli 2001	                   
 *      Modified: 20 Sept 2001                      	
 *	Modified: 08 Nov  2001                     
 *	Modified: 12 Mar  2002 (jece & navanee)    	
 *	Modified: 25 Aug  2004 (navanee)
 *	Modified: Jan 2005 (Raj - Added Half Word/Byte Read & Write)	 
 *
 ----------------------------------------------------------------------*/

	/*--------------------------------------------------*/
	/* MicroBlaze jump table                            */
        /* 0x0  - Reset address                             */	
	/* 0x8  - Exception address                         */
        /* 0x10 - Interrupt address                         */
        /* 0x18 - Break signal address (short branch)       */
	/* 0x1c - Breakpoint address (short branch)         */	
	/* 0x20 - Hardware Exception addr (MB ref guide)    */
        /* 0x28 - Program Exit(crt1.s) (short branch)       */
	/*--------------------------------------------------*/

	.section .vectors.reset, "ax"
	.align 2 
_vector_reset:       		
	brki r16, StartUp_Entry
	bri Program_Exit
	
	.section .vectors.debug_sw_break, "ax"
        .align 2
_vector_debug_sw_break:       		
	bri Brk_Signal_Entry
	bri Debug_Entry

        .section .text
        .align 2		
        .globl _start
	.globl	xmd_stub
	.ent	xmd_stub 
_start:	
xmd_stub:
	.equ uart_rx_data,         DEBUG_PERIPHERAL_BASEADDRESS
	.equ uart_tx_data,         DEBUG_PERIPHERAL_BASEADDRESS +  4
	.equ uart_stat,	           DEBUG_PERIPHERAL_BASEADDRESS +  8
	.equ uart_ctrl,		   DEBUG_PERIPHERAL_BASEADDRESS + 12
	
	.equ uart_tx_buffer_empty, 0b100
	.equ uart_rx_data_valid,   0b001
	
	.equ uart_rst_rx_fifo,      0b010
	.equ uart_rst_tx_fifo,      0b001
	
	.equ Reached_End_Of_Program, 0x00
	.equ Reached_Breakpoint,     0x01
	.equ Received_brk_signal,    0x02
	.equ ProcReset_signal,       0x03
	.equ Escape_Char,            0xFC
	.equ ReSync_Char,            0xFE
	.equ Stub_ID,                0x40

	.equ Reset_TxFifo,	     0x01
	.equ Reset_RxFifo,	     0x02
	.equ Clear_ExtBreak,	     0x04
	.equ Intr_Enabled,	     0x10
	
	.equ version, 4


	/*--------------------------------------------------*/
	/* ReSync                                           */
	/* - Remove any stale data from the RX and TX fifo  */
	/* - Send back the stubID + version number          */
	/*--------------------------------------------------*/
ReSync:
	lwi  r15, r0, uart_stat
	andi r15,r15, Intr_Enabled
	ori  r15, r15, uart_rst_tx_fifo + uart_rst_rx_fifo
	swi  r15, r0, uart_ctrl
	brlid r29,Wait_for_TX_space
	addi r30,r0,Stub_ID + version
	swi  r30,r0,uart_tx_data

	/*--------------------------------------------------*/
	/* Main command entry code                          */
	/* - Wait for the next command                      */
	/*    0 = Download Program                          */
	/*    4 = Continue                                  */
	/*    8 = Read Registers                            */
	/*   12 = Write Register                            */
	/*   16 = Read Memory                               */
	/*   20 = Write Memory                              */
	/*   24 = Resync                                    */
	/*   28 = Return Debug Addr - Deprecated	    */
	/*   32 = Continue				    */	
	/*   36 = SingleStep                                */
	/*   40 = Read Memory Half Word			    */
	/*   44 = Read Memory Byte			    */
	/*   48 = Write Memory Half Word		    */
	/*   52 = Write Memory Byte			    */
	/* - Call the command without any link              */
	/*--------------------------------------------------*/
Wait_for_command:
		brlid r29,Get_Char
	nop
	lwi r29,r3,jump_table
	bra r29

	/*--------------------------------------------------*/
	/* This section will end up in the rodata section    */
	/* Is place here for helping understanding the code */
	/*--------------------------------------------------*/
	.rodata
	.align 2
jump_table:
  	.int Load_Memory
  	.int Execute
  	.int Read_Register 
  	.int Write_Register
	.int Read_Memory
  	.int Write_Memory 
	.int ReSync
	.int Return_Dbg_Addr
	.int Continue
	.int SingleStep
	.int Read_Memory_Half
	.int Read_Memory_Byte
	.int Write_Memory_Half
	.int Write_Memory_Byte
	
	/*--------------------------------------------------*/
	/* Go back to the text section(instruction)         */
	/*--------------------------------------------------*/	
	.text


	/*--------------------------------------------------*/	
	/* Read all registers command                       */
	/* Get the values of all registers                  */
	/*   from the register_dump where the register      */
	/*   values are stored                              */
	/* - Send all values to the terminal (through UART) */
	/*   33 words will be sent: 1-31 and pc and msr     */
	/*   Note that the pc value sent is always incorrect*/
	/*   It has to be in the table for consistency      */
	/* - Jump to the main command entry code            */
	/*--------------------------------------------------*/	
Read_Register:	
	addi r28, r0, register_dump+4
	addi r26, r0, 33
Read_Register_1:		
	brlid r15, Send_32Bit
	lw   r5, r28, r0
	addi r26,r26,-1
	bneid r26, Read_Register_1
	addi r28,r28,4
	bri Wait_for_command
	
	/*--------------------------------------------------*/	
	/* Write to a register value command                */
	/* - Get a byte which is the register asked for     */
	/*   The byte has to be Register_Number*4,          */
	/*   since the value will be used as word address   */
        /*   and the addressing is always in byte addresses */
	/*   ex. Want to see value of register 15           */
	/*       The byte value has to be 4*15 = 60         */
	/* - Get 32 bits (4 bytes) which is the new value   */
	/*   for the register                               */
	/* - Store the new value to the stored value for    */
	/*   that register from the register_dump where the */
	/*   register values are stored                     */
	/* - Jump to the main command entry code            */
	/*--------------------------------------------------*/	
Write_Register:	
	/* Write a register value                    */
	/* Next byte is which register               */
	/* Next four bytes is the new register value */
	brlid r29,Get_Char
	nop
	brlid r15,Get_32Bit
	or r28,r3,r0
	swi r3,r28,register_dump
	bri Wait_for_command


	/*--------------------------------------------------*/	
	/* Read from memory command                         */
	/* - Get 32 bits (4 bytes) which is the memory      */
	/*   address.                                       */
	/* - Read the value from that memory address        */
	/*   for the register                               */
	/* - Send that value to the terminal (through UART) */
	/* - Jump to the main command entry code            */
	/*--------------------------------------------------*/	
Read_Memory:
	/* Next four bytes is the address */
	/* The response will be the value in that address */
	/* send as four bytes */
	brlid r15,Get_32Bit
	nop
	brlid r15,Send_32Bit
	lw r5,r3,r0
	bri Wait_for_command

	/*--------------------------------------------------*/	
	/* Read from memory Half Word command               */
	/* - Get 32 bits (4 bytes) which is the memory      */
	/*   address.                                       */
	/* - Read the value from that memory address        */
	/*   for the register                               */
	/* - Send that value to the terminal (through UART) */
	/* - Jump to the main command entry code            */
	/*--------------------------------------------------*/	
Read_Memory_Half:
	/* Next four bytes is the address */
	/* The response will be the value in that address */
	/* send as two bytes */
	brlid r15,Get_32Bit
	nop
	brlid r15,Send_16Bit
	lhu r5,r3,r0
	bri Wait_for_command

	/*--------------------------------------------------*/	
	/* Read from memory Byte command                    */
	/* - Get 32 bits (4 bytes) which is the memory      */
	/*   address.                                       */
	/* - Read the value from that memory address        */
	/*   for the register                               */
	/* - Send that value to the terminal (through UART) */
	/* - Jump to the main command entry code            */
	/*--------------------------------------------------*/		
Read_Memory_Byte:
	/* Next four bytes is the address */
	/* The response will be the value in that address */
	/* send a byte */
	brlid r15,Get_32Bit
	nop
	brid Send_Status
	lbu r15,r3,r0

	/*--------------------------------------------------*/	
	/* Write to memory command                          */
	/* - Get 32 bits (4 bytes) which is the memory      */
	/*   address.                                       */
	/* - Get 32 bits (4 bytes) which is the new value   */
	/*   for that memory address.                       */
	/* - Write the new value to the memory.             */
	/* - Jump to the main command entry code            */
	/*--------------------------------------------------*/	
Write_Memory:
	/* Next four byte is the address */
	/* Then the next four byte is the new data */
	brlid r15,Get_32Bit
	nop
	brlid r15,Get_32Bit
	or r28,r3,r0
	swi r3,r28,0
	bri Wait_for_command

Write_Memory_Half:
	/* Next four bytes is the address */
	/* Then the next two bytes is the new data */
	brlid r15,Get_32Bit
	nop
	brlid r15,Get_16Bit
	or r28,r3,r0
	shi r3,r28,0
	bri Wait_for_command

Write_Memory_Byte:
	/* Next four byte is the address */
	/* Then the next byte is the new data */
	brlid r15,Get_32Bit
	nop
	brlid r29,Get_Char
	or r28,r3,r0
	sbi r3,r28,0
	bri Wait_for_command	

	/*--------------------------------------------------*/	
	/* Download program command                         */
	/* - Get 32 bits (4 bytes) which is the start       */
	/*   address for the downloading                    */
	/* - Get 32 bits (4 bytes) which is the number of   */
	/*   bytes in the downloading                       */
	/* - Loop                                           */
	/*   - Get a byte                                   */
	/*   - Store it at the memory address               */
	/*   - Increment the memory address with 1          */
	/*   - Decrement the number of bytes by 1           */
	/*   - Continue until all bytes has been received   */
	/* - Jump to the main command entry code            */
	/*--------------------------------------------------*/	
Load_Memory:	
	/* Next 4 bytes is the start address */
	/* The next four bytes is the number of bytes */
	brlid r15,Get_32Bit
	nop
	brlid r15,Get_32Bit
	or r28,r3,r0
	or r25,r3,r0
Load_Memory_1:	
	brlid r29,Get_Char
	addi r25,r25,-1
	sb r3,r28,r0
	bneid r25,Load_Memory_1
	addi r28,r28,1
	bri Wait_for_command				



	/*--------------------------------------------------*/
	/* NOTE : EXECUTE FOR THE SAKE OF COMPATIBILITY WITH MDK 2.2 */
	/* Execute program command                          */
	/* - Get 32 bits (4 bytes) which is the memory      */
	/*   address.                                       */
	/* - Load all register from the register_dump       */
	/*--------------------------------------------------*/	
Execute:
	/* Next four byte is the start address */
	brlid r15,Get_32Bit
	nop
	/* Set the R16 value in the reg dump to the start addr  */
	/* When control falls thru to the Continue command      */
	/* it will start execution from the addr in the new R16 */
	swi r3,r0,register_dump+64
	/* Fall thru to the following Continue command */

	
	/*--------------------------------------------------*/
	/* Continue command                                 */
	/* continue at the instruction after the brk        */
	/*  - Restore all registers                         */
	/*  - jump to the address for the BRK instruction   */
	/*--------------------------------------------------*/
Continue:
	brlid r15, Get_All_Registers
	nop
	rtbd r16, 0
	lwi r15,r0,register_dump+15*4


	/*--------------------------------------------------*/
	/* SingleStep command                               */
	/* step single instruction without enabling         */
	/* the BIP flag                                     */
	/*  - Restore all registers                         */
	/*  - jump to the address for the BRK instruction   */
	/*--------------------------------------------------*/
SingleStep:
	brlid r15, Get_All_Registers
	nop
	rtsd r16, 0
	lwi r15,r0,register_dump+15*4

	/*--------------------------------------------------*/	
	/* When Processor resets                            */
	/*   - save all registers to the register_dump      */
	/*   - send 0x3 ('Processor reset') to host         */
	/*   - Jump to the main command entry code          */
	/*--------------------------------------------------*/	
	
StartUp_Entry:
	swi r15,r0,register_dump+60
	brid Dump_All_Registers
	ori r15,r0,ProcReset_signal
	/* Control goes to SendStatus from DumpAllRegisters */
	
	
	/*--------------------------------------------------*/	
	/* Program End entry                                */
	/* When control returns from the program or         */
	/* on exit()                                        */
	/*   - save all register to the register_dump       */
	/*   - send 0x0 ('Reached_End_Of_Program') to host  */
	/*   - Jump to the main command entry code          */
	/*--------------------------------------------------*/	

Program_Exit:
	swi r15,r0,register_dump+60
	brid Dump_All_Registers
	ori r15,r0,Reached_End_Of_Program
	/* Control goes to SendStatus from DumpAllRegisters */
	
	/*--------------------------------------------------*/
	/* Debug Entry code                                 */
	/* The code that breakpoint instruction will call   */
	/*  - Will save all registers                       */
	/*  - Send 0x01 to the host                         */
	/*  - Jump to the main command entry code           */
	/*--------------------------------------------------*/
Debug_Entry:
  	swi r15,r0,register_dump+60 /* Dump_All_regs doesnt do this */
	brid Dump_All_Registers
	ori r15,r0, Reached_Breakpoint
	/* Control goes to SendStatus from DumpAllRegisters */
	
	/*--------------------------------------------------*/
	/* Brk signal handler                               */
	/*  - Will save all registers                       */
	/*  - Send 0x02 to the host                         */
	/*  - Jump to the main command entry code           */
	/*--------------------------------------------------*/
Brk_Signal_Entry:
  	swi r15,r0,register_dump+60 /* Dump_All_regs doesnt do this */
	lwi r15, r0, uart_stat
	andi r15,r15, Intr_Enabled
	ori r15, r15, Clear_ExtBreak
	swi r15, r0, uart_ctrl
	brid Dump_All_Registers
	ori r15,r0, Received_brk_signal
	/* Control goes to SendStatus from DumpAllRegisters */
	

	/*  Send the status to the host                     */
Send_Status:
	brlid r29,Wait_for_TX_space
	nop
	swi  r15,r0,uart_tx_data
	bri Wait_for_command



	/*--------------------------------------------------*/
	/* NOTE : ADDED FOR THE SAKE OF COMPATIBILITY WITH MDK 2.2 */
	/* Return Debug Address Entry command               */
	/* The code will return the address for the         */
	/* Debug_Entry code section as four bytes           */
	/*--------------------------------------------------*/
Return_Dbg_Addr:
	brlid r15,Send_32Bit
	addi r5,r0,Debug_Entry
	bri Wait_for_command


/*--------------------------------------------------*/
/* Low Level Helper routines                        */	
/*--------------------------------------------------*/	


	/*--------------------------------------------------*/
	/* Get a byte function                              */
	/*  - loop until a character has been received      */
	/*  - read the character                            */
	/* Uses: R3                                         */
	/* Parameters: None                                 */
	/* Returns using R27                                */
	/* Result in: R3                                    */
	/*--------------------------------------------------*/
Get_Byte:	
	/* Get 1 byte and return it in r3 */
	lwi  r3,r0,uart_stat
	andi r3,r3,uart_rx_data_valid     /* rx data present */
	beqi r3, Get_Byte
	lwi  r3,r0,uart_rx_data
	rtsd r27,8
	nop
	
	/*--------------------------------------------------*/
	/* Get a char function                              */
	/*  - Get a byte from the UART                      */
	/*  - Check if it's an escape character             */
	/*  - if it is                                      */
	/* Uses: R3, R26, R27                               */
	/* Parameters: None                                 */
	/* Returns using R29                                */
	/* Result in: R3                                    */
	/*--------------------------------------------------*/
Get_Char:	
	brlid r27,Get_Byte
	nop
	xori r26,r3,Escape_Char
	bnei r26,No_Escape
	brlid r27,Get_Byte
	nop
Get_Char_Return:		
	rtsd r29,8
	nop
No_Escape:	
	xori r26,r3,ReSync_Char
	beqi r26,ReSync
	bri Get_Char_Return

	/*--------------------------------------------------*/
	/* Get a half word function                         */
	/*  - Get 2 bytes using Get_Char function           */
	/*  - Store each byte in a memory location,temp_mem */
	/*    using store byte SBI instruction              */
	/*    stores the received byte in the order         */
	/*    addr+1,addr+0			            */
	/* Uses: R3,R29,R30,R31                             */
	/* Parameters: None                                 */
	/* Returns using R15                                */
	/* Result in: R3                                    */
	/*--------------------------------------------------*/	
Get_16Bit:
	and  r30,r0,r0
	addi r31,r0,1
Get_16Bit_1:
	/* Get the next byte */
	brlid r29,Get_Char
	nop
	sbi r3,r31,temp_mem
	addi r31,r31,-1
	bgei r31,Get_16Bit_1
	rtsd r15,8
	lhui r3,r0,temp_mem	
				
	/*--------------------------------------------------*/
	/* Get a word function                              */
	/*  - Get 4 bytes using Get_Char function           */
	/*  - Store each byte in a memory location,temp_mem */
	/*    using store byte SBI instruction              */
	/*    stores the received byte in the order         */
	/*    addr+3,addr+2,addr+1,addr+0                   */
	/* Uses: R3,R29,R30,R31                             */
	/* Parameters: None                                 */
	/* Returns using R15                                */
	/* Result in: R3                                    */
	/*--------------------------------------------------*/
Get_32Bit:
	and  r30,r0,r0
	addi r31,r0,3
Get_32Bit_1:
	/* Get the next byte */
	brlid r29,Get_Char
	nop
	sbi r3,r31,temp_mem
	addi r31,r31,-1
	bgei r31,Get_32Bit_1
	rtsd r15,8
	lwi r3,r0,temp_mem


	/*--------------------------------------------------*/
	/* Wait for UART TX is become empty function        */
	/*  - loop until UART TXC buffer is empty           */
	/* Uses: R25                                        */
	/* Parameters: None                                 */
	/* Returns using R29                                */
	/* Result in: no result                             */
	/*--------------------------------------------------*/
Wait_for_TX_space:
	lwi  r25,r0,uart_stat
	andi r25,r25,uart_tx_buffer_empty   /* tx buffer empty */
	beqi r25, Wait_for_TX_space
	rtsd r29,8
	nop

	/*--------------------------------------------------*/
	/* Send a half word function                        */
	/*  - Store the half word to send in temp_mem       */
	/*  - loop                                          */
	/*     - Wait for TX to become empty                */
	/*     - Read a byte out of temp_mem                */
	/*       reads the bytes in the order               */
	/*       addr+1,addr+0			            */
	/*     - Send that byte (Write to the UART_TX fifo) */
	/*     - Repeat for all 2 bytes                     */ 
	/* Uses: R3,R25,R29,R30,R31                         */
	/* Parameters: R5 Half Word to send                 */
	/* Returns using R15                                */
	/* Result in: No result                             */
	/*--------------------------------------------------*/
Send_16Bit:	
	addi r31,r0,1
	brid Send_32Bit_1
	shi r5,r0,temp_mem
	
	/*--------------------------------------------------*/
	/* Send a word function                             */
	/*  - Store the word to send in temp_mem            */
	/*  - loop                                          */
	/*     - Wait for TX to become empty                */
	/*     - Read a byte out of temp_mem                */
	/*       reads the bytes in the order               */
	/*       addr+3,addr+2,addr+1,addr+0                */
	/*     - Send that byte (Write to the UART_TX fifo) */
	/*     - Repeat for all 4 bytes                     */ 
	/* Uses: R3,R25,R29,R30,R31                         */
	/* Parameters: R5 Word to send                      */
	/* Returns using R15                                */
	/* Result in: No result                             */
	/*--------------------------------------------------*/
Send_32Bit:
	addi r31,r0,3
	swi r5,r0,temp_mem
	
Send_32Bit_1:	
	/* Wait for free space in the transmit buffer */
	brlid r29, Wait_for_TX_space

	/* Get the right byte from the word to transmit */
	lbui r30,r31,temp_mem
	swi  r30,r0,uart_tx_data 

	addi r31,r31,-1
	bgei r31, Send_32Bit_1

	rtsd r15,8
	nop

	/*--------------------------------------------------*/
	/* Save register to register_dump function          */
	/*  - Store all registers to the register dump      */
	/*    except R0  : makes no sense always zero       */
	/*           R15 : needed for the return address    */
	/*  - R15 has to saved before calling this function */
	/*    to the address register_dump+15*4             */
	/* Uses:                                            */
	/* Parameters:                                      */
	/* Returns using R15                                */
	/* Result in: No result                             */
	/*--------------------------------------------------*/
Dump_All_Registers:
/*	swi r0,r0,register_dump + 0 */
	swi r1,r0,register_dump + 4
	swi r2,r0,register_dump + 8
	swi r3,r0,register_dump+12
	swi r4,r0,register_dump+16
	swi r5,r0,register_dump+20
	swi r6,r0,register_dump+24
	swi r7,r0,register_dump+28
	swi r8,r0,register_dump+32
	swi r9,r0,register_dump+36
	swi r10,r0,register_dump+40
	swi r11,r0,register_dump+44
	swi r12,r0,register_dump+48
	swi r13,r0,register_dump+52
	swi r14,r0,register_dump+56
/*  	swi r15,r0,register_dump+60 */
	swi r16,r0,register_dump+64
	swi r17,r0,register_dump+68
	swi r18,r0,register_dump+72
	swi r19,r0,register_dump+76
	swi r20,r0,register_dump+80
	swi r21,r0,register_dump+84
	swi r22,r0,register_dump+88
	swi r23,r0,register_dump+92
	swi r24,r0,register_dump+96
	swi r25,r0,register_dump+100
	swi r26,r0,register_dump+104
	swi r27,r0,register_dump+108
	swi r28,r0,register_dump+112
	swi r29,r0,register_dump+116
	swi r30,r0,register_dump+120
	swi r31,r0,register_dump+124
	mfs r31, rmsr
	brid Send_Status
	swi r31,r0,register_dump+132

	/*--------------------------------------------------*/
	/* Read registers from register_dump function       */
	/*  - Read all registers to the register dump       */
	/*    except R0  : makes no sense always zero       */
	/*           R15 : needed for the return address    */
	/*           R16 : contains the debug return address*/
	/*                 and can't be overwritten         */
	/*           R29 : is used by the eXecute command   */
	/*                 for storing the execution address*/
	/* Uses:                                            */
	/* Parameters:                                      */
	/* Returns using R15                                */
	/* Result in: No result                             */
	/*--------------------------------------------------*/
Get_All_Registers:	
	/* lwi r0,r0,register_dump + 0 */
	lwi r1,r0,register_dump + 4
	lwi r2,r0,register_dump + 8
	lwi r3,r0,register_dump+12
	lwi r4,r0,register_dump+16
	lwi r5,r0,register_dump+20
	lwi r6,r0,register_dump+24
	lwi r7,r0,register_dump+28
	lwi r8,r0,register_dump+32
	lwi r9,r0,register_dump+36
	lwi r10,r0,register_dump+40
	lwi r11,r0,register_dump+44
	lwi r12,r0,register_dump+48
	lwi r13,r0,register_dump+52
	lwi r14,r0,register_dump+56
	/* lwi r15,r0,register_dump+60 */
	lwi r16,r0,register_dump+64
	lwi r17,r0,register_dump+68
	lwi r18,r0,register_dump+72
	lwi r19,r0,register_dump+76
	lwi r20,r0,register_dump+80
	lwi r21,r0,register_dump+84
	lwi r22,r0,register_dump+88
	lwi r23,r0,register_dump+92
	lwi r24,r0,register_dump+96
	lwi r25,r0,register_dump+100
	lwi r26,r0,register_dump+104
	lwi r27,r0,register_dump+108
	lwi r28,r0,register_dump+112
	lwi r29,r0,register_dump+116
	lwi r30,r0,register_dump+120
	lwi r31,r0,register_dump+132
	mts rmsr, r31
	rtsd r15,8
	lwi r31,r0,register_dump+124
	
	/* Temporary memory needed for the byte-to-word */
	/* and word-to-byte translation                 */
	.data
	.align 2
temp_mem:			
	.int 0

	/* 34 words containing the register_dump (32 regs + pc + msr)       */
	.align 2
register_dump:
	.fill 34, 4, 0

	.end	xmd_stub 

