/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PM5639 Display Unit														*/
/*	Module:		COM.H																			*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	951017																		*/
/*	Rev. date:	970822 KEn	DEV															*/
/*	Status:		Version 2.3																	*/
/*																									*/
/*	Changes:																						*/
/*	970822 Released as version 2.3														*/
/*	951207 Released as version 2.2														*/
/*	951127 Added readstr() to be used in but.c										*/
/*	951126 Added new function: sendbyte( char val)									*/
/***************************************************************************/

extern void pause( unsigned char);
extern void sendbyte( char val);
extern void sendi( char* s);
extern void send( char* s);
extern bit read_k( void);
extern bit store_phosphor( char no);
extern bit recall_phosphor( char no);
extern char read_id( void);
extern void sendname( char no);
extern bit readstr( void);
extern void read_headid( void);
extern bit install_head( void);