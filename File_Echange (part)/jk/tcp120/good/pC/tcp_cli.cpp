/* TCP echo client to be run against Silicon Laboratories ETHERNET-DK in TCP echo server mode.

  compile with: cl tcp_cli.cpp wsedesc.cpp wsock32.lib DHCPOFFERs


   Usage: tcp_cli                            ( defaults to 216.233.5.26, 7 )
          tcp_cli [dest ip addr]             ( defaults to port 7 )
          tcp_cli [dest ip addr] [dest port]

  PJB, 11/20/01, 10/24/02
*/

#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include <time.h>
#include "dhcp.h"

struct ASTAT
  {
  ADAPTER_STATUS adapt;
  NAME_BUFFER    NameBuff[30];
  } Adapter;

unsigned char client_hardware_address[MAX_DHCP_CHADDR_LENGTH] = "";

char network_interface_name[8] = "3Com Et"; 

unsigned long packet_xid = 0;

unsigned long dhcp_lease_time     = 0;
unsigned long dhcp_renewal_time   = 0;
unsigned long dhcp_rebinding_time = 0;

int dhcpoffer_timeout = 2;

dhcp_offer       *dhcp_offer_list       = NULL;
requested_server *requested_server_list = NULL;

int valid_responses     = 0;     /* number of valid DHCPOFFERs we received */
int requested_servers   = 0;   
int requested_responses = 0;

int            request_specific_address   = FALSE;
int            received_requested_address = FALSE;
struct in_addr requested_address;


#define DEBUG
// Function prototype
void StreamClient(long lIP_addr, short nPort);
int create_dhcp_socket(void);
int GetAdapterInfo(int nAdapterNum, char * sMAC);
int send_dhcp_discover(int sock);
int send_dhcp_packet(void *buffer, int buffer_size, int sock, struct sockaddr_in *dest);
int get_dhcp_offer(int sock);
int receive_dhcp_packet(void *buffer, int buffer_size, int sock, int timeout, struct sockaddr_in *address);
int add_dhcp_offer(struct in_addr source,dhcp_packet *offer_packet);
int close_dhcp_socket(int sock);
int free_dhcp_offer_list(void);
int free_requested_server_list(void);
int get_results(void);

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

	int dhcp_socket;

  //
  // Initialize WinSock and check the version
  //
  nRet = WSAStartup(wVersionRequested, &wsaData);
  if (nRet != 0 || wsaData.wVersion != wVersionRequested)
    { 
    fprintf(stderr,"\nError: wrong WinSock version \n");
    return;
    }

  // Get adapter list
  memset(&Ncb, 0, sizeof(NCB));
  Ncb.ncb_command = NCBENUM;
  Ncb.ncb_buffer  = (unsigned char *)&AdapterList;
  Ncb.ncb_length  = sizeof(AdapterList);
  Netbios(&Ncb);

  //printf("Found %d adapters\n", AdapterList.length);
  //printf("lana = %s\n", AdapterList.lana);
  // Get all of the local ethernet addresses
  for (i = 0; i < AdapterList.length; ++i)
    {
    if (GetAdapterInfo(AdapterList.lana[i], tmp))
      {
      printf("Adapter %d has MAC: %s\n", AdapterList.lana[i], tmp);
      }
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
    {
    printf("Remote server address: %s\n", argv[1]);
    strcpy(ip_addr, argv[1]);
    }
  else
    {
    strcpy(ip_addr,"216.233.5.26");
    printf("Remote server address: %s\n", ip_addr);
    }
  lIP_addr = inet_addr(ip_addr);
  if (lIP_addr == INADDR_NONE)
    {
    printf("Bad IP address!");
    return;
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
  char               buffer[50];
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
	//strncpy(interface.ifr_ifrn.ifrn_name, network_interface_name, IFNAMSIZ);
	strncpy(buffer, network_interface_name, IFNAMSIZ);
	if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&buffer[0], sizeof(buffer)) < 0)
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
  
