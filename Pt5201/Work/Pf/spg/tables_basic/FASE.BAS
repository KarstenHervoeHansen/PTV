PI = 3.14159265359
OPEN "fase.bin" FOR OUTPUT AS #1
i=0
FOR SI=0 TO 255
  FOR CO=0 TO 255
    IF SI < CO THEN
      IF CO = 0 THEN
        FASE = O
      ELSE
        FASE = 180/PI*ATN(SI/CO)
      END IF
    ELSE
      IF SI = O THEN
        FASE = 0
      ELSE
        FASE = 90 - 180/PI*ATN(CO/SI)
      END IF
    END IF
    FASE= INT(256/90*FASE+.5)
    IF FASE=256 THEN
      FASE=255
    END IF
    if i=16 then
     PRINT #1,fase;","
     i=0
    else
     PRINT #1,fase;",";
     i=i+1
    end if
  NEXT CO
NEXT SI
CLOSE 1