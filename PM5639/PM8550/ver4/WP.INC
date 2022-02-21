(*
WP.INC   Utility for PM8550.PAS file
PROCEDURE PauseKey;
PROCEDURE WriteProtectOff;
PROCEDURE WriteProtectOn;
PROCEDURE WriteProtectMenu;

*)

FUNCTION EnterKey : BOOLEAN;
VAR Ch       : CHAR;
    ScanCode : BYTE;
BEGIN
 Write('Press ENTER to continue  -  press ESC to abort');
 REPEAT
  __FlushKey;
  Ch := __GetKey(TRUE,ScanCode,NIL,NIL);
 UNTIL Ch IN [#13,#27];
 EnterKey := Ch = #13;
END;


PROCEDURE WriteProtectOff;
VAR Y        : WORD;
    Data1,
    Data2    : STR_5;
    WPAddrOk,
    SensWP,
    SensCon : BOOLEAN;
BEGIN
 Check    := __TitleWin(WPWin,' Write Protect OFF ',
        			   TittleFore,TittleBack,_TopCenter);
 IF (SensorSWStr = '05.7') OR (SensorSWStr = '05.6') THEN
  BEGIN
    Check := __DispWin(WPWin);
    Writeln(' To set write protect OFF, please do the following:');
    Writeln(' 1) Switch OFF the display unit to power off the sensor');
    Writeln(' 2) Remove the handle assembly (only 4 screws).');
    Writeln(' 3) Place the disasembled color sensor with the suction');
    Writeln('    pad down and the connector to the left. The coding');
    Writeln('    field is now on top of the PCB.');
    Writeln(' 4) Cut the connection 6b-6c in the coding field on the');
    Writeln('    the PCB with a scalpel.');
    Writeln('    Layout of coding field:      1 2 3 4 5 6');
    Writeln('                               a ÛÄÛÄÛÄÛÄÛ  ');
    Writeln;
    Writeln('                               b Û Û Û Û Û Û');
    Write  ('                                           ³');
    __QuikWin(WhereX,WhereY,MenuFore,MenuBack + _BLINK,#17); Writeln;
    Writeln('                               c Û Û Û Û Û Û');
    Writeln(' 5) Mount the handle assembly (only 4 screws).');
    Writeln(' 6) Switch ON the display unit.');
    Writeln('    The color sensor is now ready for calibration.');
    __GotoWin(2,WhereY+1);
    Y := __PauseKey('');
    Check := __RemWin;
   REPEAT
    CheckWriteProtected(SensWP,SensCon);
    Finished := RetryYesOrNo(SensCon,UserTxt[6],UserTxt[7],2);
   UNTIL SensCon OR Finished;
   IF Finished THEN
    Exit;
   IF SensWP THEN
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'ON ');
      UserMessage(UserTxt[12],'',2);
     END
    ELSE
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'OFF');
      UserMessage(UserTxt[11],'',1);
     END;
  END
 ELSE
 IF (SensorSWStr = '05.8') THEN
  BEGIN
    Check := __DispWin(WPWin);
    Writeln(' To set write protect OFF, please do the following:');
    Writeln(' 1) Switch OFF the display unit to power off the sensor');
    Writeln(' 2) Remove the handle assembly (only 4 screws).');
    Writeln(' 3) Place the disasembled color sensor with the suction');
    Writeln('    pad down and the connector to the left. The coding');
    Writeln('    field is now on top of the PCB.');
    Writeln(' 4) Cut the connection 6b-6c in the coding field on the');
    Writeln('    the PCB with a scalpel.');
    Writeln('    Layout of coding field:     1 2 3 4 5 6');
    Writeln('                              a ÛÄÛÄÛÄÛÄÛ  ');
    Writeln;
    Writeln('                              b Û Û Û Û Û Û');
    Write  ('                                          ³');
    __QuikWin(WhereX,WhereY,MenuFore,MenuBack + _BLINK,#17); Writeln;
    Writeln('                              c Û Û Û Û Û Û');
    Writeln;
    Writeln(' 5) Switch ON the display unit.');
    __GotoWin(2,WhereY+1);
    IF NOT EnterKey THEN
     BEGIN
      Check := __RemWin; Exit;
     END;
    ClrScr;
    COMStatus := WriteCOMPort(PortNo,'MS,');
    COMStatus := WriteCOMPort(PortNo,'MS,');
    COMStatus := WriteCOMPort(PortNo,'MX,');
    REPEAT
     WriteEEPROM(54,$FF);
     WriteEEPROM(55,$FF);
     COMStatus := ClearInputQue(PortNo);
     COMStatus := WriteCOMPort(PortNo,'MA54,');
     COMStatus := WriteCOMPort(PortNo,'RM,');
     COMStatus := ReadlnCOMPort(PortNo,Svar,1);
     Delay(10);
     Data1 := Svar;
     COMStatus := WriteCOMPort(PortNo,'RM,');
     COMStatus := ReadlnCOMPort(PortNo,Svar,1);
     Delay(10);
     Data2 := Svar;
     WPAddrOk := (Data1 = '255') AND (Data2 = '255');
     Finished := RetryYesOrNo(WPAddrOk,UserTxt[6],UserTxt[7],2);
    UNTIL WPAddrOk OR Finished;
    IF Finished THEN
     BEGIN
      Check := __RemWin;
      Exit;
     END;

    Writeln(' 6) Switch OFF the display unit to power off the sensor');
    Writeln('    Now the color sensor is ready for calibration');
    Writeln('    though if you want to write protect the calibra-');
    Writeln('    tion data after the calibration continue with');
    Writeln('    paragraph 7. If you don''t want to write protect');
    Writeln('    the color sensor proceed with paragraph 8.');
    Writeln('    You may use the color sensor without write protect.');
    Writeln(' 7) Connect the coding field 6b-6c with solder.');
    Writeln('    CAUTION: To prevent static discharge to enter the');
    Writeln('    PCB, connect ground of the soldering iron to the');
    Writeln('    shield of the connector.');
    Writeln('    Layout of coding field:    1 2 3 4 5 6');
    Writeln('                             a ÛÄÛÄÛÄÛÄÛ  ');
    Writeln;
    Writeln('                             b Û Û Û Û Û Û');
    Write  ('                                         ³');
    __QuikWin(WhereX,WhereY,MenuFore,MenuBack + _BLINK,#17); Writeln;
    Writeln('                             c Û Û Û Û Û Û');
    Writeln(' 8) Mount the handle assembly (only 4 screws).');
    Writeln(' 9) Switch ON the display unit.');
    __GotoWin(2,WhereY);
    Y := __PauseKey('');
    Check := __RemWin;
   REPEAT
    CheckWriteProtected(SensWP,SensCon);
    Finished := RetryYesOrNo(SensCon,UserTxt[6],UserTxt[7],2);
   UNTIL SensCon OR Finished;
   IF Finished THEN
    Exit;
   IF SensWP THEN
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'ON ');
      UserMessage(UserTxt[12],'',2);
     END
    ELSE
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'OFF');
      UserMessage(UserTxt[11],'',1);
     END;
 END
 ELSE  {-------- sensors with SW-controlled WP ------------------}
  BEGIN
   REPEAT
    COMStatus := WriteCOMPort(PortNo,'MS,');    Delay(10);
    COMStatus := WriteCOMPort(PortNo,'WR79,');  Delay(10);
    COMStatus := WriteCOMPort(PortNo,'WR102,'); Delay(10);
    COMStatus := WriteCOMPort(PortNo,'WR102,'); Delay(10);
    CheckWriteProtected(SensWP,SensCon);
    Finished := RetryYesOrNo(SensCon,UserTxt[6],UserTxt[7],2);
   UNTIL (NOT SensWP AND SensCon) OR Finished;
   IF NOT Finished THEN
    BEGIN
     __QuikWin(Col4,Row11,MainFore,MainBack,'OFF');
     UserMessage(UserTxt[11],'',1);
    END; {if}
  END;

