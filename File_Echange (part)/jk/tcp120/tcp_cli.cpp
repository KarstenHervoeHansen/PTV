/* TCP echo client to be run against Silicon Laboratories ETHERNET-DK in TCP echo server mode.

  compile with: cl tcp_cli.cpp wsedesc.cpp wsock32.lib

   Usage: tcp_cli                            ( defaults to 216.233.5.26, 7 )
          tcp_cli [dest ip addr]             ( defaults to port 7 )
          tcp_cli [dest ip addr] [dest port]

  PJB, 11/20/01, 10/24/02
*/

#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include "dhcp.h"

struct ASTAT
  {
  ADAPTER_STATUS adapt;
  NAME_BUFFER    NameBuff[30];
  } Adapter;

//#define DEBUG
// Function prototype
void StreamClient(long lIP_addr, short nPort);
int create_dhcp_socket(void);
int GetAdapterInfo(int nAdapterNum, char * sMAC);

// Helper macro for displaying errors
LPCSTR WSErrorDescription(int);
#define PRINTERROR(s) \
    fprintf(stderr,"\n%s: %s\n", s, WSErrorDescription(WSAGetLastError()))

////////////////////////////////////////////////////////////

void main(int argc, char **argv)
  {
  WORD    wVersionRequested = MAKEWORD(1, 1);
  WSADATA wsaData;
  int     nRet;
  short   nPort;
  char    tmp[25];

  LANA_ENUM AdapterList;
  NCB       Ncb;        /* Network Control Block */
  int       i;

  //
  // Initialize WinSock and check the version
  //
  nRet = WSAStartup(wVersionRequested, &wsaData);
  if (nRet != 0 || wsaData.wVersion != wVersionRequested)
    { 
    fprintf(stderr,"\nError: wrong WinSock version\n");
    return;
    }

  char          ip_addr[16];
  unsigned long lIP_addr;
  memset(ip_addr, 0,  16);

  /* get Port number from command line, use 7 if none given */
  if (argc == 3)
    {
    nPort = atoi(argv[2]);
    if (nPort <= 0)
      {
      printf("Bad port number!");
      return;
      }
    }
  else
    nPort = 7;    // well known echo port

  /* get IP address from command line, use 216.233.5.26 if none given */
  if (argc >= 2)
    strcpy(ip_addr, argv[1]);
  else
    strcpy(ip_addr,"216.233.5.26");

  lIP_addr = inet_addr(ip_addr);
  if (lIP_addr == INADDR_NONE)
    {
    printf("Bad IP address!");
    return;
    }

  create_dhcp_socket();

  // Get adapter list
  memset(&Ncb, 0, sizeof(NCB));
  Ncb.ncb_command = NCBENUM;
  Ncb.ncb_buffer  = (unsigned char *)&AdapterList;
  Ncb.ncb_length  = sizeof(AdapterList);
  Netbios(&Ncb);

  // Get all of the local ethernet addresses
  for (i = 0; i < AdapterList.length; ++i)
    {
    if (GetAdapterInfo(AdapterList.lana[i], tmp))
      {
      printf("Adapter %d has MAC: %s\n", AdapterList.lana[i], tmp);
      }
    }


  // Go do the stuff a stream client does
  //
  StreamClient(lIP_addr, nPort);

  
  //
  // Release WinSock
  //
  WSACleanup();
}

////////////////////////////////////////////////////////////

void StreamClient(long lIP_addr, short nPort)
  {
  printf("\nTCP Client sending to TCP Echo Server...");

  //
  // Create a TCP/IP stream socket
  //
  SOCKET  theSocket;

  theSocket = socket(AF_INET,         // Address family
                     SOCK_STREAM,     // Socket type
                     IPPROTO_TCP);    // Protocol

  if (theSocket == INVALID_SOCKET)
    {
    PRINTERROR("socket()");
    return;
    }
  printf("Socket created.\n");
  //
  // Fill in the address structure for the ermote server
  //
  SOCKADDR_IN saServer;

  saServer.sin_family           = AF_INET;
  saServer.sin_addr.S_un.S_addr = lIP_addr;
  saServer.sin_port             = htons(nPort);

  //
  // connect to the server
  //
  int nRet;

  nRet = connect(theSocket,                  // Socket
                 (LPSOCKADDR)&saServer,      // Server address
                 sizeof(struct sockaddr));   // Length of server address structure

  if (nRet == SOCKET_ERROR)
    {
    PRINTERROR("connect()");
    closesocket(theSocket);
    return;
    }

  printf("Connected.\n");

  int nPacket = 0;

  while (1)
    {
    //
    // Send data to the server
    //
    char szBuf[1500];

    nPacket++;
    sprintf(szBuf, "This is TCP echo packet #%d",nPacket);

    nRet = send(theSocket,          // Connected socket
                szBuf,              // Data buffer
                strlen(szBuf),      // Length of data
                0);                 // Flags

    if (nRet == SOCKET_ERROR)
      {
      PRINTERROR("send()");
      break;
      }

    //
    // Wait for a reply
    //
    nRet = recv(theSocket,          // Connected socket
                szBuf,              // Receive buffer
                sizeof(szBuf),      // Size of receive buffer
                0);                 // Flags

    if (nRet == SOCKET_ERROR)
      {
      PRINTERROR("recv()");
      break;
      }
    //
    // Display the received data
    //
    printf("\nData received: %s", szBuf);
    if (nPacket == 15)
      break;
    }
  closesocket(theSocket);
  printf("Socket closed.\n");
  return;
  }


