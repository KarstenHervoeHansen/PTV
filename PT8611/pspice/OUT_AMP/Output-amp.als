* Schematics Aliases *

.ALIASES
V_V2            V2(+=$N_0001 -=0 )
V_V3            V3(+=0 -=$N_0002 )
V_V4            V4(+=in -=0 )
R_R1            R1(1=$N_0003 2=out )
R_R2            R2(1=$N_0004 2=$N_0003 )
X_U1A           U1A(+=in -=$N_0003 V+=$N_0001 V-=$N_0002 OUT=out )
R_R3            R3(1=0 2=$N_0004 )
C_C3            C3(1=$N_0003 2=out )
C_C4            C4(1=0 2=in )
C_C5            C5(1=0 2=$N_0003 )
_    _(in=in)
_    _(out=out)
.ENDALIASES

