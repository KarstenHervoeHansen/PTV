National Instruments Serial Software for Windows Me/9x for the
AT-232, AT-485, PCMCIA-232, PCMCIA-485, PCI-232, PCI-485,
PXI-8420, PXI-8421, PXI-8422 and PXI-8423
Version 1.45
Copyright 2000 National Instruments Corporation.
All Rights Reserved.

Thank you for purchasing this Serial interface board and NI-Serial software
for Microsoft Windows Me/9x.

We appreciate communicating with the people who use our products.  We are
also very interested in hearing about the applications you develop using
our products. Our web-site at "ni.com" provides advanced support and 
services, and contact information.


WARNING: For users of PCMCIA-232 or PCMCIA-485 cards in Toshiba's
Satellite Pro 420CDS or 2150CDS notebook
----------------------------------------------------------------------------
We have noticed that there is a device on the notebook at I/O address
1F8H that only decodes the lower 10 address lines, which Windows 95 does 
not recognize. If any of our serial ports are configured at base I/O address
1F8h, 5F8h, 9F8h, or DF8h, one of two things happens: the computer
locks up when the card is inserted or the diagnostic fails.

To avoid this problem, complete the following steps to reserve these address
spaces BEFORE you insert your card.
1. Select Start>>Settings>>Control Panel.
2. Double-click on the System icon.
3. In the System Properties dialog box that appears, select the 
   Device Manager tab.
4. Double-click on the Computer icon at the top of the Device Manager
   list of devices.
5. Click on the Reserve Resources tab.
6. Select the Input/Output (I/O) button.
7. Enter each of the following address ranges by selecting the Add button
   for each.
	Start Value	End Value
	01F8		01FF	
	05F8		05FF
	09F8		09FF
	0DF8		0DFF
8. After you have entered these values and selected OK, restart Windows 95
   so that it can reserve these resources.		
9. You can now insert your card.