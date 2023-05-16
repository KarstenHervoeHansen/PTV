;---------------------------------------------------------------------------
;	Copyright (C) 2002 CYGNAL INTEGRATED PRODUCTS, INC.
; 	All rights reserved.
;
;
; 	FILE NAME  	: C8051F040.INC 
; 	TARGET MCUs	: C8051F040, 'F041, 'F042, 'F043 
; 	DESCRIPTION	: Register/bit definitions for the C8051F04x product family.  
;
; 	REVISION 1.1 	
;
;---------------------------------------------------------------------------

;  BYTE Registers
P0       DATA 080H    ; PORT 0                                        	
SP       DATA 081H    ; STACK POINTER                                 
DPL      DATA 082H    ; DATA POINTER - LOW BYTE                       
DPH      DATA 083H    ; DATA POINTER - HIGH BYTE                      
SFRPAGE  DATA 084H    ; PAGE SELECT                               
SFRNEXT  DATA 085H    ; STACK NEXT PAGE                           
SFRLAST  DATA 086H    ; STACK LAST PAGE                           
PCON     DATA 087H    ; POWER CONTROL                                 
TCON     DATA 088H    ; TIMER CONTROL                                 
CPT0CN   DATA 088H    ; COMPARATOR 0 CONTROL                          
CPT1CN   DATA 088H    ; COMPARATOR 1 CONTROL                          
CPT2CN   DATA 088H    ; COMPARATOR 2 CONTROL                          
TMOD     DATA 089H    ; TIMER MODE                                    
CPT0MD   DATA 089H    ; COMPARATOR 0 MODE                             
CPT1MD   DATA 089H    ; COMPARATOR 1 MODE                             
CPT2MD   DATA 089H    ; COMPARATOR 2 MODE                             
TL0      DATA 08AH    ; TIMER 0 - LOW BYTE                            
OSCICN   DATA 08AH    ; INTERNAL OSCILLATOR CONTROL                   
TL1      DATA 08BH    ; TIMER 1 - LOW BYTE                            
OSCICL   DATA 08BH    ; INTERNAL OSCILLATOR CALIBRATION               
TH0      DATA 08CH    ; TIMER 0 - HIGH BYTE                           
OSCXCN   DATA 08CH    ; EXTERNAL OSCILLATOR CONTROL                   	
TH1      DATA 08DH    ; TIMER 1 - HIGH BYTE                           
CKCON    DATA 08EH    ; TIMER 0/1 CLOCK CONTROL                       
PSCTL    DATA 08FH    ; FLASH WRITE/ERASE CONTROL                     
P1       DATA 090H    ; PORT 1                                        
SSTA0    DATA 091H    ; UART 0 STATUS                                 
SFRPGCN  DATA 096H    ; PAGE CONTROL                              
CLKSEL   DATA 097H    ; SYSTEM CLOCK SELECT                           
SCON0    DATA 098H    ; UART 0 CONTROL                                
SCON1    DATA 098H    ; UART 1 CONTROL                                
SBUF0    DATA 099H    ; UART 0 BUFFER                                 
SBUF1    DATA 099H    ; UART 1 BUFFER                                 
SPI0CFG  DATA 09AH    ; SPI 0 CONFIGURATION                           
SPI0DAT  DATA 09BH    ; SPI 0 DATA                                    
P4MDOUT  DATA 09CH    ; PORT 4 OUTPUT MODE                            
SPI0CKR  DATA 09DH    ; SPI 0 CLOCK RATE CONTROL                      
P5MDOUT  DATA 09DH    ; PORT 5 OUTPUT MODE                            
P6MDOUT  DATA 09EH    ; PORT 6 OUTPUT MODE                            
P7MDOUT  DATA 09FH    ; PORT 7 OUTPUT MODE                            
P2       DATA 0A0H    ; PORT 2                                        
EMI0TC   DATA 0A1H    ; EMIF TIMING CONTROL                           
EMI0CN   DATA 0A2H    ; EMIF CONTROL                                  
EMI0CF   DATA 0A3H    ; EMIF CONFIGURATION                            
P0MDOUT  DATA 0A4H    ; PORT 0 OUTPUT MODE                            
P1MDOUT  DATA 0A5H    ; PORT 1 OUTPUT MODE                            
P2MDOUT  DATA 0A6H    ; PORT 2 OUTPUT MODE CONFIGURATION              
P3MDOUT  DATA 0A7H    ; PORT 3 OUTPUT MODE CONFIGURATION              
IE       DATA 0A8H    ; INTERRUPT ENABLE                              
SADDR0   DATA 0A9H    ; UART 0 SLAVE ADDRESS                          
SADDR1   DATA 0A9H    ; UART 1 SLAVE ADDRESS                          
P1MDIN   DATA 0ADH    ; PORT 1 INPUT MODE                             
P2MDIN   DATA 0AEH    ; PORT 2 INPUT MODE                             
P3MDIN   DATA 0AFH    ; PORT 3 INPUT MODE                             
P3       DATA 0B0H    ; PORT 3                                        
FLSCL    DATA 0B7H    ; FLASH TIMING PRESCALAR                        
FLACL    DATA 0B7H    ; FLASH ACCESS LIMIT                            
IP       DATA 0B8H    ; INTERRUPT PRIORITY                            
SADEN0   DATA 0B9H    ; UART 0 SLAVE ADDRESS MASK                     
AMX2CF   DATA 0BAH    ; ADC 2 MUX CONFIGURATION                       
AMX0PRT  DATA 0BDH    ; ADC 0 MUX PORT PIN SELECT REGISTER            
AMX0CF   DATA 0BAH    ; ADC 0 CONFIGURATION REGISTER                  
AMX0SL   DATA 0BBH    ; ADC 0 AND ADC 1 MODE SELECTION                
AMX2SL   DATA 0BBH    ; ADC 2 MUX CHANNEL SELECTION                   
ADC0CF   DATA 0BCH    ; ADC 0 CONFIGURATION                           
ADC2CF   DATA 0BCH    ; ADC 2 CONFIGURATION                           
ADC0L    DATA 0BEH    ; ADC 0 DATA - LOW BYTE                         
ADC2     DATA 0BEH    ; ADC 2 DATA - LOW BYTE                         
ADC0H    DATA 0BFH    ; ADC 0 DATA - HIGH BYTE                        
SMB0CN   DATA 0C0H    ; SMBUS 0 CONTROL                               
CAN0STA  DATA 0C0H    ; CAN 0 STATUS                                  
SMB0STA  DATA 0C1H    ; SMBUS 0 STATUS                                
SMB0DAT  DATA 0C2H    ; SMBUS 0 DATA                                  
SMB0ADR  DATA 0C3H    ; SMBUS 0 SLAVE ADDRESS                         
ADC0GTL  DATA 0C4H    ; ADC 0 GREATER-THAN REGISTER - LOW BYTE        
ADC2GT   DATA 0C4H    ; ADC 2 GREATER-THAN REGISTER - LOW BYTE        
ADC0GTH  DATA 0C5H    ; ADC 0 GREATER-THAN REGISTER - HIGH BYTE       
ADC0LTL  DATA 0C6H    ; ADC 0 LESS-THAN REGISTER - LOW BYTE           
ADC2LT   DATA 0C6H    ; ADC 2 LESS-THAN REGISTER - LOW BYTE           
ADC0LTH  DATA 0C7H    ; ADC 0 LESS-THAN REGISTER - HIGH BYTE          
TMR2CN   DATA 0C8H    ; TIMER 2 CONTROL                               
TMR3CN   DATA 0C8H    ; TIMER 3 CONTROL                               
TMR4CN   DATA 0C8H    ; TIMER 4 CONTROL                               
P4       DATA 0C8H    ; PORT 4                                        
TMR2CF   DATA 0C9H    ; TIMER 2 CONFIGURATION                         
TMR3CF   DATA 0C9H    ; TIMER 3 CONFIGURATION                         
TMR4CF   DATA 0C9H    ; TIMER 4 CONFIGURATION                         
RCAP2L   DATA 0CAH    ; TIMER 2 CAPTURE REGISTER - LOW BYTE           
RCAP3L   DATA 0CAH    ; TIMER 3 CAPTURE REGISTER - LOW BYTE           
RCAP4L   DATA 0CAH    ; TIMER 4 CAPTURE REGISTER - LOW BYTE           
RCAP2H   DATA 0CBH    ; TIMER 2 CAPTURE REGISTER - HIGH BYTE          
RCAP3H   DATA 0CBH    ; TIMER 3 CAPTURE REGISTER - HIGH BYTE          
RCAP4H   DATA 0CBH    ; TIMER 4 CAPTURE REGISTER - HIGH BYTE          
TMR2L    DATA 0CCH    ; TIMER 2 - LOW BYTE                            
TMR3L    DATA 0CCH    ; TIMER 3 - LOW BYTE                            
TMR4L    DATA 0CCH    ; TIMER 4 - LOW BYTE                            
TMR2H    DATA 0CDH    ; TIMER 2 - HIGH BYTE                           
TMR3H    DATA 0CDH    ; TIMER 3 - HIGH BYTE                           
TMR4H    DATA 0CDH    ; TIMER 4 - HIGH BYTE                           
SMB0CR   DATA 0CFH    ; SMBUS 0 CLOCK RATE                            
PSW      DATA 0D0H    ; PROGRAM STATUS WORD                           
REF0CN   DATA 0D1H    ; VOLTAGE REFERENCE 0 CONTROL                   
DAC0L    DATA 0D2H    ; DAC 0 REGISTER - LOW BYTE                     
DAC1L    DATA 0D2H    ; DAC 1 REGISTER - LOW BYTE                     
DAC0H    DATA 0D3H    ; DAC 0 REGISTER - HIGH BYTE                    
DAC1H    DATA 0D3H    ; DAC 1 REGISTER - HIGH BYTE                    
DAC0CN   DATA 0D4H    ; DAC 0 CONTROL                                 
DAC1CN   DATA 0D4H    ; DAC 1 CONTROL                                 
HVA0CN   DATA 0D6H    ; HVDA CONTROL REGISTER                         
PCA0CN   DATA 0D8H    ; PCA 0 COUNTER CONTROL                         
CAN0DATL DATA 0D8H    ; CAN 0 DATA - LOW BYTE                         
P5       DATA 0D8H    ; PORT 5                                        
PCA0MD   DATA 0D9H    ; PCA 0 COUNTER MODE                            
CAN0DATH DATA 0D9H    ; CAN 0 DATA - HIGH BYTE                        
PCA0CPM0 DATA 0DAH    ; PCA 0 MODULE 0 CONTROL                        
CAN0ADR  DATA 0DAH    ; CAN 0 ADDRESS                                 
PCA0CPM1 DATA 0DBH    ; PCA 0 MODULE 1 CONTROL                        
CAN0TST  DATA 0DBH    ; CAN 0 TEST                                    
PCA0CPM2 DATA 0DCH    ; PCA 0 MODULE 2 CONTROL                        
PCA0CPM3 DATA 0DDH    ; PCA 0 MODULE 3 CONTROL                        
PCA0CPM4 DATA 0DEH    ; PCA 0 MODULE 4 CONTROL                        
PCA0CPM5 DATA 0DFH    ; PCA 0 MODULE 5 CONTROL                        
ACC      DATA 0E0H    ; ACCUMULATOR                                   
PCA0CPL5 DATA 0E1H    ; PCA 0 MODULE 5 CAPTURE/COMPARE - LOW BYTE     
XBR0     DATA 0E1H    ; CROSSBAR CONFIGURATION REGISTER 0             
PCA0CPH5 DATA 0E2H    ; PCA 0 MODULE 5 CAPTURE/COMPARE - HIGH BYTE    
XBR1     DATA 0E2H    ; CROSSBAR CONFIGURATION REGISTER 1             
XBR2     DATA 0E3H    ; CROSSBAR CONFIGURATION REGISTER 2             
XBR3     DATA 0E4H    ; CROSSBAR CONFIGURATION REGISTER 3             
EIE1     DATA 0E6H    ; EXTERNAL INTERRUPT ENABLE 1                   
EIE2     DATA 0E7H    ; EXTERNAL INTERRUPT ENABLE 2                   
ADC0CN   DATA 0E8H    ; ADC 0 CONTROL                                 	
ADC2CN   DATA 0E8H    ; ADC 2 CONTROL                                 
P6       DATA 0E8H    ; PORT 6                                        
PCA0CPL2 DATA 0E9H    ; PCA 0 MODULE 2 CAPTURE/COMPARE - LOW BYTE     
PCA0CPH2 DATA 0EAH    ; PCA 0 MODULE 2 CAPTURE/COMPARE - HIGH BYTE    
PCA0CPL3 DATA 0EBH    ; PCA 0 MODULE 3 CAPTURE/COMPARE - LOW BYTE     
PCA0CPH3 DATA 0ECH    ; PCA 0 MODULE 3 CAPTURE/COMPARE - HIGH BYTE    
PCA0CPL4 DATA 0EDH    ; PCA 0 MODULE 4 CAPTURE/COMPARE - LOW BYTE     
PCA0CPH4 DATA 0EEH    ; PCA 0 MODULE 4 CAPTURE/COMPARE - HIGH BYTE    
RSTSRC   DATA 0EFH    ; RESET SOURCE                                  
B        DATA 0F0H    ; B REGISTER                                    
EIP1     DATA 0F6H    ; EXTERNAL INTERRUPT PRIORITY REGISTER 1        
EIP2     DATA 0F7H    ; EXTERNAL INTERRUPT PRIORITY REGISTER 2        
SPI0CN   DATA 0F8H    ; SPI 0 CONTROL                                 	
CAN0CN   DATA 0F8H    ; CAN 0 CONTROL                                 	
P7       DATA 0F8H    ; PORT 7                                        
PCA0L    DATA 0F9H    ; PCA 0 TIMER - LOW BYTE                        
PCA0H    DATA 0FAH    ; PCA 0 TIMER - HIGH BYTE                       
PCA0CPL0 DATA 0FBH    ; PCA 0 MODULE 0 CAPTURE/COMPARE - LOW BYTE     
PCA0CPH0 DATA 0FCH    ; PCA 0 MODULE 0 CAPTURE/COMPARE - HIGH BYTE    
PCA0CPL1 DATA 0FDH    ; PCA 0 MODULE 1 CAPTURE/COMPARE - LOW BYTE     
PCA0CPH1 DATA 0FEH    ; PCA 0 MODULE 1 CAPTURE/COMPARE - HIGH BYTE    
WDTCN    DATA 0FFH    ; WATCHDOG TIMER CONTROL                        
	

