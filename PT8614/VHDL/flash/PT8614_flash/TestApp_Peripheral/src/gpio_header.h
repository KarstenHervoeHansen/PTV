#define TESTAPP_GEN

/* $Id: gpio_header.h,v 1.1.2.2 2005/10/06 23:58:23 sjen Exp $ */


#include "xbasic_types.h"
#include "xstatus.h"

XStatus GpioOutputExample(Xuint16 DeviceId, Xuint32 GpioWidth);
XStatus GpioInputExample(Xuint16 DeviceId, Xuint32 *DataRead);


