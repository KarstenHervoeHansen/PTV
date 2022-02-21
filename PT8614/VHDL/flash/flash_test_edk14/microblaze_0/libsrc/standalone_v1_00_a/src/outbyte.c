#include "xparameters.h"
#include "xuartlite_l.h"

void outbyte(char c) {
	 XUartLite_SendByte(STDOUT_BASEADDRESS, c);
}
