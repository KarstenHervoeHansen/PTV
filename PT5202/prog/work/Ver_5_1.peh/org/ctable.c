/* ctable.c: Tables start at TSTART  */

#define TSTART 0x800

_ROM_ UI TEST_SMPTECBarTable[] _at(TSTART) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	164*256	+	SRAMBank1 + SyncLineC,
	19*256	+	SRAMBank2 + SyncLineC,
	61*256	+	SRAMBank3 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	163*256	+	SRAMBank1 + SyncLineD,
	19*256	+	SRAMBank2 + SyncLineD,
	61*256	+	SRAMBank3 + SyncLineD,
	0
};

#define TA1 (TSTART + sizeof(TEST_SMPTECBarTable))

_ROM_	UI PALBlackTable[] _at(TA1) =
{
	22*256 	+							SyncLineB,
	144*256	+							SyncLineC,
	144*256	+							SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	144*256	+							SyncLineD,
	144*256	+							SyncLineD,
	2*256		+							SyncLineA,
	0
};

#define TA2 (TA1 + sizeof(PALBlackTable))

_ROM_ UI PALFullFieldTable[] _at(TA2)=
{
	22*256 	+							SyncLineB,
	144*256	+	SRAMBank1 + SyncLineC,
	144*256	+	SRAMBank1 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	144*256	+	SRAMBank1 + SyncLineD,
	144*256	+	SRAMBank1 + SyncLineD,
	2*256		+							SyncLineA,
	0
};
#define TA3 (TA2 + sizeof(PALFullFieldTable))

_ROM_ UI PALSplitFieldTable[] _at(TA3) =
{
	22*256 	+							SyncLineB,
	192*256	+	SRAMBank1 + SyncLineC,
	96*256	+	SRAMBank2 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	192*256	+	SRAMBank1 + SyncLineD,
	96*256	+	SRAMBank2 + SyncLineD,
	2*256		+							SyncLineA,
	0
};
#define TA4 (TA3 + sizeof(PALSplitFieldTable))

_ROM_ UI PALWindowTable[] _at(TA4) =
{
	22*256 	+							SyncLineB,
	24*256	+         		SyncLineC,
	1*256		+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	1*256		+	  		      SyncLineC,
	1*256		+	SRAMBank3 + SyncLineC,
	44*256	+	      		  SyncLineC,
	144*256	+	SRAMBank4 + SyncLineC,
	72*256	+	    		    SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	24*256	+         		SyncLineD,
	1*256		+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	1*256		+	      		  SyncLineD,
	1*256		+	SRAMBank3 + SyncLineD,
	44*256	+	 			      SyncLineD,
	144*256	+	SRAMBank4 + SyncLineD,
	72*256	+	      		  SyncLineD,
	2*256		+							SyncLineA,
	0
};

#define TA5 (TA4 + sizeof(PALWindowTable))

_ROM_ UI PALCheckfieldTable[] _at(TA5) =
{
	22*256 	+							SyncLineB,
	1*256		+	SRAMBank1 + SyncLineC,
	143*256	+	SRAMBank2 + SyncLineC,
	144*256	+	SRAMBank3 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	144*256	+	SRAMBank2 + SyncLineD,
	144*256	+	SRAMBank3 + SyncLineD,
	2*256		+							SyncLineA,
	0
};

#define TA6 (TA5 + sizeof(PALCheckfieldTable))

_ROM_ UI PALCrosshatchTable[] _at(TA6) =
{
	22*256 	+							SyncLineB,
	7*256		+ SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	7*256	  +	SRAMBank1 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	7*256		+ SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	7*256	  +	SRAMBank1 + SyncLineD,
	2*256		+							SyncLineA,
	0
};

#define TA7 (TA6 + sizeof(PALCrosshatchTable))

_ROM_ UI PALPLUGETable[] _at(TA7) =
{
	22*256 	+							SyncLineB,
	38*256	+       		  SyncLineC,
	53*256	+	SRAMBank1 + SyncLineC,
	53*256	+	SRAMBank2 + SyncLineC,
	53*256	+	SRAMBank3 + SyncLineC,
	53*256	+	SRAMBank4 + SyncLineC,
	38*256	+	  		      SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	38*256	+     		    SyncLineD,
	53*256 	+	SRAMBank1 + SyncLineD,
	53*256 	+	SRAMBank2 + SyncLineD,
	53*256 	+	SRAMBank3 + SyncLineD,
	53*256 	+	SRAMBank4 + SyncLineD,
	38*256	+	 		    	  SyncLineD,
	2*256		+							SyncLineA,
	0
};

