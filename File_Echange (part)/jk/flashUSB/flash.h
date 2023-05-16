/* file flash.h */

//
// DLL Functions
//
#define XX extern "C" __declspec(dllexport) int __stdcall

XX Connect(int nComPort=1, int nDisableDialogBoxes=0, int nECprotocol=0, int nBaudRateIndex=0);
XX ConnectUSB(std::string sSerialNum="", int nECprotocol=0, int nPowerTarget=0, int nDisableDialogBoxes=0);
XX Download(char* sDownloadFile, int nDeviceErase=0, int nDisableDialogBoxes=0, int nDownloadScratchPadSFLE = 0, int nBankSelect = -1, int nLockFlash=0);
XX Disconnect(int nComPort=1);
XX DisconnectUSB();
XX GetRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
XX SetRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
XX GetXRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
XX SetXRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
XX GetCodeMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
XX SetCodeMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength,
                           BOOL bDisableDialogs=0);

extern "C" __declspec(dllexport) BOOL __stdcall Connected();
XX SetTargetGo();                         
extern "C" __declspec(dllexport) BOOL __stdcall SetTargetHalt();

XX FLASHErase(int nComPort=1, int nDisableDialogBoxes=0, int nECprotocol=0);
XX FLASHEraseUSB(std::string sSerialNum, int nDisableDialogBoxes=0, int nECprotocol=0);

XX SetJTAGDeviceAndConnect(int nComPort=1, int nDisableDialogBoxes=0, 
                        BYTE DevicesBeforeTarget=0, BYTE DevicesAfterTarget=0,
                        WORD IRBitsBeforeTarget=0, WORD IRBitsAfterTarget=0);
XX SetJTAGDeviceAndConnectUSB(std::string sSerialNum, int nPowerTarget=0, int nDisableDialogBoxes=0, 
                        BYTE DevicesBeforeTarget=0, BYTE DevicesAfterTarget=0,
                        WORD IRBitsBeforeTarget=0, WORD IRBitsAfterTarget=0);

XX ISupportBanking(int * nSupportedBanks);
XX GetSAFirmwareVersion();
extern "C" __declspec(dllexport) char* __stdcall GetDLLVersion();

XX USBDebugDevices(DWORD *dwDevices);
XX GetUSBDeviceSN(DWORD dwDeviceNum, std::string *psSerialNum);
XX GetUSBDLLVersion(std::string *pVersionString);
XX GetUSBFirmwareVersion();

