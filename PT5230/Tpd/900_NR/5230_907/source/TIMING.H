/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		TIMING.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960211																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

extern long AnlFLTToSamples( UC system, char F, int L, float T);
extern void AnlSamplesToFLT( UC system, long samples,\
												 char* sign, char* F, int* L, float* T);

extern long SDIFLTToSamples( UC system, char F, int L, float T);
extern void SDISamplesToFLT( UC system, long samples,\
												 char* sign, char* F, int* L, float* T);

extern void WriteTimingFLT( char sign, char field, int line, float time);
extern void PrintTimingFLT( char sign, char field, int line, float time);
