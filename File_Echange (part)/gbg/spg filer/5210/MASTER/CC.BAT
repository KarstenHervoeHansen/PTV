CPP.EXE -SP2,2,2,2,2,2,4 -S1,2,2,4,4,4 -DLARGE_MODEL -DHI_TECH_C -D_XA_ -IC:\xa\include\ -E2 %1.c temp.000
p1.exe -B -QDi,code -QB,banked -E2 -S temp.000 temp.001 temp.002
CGXA.exe -MRAMDATA -MEXTRAM -MLRG_CODE temp.001 %1.as
