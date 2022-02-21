National Instruments Serial Software for Windows 2000 for the
AT-232, AT-485, PCMCIA-232, PCMCIA-485, PCI-232, PCI-485, 
PXI-8420, PXI-8421, PXI-8422 and PXI-8423
Version 1.45
Copyright 2000 National Instruments Corporation.
All Rights Reserved.

Thank you for purchasing this Serial interface board and NI-Serial software
for Microsoft Windows 2000.

We appreciate communicating with the people who use our products.  We are
also very interested in hearing about the applications you develop using
our products. Our web-site at "ni.com" provides advanced support and 
services, and contact information.


Note :
------------
The function 'CreateFile' from Microsoft's Win32 API will fail for port names
COM10 and greater. To be able to use these ports and for the function to 
succeed, simply append the string "\\\\.\\" before the port name.
For example, to open COM10 use the string "\\\\.\\COM10" for the port name.
(This is because the full name of the port is actually \\.\COM10 and all
'\'-characters need to be doubled as per the C language syntax.)