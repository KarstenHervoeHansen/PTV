CSEG	AT	0xFFFA	// 0xA2 = Debug Mode - Bootloader runs user code if CRC fails.
DB	0x01	 				// 0x01 = RUN Mode.	

CSEG	AT	0xFFFB	// Run Code CRC Value 0xFFFB + 0xFFFC
DB	0x81, 0xC1;		// The CRC Check in the bootloader does not calculate CRC on the data set in this file.
									// (The version number can safely be changed without changing the CRC values.)

CSEG	AT	0xFFFD	// Run Code Software Version 0xFFFD + 0xFFFE + 0xFFFF
DB	2, 0, 0x02;		// SoftwareVersionMajor, SoftwareVersionMinor, SoftwareID.
									// SoftwareID: 0x01 = Bootloader;
									// SoftwareID: 0x02 = Run Code;


// !!!!!!!!!!!!!!!! Rember to update version number in i2c_slave.c !!!!!!!!!!!!!!!!!!!!!

END

/*
		0.00
		 |
		 |->	0.02	BS 2010-12-16 Development version.
		 |		Released to production.
		 |		Leap seconds must be implemented correctly when we have working hardware.
		 | 		 		
		 |->  0.03  BS2011-01-05 Development version.
		 |		Leapseconds implemented.
		 |		Command GPSSTATUS implemented for Telnet in command mode.
 		 |
		 |->  0.04  BS2011-01-06 Development version.
		 |		RS232 command timeout adjusted to 5000.
		 |		tcp_resend_ticks raised to 20.
 		 |								 
		 |->  0.06  BS2011-01-07 Development version.
		 |		Netfinder and Telnet security issue fixed.
		 |		The CP2200 flash init byte has been changed to 0x02 because of changes to the flash memory map.
		 |		
		 |->  0.07  BS2011-01-10 Development version.
		 |		Released to production.
		 |		Function ntp_process_packet() changed in order to find the "missing" second.
		 |		Tests suggest that there might be a bug in NTP-Client.EXE. The missing second always appear in the same 
		 |		request number every time though different request numbers on different computers.
		 |	
		 |		Ethernet initialisation has also been changed. The WHILE(1) loops has been eliminated during startup.
		 |
		 |->	0.08  	BS2011-01-21 Released to production.
		 |		DHCP to I2C fixed. if DHCP_Enable <> 0 then DHCP_Enable := 1;
		 |	
		 |->  0.09  BS2011-01-21 Release to production.
		 |		The NetFinder name is loaded to RAM during startup and restart. If read directly from flash the NetFinder menu
		 |		would show square signs if no network cable was connected.
		 |		
		 |->  1.00  BS2011-01-25 Release to production.
		 |		Implemented software version in Configuration menu.
		 |	
		 |->	1.01 	BS2011-04-07 Released to production.
		 |		DHCP is now renewed from Server Idle Function.
		 |		DHCP status can be read through Telnet.(@AT+++ -> DHCP)
		 |		GPS Lock Status can now be read trhough Telnet. (@AT+++ -> GPSLOCK)
		 |		A help menu for debug is implemented in Telnet. (@AT+++ -> HELP)
		 |
	   |->  1.02  BS2011-04-29 Released to production.
		 |		DHCP renew function changed.
		 |		Interrupt routines that read data larger than a byte is halted while others are writing.
		 |
	   |->  1.03  BS2012-02-22 Debug version.
		 |		Debug data can be sent to UART0.
		 |		mn_ether_init returns after 1 failed attempt.
		 |		SMBus interrupt no longer have priority.
		 |		Timer 2 interrupt now have priority.
		 |		Timer 2 is now used as a software watchdog timer. (Only enabled while the server is running.)
		 |		Netfinder RTC removed.
		 |		str2COM0 is blocking until data is transmitted.
	   |
		 |->  1.04  BS2012-02-29 Released to production.
		 |		Debug disabled.
		 |
		 |->  1.05  BS2012-02-29 Debug Version.
		 |		Software WD disabled.
     |		
		 |->  1.06  BS2012-03-05 Released to production.
		 |		Leap year bug fixed.
		 |		Software WD Enabled.
		 |
		 |->  1.07  BS2012-03-05 Debug Version.
		 |		Software WD disabled.
     |		
		 |->  1.08  BS2012-05-23 Released to production.
		 |		Software WD Enabled.
		 |		UBlox msg_buf moved to XRAM @ 0x1C00.
		 |		Minor changes to Uart 0 buffers and Telnet states.
     |		
		 |->  1.09  BS2012-05-23 Debug Version.
		 |		Software WD Enabled.
		 |		Netfinder Protocol Disabled.
     |		
		 |->  1.10  BS2012-06-27 Released to production.
		 |		Software WD Disabled.
		 |		Timer 2 functions moved to Timer 4 and Timer 2 interrupt is disabled. 
		 |		Timer 2 is now used as baudrate generator for uart 0.
		 |		This prevents a system crash during a DOS attack. There is an issue with sharing Timer 2 with bootloader.
		 |		UBlox msg_buf size changed to 300.
		 |		Telnet inactivity timeout extended to two minutes.
		 |
		 |->  2.00  BS2012-07-04 Released to long term test.
		 |		Software version updated to 2.0 and I2C Slave will now show the correct version.
		 |		Multiple bug fixes. UART1 has priority. Telnet timeout configured to 2 minutes.
		 |		Fraction counter resets after 200 ticks which has solved the "missing second" problem.
		 | 		SNTP has a timeout that will reset the socket if no packet has arrived for 4 seconds. (Workaround)

*/
