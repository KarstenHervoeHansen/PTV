#include "xparameters.h"
#include "xuartlite_l.h"

char inbyte(void) {
	 return XUartLite_RecvByte(STDIN_BASEADDRESS);
}
