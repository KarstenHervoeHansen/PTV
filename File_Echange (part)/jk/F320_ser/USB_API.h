#ifndef  _USB_API_H_
#define  _USB_API_H_

// UINT type definition
#ifndef _UINT_DEF_
#define _UINT_DEF_
typedef unsigned int UINT;
#endif  /* _UINT_DEF_ */

// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif   /* _BYTE_DEF_ */

#define USB_RESET       0x01
#define TX_COMPLETE     0x02
#define RX_COMPLETE     0x04
#define FIFO_PURGE      0x08
#define DEVICE_OPEN     0x10
#define DEVICE_CLOSE    0x20
#define DEV_CONFIGURED  0x40
#define DEV_SUSPEND     0x80

void  USB_Init(UINT, UINT, BYTE*, BYTE*, BYTE*, BYTE, BYTE, UINT) large;        
UINT  Block_Write(BYTE*, UINT); //reentrant;
BYTE  Block_Read(BYTE*, BYTE);  //reentrant;
BYTE  Get_Interrupt_Source(void);
void  USB_Int_Enable(void);
void  USB_Int_Disable(void);
void  USB_Disable(void);
void  USB_Suspend(void);


#endif  /* _USB_API _H_ */