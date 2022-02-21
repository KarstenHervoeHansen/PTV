DEFDBL A-Z
i=0
pi = 3.14159265359#
freq = 4433618.75
fsamp = 13500000#
rem freq=1
rem fsamp=4
cosfi = COS(2 * pi * freq / fsamp)
costfi = COS(2 * 2 * pi * freq / fsamp)
SINfi = SIN(2 * pi * freq / fsamp)
SINtfi = SIN(2 * 2 * pi * freq / fsamp)

OPEN "pytg.bin" FOR OUTPUT AS #1
FOR SI = 0 TO 255
  FOR CO = 0 TO 255
    y = SI MOD 128
    x = CO MOD 128
    IF SI >=128 THEN Y=-Y
    IF CO >=128 THEN X=-X
    IF y = 0 THEN y = .001
    IF x = 0 THEN x = .001
    cal2fase = ATN((cosfi - x / y * costfi) / (SINfi - x / y * SINtfi))
    IF -x < 0 AND -y > 0 AND cal2fase < 0 THEN
      cal2fase = cal2fase + pi
    END IF
    IF (-x > 0 AND cal2fase > 0) OR (-x > 0 AND -y > 0) THEN
      cal2fase = cal2fase + pi
    END IF
    IF x > y THEN
      am = x / 2 / SINfi / COS(cal2fase + 2 * pi * freq / fsamp)
    ELSE
      am = y / 2 / SINfi / COS(cal2fase + 2 * 2 * pi * freq / fsamp)
    END IF
      a1 = x / 2 / SINfi / COS(cal2fase + 2 * pi * freq / fsamp)
      a2 = y / 2 / SINfi / COS(cal2fase + 2 * 2 * pi * freq / fsamp)
    IF am < 0 THEN
      am = -am
    end if
    am = INT(2 * am * 1000 / 256 + .5)
    IF am > 255 THEN
      out1 = 255
    ELSE
      out1 = am
    END IF
     if i=16 then
      i=0      
      PRINT #1, (out1);","
     else
      i=i+1      
      PRINT #1, (out1);",";
     end if  
  NEXT CO
NEXT SI
CLOSE 1

