/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:   TIMING.H                                                     */
/*  Author:   Kim Engedahl, DEV                                            */
/*  Org. date:  960211                                                     */
/*  Rev. date:  980616, KEn, DEV                                           */
/*  Status:   Version 1.0                                                  */
/*                                                                         */
/*  Changes:                                                               */
/* 980616: Released as 4008 002 06812                                      */
/* 980514: Released as 4008 002 06811                                      */
/***************************************************************************/
extern long prepare_samples4fpga(long samps, UC sys, code HDPARAMS *table);

extern long adjust_hd_samples(long samps, UC format, code HDPARAMS *table);

extern long add_genlock_delay(UC form, long tlg_delay, long genlock_delay, code HDPARAMS *table);
extern void HDTPGSamplesToFLT(UC, long, char*, char*, int*, float*, UC);

extern long AnlFLTToSamples(UC system, char F, int L, float T);
extern void AnlSamplesToFLT(UC system, long samples,\
                             char* sign, char* F, int* L, float* T);

extern long SDIFLTToSamples(UC system, char F, int L, float T);
extern void SDISamplesToFLT(UC system, long samples,\
                             char* sign, char* F, int* L, float* T);

extern long TLGFLTToSamples(UC, char, int, float, code HDPARAMS *);
extern void TLGSamplesToFLT(UC frmt, long samples, char* sign, char* F, int* L, float* T,  code HDPARAMS *table);

extern void WriteTimingFLT(char sign, char field, int line, float time);
extern void WriteTimingFLTTLG(char sign, char field, int line, float time);
extern void PrintTimingFLT(char sign, char field, int line, float time);
extern void PrintTimingFLTTLG(char sign, char field, int line, float time);
