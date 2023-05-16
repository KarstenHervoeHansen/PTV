#include <snmp.h>
#include <conio.h>
#include <stdio.h>

typedef BOOL(WINAPI * pSnmpExtensionInit) (
        IN DWORD dwTimeZeroReference,
        OUT HANDLE * hPollForTrapEvent,
        OUT AsnObjectIdentifier * supportedView);

typedef BOOL(WINAPI * pSnmpExtensionTrap) (
        OUT AsnObjectIdentifier * enterprise,
        OUT AsnInteger * genericTrap,
        OUT AsnInteger * specificTrap,
        OUT AsnTimeticks * timeStamp,
        OUT RFC1157VarBindList * variableBindings);

typedef BOOL(WINAPI * pSnmpExtensionQuery) (
        IN BYTE requestType,
        IN OUT RFC1157VarBindList * variableBindings,
        OUT AsnInteger * errorStatus,
        OUT AsnInteger * errorIndex);

typedef BOOL(WINAPI * pSnmpExtensionInitEx) (
        OUT AsnObjectIdentifier * supportedView);
    
void main()
  {
  WSADATA WinsockData;

    HINSTANCE m_hInst;
    pSnmpExtensionInit m_Init;
    pSnmpExtensionInitEx m_InitEx;
    pSnmpExtensionQuery m_Query;
    pSnmpExtensionTrap m_Trap;
    HANDLE PollForTrapEvent;
    AsnObjectIdentifier SupportedView;
    UINT OID_ifEntryType[] = {
        1, 3, 6, 1, 2, 1, 2, 2, 1, 3
    };
    UINT OID_ifEntryNum[] = {
        1, 3, 6, 1, 2, 1, 2, 1
    };
    UINT OID_ipMACEntAddr[] = {
        1, 3, 6, 1, 2, 1, 2, 2, 1, 6
    };                          //, 1 ,6 };
    AsnObjectIdentifier MIB_ifMACEntAddr =
        { sizeof(OID_ipMACEntAddr) / sizeof(UINT), OID_ipMACEntAddr };
    AsnObjectIdentifier MIB_ifEntryType = {
        sizeof(OID_ifEntryType) / sizeof(UINT), OID_ifEntryType
    };
    AsnObjectIdentifier MIB_ifEntryNum = {
        sizeof(OID_ifEntryNum) / sizeof(UINT), OID_ifEntryNum
    };
    RFC1157VarBindList varBindList;
    RFC1157VarBind varBind[2];
    AsnInteger errorStatus;
    AsnInteger errorIndex;
    AsnObjectIdentifier MIB_NULL = {
        0, 0
    };
    int ret;
    int dtmp;
    int i = 0, j = 0;
    BOOL found = FALSE;
    char TempEthernet[13];
    m_Init = NULL;
    m_InitEx = NULL;
    m_Query = NULL;
    m_Trap = NULL;


  if (WSAStartup(MAKEWORD(2, 0), &WinsockData) != 0)
    {
    fprintf(stderr, "This program requires Winsock 2.x!\n");
    return;
    }


    /* Load the SNMP dll and get the addresses of the functions
       necessary */
    m_hInst = LoadLibrary("inetmib1.dll");
    if (m_hInst < (HINSTANCE) HINSTANCE_ERROR) {
        m_hInst = NULL;
        return;
    }
    m_Init =
        (pSnmpExtensionInit) GetProcAddress(m_hInst, "SnmpExtensionInit");
    m_InitEx =
        (pSnmpExtensionInitEx) GetProcAddress(m_hInst,
                                              "SnmpExtensionInitEx");
    m_Query =
        (pSnmpExtensionQuery) GetProcAddress(m_hInst,
                                             "SnmpExtensionQuery");
    m_Trap =
        (pSnmpExtensionTrap) GetProcAddress(m_hInst, "SnmpExtensionTrap");
    m_Init(GetTickCount(), &PollForTrapEvent, &SupportedView);

    /* Initialize the variable list to be retrieved by m_Query */
    varBindList.list = varBind;
    varBind[0].name = MIB_NULL;
    varBind[1].name = MIB_NULL;

    /* Copy in the OID to find the number of entries in the
       Inteface table */
    varBindList.len = 1;        /* Only retrieving one item */
    SNMP_oidcpy(&varBind[0].name, &MIB_ifEntryNum);
    ret =
        m_Query(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus,
                &errorIndex);
    printf("# of adapters in this system : %i\n",
           varBind[0].value.asnValue.number); varBindList.len = 2;

    /* Copy in the OID of ifType, the type of interface */
    SNMP_oidcpy(&varBind[0].name, &MIB_ifEntryType);

    /* Copy in the OID of ifPhysAddress, the address */
    SNMP_oidcpy(&varBind[1].name, &MIB_ifMACEntAddr);

    do {

        /* Submit the query.  Responses will be loaded into varBindList.
           We can expect this call to succeed a # of times corresponding
           to the # of adapters reported to be in the system */
        ret =
            m_Query(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus,
                    &errorIndex); if (!ret) ret = 1;

        else
            /* Confirm that the proper type has been returned */
            ret =
                SNMP_oidncmp(&varBind[0].name, &MIB_ifEntryType,
                             MIB_ifEntryType.idLength); if (!ret) {
            j++;
            dtmp = varBind[0].value.asnValue.number;
            printf("Interface #%i type : %i\n", j, dtmp);

            /* Type 6 describes ethernet interfaces */
            if (dtmp == 6) {

                /* Confirm that we have an address here */
                ret =
                    SNMP_oidncmp(&varBind[1].name, &MIB_ifMACEntAddr,
                                 MIB_ifMACEntAddr.idLength);
                if ((!ret)
                    && (varBind[1].value.asnValue.address.stream != NULL)) {
                    if (
                        (varBind[1].value.asnValue.address.stream[0] ==
                         0x44)
                        && (varBind[1].value.asnValue.address.stream[1] ==
                            0x45)
                        && (varBind[1].value.asnValue.address.stream[2] ==
                            0x53)
                        && (varBind[1].value.asnValue.address.stream[3] ==
                            0x54)
                        && (varBind[1].value.asnValue.address.stream[4] ==
                            0x00)) {

                        /* Ignore all dial-up networking adapters */
                        printf("Interface #%i is a DUN adapter\n", j);
                        continue;
                    }
                    if (
                        (varBind[1].value.asnValue.address.stream[0] ==
                         0x00)
                        && (varBind[1].value.asnValue.address.stream[1] ==
                            0x00)
                        && (varBind[1].value.asnValue.address.stream[2] ==
                            0x00)
                        && (varBind[1].value.asnValue.address.stream[3] ==
                            0x00)
                        && (varBind[1].value.asnValue.address.stream[4] ==
                            0x00)
                        && (varBind[1].value.asnValue.address.stream[5] ==
                            0x00)) {

                        /* Ignore NULL addresses returned by other network
                           interfaces */
                        printf("Interface #%i is a NULL address\n", j);
                        continue;
                    }
                    sprintf(TempEthernet, "%02x%02x%02x%02x%02x%02x",
                            varBind[1].value.asnValue.address.stream[0],
                            varBind[1].value.asnValue.address.stream[1],
                            varBind[1].value.asnValue.address.stream[2],
                            varBind[1].value.asnValue.address.stream[3],
                            varBind[1].value.asnValue.address.stream[4],
                            varBind[1].value.asnValue.address.stream[5]);
                    printf("MAC Address of interface #%i: %s\n", j,
                           TempEthernet);}
            }
        }
    } while (!ret);         /* Stop only on an error.  An error will occur
                               when we go exhaust the list of interfaces to
                               be examined */
    getch();

    /* Free the bindings */
    SNMP_FreeVarBind(&varBind[0]);
    SNMP_FreeVarBind(&varBind[1]);
}


