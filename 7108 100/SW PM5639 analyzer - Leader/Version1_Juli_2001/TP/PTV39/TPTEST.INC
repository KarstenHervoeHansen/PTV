
(*
Denne fil indeholder f›lgende procedurer:
PROCEDURE SupplyCurrentTest;      {F1}
PROCEDURE AnalogProbeTest;        {F1}
*)

PROCEDURE SupplyCurrentTest;
{Test af str›mforbrug til color sensoren. Der m†les over R29 i lyskassen.
 V‘rdien af modstanden R29 er korrigeret i konstanten 'R29'. M†lev‘rdier
 og tolerancer er givet som konstanter efter keyword CONST i
 hovedprogrammerne}
VAR PM2534Str  : STR_20;
    Value      : REAL;
    k          : INTEGER;
    Status     : BYTE;
    S1,S2      : STR_10;
BEGIN
 Write('Test af str›mforbrug:');
 RelayOn;
 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'OUTPUT PM2534; RNG 2; MSP 3; OUT N');
 Delay(500);
 Reset(IeIn);
 Writeln(IeOut,'ENTER PM2534 LF');
 Readln(IeIn,PM2534Str);
 Val(PM2534Str,Value,k);             {der m†les over R29}
 Value := Value * 1000 / R29;      {til mA}
 IF (Value > CurrentMin) AND (Value < CurrentMax) THEN
   CurrentTestOk := TRUE
  ELSE
   CurrentTestOK := FALSE;

 IF NOT CurrentTestOk THEN
  TestFailed := TRUE;

 Write(Value:4:0,' mA   (',CurrentMin:4:0,' -',CurrentMax:4:0,' mA)');
 TrueOrFalse(CurrentTestOk);
 RelayOff;
 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'LOCAL PM2534');
END;

PROCEDURE AnalogProbeTest;
{Test af de analoge sp‘ndinger +5Va, -5Va og A/D-ref. Der testes med m†leproben
 fra lyskassen.
 M†lev‘rdier og tolerancer er givet som konstanter efter keyword CONST.}
VAR  PM2534Str   : STR_20;
     S1,S2       : STR_10;
     MeasPos,
     MeasNeg,
     MeasRef,
     MinLevel,
     Maxlevel    : REAL;
     k           : INTEGER;
BEGIN
 Analog5VTestValgt := TRUE;
 ADRefTestValgt := TRUE;
 AnalogPos5VTestOk := TRUE;
 AnalogNeg5VTestOk := TRUE;
 ADRefTestOk := TRUE;
 Writeln('Test af analog ñ 5V og A/D-ref?    Ja/Nej');
 IF TastJ <> ScanCode THEN
  BEGIN
   ADRefTestValgt := FALSE;
   Analog5VTestValgt := FALSE;
   Writeln('Test af analog ñ 5V og A/D-ref ikke udf›rt!');
   Exit;
  END; {if}
 Reset(IeIn);
 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'OUTPUT PM2534; RNG 4; MSP 3; OUT N');
 Delay(500);
 Write('M†l med probe p† TP4 - tryk en tast');
 WaitForAnyKey;
 Writeln(IeOut,'ENTER PM2534 LF');
 Readln(IeIn,PM2534Str);
 Val(PM2534Str,MeasPos,k);
 MeasPos := MeasPos - CableLoose;
 MinLevel := ValuePos5Va - (ValuePos5Va * TolLower5Va / 100);
 MaxLevel := ValuePos5Va + (ValuePos5Va * TolLower5Va / 100);
 AnalogPos5VTestOk := (MeasPos > MinLevel) AND (MeasPos < MaxLevel);
 Write(MeasPos:6:2,' V     (',MinLevel:4:2,' - ',MaxLevel:4:2,' V)');
 TrueOrFalse(AnalogPos5VTestOk);
 IF NOT AnalogPos5VTestOk THEN
  TestFailed := TRUE;

 Write('M†l med probe p† TP5 - tryk en tast');
 WaitForAnyKey;
 Reset(IeIn);
 Writeln(IeOut,'ENTER PM2534 LF');
 Readln(IeIn,PM2534Str);
 Val(PM2534Str,MeasNeg,k);
 MeasNeg := MeasNeg - CableLoose;
 MinLevel := ValueNeg5Va + (ValueNeg5Va * TolLower5Va / 100);
 MaxLevel := ValueNeg5Va - (ValueNeg5Va * TolLower5Va / 100);
 AnalogNeg5VTestOk := (MeasNeg > MinLevel) AND (MeasNeg < MaxLevel);
 Write(MeasNeg:6:2,' V     (',MinLevel:4:2,' - ',MaxLevel:4:2,' V)');
 TrueOrFalse(AnalogNeg5VTestOk);
 IF NOT AnalogNeg5VTestOk THEN
  TestFailed := TRUE;

 Write('M†l med probe p† TP8 - tryk en tast ');
 WaitForAnyKey;
 Reset(IeIn);
 Writeln(IeOut,'ENTER PM2534 LF');
 Readln(IeIn,PM2534Str);
 Val(PM2534Str,MeasRef,k);
 MeasRef := MeasRef - CableLoose;
 MinLevel := ValueRef - (ValueRef * TolRef / 100);
 MaxLevel := ValueRef + (ValueRef * TolRef / 100);
 ADRefTestOk:= (MeasRef > MinLevel) AND (MeasRef < MaxLevel);
 Write(MeasRef:6:3,' V     (',MinLevel:4:3,' - ',MaxLevel:4:3,' V)');
 TrueOrFalse(ADRefTestOk);
 IF NOT ADRefTestOk THEN
  TestFailed := TRUE;

 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'LOCAL PM2534');
END;