;  BIT Registers  

  ;TCON  0x88 
 TF1   BIT TCON.7               ;TIMER 1 OVERFLOW FLAG      
 TR1   BIT TCON.6               ;TIMER 1 ON/OFF CONTROL     
 TF0   BIT TCON.5               ;TIMER 0 OVERFLOW FLAG      
 TR0   BIT TCON.4               ;TIMER 0 ON/OFF CONTROL     
 IE1   BIT TCON.3               ;EXT. INTERRUPT 1 EDGE FLAG 
 IT1   BIT TCON.2               ;EXT. INTERRUPT 1 TYPE      
 IE0   BIT TCON.1               ;EXT. INTERRUPT 0 EDGE FLAG 
 IT0   BIT TCON.0               ;EXT. INTERRUPT 0 TYPE      

  ;CPT0CN  0x88 
 CP0EN   BIT CPT0CN.7           ;COMPARATOR 0 ENABLE                 
 CP0OUT  BIT CPT0CN.6           ;COMPARATOR 0 OUTPUT                 
 CP0RIF  BIT CPT0CN.5           ;COMPARATOR 0 RISING EDGE INTERRUPT  
 CP0FIF  BIT CPT0CN.4           ;COMPARATOR 0 FALLING EDGE INTERRUPT 
 CP0HYP1 BIT CPT0CN.3           ;COMPARATOR 0 POSITIVE HYSTERESIS 1  
 CP0HYP0 BIT CPT0CN.2           ;COMPARATOR 0 POSITIVE HYSTERESIS 0  
 CP0HYN1 BIT CPT0CN.1           ;COMPARATOR 0 NEGATIVE HYSTERESIS 1  
 CP0HYN0 BIT CPT0CN.0           ;COMPARATOR 0 NEGATIVE HYSTERESIS 0  

  ;CPT1CN  0x88 
 CP1EN   BIT CPT1CN.7           ;COMPARATOR 1 ENABLE                 
 CP1OUT  BIT CPT1CN.6           ;COMPARATOR 1 OUTPUT                 
 CP1RIF  BIT CPT1CN.5           ;COMPARATOR 1 RISING EDGE INTERRUPT  
 CP1FIF  BIT CPT1CN.4           ;COMPARATOR 1 FALLING EDGE INTERRUPT 
 CP1HYP1 BIT CPT1CN.3           ;COMPARATOR 1 POSITIVE HYSTERESIS 1  
 CP1HYP0 BIT CPT1CN.2           ;COMPARATOR 1 POSITIVE HYSTERESIS 0  
 CP1HYN1 BIT CPT1CN.1           ;COMPARATOR 1 NEGATIVE HYSTERESIS 1  
 CP1HYN0 BIT CPT1CN.0           ;COMPARATOR 1 NEGATIVE HYSTERESIS 0  

  ;CPT2CN  0x88 
 CP2EN   BIT CPT2CN.7           ;COMPARATOR 2 ENABLE                 
 CP2OUT  BIT CPT2CN.6           ;COMPARATOR 2 OUTPUT                 
 CP2RIF  BIT CPT2CN.5           ;COMPARATOR 2 RISING EDGE INTERRUPT  
 CP2FIF  BIT CPT2CN.4           ;COMPARATOR 2 FALLING EDGE INTERRUPT 
 CP2HYP1 BIT CPT2CN.3           ;COMPARATOR 2 POSITIVE HYSTERESIS 1  
 CP2HYP0 BIT CPT2CN.2           ;COMPARATOR 2 POSITIVE HYSTERESIS 0  
 CP2HYN1 BIT CPT2CN.1           ;COMPARATOR 2 NEGATIVE HYSTERESIS 1  
 CP2HYN0 BIT CPT2CN.0           ;COMPARATOR 2 NEGATIVE HYSTERESIS 0  

  ;SCON0  0x98 
 SM00 BIT SCON0.7               ;UART 0 MODE 0            
 SM10 BIT SCON0.6               ;UART 0 MODE 1            
 SM20 BIT SCON0.5               ;UART 0 MCE               
 REN0 BIT SCON0.4               ;UART 0 RX ENABLE         
 TB80 BIT SCON0.3               ;UART 0 TX BIT 8          
 RB80 BIT SCON0.2               ;UART 0 RX BIT 8          
 TI0  BIT SCON0.1               ;UART 0 TX INTERRUPT FLAG 
 RI0  BIT SCON0.0               ;UART 0 RX INTERRUPT FLAG 

  ;SCON1  0x98 
 S0MODE BIT SCON1.7             ;UART 1 MODE              	
 MCE1   BIT SCON1.5             ;UART 1 MCE               
 REN1   BIT SCON1.4             ;UART 1 RX ENABLE         
 TB81   BIT SCON1.3             ;UART 1 TX BIT 8          
 RB81   BIT SCON1.2             ;UART 1 RX BIT 8          
 TI1    BIT SCON1.1             ;UART 1 TX INTERRUPT FLAG 
 RI1    BIT SCON1.0             ;UART 1 RX INTERRUPT FLAG 

  ;IE  0xA8 
 EA    BIT IE.7                 ;GLOBAL INTERRUPT ENABLE      	
 ET2   BIT IE.5                 ;TIMER 2 INTERRUPT ENABLE     
 ES0   BIT IE.4                 ;UART0 INTERRUPT ENABLE       
 ET1   BIT IE.3                 ;TIMER 1 INTERRUPT ENABLE     
 EX1   BIT IE.2                 ;EXTERNAL INTERRUPT 1 ENABLE  
 ET0   BIT IE.1                 ;TIMER 0 INTERRUPT ENABLE     
 EX0   BIT IE.0                 ;EXTERNAL INTERRUPT 0 ENABLE  

  ;IP  0xB8 
 PT2   BIT IP.5                 ;TIMER 2 PRIORITY						
 PS    BIT IP.4                 ;SERIAL PORT PRIORITY				
 PT1   BIT IP.3                 ;TIMER 1 PRIORITY					
 PX1   BIT IP.2                 ;EXTERNAL INTERRUPT 1 PRIORITY	
 PT0   BIT IP.1                 ;TIMER 0 PRIORITY					
 PX0   BIT IP.0                 ;EXTERNAL INTERRUPT 0 PRIORITY			

 ;SMB0CN 0xC0 
 BUSY   BIT SMB0CN.7            ;SMBUS 0 BUSY                    
 ENSMB  BIT SMB0CN.6            ;SMBUS 0 ENABLE                  
 STA    BIT SMB0CN.5            ;SMBUS 0 START FLAG              
 STO    BIT SMB0CN.4            ;SMBUS 0 STOP FLAG               
 SI     BIT SMB0CN.3            ;SMBUS 0 INTERRUPT PENDING FLAG  
 AA     BIT SMB0CN.2            ;SMBUS 0 ASSERT/ACKNOWLEDGE FLAG 
 SMBFTE BIT SMB0CN.1            ;SMBUS 0 FREE TIMER ENABLE       
 SMBTOE BIT SMB0CN.0            ;SMBUS 0 TIMEOUT ENABLE          

 ;CAN0STA 0xC0 
 BOFF   BIT CAN0STA.7           ;Bus Off Status                  
 EWARN  BIT CAN0STA.6           ;Warning Status                   
 EPASS  BIT CAN0STA.5           ;Error Passive                   
 RXOK   BIT CAN0STA.4           ;Received Message Successfully   
 TXOK   BIT CAN0STA.3           ;Transmit a Message Successfully 
 LEC2   BIT CAN0STA.2           ;Last error code bit 2           
 LEC1   BIT CAN0STA.1           ;Last error code bit 1           
 LEC0   BIT CAN0STA.0           ;Last error code bit            

  ;TMR2CN  0xC8 
 TF2   BIT TMR2CN.7             ;TIMER 2 OVERFLOW FLAG        
 EXF2  BIT TMR2CN.6             ;TIMER 2 EXTERNAL FLAG        
 EXEN2 BIT TMR2CN.3             ;TIMER 2 EXTERNAL ENABLE FLAG 	
 TR2   BIT TMR2CN.2             ;TIMER 2 ON/OFF CONTROL       
 CT2   BIT TMR2CN.1             ;TIMER 2 COUNTER SELECT       
 CPRL2 BIT TMR2CN.0             ;TIMER 2 CAPTURE SELECT       

  ;TMR3CN  0xC8 
 TF3   BIT TMR3CN.7             ;TIMER 3 OVERFLOW FLAG        
 EXF3  BIT TMR3CN.6             ;TIMER 3 EXTERNAL FLAG        
 EXEN3 BIT TMR3CN.3             ;TIMER 3 EXTERNAL ENABLE FLAG 	
 TR3   BIT TMR3CN.2             ;TIMER 3 ON/OFF CONTROL       
 CT3   BIT TMR3CN.1             ;TIMER 3 COUNTER SELECT       
 CPRL3 BIT TMR3CN.0             ;TIMER 3 CAPTURE SELECT       

  ;TMR4CN  0xC8 
 TF4   BIT TMR4CN.7             ;TIMER 4 OVERFLOW FLAG        
 EXF4  BIT TMR4CN.6             ;TIMER 4 EXTERNAL FLAG        
 EXEN4 BIT TMR4CN.3             ;TIMER 4 EXTERNAL ENABLE FLAG 	
 TR4   BIT TMR4CN.2             ;TIMER 4 ON/OFF CONTROL       
 CT4   BIT TMR4CN.1             ;TIMER 4 COUNTER SELECT       
 CPRL4 BIT TMR4CN.0             ;TIMER 4 CAPTURE SELECT       

  ;PSW 0xD0 
 CY  BIT PSW.7                  ;CARRY FLAG              	
 AC  BIT PSW.6                  ;AUXILIARY CARRY FLAG    
 F0  BIT PSW.5                  ;USER FLAG 0             
 RS1 BIT PSW.4                  ;REGISTER BANK SELECT 1  
 RS0 BIT PSW.3                  ;REGISTER BANK SELECT 0  
 OV  BIT PSW.2                  ;OVERFLOW FLAG           
 F1  BIT PSW.1                  ;USER FLAG 1             
 P   BIT PSW.0                  ;ACCUMULATOR PARITY FLAG 

 ;PCA0CN 0xD8 
 CF   BIT PCA0CN.7              ;PCA 0 COUNTER OVERFLOW FLAG   
 CR   BIT PCA0CN.6              ;PCA 0 COUNTER RUN CONTROL BIT 
 CCF5 BIT PCA0CN.5              ;PCA 0 MODULE 5 INTERRUPT FLAG 
 CCF4 BIT PCA0CN.4              ;PCA 0 MODULE 4 INTERRUPT FLAG 
 CCF3 BIT PCA0CN.3              ;PCA 0 MODULE 3 INTERRUPT FLAG 
 CCF2 BIT PCA0CN.2              ;PCA 0 MODULE 2 INTERRUPT FLAG 
 CCF1 BIT PCA0CN.1              ;PCA 0 MODULE 1 INTERRUPT FLAG 
 CCF0 BIT PCA0CN.0              ;PCA 0 MODULE 0 INTERRUPT FLAG 


 ;ADC0CN 0xE8 
 AD0EN   BIT ADC0CN.7           ;ADC 0 ENABLE                   
 AD0TM   BIT ADC0CN.6           ;ADC 0 TRACK MODE               
 AD0INT  BIT ADC0CN.5           ;ADC 0 EOC INTERRUPT FLAG       
 AD0BUSY BIT ADC0CN.4           ;ADC 0 BUSY FLAG                
 AD0CM1  BIT ADC0CN.3           ;ADC 0 CONVERT START MODE BIT 1 
 AD0CM0  BIT ADC0CN.2           ;ADC 0 CONVERT START MODE BIT 0 
 AD0WINT BIT ADC0CN.1           ;ADC 0 WINDOW INTERRUPT FLAG    


 ;ADC2CN 0xE8 
 AD2EN   BIT ADC2CN.7           ;ADC 2 ENABLE                   
 AD2TM   BIT ADC2CN.6           ;ADC 2 TRACK MODE               
 AD2INT  BIT ADC2CN.5           ;ADC 2 EOC INTERRUPT FLAG       
 AD2BUSY BIT ADC2CN.4           ;ADC 2 BUSY FLAG                
 AD2WINT BIT ADC2CN.3           ;ADC 2 WINDOW INTERRUPT FLAG    
 AD2CM2  BIT ADC2CN.2           ;ADC 2 CONVERT START MODE BIT 2 
 AD2CM1  BIT ADC2CN.1           ;ADC 2 CONVERT START MODE BIT 1 
 AD2CM0  BIT ADC2CN.0           ;ADC 2 CONVERT START MODE BIT 0 

 ;SPI0CN 0xF8 
 SPIF   BIT SPI0CN.7            ;SPI 0 INTERRUPT FLAG       
 WCOL   BIT SPI0CN.6            ;SPI 0 WRITE COLLISION FLAG 
 MODF   BIT SPI0CN.5            ;SPI 0 MODE FAULT FLAG      
 RXOVRN BIT SPI0CN.4            ;SPI 0 RX OVERRUN FLAG
 NSSMD1 BIT SPI0CN.3						;SPI 0 SLAVE SELECT MODE BIT 1
 NSSMD0 BIT SPI0CN.2            ;SPI 0 SLAVE SELECT MODE BIT 0      
 TXBMT  BIT SPI0CN.1            ;SPI 0 TX BUFFER EMPTY        
 SPIEN  BIT SPI0CN.0            ;SPI 0 SPI ENABLE           			

 ;CAN0CN 0xF8 
 CANINIT BIT CAN0CN.0          ;CAN Initialization bit 
 CANIE   BIT CAN0CN.1          ;CAN Module Interrupt Enable Bit 
 CANSIE  BIT CAN0CN.2          ;CAN Status change Interrupt Enable Bit 
 CANEIE  BIT CAN0CN.3          ;CAN Module Error Interrupt Enable Bit 
 CANIF   BIT CAN0CN.4          ;CAN Module Interrupt Flag 
 CANDAR  BIT CAN0CN.5          ;CAN Disable Automatic Retransmission bit 
 CANCCE  BIT CAN0CN.6          ;CAN Configuration Change Enable bit 
 CANTEST BIT CAN0CN.7          ;CAN Test Mode Enable bit 



 ;SFR PAGE DEFINITIONS 
  CONFIG_PAGE       EQU 0FH      ;SYSTEM AND PORT CONFIGURATION PAGE 
  LEGACY_PAGE       EQU 00H     ;LEGACY SFR PAGE                    
  TIMER01_PAGE      EQU 00H     ;TIMER 0 AND TIMER 1                
  CPT0_PAGE         EQU 01H     ;COMPARATOR 0                       
  CPT1_PAGE         EQU 02H     ;COMPARATOR 1                       
  CPT2_PAGE         EQU 03H     ;COMPARATOR 2                       
  UART0_PAGE        EQU 00H     ;UART 0                             
  UART1_PAGE        EQU 01H     ;UART 1                             
  SPI0_PAGE         EQU 00H     ;SPI 0                              
  EMI0_PAGE         EQU 00H     ;EXTERNAL MEMORY INTERFACE          
  ADC0_PAGE         EQU 00H     ;ADC 0                              
  ADC2_PAGE         EQU 02H     ;ADC 2                              
  SMB0_PAGE         EQU 00H     ;SMBUS 0                            
  TMR2_PAGE         EQU 00H     ;TIMER 2                            
  TMR3_PAGE         EQU 01H     ;TIMER 3                            
  TMR4_PAGE         EQU 02H     ;TIMER 4                            
  DAC0_PAGE         EQU 00H     ;DAC 0                              
  DAC1_PAGE         EQU 01H     ;DAC 1                              
  PCA0_PAGE         EQU 00H     ;PCA 0                              
  CAN0_PAGE         EQU 01H     ;CAN 0                              