/*****************************************************************************/
/* NAME       int GetAdapterInfo(int nAdapterNum, char * sMAC)               */
/*                                                                           */
/* PURPOSE    To get MAC address of a NIC                                    */
/*                                                                           */
/* ARGUMENTS  int nAdapterNum - Number of the adapter                        */
/*            char * sMAC     - Buffer to hold acquired MAC address          */
/*                                                                           */
/* RETURNS    1 if OK or 0 on ERROR                                          */
/*                                                                           */
/* EXTERNS    None                                                           */
/*                                                                           */
/* NOTES:     MAC address is also copied to client_hardware_address[]        */
/*****************************************************************************/
int GetAdapterInfo(int nAdapterNum, char * sMAC)
  {
  // Reset the LAN adapter so that we can begin querying it 
  NCB Ncb;                  /* Network Control Block from NetBIOS 3.0 (NB30.h)*/

  memset(&Ncb, 0, sizeof(Ncb));
  Ncb.ncb_command  = NCBRESET;
  Ncb.ncb_lana_num = nAdapterNum;

  if (Netbios(&Ncb) != NRC_GOODRET) //#define NRC_GOODRET 0x00 good return
    {
    return 0;
    }
  
  // Prepare to get the adapter status block 
#if 0
struct ASTAT
  {
  ADAPTER_STATUS adapt;
  NAME_BUFFER    NameBuff[30];
  } Adapter;

typedef struct _NAME_BUFFER
  {
  UCHAR   name[NCBNAMSZ];
  UCHAR   name_num;
  UCHAR   name_flags;
  } NAME_BUFFER, *PNAME_BUFFER;

#endif
/*
 *  Structure returned to the NCB command NCBASTAT is ADAPTER_STATUS followed
 *  by an array of NAME_BUFFER structures.
 */

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
	 memcpy(&client_hardware_address[0], &Adapter.adapt.adapter_address, 6);

    sprintf(sMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
            int (client_hardware_address[0]),
            int (client_hardware_address[1]),
            int (client_hardware_address[2]),
            int (client_hardware_address[3]),
            int (client_hardware_address[4]),
            int (client_hardware_address[5]));
    //printf("Name: %s\n", Adapter.NameBuff[0].name);
    return 1;
    }
  else
    {
    printf("Error %d on ASTAT.\n");
    return 0;
    }
  }


/*****************************************************************************/
/* NAME       int send_dhcp_discover(int sock)                               */
/*                                                                           */
/* PURPOSE    sends a DHCPDISCOVER broadcast message in an attempt           */
/*            to find DHCP servers                                           */
/*                                                                           */
/* ARGUMENTS  int sock - socket                                              */
/*                                                                           */
/* RETURNS    OK or ERROR                                                    */
/*                                                                           */
/* EXTERNS    None                                                           */
/*                                                                           */
/* NOTES:                                                                    */
#if 0
typedef struct dhcp_packet_struct
  {
  unsigned char  op;                   /* packet type */
  unsigned char  htype;                /* type of hardware address for this machine (Ethernet, etc) */
  unsigned char  hlen;                 /* length of hardware address (of this machine) */
  unsigned char  hops;                 /* hops */
  unsigned long  xid;                  /* random transaction id number - chosen by this machine */
  unsigned int   secs;                 /* seconds used in timing */
  unsigned int   flags;                /* flags */
  struct in_addr ciaddr;               /* IP address of this machine (if we already have one) */
  struct in_addr yiaddr;               /* IP address of this machine (offered by the DHCP server) */
  struct in_addr siaddr;               /* IP address of DHCP server */
  struct in_addr giaddr;               /* IP address of DHCP relay */
  unsigned char  chaddr [MAX_DHCP_CHADDR_LENGTH];   /* hardware address of this machine */
  char           sname  [MAX_DHCP_SNAME_LENGTH];    /* name of DHCP server */
  char           file   [MAX_DHCP_FILE_LENGTH];     /* boot file name (used for diskless booting?) */
  char           options[MAX_DHCP_OPTIONS_LENGTH];  /* options */
  }dhcp_packet;