snmpapi.cpp

/******************************************************************
*
*      Copyright (C) Stas Khirman 1998.  All rights reserved.         
*                                                                     
*       This program is distributed WITHOUT ANY WARRANTY
*
*******************************************************************/

/*************************************************
*     
*       Reproduction of SNMP.LIB and SNMPAPI.LIB base
*           functions
*
* Author: Stas Khirman (staskh@rocketmail.com)
*
*
* Free software: no warranty; use anywhere is ok; spread the      
* sources; note any modifications; share variations and           
* derivatives (including sending to staskh@rocketmail.com).       
*                                                                     
*
*************************************************/
#include <snmp.h>

SNMPAPI
SNMP_FUNC_TYPE
SnmpUtilOidCpy(
    OUT AsnObjectIdentifier *DstObjId,
    IN  AsnObjectIdentifier *SrcObjId
    )
{
  DstObjId->ids = (UINT *)GlobalAlloc(GMEM_ZEROINIT,SrcObjId->idLength * 
          sizeof(UINT));
  if(!DstObjId->ids){
    SetLastError(1);
    return 0;
  }

  memcpy(DstObjId->ids,SrcObjId->ids,SrcObjId->idLength*sizeof(UINT));
  DstObjId->idLength = SrcObjId->idLength;

  return 1;
}


VOID
SNMP_FUNC_TYPE
SnmpUtilOidFree(
    IN OUT AsnObjectIdentifier *ObjId
    )
{
  GlobalFree(ObjId->ids);
  ObjId->ids = 0;
  ObjId->idLength = 0;
}

SNMPAPI
SNMP_FUNC_TYPE
SnmpUtilOidNCmp(
    IN AsnObjectIdentifier *ObjIdA,
    IN AsnObjectIdentifier *ObjIdB,
    IN UINT                 Len
    )
{
  UINT CmpLen;
  UINT i;
  int  res;

  CmpLen = Len;
  if(ObjIdA->idLength < CmpLen)
    CmpLen = ObjIdA->idLength;
  if(ObjIdB->idLength < CmpLen)
    CmpLen = ObjIdB->idLength;

  for(i=0;i<CmpLen;i++){
    res = ObjIdA->ids[i] - ObjIdB->ids[i];
    if(res!=0)
      return res;
  }
  return 0;
}

VOID
SNMP_FUNC_TYPE
SnmpUtilVarBindFree(
    IN OUT RFC1157VarBind *VarBind
    )
{
  BYTE asnType;
  // free object name
  SnmpUtilOidFree(&VarBind->name);

  asnType = VarBind->value.asnType;

  if(asnType==ASN_OBJECTIDENTIFIER){
    SnmpUtilOidFree(&VarBind->value.asnValue.object);
  }
  else if(
        (asnType==ASN_OCTETSTRING) ||
        (asnType==ASN_RFC1155_IPADDRESS) ||
        (asnType==ASN_RFC1155_OPAQUE) ||
        (asnType==ASN_SEQUENCE)){
    if(VarBind->value.asnValue.string.dynamic){
      GlobalFree(VarBind->value.asnValue.string.stream);
    }
  }

  VarBind->value.asnType = ASN_NULL;

}