#define TA8 (TA7 + sizeof(PALPLUGETable))

_ROM_ UI NTSCBlackTable[] _at(TA8) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	122*256	+							SyncLineC,
	122*256	+							SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	122*256	+							SyncLineD,
	121*256	+							SyncLineD,
	0
};

#define TA9 (TA8 + sizeof(NTSCBlackTable))

_ROM_ UI NTSCFullFieldTable[] _at(TA9) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	122*256	+	SRAMBank1	+	SyncLineC,
	122*256	+	SRAMBank1 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	122*256	+	SRAMBank1 + SyncLineD,
	121*256	+	SRAMBank1 + SyncLineD,
	0
};

#define TA10 (TA9 + sizeof(NTSCFullFieldTable))

_ROM_ UI SMPTECBarTable[] _at(TA10) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	164*256	+	SRAMBank1 + SyncLineC,
	19*256	+	SRAMBank2 + SyncLineC,
	61*256	+	SRAMBank3 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	163*256	+	SRAMBank1 + SyncLineD,
	19*256	+	SRAMBank2 + SyncLineD,
	61*256	+	SRAMBank3 + SyncLineD,
	0
};

#define TA11 (TA10 + sizeof(SMPTECBarTable))

_ROM_ UI NTSCWindowTable[] _at(TA11) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	30*256	+	        		SyncLineC,
	1*256		+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	1*256		+	    		    SyncLineC,
	1*256		+	SRAMBank3 + SyncLineC,
	29*256	+	 			    	SyncLineC,
	119*256	+	SRAMBank4 + SyncLineC,
	62*256	+			        SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	29*256	+	       			SyncLineD,
	1*256		+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	1*256		+	  		      SyncLineD,
	1*256		+	SRAMBank3 + SyncLineD,
	29*256	+	      		  SyncLineD,
	119*256	+	SRAMBank4 + SyncLineD,
	62*256	+	    		    SyncLineD,
	0
};

#define TA12 (TA11 + sizeof(NTSCWindowTable))

_ROM_ UI NTSCCheckfieldTable[] _at(TA12) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	1*256		+	SRAMBank1 + SyncLineC,
	121*256	+	SRAMBank2 + SyncLineC,
	122*256	+	SRAMBank3 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	122*256	+	SRAMBank2 + SyncLineD,
	121*256	+	SRAMBank3 + SyncLineD,
	0
};

#define TA13 (TA12 + sizeof(NTSCCheckfieldTable))

_ROM_ UI NTSCCrosshatchTable[] _at(TA13) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	9*256		+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	7*256		+	SRAMBank1 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	8*256		+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	6*256		+	SRAMBank1 + SyncLineD,
	0
};

#define TA14 (TA13 + sizeof(NTSCCrosshatchTable))

_ROM_ UI NTSCPLUGETable[] _at(TA14) =
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	53*256	+	 		       	SyncLineC,
	56*256	+	SRAMBank1 + SyncLineC,
	19*256	+	SRAMBank2 + SyncLineC,
	19*256	+	SRAMBank3 + SyncLineC,
	19*256	+	SRAMBank4 + SyncLineC,
	78*256	+	    		    SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	52*256	+	    		    SyncLineD,
	56*256	+	SRAMBank1 + SyncLineD,
	19*256	+	SRAMBank2 + SyncLineD,
	19*256	+	SRAMBank3 + SyncLineD,
	19*256	+	SRAMBank4 + SyncLineD,
	78*256	+	       		 	SyncLineD,
	0
};

#define TA15 (TA14 + sizeof(NTSCPLUGETable))

	_ROM_ UI Sound525[] _at(TA15) = 
    {
  	0x083, 0x181, 0x085, 0x003, 0x086, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181,	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x083, 0x081
    };

#define TA16 (TA15 + sizeof(Sound525))

	_ROM_ UI Sound625[65] _at(TA16) = 
	  {
  	0x181,  0x084,  0x003,  0x085,  0x181,  0x089,  0x181,  0x089,
  	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
  	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
  	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
  	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
  	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
  	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
  	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x088,
  	0x181
    };