#endif
/*****************************************************************************/
int send_dhcp_discover(int sock)
  {
	dhcp_packet        discover_packet;
#if 0
struct sockaddr_in
  {
  short   sin_family;
  u_short sin_port;
  struct  in_addr sin_addr;
  char    sin_zero[8];
  };
#endif
	struct sockaddr_in sockaddr_broadcast;

	/* clear the packet data structure */
  memset(&discover_packet, 0, sizeof(discover_packet));

	/* boot request flag (backward compatible with BOOTP servers) */
	discover_packet.op = BOOTREQUEST;

	/* hardware address type */
	discover_packet.htype = ETHERNET_HARDWARE_ADDRESS;

	/* length of our hardware address */
	discover_packet.hlen = ETHERNET_HARDWARE_ADDRESS_LENGTH;

	discover_packet.hops = 0;

	/* transaction id is supposed to be random */
	srand(time(NULL));
	packet_xid = rand();
	discover_packet.xid = htonl(packet_xid);

	/**** WHAT THE HECK IS UP WITH THIS?!?  IF I DON'T MAKE THIS CALL, ONLY ONE SERVER RESPONSE IS PROCESSED!!!! ****/
	/* downright bizzarre... */

	ntohl(discover_packet.xid);

	/*discover_packet.secs=htons(65535);*/
	discover_packet.secs = 0xFF;

	/* tell server it should broadcast its response */ 
	discover_packet.flags = htons(DHCP_BROADCAST_FLAG);

	/* our hardware address */
	memcpy(discover_packet.chaddr, client_hardware_address, ETHERNET_HARDWARE_ADDRESS_LENGTH);
  printf("MAC to Discover: %02X:%02X:%02X:%02X:%02X:%02X\n",
          int (discover_packet.chaddr[0]),
          int (discover_packet.chaddr[1]),
          int (discover_packet.chaddr[2]),
          int (discover_packet.chaddr[3]),
          int (discover_packet.chaddr[4]),
          int (discover_packet.chaddr[5]));

	/* first four bytes of options field is magic cookie (as per RFC 2132) */
	discover_packet.options[0]='\x63';
	discover_packet.options[1]='\x82';
	discover_packet.options[2]='\x53';
	discover_packet.options[3]='\x63';

	/* DHCP message type is embedded in options field */
	discover_packet.options[4] = DHCP_OPTION_MESSAGE_TYPE;    /* DHCP message type option identifier */
	discover_packet.options[5] = '\x01';                      /* DHCP message option length in bytes */
	discover_packet.options[6] = DHCPDISCOVER;

	/* the IP address we're requesting */
	if (request_specific_address == TRUE)
    {
		discover_packet.options[7] = DHCP_OPTION_REQUESTED_ADDRESS;
		discover_packet.options[8] = '\x04';
		memcpy(&discover_packet.options[9], &requested_address, sizeof(requested_address));
    }
	
	/* send the DHCPDISCOVER packet to broadcast address */
  sockaddr_broadcast.sin_family      = AF_INET;
  sockaddr_broadcast.sin_port        = htons(DHCP_SERVER_PORT);
  sockaddr_broadcast.sin_addr.s_addr = INADDR_BROADCAST; // #define INADDR_BROADCAST (u_long)0xffffffff
	memset(&sockaddr_broadcast.sin_zero, 0, sizeof(sockaddr_broadcast.sin_zero));


	printf("DHCPDISCOVER to %s port %d\n", inet_ntoa(sockaddr_broadcast.sin_addr), ntohs(sockaddr_broadcast.sin_port));
	printf("DHCPDISCOVER XID: %lu (0x%X)\n", ntohl(discover_packet.xid), ntohl(discover_packet.xid));
	printf("IP address of this machine ciaddr:  %s\n", inet_ntoa(discover_packet.ciaddr));
	printf("IP address of this machine (offered by the DHCP server) yiaddr:  %s\n", inet_ntoa(discover_packet.yiaddr));
	printf("IP address of DHCP server siaddr:  %s\n", inet_ntoa(discover_packet.siaddr));
	printf("IP address of DHCP relay giaddr:  %s\n", inet_ntoa(discover_packet.giaddr));

	/* send the DHCPDISCOVER packet out */
	send_dhcp_packet(&discover_packet, sizeof(discover_packet), sock, &sockaddr_broadcast);

	printf("\n\n");

	return OK;
  }

/* sends a DHCP packet */
int send_dhcp_packet(void *buffer, int buffer_size, int sock, struct sockaddr_in *dest)
  {
	struct sockaddr_in myname;
	int                result;

	result = sendto(sock, (char *)buffer, buffer_size, 0, (struct sockaddr *)dest, sizeof(*dest));

#ifdef DEBUG
	printf("send_dhcp_packet result: %d\n", result);
#endif

	if (result<0)
		return ERROR;

	return OK;
  }

