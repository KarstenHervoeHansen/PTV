/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		TIMING.H																		*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	960211																		*/
/*	Rev. date:	971211, KEn VDE															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

extern long AnalogFLTToSamples( UC system, char F, int L, float T);
extern void AnalogSamplesToFLT( UC system, long samples,\
												 char* sign, char* F, int* L, float* T);

extern long SDIFLTToSamples( UC system, char F, int L, float T);
extern void SDISamplesToFLT( UC system, long samples,\
												 char* sign, char* F, int* L, float* T);

extern void WriteTimingFLT( char sign, char field, int line, float time);
extern void PrintTimingFLT( char sign, char field, int line, float time);