/* creates a socket for DHCP communication */
int create_dhcp_socket(void)
  {
  struct sockaddr_in myname;
#if 0
	struct ifreq       interface;
#endif
  int                sock;
  int                flag = 1;

        /* Set up the address we're going to bind to. */

  memset(&myname, 0, sizeof(myname));
  myname.sin_family      = AF_INET;
  myname.sin_port        = htons(DHCP_CLIENT_PORT);
  myname.sin_addr.s_addr = INADDR_ANY;                 /* listen on any address */
  memset(&myname.sin_zero, 0, sizeof(myname.sin_zero));

        /* create a socket for DHCP communications */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0)
    {
		printf("Error: Could not create socket!\n");
		exit(STATE_UNKNOWN);
    }

#ifdef DEBUG
	printf("DHCP socket: %d\n", sock);
#endif

  /* set the reuse address flag so we don't get errors when restarting */
  flag = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag)) < 0)
    {
    printf("Error: Could not set reuse address option on DHCP socket!\n");
    exit(STATE_UNKNOWN);
    }
  
  /* set the broadcast option - we need this to listen to DHCP broadcast messages */
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&flag, sizeof flag) < 0)
    {
    printf("Error: Could not set broadcast option on DHCP socket!\n");
    exit(STATE_UNKNOWN);
    }
#if 0
	/* bind socket to interface */
	strncpy(interface.ifr_ifrn.ifrn_name, network_interface_name, IFNAMSIZ);
	if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface)) < 0)
    {
    printf("Error: Could not bind socket to interface %s.  Check your privileges...\n", network_interface_name);
    exit(STATE_UNKNOWN);
    }
#endif
  /* bind the socket */
  if (bind(sock, (struct sockaddr *)&myname, sizeof(myname)) < 0)
    {
    printf("Error: Could not bind to DHCP socket (port %d)!  Check your privileges...\n", DHCP_CLIENT_PORT);
    exit(STATE_UNKNOWN);
    }
  printf("DHCP socket on port %d  created sock = %d...\n", DHCP_CLIENT_PORT, sock);
  return sock;
  }
  
int GetAdapterInfo(int nAdapterNum, char * sMAC)
  {
  // Reset the LAN adapter so that we can begin querying it 
  NCB Ncb;

  memset(&Ncb, 0, sizeof(Ncb));
  Ncb.ncb_command  = NCBRESET;
  Ncb.ncb_lana_num = nAdapterNum;

  if (Netbios(&Ncb) != NRC_GOODRET)
    {
    return 0;
    }
  
  // Prepare to get the adapter status block 
  memset(&Ncb, 0, sizeof(Ncb));
  Ncb.ncb_command  = NCBASTAT;
  Ncb.ncb_lana_num = nAdapterNum;
  strcpy((char *)Ncb.ncb_callname, "*");

  memset(&Adapter, 0, sizeof(Adapter));
  Ncb.ncb_buffer = (unsigned char *)&Adapter;
  Ncb.ncb_length = sizeof(Adapter);
  
  // Get the adapter's info and, if this works, return it in standard,
  // colon-delimited form.
  if (Netbios(&Ncb) == 0)
    {
    sprintf(sMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
            int (Adapter.adapt.adapter_address[0]),
            int (Adapter.adapt.adapter_address[1]),
            int (Adapter.adapt.adapter_address[2]),
            int (Adapter.adapt.adapter_address[3]),
            int (Adapter.adapt.adapter_address[4]),
            int (Adapter.adapt.adapter_address[5]));
    return 1;
    }
  else
    {
    printf("Error %d on ASTAT.\n");
    return 0;
    }
  }
