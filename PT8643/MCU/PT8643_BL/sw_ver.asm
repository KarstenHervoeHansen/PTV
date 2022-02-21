CSEG	AT	0x23FD
// SoftwareVersionMajor, SoftwareVersionMinor, SoftwareID.
// SoftwareID: 0x01 = Bootloader;
// SoftwareID: 0x02 = Run Code;
DB	1, 0, 0x01;

CSEG	AT	0x23FB
// Bootloader CRC.
DB	0x9E, 0x1B;


END

/*
		0.00
		 |
		 |->	0.06 	BS2011-01-07 Released to production.
		 |
		 |->	0.07 	BS2011-01-21 Released to production.
		 |					Implemented Factory Reset through I2C Slave.
		 |					MAC Address can now be read using FirmwareLoader.
		 |
		 |->	1.00	BS2012-06-28 Released to production.
		 |					I2C Software version fixed.
		 |					All timers and interrupts are disabled before jump to run code.
					 			  
*/
