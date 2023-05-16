// Visual C++ 5.0: cl -GX getmac-netbios.cpp netapi32.lib
// Borland C++ 5.0: bcc32 getmac-netbios.cpp

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <strstream>
#include <string>

using namespace std;

bool GetAdapterInfo(int nAdapterNum, string & sMAC)
{
    // Reset the LAN adapter so that we can begin querying it 
    NCB Ncb;
    memset(&Ncb, 0, sizeof(Ncb));
    Ncb.ncb_command = NCBRESET;
    Ncb.ncb_lana_num = nAdapterNum;
    if (Netbios(&Ncb) != NRC_GOODRET) {
        char acTemp[80];
        ostrstream outs(acTemp, sizeof(acTemp));
        outs << "error " << Ncb.ncb_retcode << " on reset" << ends;
        sMAC = acTemp;
        return false;
    }
    
    // Prepare to get the adapter status block 
    memset(&Ncb, 0, sizeof(Ncb));
    Ncb.ncb_command = NCBASTAT;
    Ncb.ncb_lana_num = nAdapterNum;
    strcpy((char *) Ncb.ncb_callname, "*");
    struct ASTAT {
        ADAPTER_STATUS adapt;
        NAME_BUFFER NameBuff[30];
    } Adapter;
    memset(&Adapter, 0, sizeof(Adapter));
    Ncb.ncb_buffer = (unsigned char *)&Adapter;
    Ncb.ncb_length = sizeof(Adapter);
    
    // Get the adapter's info and, if this works, return it in standard,
    // colon-delimited form.
    if (Netbios(&Ncb) == 0) {
        char acMAC[18];
        sprintf(acMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
                int (Adapter.adapt.adapter_address[0]),
                int (Adapter.adapt.adapter_address[1]),
                int (Adapter.adapt.adapter_address[2]),
                int (Adapter.adapt.adapter_address[3]),
                int (Adapter.adapt.adapter_address[4]),
                int (Adapter.adapt.adapter_address[5]));
        sMAC = acMAC;
        return true;
    }
    else {
        char acTemp[80];
        ostrstream outs(acTemp, sizeof(acTemp));
        outs << "error " << Ncb.ncb_retcode << " on ASTAT" << ends;
        sMAC = acTemp;
        return false;
    }
}

int main()
{
    // Get adapter list
    LANA_ENUM AdapterList;
    NCB Ncb;
    memset(&Ncb, 0, sizeof(NCB));
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (unsigned char *)&AdapterList;
    Ncb.ncb_length = sizeof(AdapterList);
    Netbios(&Ncb);

    // Get all of the local ethernet addresses
    string sMAC;
    for (int i = 0; i < AdapterList.length; ++i) {
        if (GetAdapterInfo(AdapterList.lana[i], sMAC)) {
            cout << "Adapter " << int (AdapterList.lana[i]) <<
                    "'s MAC is " << sMAC << endl;
        }
        else {
            cerr << "Failed to get MAC address! Do you" << endl;
            cerr << "have the NetBIOS protocol installed?" << endl;
            break;
        }
    }

    return 0;
}

  printf("Adapter %d has MAC: %s\n", AdapterList.lana[i], tmp);
