/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		LOADER.C			  															*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void IntelLoader( char inchr)												 		*/
/*																									*/
/* Changes:																						*/
/***************************************************************************/
#include "system.h"
#include "loader.h"
#include "flash.h"

// StIdle:
// StLength:		1 byte specifies number of bytes in content field
// StLoadOffset:	2 bytes
// StRecType: 		1 byte
// StData:			length, (i.e. LT 256 bytes)
// StChecksum:		1 byte
// StError:
// Note: 1 byte equals 2 characters from the incoming stream

enum states	{ StIdle = 0, StRecLength, StLoadOffset, StRecType, StData,
				  StExtLinearAddr, StStartLinearAddr, StChecksum };

_rom char _LO_EnteringLoaderTxt[]	= "ENTERING LOADER STATE\r\n";
_rom char _LO_LeavingLoaderTxt[]		= "LEAVING LOADER STATE\r\n";
_rom char _LO_CharNotValidTxt[] 	  	= "CHAR NOT VALID\r\n";
_rom char _LO_RecLengthErrorTxt[]  	= "REC LENGTH ERROR\r\n";
_rom char _LO_RecTypeErrorTxt[] 	  	= "REC TYPE ERROR\r\n";
_rom char _LO_ChecksumErrorTxt[]   	= "CHECKSUM ERROR\r\n";

static _near dword LoadAddress;

static _near byte State, Checksum, CharCnt, ByteCnt;
static _near byte RecLength, RecByte;
static _near word RecWord;

/***************************************************************************/
/*	IntelLoader											  					 		LOADER.C	*/
/*																									*/
/* Author:		K. Engedahl																	*/
/* Revised:		990902, KEn, DEV	     													*/
/*																									*/
/*	Function:	Download file to flash prom device in Intel format.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte IntelLoader( char inchr) {
	
	byte value, error;

	if ( inchr == ':') {
		ByteCnt = CharCnt = Checksum = 0;
		RecWord = RecByte = RecLength = 0;

		State = StRecLength;

		return( _LO_RecStart);
	}

	error = _LO_NoError;

	if ( isxdigit( inchr)) {
		if ( isdigit( inchr))
			value = inchr - '0';
		else
			value = toupper( inchr) - 'A' + 10;

		if ( !( CharCnt++ & 0x01))
			RecByte = value;
		else {
			RecByte *= 16;
			RecByte += value;
			
			Checksum += RecByte;

			switch ( State) {
				case StIdle:
					break;

				case StRecLength:
					RecLength = RecByte;

					ByteCnt = 0;
				
					State++;
					break;

				case StLoadOffset:
					RecWord *= 256;
					RecWord += RecByte;
	
					if ( ++ByteCnt == 2) {
						ByteCnt = 0;

						State++;
					}
					break;

				case StRecType:
					switch ( RecByte) {
						case 0:			    	// Data record incl. load offset
							LoadAddress &= 0x000F0000;
							LoadAddress += RecWord;
													// If ODD offset address..
													//  get previous data byte and start
													//  downloading from that address
							if ( LoadAddress & 0x01) {
								RecWord = flash_read_word( --LoadAddress) & 0x00FF;

								ByteCnt = 1;
							}
							else
								RecWord = ByteCnt = 0;

							State = StData;
							break;

						case 1:				  	// End of file
							if ( RecLength == 0)
								State = StChecksum;
							else 
								error = _LO_RecLengthError;
							break;

						case 4:					// Extended linear address (32-bit)
							if ( RecLength == 2) {
								State = StExtLinearAddr;

								RecWord = ByteCnt = 0;
							}
							else
								error = _LO_RecLengthError;
							break;

						case 5:	  				// Start linear address (32-bit)
							if ( RecLength == 2) {
								State = Checksum;

								RecWord = ByteCnt = 0;
							}
							else
								error = _LO_RecLengthError;
							break;

						default:
							error = _LO_RecTypeError;
							break;
					}
					break;

				case StData:
					if ( ++ByteCnt == 2)
						RecWord += 256*RecByte;
					else
						RecWord = RecByte;

					if ( --RecLength) {
						if ( ByteCnt == 2) {
							flash_write_word(( LoadAddress), RecWord);

							LoadAddress += 2;

							RecWord = ByteCnt = 0;

							if ( RecLength == 0)
								State = StChecksum;
						}			
					}
					else {
						if ( ByteCnt == 1)  		// If ODD number of bytes..
						RecWord += flash_read_word( LoadAddress) & 0xFF00;

						flash_write_word(( LoadAddress), RecWord);

						State = StChecksum;
					}
					break;

				case StStartLinearAddr:
					RecWord *= 256;
					RecWord += RecByte;

					if ( ++ByteCnt == 2) {
						ByteCnt = 0;

						State = StChecksum;
					}
					break;

				case StExtLinearAddr:
					RecWord *= 256;
					RecWord += RecByte;

					if ( ++ByteCnt == 2) {
						LoadAddress &= 0x0000FFFF;
						LoadAddress += ((dword) ( 0x10000*RecWord) & 0x000F0000);

						ByteCnt = 0;

						State = StChecksum;
					}
					break;

				case StChecksum:
					State = StIdle;

					if ( Checksum)
						error = _LO_ChecksumError;
					break;
			}
		}
	}

	if ( error)
		State = StIdle;

	return( error);
}