PI = 3.14159265359
i=0
OPEN "div.s" FOR OUTPUT AS #1
FOR SI=0 TO 255
  FOR CO=0 TO 255
    IF CO = 0 THEN
      OUT1 =  INT(255)
    ELSE
      OUT1= INT(SI / CO * 256)
      IF OUT1 >= 256 THEN
        OUT1 = 255
      END IF
    END IF
      if i=16 then
       i=0
       PRINT #1,out1;","
      else
       i=i+1
       print #1,out1;",";
      end if 
  NEXT CO
NEXT SI
CLOSE 1