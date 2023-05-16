//-----------------------------------------------------------------------------
// mn_userconst.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
//
// Description:
//  This file contains user defined constants used by the TCP/IP Stack.
//
// Generated by TCP/IP Configuration Wizard Version 3.23.
//

#ifndef  MN_USERCONST_H
#define  MN_USERCONST_H    1
//-----------------------------------------------------------------------------
// Default IP Address Configuration
//-----------------------------------------------------------------------------
//
// This section configures the default IP address for the MCU and
// the destination, if known. If SMTP is enabled, the SMTP server address
// should be specified.
//
// Note: If dialing into an ISP, the initial IP addresses will be overwritten
// when a new address is negotiated.
//
#define IP_SRC_ADDR  { 0, 0, 0, 1 } // IP address for the MCU
#define IP_DEST_ADDR { 255, 255, 255, 255 } // IP address for the destination

#define ETH_SRC_HW_ADDR { 0x00, 0x0B, 0x3C, 0x00, 0x00, 0x00 }
#define ETH_DEST_HW_ADDR { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
#define GATEWAY_IP_ADDR { 255, 255, 255, 255 }
#define SUBNET_MASK_ADDR { 255, 255, 255, 0 }

//-----------------------------------------------------------------------------
// User Selected Stack Settings
//-----------------------------------------------------------------------------
//
// This section configures the user selected stack settings.
//

// Sets the part family.
// This allows part-specific operation in the library code.
#define device_id          C8051F120

// If set, locates the transmit buffer in the USB FIFO space of the C8051F34x.
#define  c8051f340_usb_fifo   0

// Sets the number of sockets that can be used. The value must be between
// 1 and 127. Each socket uses about 46 bytes of RAM.
#define num_sockets     3

// Sets the size of the buffer used for transmission
#define xmit_buff_size    1514

// Sets the size of the buffer used for reception
#define recv_buff_size    1514

// Number of system ticks to wait for a packet
#define socket_wait_ticks 100

// Sets the time to live field in the IP packet
#define ip_time_to_live     64

// Sets the time to live field in an IP packet for multicast packets
#define multicast_ttl   1

// Timer 0  reload values such that Timer 0 overflows in 10ms.
// This defines a system tick.
#define tl0_flash        0x3E
#define th0_flash        0xB0

// Number of system ticks to wait for an ethernet packet
#define ether_wait_ticks  5

// Number of system ticks to keep entries in the ARP cache
#define arp_keep_ticks    6000

// Number of times an ARP packet is re-transmitted
#define arp_resend_trys   6

// Number of system ticks to wait for an ARP packet
#define arp_wait_ticks    600

// Number of entries in the ARP cache
#define arp_cache_size    4

// When set to 1, the ARP cache is updated after every valid packet is received.
// The ARP cache is always updated on PING requests
#define arp_auto_update   0

// Number of times a DHCP packet is transmitted before terminating the connection
#define dhcp_resend_trys  3

// Lease time, in seconds, to ask for. Default is 36000 (10hrs).
#define dhcp_default_lease_time 36000

// If PING is enabled the value is the size of the data from a PING request that
// can be stored. 9 bytes are added to the value to store part of the PING request
// header also. If the PING request contains more data than the specified value the
// packet will be discarded and no reply sent. The default value is 32.
#define ping_buff_size    32

// This value is both the amount of data you are willing to accept from the remote
// connection and the amount of data you are sending in a single packet. This value must
// be greater than 0 and less than or equal to 1460. A larger value will yield better
// throughput but require larger buffers. Note: The TCP/IP Stack uses a fixed window when
// receiving, not a sliding window as specified in RFC 793. If using PPP, the
// RECV_BUFF_SIZE and XMIT_BUFF_SIZE should be at least double the TCP
// window to allow for escaped characters. If using ethernet the RECV_BUFF_SIZE and
// XMIT_BUFF_SIZE should be at least TCP_WINDOW + 58.
#define tcp_window    512

// Number of system ticks to wait before retransmitting a TCP packet
#define tcp_resend_ticks  100

// Number of times a TCP packet is transmitted before aborting the connection
#define tcp_resend_trys   12

// The number of entries in the directory table in the virtual file system. Can be 1 to 255.
#define num_vf_pages    1

// The C8051F120 has 8K of on-chip XRAM. Off-chip access begins at 0x2000.
// Load EMI0CN with 0x20.
#define  BASE_ADDRESS   0x20

// This value is loaded into EMI0TC.
#define EMIF_TIMING 0xFF 

#endif      // MN_USERCONST_H