/* waits for a DHCPOFFER message from one or more DHCP servers  */
int get_dhcp_offer(int sock)
  {
	dhcp_packet        offer_packet;
	struct sockaddr_in source;
	int                result;
	int                responses;
	int                x;
	time_t             start_time;
	time_t             current_time;

	time(&start_time);

	/* receive as many responses as we can */
	for(responses = 0, valid_responses = 0; ;)
    {
		time(&current_time);
		if ((current_time-start_time) >= dhcpoffer_timeout)
			break;

		memset(&source, 0, sizeof(source));
		memset(&offer_packet, 0, sizeof(offer_packet));

		result = receive_dhcp_packet(&offer_packet, sizeof(offer_packet), sock, dhcpoffer_timeout, &source);
		
		if (result != OK)
      {
			printf("get_dhcp_offer():Result=ERROR\n");
			continue;
      }
		else
      {
			printf("get_dhcp_offer():Result=OK\n");
			responses++;
      }

		printf("get_dhcp_offer():DHCPOFFER from IP address %s\n", inet_ntoa(source.sin_addr));
		printf("get_dhcp_offer():DHCPOFFER XID: %lu (0x%X)\n", ntohl(offer_packet.xid), ntohl(offer_packet.xid));
    printf("get_dhcp_offer():MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            int (offer_packet.chaddr[0]),
            int (offer_packet.chaddr[1]),
            int (offer_packet.chaddr[2]),
            int (offer_packet.chaddr[3]),
            int (offer_packet.chaddr[4]),
            int (offer_packet.chaddr[5]));

		/* check packet xid to see if its the same as the one we used in the discover packet */
		if (ntohl(offer_packet.xid) != packet_xid)
      {
			printf("get_dhcp_offer():DHCPOFFER XID (%lu)  did not match DHCPDISCOVER XID (%lu) - ignoring packet\n", ntohl(offer_packet.xid),packet_xid);
			continue;
      }

		/* check hardware address */
		result = OK;
		printf("DHCPOFFER chaddr: ");
		for(x = 0; x < ETHERNET_HARDWARE_ADDRESS_LENGTH; x++)
      {
			printf("%02X", (unsigned char)offer_packet.chaddr[x]);
			if (offer_packet.chaddr[x] != client_hardware_address[x])
        {
				result=ERROR;
        }
      }
		printf("\n");
		if (result == ERROR)
      {
			printf("get_dhcp_offer():DHCPOFFER hardware address did not match our own - ignoring packet\n");
			continue;
      }

		printf("DHCPOFFER ciaddr: %s\n", inet_ntoa(offer_packet.ciaddr));
		printf("DHCPOFFER yiaddr: %s\n", inet_ntoa(offer_packet.yiaddr));
		printf("DHCPOFFER siaddr: %s\n", inet_ntoa(offer_packet.siaddr));
		printf("DHCPOFFER giaddr: %s\n", inet_ntoa(offer_packet.giaddr));

		add_dhcp_offer(source.sin_addr,&offer_packet);

		valid_responses++;
    }

	printf("Total responses seen on the wire: %d\n", responses);
	printf("Valid responses for this machine: %d\n", valid_responses);

	return OK;
  }

