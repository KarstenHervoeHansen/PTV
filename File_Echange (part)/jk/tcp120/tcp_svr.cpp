/* TCP echo server to be run against Silicon Laboratories ETHERNET-DK in TCP echo client mode.

  compile with: cl tcp_svr.cpp wsedesc.cpp wsock32.lib

  PJB, 11/21/01, 12/14/01
*/
#include <stdio.h>
#include <winsock.h>
#include <io.h>

#if 1
// Ports
#define DEFAULT_PORT       1500
#define ECHO_PORT          7
#define HTTP_PORT          80
#define SMTP_PORT          25
#define FTP_DATA_PORT      20
#define FTP_CONTROL_PORT   21
#define BOOTP_SERVER_PORT  67
#define BOOTP_CLIENT_PORT  68
#define DHCP_SERVER_PORT   67
#define DHCP_CLIENT_PORT   68
#define TFTP_SERVER_PORT   69
#define DNS_SERVER_PORT    53
#define DNS_CLIENT_PORT    1027
#endif


// Function prototype
int StreamServer(short nPort);

// Helper macro for displaying errors
LPCSTR WSErrorDescription(int);
#define PRINTERROR(s) \
    fprintf(stderr,"\n%s: %s\n", s, WSErrorDescription(WSAGetLastError()))

////////////////////////////////////////////////////////////

void main(int argc, char **argv)
  {
  WORD    wVersionRequested = MAKEWORD(1,1);
  WSADATA wsaData;
  int     nRet;
  short   nPort;
  
  nPort = 7;    // well known echo port
  
  // Initialize WinSock and check version
  //
  nRet = WSAStartup(wVersionRequested, &wsaData);
  if (nRet != 0 || wsaData.wVersion != wVersionRequested)
    { 
    fprintf(stderr,"\nError: wrong WinSock version\n");
    return;
    }
  

  //
  // Do the stuff a stream server does
  //
  do
    {
    nRet = StreamServer(nPort);
    } while (nRet == 0);
  
  //
  // Release WinSock
  //
  WSACleanup();
  }

////////////////////////////////////////////////////////////

int StreamServer(short nPort)
  {
  //
  // Create a TCP/IP stream socket to "listen" with
  //
  SOCKET  listenSocket;
  
  listenSocket = socket(AF_INET,          // Address family   ////////////////////////////////////////////////////
                        SOCK_STREAM,      // Socket type
                        IPPROTO_TCP);     // Protocol
  if (listenSocket == INVALID_SOCKET)
    {
    PRINTERROR("socket()");
    return (SOCKET_ERROR);
    }
  
  
  //
  // Fill in the address structure
  //
  SOCKADDR_IN saServer;   
  
  saServer.sin_family      = AF_INET;
  saServer.sin_addr.s_addr = INADDR_ANY;     // Let WinSock supply address
  saServer.sin_port        = htons(nPort);   // Use port from command line
  
  //
  // bind the name to the socket
  //
  int nRet;

  nRet = bind(listenSocket,             // Socket            ////////////////////////////////////////////////////
              (LPSOCKADDR)&saServer,    // Our address
              sizeof(struct sockaddr)); // Size of address structure
  if (nRet == SOCKET_ERROR)
    {
    PRINTERROR("bind()");
    closesocket(listenSocket);
    return (nRet);
    }

  //
  // print out where the server is waiting
  //
  int nLen;
  nLen = sizeof(SOCKADDR);
  char szBuf[1500];

  nRet = gethostname(szBuf, sizeof(szBuf));                ////////////////////////////////////////////////////
  printf("Local host name = %s\n", szBuf);
  if (nRet == SOCKET_ERROR)
    {
    PRINTERROR("gethostname()");
    closesocket(listenSocket);
    return (nRet);
    }

  //
  // Show the server name and port number
  //
  printf("\nTCP Echo Server %s is waiting...\n",szBuf);

  //
  // Set the socket to listen
  //

  nRet = listen(listenSocket,         // Bound socket    ////////////////////////////////////////////////////
                SOMAXCONN);           // Number of connection request queue
  if (nRet == SOCKET_ERROR)
    {
    PRINTERROR("listen()");
    closesocket(listenSocket);
    return (nRet);
    }

  //
  // Wait for an incoming request
  //
  SOCKET  remoteSocket;
  printf("\nListening .. on remote socket..\n");

  remoteSocket = accept(listenSocket,     // Listening socket   ////////////////////////////////////////////////////
                        NULL,             // Optional client address
                        NULL);
  if (remoteSocket == INVALID_SOCKET)
    {
    PRINTERROR("accept()");
    closesocket(listenSocket);
    return (SOCKET_ERROR);
    }
  printf("\nAccepted..\n");

  //
  // We're connected to a client
  // New socket descriptor returned already
  // has clients address

  long lPacket = 0L;

  printf("\nTCPWainting for daata ...\n");

  //
  // Receive data from the client
  //
  while (1)
    {
    memset(szBuf, 0, sizeof(szBuf));
    nRet = recv(remoteSocket,           // Connected client   ////////////////////////////////////////////////////
                szBuf,                  // Receive buffer
                sizeof(szBuf),          // Lenght of buffer
                0);                     // Flags
    if (nRet == SOCKET_ERROR)
      {
      PRINTERROR("recv()");
      break;
      }
    else if (nRet == 0)
      {
      printf("Connection closed\n");
      break;
      }

    //
    // Display received data
    //
    ++lPacket;
    printf("\nReceived #%ld: %s", lPacket, szBuf);
    sprintf(szBuf, "SVRDat#%ld", lPacket);
    printf("     len of SVR... is %d", strlen(szBuf));
    //
    // Send data back to the client
    //
    nRet = send(remoteSocket,         // Connected socket   ////////////////////////////////////////////////////
                szBuf,                // Data buffer
                strlen(szBuf),        // Lenght of data
                0);                   // Flags
    if (nRet == SOCKET_ERROR)
      {
      PRINTERROR("send()");
      break;
      }
   if (lPacket == 15)
    break;
   }

  //
  // Close BOTH sockets before exiting
  //
  closesocket(remoteSocket);
  closesocket(listenSocket);
  return (nRet);
  }
