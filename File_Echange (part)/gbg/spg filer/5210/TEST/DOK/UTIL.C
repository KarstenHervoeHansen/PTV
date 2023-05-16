#include <utility.h>
static int IOAddr = 0x210;

void VLMdata(unsigned short xdata, unsigned short sel)
{
// bit 0: SCL    bit 1: SDA    bit 2..4: SEL 0..2   
unsigned char n,p;

	xdata = xdata << 6;
	p = 0xe0 + (sel * 4);	// control data bit 2,3,4  (bit 5,6,7=1)
	for (n = 0; n < 10; ++n)
	 {
	  if ((xdata & 0x8000) == 0)
	    p = p & 0xfd;		// set SDA
	  if ((xdata & 0x8000) == 0x8000)
	    p = p | 0x02;			
	  outp(IOAddr,p); ++p;
	  outp(IOAddr,p); --p;
	  outp(IOAddr,p);
	  
	  xdata = xdata << 1;
	  
	} // for
} // vlmdata


void SetVLM (unsigned short StartLine, 
			 unsigned short StopLine, 
			 unsigned short StartPos, 
			 unsigned short StopPos)
{
	VLMdata(StartLine,0);
	VLMdata(StopLine,1);
	VLMdata(StartPos,2);
	VLMdata(StopPos,3);
} // SetVLM