/* receives a DHCP packet */
int receive_dhcp_packet(void *buffer, int buffer_size, int sock, int timeout, struct sockaddr_in *address)
  {
  struct    timeval  tv;
  fd_set             readfds;
	int                recv_result;
	int                address_size;
	struct sockaddr_in source_address;


  /* wait for data to arrive (up time timeout) */
  tv.tv_sec  = timeout;
  tv.tv_usec = 0;
  FD_ZERO(&readfds);
  FD_SET(sock, &readfds);
  select(sock + 1, &readfds, NULL, NULL, &tv);

  /* make sure some data has arrived */
  if (!FD_ISSET(sock, &readfds))
    {
#ifdef DEBUG
    printf("receive_dhcp_packet:No (more) data recieved\n");
#endif
    return ERROR;
    }
  else
    {
    printf("receive_dhcp_packet:   (more) data recieved\n");
		/* why do we need to peek first?  i don't know, its a hack.  without it, the source address of the first packet received was
		   not being interpreted correctly.  sigh... */
		memset(&source_address, 0, sizeof(source_address));
#if 0
    recv_result = recvfrom(sock, (char *)buffer, buffer_size, MSG_PEEK, (struct sockaddr *)&source_address, &address_size);
#ifdef DEBUG
		printf("recv_result_1: %d\n",recv_result);
#endif
#endif
    recv_result = recvfrom(sock, (char *)buffer, buffer_size, 0, (struct sockaddr *)&source_address, &address_size);
#ifdef DEBUG
		printf("recv_result_2: %d\n",recv_result);
#endif

    if (recv_result == -1)
      {
#ifdef DEBUG
			printf("recvfrom() failed, ");
			printf("errno: (%d) -> %s\n", errno,strerror(errno));
#endif
      return ERROR;
      }
		else
      {
#ifdef DEBUG
			printf("receive_dhcp_packet() result: %d\n",recv_result);
			printf("receive_dhcp_packet() source: %s\n",inet_ntoa(source_address.sin_addr));
#endif

			memcpy(address, &source_address, sizeof(source_address));
			return OK;
      }
    }
	//return OK;
  }

/* adds a DHCP OFFER to list in memory */
int add_dhcp_offer(struct in_addr source,dhcp_packet *offer_packet)
  {
	dhcp_offer *new_offer;
	int x;
	unsigned y;
	unsigned option_type;
	unsigned option_length;

	if (offer_packet == NULL)
		return ERROR;

	/* process all DHCP options present in the packet */
	for(x = 4; x < MAX_DHCP_OPTIONS_LENGTH;)
    {
		/* end of options (0 is really just a pad, but bail out anyway) */
		if ((int)offer_packet->options[x] == -1 || (int)offer_packet->options[x] == 0)
			break;

		/* get option type */
		option_type = offer_packet->options[x++];

		/* get option length */
		option_length = offer_packet->options[x++];

#ifdef DEBUG
		printf("Option: %d (0x%02X)\n", option_type, option_length);
#endif

		/* get option data */
		if (option_type == DHCP_OPTION_LEASE_TIME)
			dhcp_lease_time = ntohl(*((unsigned long *)&offer_packet->options[x]));
		if (option_type == DHCP_OPTION_RENEWAL_TIME)
			dhcp_renewal_time = ntohl(*((unsigned long *)&offer_packet->options[x]));
		if (option_type == DHCP_OPTION_REBINDING_TIME)
			dhcp_rebinding_time = ntohl(*((unsigned long *)&offer_packet->options[x]));

		/* skip option data we're ignoring */
		else
			for(y = 0; y < option_length; y++, x++)
        ;
    }

#ifdef DEBUG
	if(dhcp_lease_time == DHCP_INFINITE_TIME)
		printf("Lease Time: Infinite\n");
	else
		printf("Lease Time: %lu seconds\n", (unsigned long)dhcp_lease_time);
	if(dhcp_renewal_time == DHCP_INFINITE_TIME)
		printf("Renewal Time: Infinite\n");
	else
		printf("Renewal Time: %lu seconds\n",(unsigned long)dhcp_renewal_time);
	if(dhcp_rebinding_time==DHCP_INFINITE_TIME)
		printf("Rebinding Time: Infinite\n");
	printf("Rebinding Time: %lu seconds\n",(unsigned long)dhcp_rebinding_time);
#endif

	new_offer=(dhcp_offer *)malloc(sizeof(dhcp_offer));

	if(new_offer == NULL)
		return ERROR;


	new_offer->server_address  = source;
	new_offer->offered_address = offer_packet->yiaddr;
	new_offer->lease_time      = dhcp_lease_time;
	new_offer->renewal_time    = dhcp_renewal_time;
	new_offer->rebinding_time  = dhcp_rebinding_time;


#ifdef DEBUG
	printf("Added offer from server @ %s", inet_ntoa(new_offer->server_address));
	printf(" of IP address %s\n", inet_ntoa(new_offer->offered_address));
#endif

	/* add new offer to head of list */
	new_offer->next = dhcp_offer_list;
	dhcp_offer_list = new_offer;

	return OK;
  }

