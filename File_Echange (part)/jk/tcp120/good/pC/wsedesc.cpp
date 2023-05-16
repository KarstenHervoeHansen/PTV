//
// WSEDesc.c -- WinSock error descriptions
//

#include <winsock.h>

typedef struct tagWSAEDESCRIPTION
{
        int iErrorCode;
        char *szDescription;
}WSAEDESCRIPTION;

WSAEDESCRIPTION ErrorDescriptions[] = {
  WSAEINTR,
  "WSAEINTR: Interrupted function call",
  WSAEBADF,
  "WSAEBADF: Bad file number",
  WSAEACCES,
  "WSAEACCES: Permission denied",
  WSAEFAULT,
  "WSAEFAULT: Address fault",
  WSAEINVAL,
  "WSAEINVAL: Invalid argument",
  WSAEMFILE,
  "WSAEMFILE: Too many open files",
  WSAEWOULDBLOCK,
  "WSAEWOULDBLOCK: Resource temporarily unavailable (Would block)",
  WSAEINPROGRESS,
  "WSAEINPROGRESS: Operation now in progress",
  WSAEALREADY,
  "WSAEALREADY: Operation already in progress",
  WSAENOTSOCK,
  "WSAENOTSOCK: Socket operation on non-socket",
  WSAEDESTADDRREQ,
  "WSAEDESTADDRREQ: Destination address required",
  WSAEMSGSIZE,
  "WSAEMSGSIZE: Message too long",
  WSAEPROTOTYPE,
  "WSAEPROTOTYPE: Protocol wrong type for socket",
  WSAENOPROTOOPT,
  "WSAENOPROTOOPT: Bad protocol option",
  WSAEPROTONOSUPPORT,
  "WSAEPROTONOSUPPORT: Protocol not supported",
  WSAESOCKTNOSUPPORT,
  "WSAESOCKTNOSUPPORT: Socket type not supported",
  WSAEOPNOTSUPP,
  "WSAEOPNOTSUPP: Operation not supported",
  WSAEPFNOSUPPORT,
  "WSAEPFNOSUPPORT: Protocol family not supported",
  WSAEAFNOSUPPORT,
  "WSAEAFNOSUPPORT: Address family not supported by protocol family",
  WSAEADDRINUSE,
  "WSAEADDRINUSE: Address already in use",
  WSAEADDRNOTAVAIL,
  "WSAEADDRNOTAVAIL: Cannot assign requested address",
  WSAENETDOWN,
  "WSAENETDOWN: Network is down",
  WSAENETUNREACH,
  "WSAENETUNREACH: Network is unreachable",
  WSAENETRESET,
  "WSAENETRESET: Network dropped connection on reset",
  WSAECONNABORTED,
  "WSAECONNABORTED: Software caused connection abort",
  WSAECONNRESET,
  "WSAECONNRESET: Connection reset by peer",
  WSAENOBUFS,
  "WSAENOBUFS: No buffer space available",
  WSAEISCONN,
  "WSAEISCONN: Socket is already connected",
  WSAENOTCONN,
  "WSAENOTCONN: Socket is not connected",
  WSAESHUTDOWN,
  "WSAESHUTDOWN: Cannot send after socket shutdown",
  WSAETOOMANYREFS,
  "WSAETOOMANYREFS: Too many references",
  WSAETIMEDOUT,
  "WSAETIMEDOUT: Connection timed out",
  WSAECONNREFUSED,
  "WSAECONNREFUSED: Connection refused",
  WSAELOOP,
  "WSAELOOP: Loop",
  WSAENAMETOOLONG,
  "WSAENAMETOOLONG: Name too long",
  WSAEHOSTDOWN,
  "WSAEHOSTDOWN: Host is down",
  WSAEHOSTUNREACH,
  "WSAEHOSTUNREACH: No route to host",
  WSAENOTEMPTY,
  "WSAENOTEMPTY: Directory not empty",
  WSAEPROCLIM,
  "WSAEPROCLIM: Too many processes",
  WSAEUSERS,
  "WSAEUSERS: Too many users",
  WSAEDQUOT,
  "WSAEDQUOT: Disk quota",
  WSAESTALE,
  "WSAESTALE: Stale",
  WSAEREMOTE,
  "WSAEREMOTE: Remote",
  WSASYSNOTREADY,
  "WSASYSNOTREADY: Network subsystem is unavailable",
  WSAVERNOTSUPPORTED,
  "WSAVERNOTSUPPORTED: WINSOCK.DLL version out of range",
  WSANOTINITIALISED,
  "WSANOTINITIALISED: Successful WSAStartup() not yet performed",
  WSAEDISCON,
  "WSAEDISCON: Graceful shutdown in progress",
  WSAHOST_NOT_FOUND,
  "WSAHOST_NOT_FOUND: Host not found",
  WSATRY_AGAIN,
  "WSATRY_AGAIN: Non-authoritative host not found",
  WSANO_RECOVERY,
  "WSANO_RECOVERY: Non-recoverable error",
  WSANO_DATA,
  "WSANO_DATA: Valid name, no data record of requested type",
#ifdef _WINSOCK2API_
  WSAENOMORE,
  "WSAENOMORE: ",
  WSAECANCELLED,
  "WSAECANCELLED: Cancelled",
  WSAEINVALIDPROCTABLE,
  "WSAEINVALIDPROCTABLE: Invalid procedure table from service provider",
  WSAEINVALIDPROVIDER,
  "WSAEINVALIDPROVIDER: Invalid service provider version number",
  WSAEPROVIDERFAILEDINIT,
  "WSAEPROVIDERFAILEDINIT: Unable to initialize a service provider",
  WSASYSCALLFAILURE,
  "WSASYSCALLFAILURE: System call failure",
  WSASERVICE_NOT_FOUND,
  "WSASERVICE_NOT_FOUND: Service not found",
  WSATYPE_NOT_FOUND,
  "WSATYPE_NOT_FOUND: Type not found",
  WSA_E_NO_MORE,
  "WSA_E_NO_MORE: ",
  WSA_E_CANCELLED,
  "WSA_E_CANCELLED: ",
  WSAEREFUSED,
  "WSAEREFUSED: Refused",
#endif

   };

//
// WSErrorDescription()
//
LPCSTR WSErrorDescription(int iErrorCode)
{
        #define NUMELEMENTS sizeof(ErrorDescriptions) / sizeof(ErrorDescriptions[0])
        int iNdx;

        for (iNdx = 0; iNdx < NUMELEMENTS; iNdx++)
        {
                if (ErrorDescriptions[iNdx].iErrorCode == iErrorCode)
                        return(ErrorDescriptions[iNdx].szDescription);
        }
        return("Unknown error");
}
