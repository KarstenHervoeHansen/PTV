//-----------------------------------------------------------------------------
// mn_funcs.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
//
// Description:
// 	This file contains function prototypes for functions available from
//  used by the TCP/IP Stack Library.
//
// Generated by TCP/IP Configuration Wizard Version 3.23.
//

#ifndef MN_FUNCS_H
#define MN_FUNCS_H   1

#include "mn_userconst.h"
#include "mn_stackconst.h"
#include "mn_defs.h"

//-----------------------------------------------------------------------------
// Funtion Prototypes
//-----------------------------------------------------------------------------

//--------------------------
// Socket Functions
//--------------------------
int mn_init(void);
SCHAR mn_open(byte [], word16, word16, byte, byte, byte, byte *, word16);
int mn_send(SCHAR, byte *, word16);
int mn_recv(SCHAR, byte *, word16);
int mn_recv_wait(SCHAR, byte *, word16, word16);
int mn_close(SCHAR);
int mn_abort(SCHAR);
PSOCKET_INFO mn_find_socket(word16,word16,byte *,byte);

//--------------------------
// Ethernet Functions
//--------------------------

int mn_ether_init(unsigned char, unsigned char, unsigned char);

void ether_reset_low();
void ether_reset_high();

//--------------------------
// Callback Functions
//--------------------------
SCHAR callback_app_process_packet(PSOCKET_INFO);
SCHAR callback_app_server_idle(PSOCKET_INFO *);
SCHAR callback_app_recv_idle(void);
SCHAR callback_app_server_process_packet(PSOCKET_INFO);
void callback_socket_closed(SCHAR socket_no);
void callback_socket_empty(PSOCKET_INFO socket_ptr);

//--------------------------
// DHCP Functions
//--------------------------
int mn_dhcp_start(byte *, word32);
int mn_dhcp_release(void);
int mn_dhcp_renew(word32);
extern unsigned char DHCPRenewCount;
//--------------------------
// VFILE Functions
//--------------------------
VF_PTR mn_vf_get_entry(byte *);
VF_PTR mn_vf_set_entry(byte *, word16, PCONST_BYTE, byte);
VF_PTR mn_vf_set_ram_entry(byte *, word16, byte *, byte);
SCHAR mn_vf_del_entry(byte *);
POST_FP mn_pf_get_entry(byte *);
PF_PTR mn_pf_set_entry(byte *, POST_FP);
byte mn_pf_del_entry(byte *);

//--------------------------
// Server Functions
//--------------------------
int mn_server(void);

//--------------------------
// CP2200 Functions
//--------------------------
unsigned char CPFLASH_ByteRead (unsigned int addr);
void CPFLASH_ByteWrite (unsigned int addr, char byte);
void CPFLASH_PageErase (unsigned int addr);

//--------------------------
// Support Functions
//--------------------------
byte mn_ustoa(byte *str,word16 num);
byte mn_uctoa(byte *str, byte num);
word16 mn_getMyIPAddr_func(byte **);
word16 mn_atous(byte *str);
void mn_wait_ticks(TIMER_TICK_T num_ticks);

//--------------------------
// Library Version Function
//--------------------------
unsigned int mn_get_library_version(void);

#endif	// MN_FUNCS_H