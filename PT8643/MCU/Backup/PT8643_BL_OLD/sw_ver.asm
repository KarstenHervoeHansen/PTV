CSEG	AT	0x23FD
// SoftwareVersionMajor, SoftwareVersionMinor, SoftwareID.
// SoftwareID: 0x01 = Bootloader;
// SoftwareID: 0x02 = Run Code;
DB	0, 7, 0x01;

CSEG	AT	0x23FB
// Bootloader CRC.
DB	0x32, 0x9A;


END

/*
		0.00
		 |
		 |->	0.06	Development version.
					 | 		 		
					 |->	1.00	BS000000
								      ...
								  		...
					    	  		...
					 			  
*/
