.ALIASES
X_U2            U2(+=N00152 -=VIN V+=N00298 V-=0 OUT=VOUT ) CN @GAIN-TEST.SCHEMATIC1(sch_1):INS75@EVAL.uA741.Normal(chips)
R_R1            R1(1=N00152 2=VOUT ) CN @GAIN-TEST.SCHEMATIC1(sch_1):INS127@ANALOG.R.Normal(chips)
V_V2            V2(+=VIN -=0 ) CN @GAIN-TEST.SCHEMATIC1(sch_1):INS372@SOURCE.VDC.Normal(chips)
V_V1            V1(+=N00298 -=0 ) CN @GAIN-TEST.SCHEMATIC1(sch_1):INS190@SOURCE.VDC.Normal(chips)
R_R2            R2(1=0 2=VIN ) CN @GAIN-TEST.SCHEMATIC1(sch_1):INS1172@ANALOG.R.Normal(chips)
_    _(vout=VOUT)
_    _(vin=VIN)
.ENDALIASES
