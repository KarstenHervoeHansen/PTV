// This file defines project specific parameters to be used in 
// different source files.
// This file is the PT5201 Compact VariTime sysc Generator  config.h

// Define the number of IIC-busses used in this project
// Or define each individual bus used;  bus 0 don't need to be defined, allways included
// #define IicBus1	

// Rev1_1 added 1 bus for ALPS tuner


#define NiicBus 1

#define StartOfPROMCode				0x00000L
#define SizeOfPROMCode 				0x40000L

#define StartOfPLDCode				0x40000L
#define SizeOfPLDCode					0x0A000L

#define StartOfEmbAudioCode		0x4A000L
#define SizeOfEmbAudioCode		0x02400L

#define StartOfCBVideoCode		0x50000L
#define SizeOfCBVideoCode			0x30000L

#ifdef __BORLANDC__
	 #undef SizeOfCBVideoCode
	 #define SizeOfCBVideoCode			0x300L
#endif