/* closes DHCP socket */
int close_dhcp_socket(int sock)
  {
	closesocket(sock);

	return OK;
  }

/* frees memory allocated to DHCP OFFER list */
int free_dhcp_offer_list(void)
  {
	dhcp_offer *this_offer;
	dhcp_offer *next_offer;

	for(this_offer=dhcp_offer_list;this_offer!=NULL;this_offer=next_offer)
    {
		next_offer=this_offer->next;
		free(this_offer);
    }

	return OK;
  }




/* frees memory allocated to requested server list */
int free_requested_server_list(void)
  {
	requested_server *this_server;
	requested_server *next_server;

	for(this_server=requested_server_list;this_server!=NULL;this_server=next_server)
    {
		next_server=this_server->next;
		free(this_server);
    }
	
	return OK;
  }

/* gets state and plugin output to return */
int get_results(void)
  {
	dhcp_offer       *temp_offer;
	requested_server *temp_server;
	int               result;
	unsigned long     max_lease_time = 0;

	received_requested_address = FALSE;

	/* checks responses from requested servers */
	requested_responses = 0;
	if (requested_servers > 0)
    {
		for(temp_server=requested_server_list;temp_server!=NULL;temp_server=temp_server->next)
      {
			for(temp_offer=dhcp_offer_list;temp_offer!=NULL;temp_offer=temp_offer->next)
        {
				/* get max lease time we were offered */
				if (temp_offer->lease_time>max_lease_time || temp_offer->lease_time==DHCP_INFINITE_TIME)
					max_lease_time = temp_offer->lease_time;
				
				/* see if we got the address we requested */
				if (!memcmp(&requested_address, &temp_offer->offered_address, sizeof(requested_address)))
					received_requested_address = TRUE;

				/* see if the servers we wanted a response from talked to us or not */
				if (!memcmp(&temp_offer->server_address, &temp_server->server_address, sizeof(temp_server->server_address)))
          {
#ifdef DEBUG
					printf("DHCP Server Match: Offerer=%s", inet_ntoa(temp_offer->server_address));
					printf(" Requested=%s\n", inet_ntoa(temp_server->server_address));
#endif				       
					requested_responses++;
	        }
        }
      }

    }
	/* else check and see if we got our requested address from any server */
	else
    {
		for(temp_offer = dhcp_offer_list; temp_offer != NULL; temp_offer = temp_offer->next)
      {
			/* get max lease time we were offered */
			if(temp_offer->lease_time > max_lease_time || temp_offer->lease_time == DHCP_INFINITE_TIME)
				max_lease_time = temp_offer->lease_time;
				
			/* see if we got the address we requested */
			if (!memcmp(&requested_address, &temp_offer->offered_address, sizeof(requested_address)))
				received_requested_address = TRUE;
      }
    }

	result = STATE_OK;
	if (valid_responses == 0)
		result = STATE_CRITICAL;
	else 
    if (requested_servers > 0 && requested_responses == 0)
		  result = STATE_CRITICAL;
	  else 
      if (requested_responses < requested_servers)
		    result = STATE_WARNING;
	    else 
        if (request_specific_address == TRUE && received_requested_address == FALSE)
		      result = STATE_WARNING;


	printf("DHCP %s: ", (result == STATE_OK) ? "ok" : "problem");

	/* we didn't receive any DHCPOFFERs */
	if (dhcp_offer_list == NULL)
    {
		printf("No DHCPOFFERs were received.\n");
		return result;
    }

	printf("Received %d DHCPOFFER(s)", valid_responses);

	if (requested_servers > 0)
		printf(", %s%d of %d requested servers responded", ((requested_responses<requested_servers) && requested_responses>0) ? "only ":"",requested_responses,requested_servers);

	if (request_specific_address == TRUE)
		printf(", requested address (%s) was %soffered", inet_ntoa(requested_address),(received_requested_address==TRUE) ? "" : "not ");

	printf(", max lease time = ");
	if(max_lease_time == DHCP_INFINITE_TIME)
		printf("Infinity");
	else
		printf("%lu sec", (unsigned long)max_lease_time);

	printf(".\n");

	return result;
  }