END; { WriteProtectOff}




PROCEDURE WriteProtectOn;
VAR Y        : WORD;
    Data1,
    Data2    : STR_5;
    WPAddrOk,
    SensWP,
    SensCon  : BOOLEAN;
BEGIN
 Check    := __TitleWin(WPWin,' Write Protect ON ',
        			   TittleFore,TittleBack,_TopCenter);
 IF (SensorSWStr = '05.7') OR (SensorSWStr = '05.6') THEN
  BEGIN
    Writeln('');
    Check := __DispWin(WPWin);
    Writeln(' The purpose of the write protection is to protect the');
    Writeln(' calibration data but you can use the color sensor');
    Writeln(' without write protection.');
    Writeln(' If you don''t want to write protect the color sensor,');
    Writeln(' leave the coding field 6b-6c disconnected.');
    Writeln;
    Writeln(' To set write protect ON, please do the following:');
    Writeln;
    Writeln(' 1) Switch OFF the display unit to power off the sensor');
    Writeln(' 2) Remove the handle assembly (only 4 screws).');
    Writeln(' 3) Connect the coding field 6b-6c with solder.');
    Writeln('    CAUTION: To prevent static discharge to enter the');
    Writeln('    PCB, connect ground of the soldering iron to the');
    Writeln('    shield of the connector.');
    Writeln(' 4) Mount the handle assembly (only 4 screws).');
    Writeln(' 5) Switch ON the display unit.');
    Writeln('    Write protect is now set ON');
    __GotoWin(2,WhereY+1);
    Y := __PauseKey('');
    Check := __RemWin;
   REPEAT
    CheckWriteProtected(SensWP,SensCon);
    Finished := RetryYesOrNo(SensCon,UserTxt[6],UserTxt[7],2);
   UNTIL SensCon OR Finished;
   IF Finished THEN
    Exit;
   IF SensWP THEN
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'ON ');
      UserMessage(UserTxt[12],'',1);
     END
    ELSE
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'OFF');
      UserMessage(UserTxt[11],'',2);
     END;
  END
 ELSE
 IF (SensorSWStr = '05.8') THEN
  BEGIN
    Check := __DispWin(WPWin);
    Writeln(' The purpose of the write protection is to protect the');
    Writeln(' calibration data but you can use the color sensor');
    Writeln(' without write protection.');
    Writeln(' NOTE: This write protect ON procedure only works, if');
    Writeln(' the connection 6b-6c in the coding field was established');
    Writeln(' as described in the write protect OFF menu');
    Writeln;
    Writeln(' To set the write protect ON, please do the following:');
    Writeln;
    Writeln(' 1) Check that the display unit is ON');
    __GotoWin(2,WhereY);
    IF NOT EnterKey THEN
     BEGIN
      Check := __RemWin; Exit;
     END;
    Writeln; Writeln;
    COMStatus := WriteCOMPort(PortNo,'MS,');
    COMStatus := WriteCOMPort(PortNo,'MS,');
    COMStatus := WriteCOMPort(PortNo,'MX,');
    WriteEEPROM(54,186);
    WriteEEPROM(55,0);
    REPEAT
     COMStatus := ClearInputQue(PortNo);
     COMStatus := WriteCOMPort(PortNo,'MA54,');
     COMStatus := WriteCOMPort(PortNo,'RM,');
     COMStatus := ReadlnCOMPort(PortNo,Svar,1);
     Delay(10);
     Data1 := Svar;
     COMStatus := WriteCOMPort(PortNo,'RM,');
     COMStatus := ReadlnCOMPort(PortNo,Svar,1);
     Delay(10);
     Data2 := Svar;
     WPAddrOk := (Data1 = '186') AND (Data2 = '000');   {OK = 186  000}
     Finished := RetryYesOrNo(WPAddrOk,UserTxt[6],UserTxt[7],2);
    UNTIL WPAddrOk OR Finished;
    IF Finished THEN
     BEGIN
      Check := __RemWin;
      Exit;
     END;
    Writeln(' 2) Switch OFF the display unit to power off the sensor');
    Writeln(' 3) Switch ON the display unit to power on the sensor');
    Writeln('    When the color sensor is powered on, the write ');
    Writeln('    protect is activated.');
    __GotoWin(2,WhereY);
    Y := __PauseKey('');
    Check := __RemWin;
   REPEAT
    CheckWriteProtected(SensWP,SensCon);
    Finished := RetryYesOrNo(SensCon,UserTxt[6],UserTxt[7],2);
   UNTIL SensCon OR Finished;
   IF Finished THEN
    Exit;
   IF SensWP THEN
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'ON ');
      UserMessage(UserTxt[12],'',1);
     END
    ELSE
     BEGIN
      __QuikWin(Col4,Row11,MainFore,MainBack,'OFF');
      UserMessage(UserTxt[11],'',2);
     END;
  END
 ELSE  {-------- sensors with SW-controlled WP ------------------}
  BEGIN
   REPEAT
    COMStatus := WriteCOMPort(PortNo,'WR0,'); Delay(10);
    CheckWriteProtected(SensWP,SensCon);
    Finished := RetryYesOrNo(SensCon,UserTxt[6],UserTxt[7],2);
   UNTIL (SensWP AND SensCon) OR Finished;
   IF NOT Finished THEN
    BEGIN
     __QuikWin(Col4,Row11,MainFore,MainBack,'ON ');
     UserMessage(UserTxt[12],'',1);
    END; {if}
  END;
END; {WriteProtectOn}



PROCEDURE WriteProtectMenu;
BEGIN
 Check := __DispWin(WPOnOffWin);
 WPPtr := NIL;
 __FlushKey;
 IF __DispMnu(WPOnOffMnu) THEN
  WPPtr := __ReadMnu(WPOnOffMnu,WPPtr,_MBEEP_UNKNOWN_MNU + _MREMOVE_MNU,Key);
  CASE Key OF
   _Key_a_x : Finished := TRUE;
   _Key_F1  : Help('writeprotect');
  ELSE
   CASE WPPtr^._Row OF
    1 : WriteProtectOn;
    2 : WriteProtectOff;
   END; {case}
  END; {case}
 Check := __RemMnu(WPOnOffMnu);
END;

