
(*
Denne fil indeholder f›lgende procedurer:
PROCEDURE SupplyCurrentTest;      {F1}
PROCEDURE AnalogProbeTest;        {F1}
PROCEDURE AnalogProbeTestBarco;   {F1}
*)

PROCEDURE SupplyCurrentTest;
{Test af str›mforbrug til color sensoren. Der m†les over R29 i lyskassen.
 V‘rdien af modstanden R29 er korrigeret i konstanten 'R29'. M†lev‘rdier
 og tolerancer er givet som konstanter efter keyword CONST i
 hovedprogrammerne}
var PM2534Str  : str_20;
    Value,
    CurMin,
    CurMax   : real;
    k          : integer;
    Status     : byte;
    S1,S2      : str_10;
begin
 Write('Test af str›mforbrug:');
 RelayOn;
 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'OUTPUT PM2534; RNG 2; MSP 3; OUT N');
 Delay(500);
 Reset(IeIn);
 Writeln(IeOut,'ENTER PM2534 LF');
 Readln(IeIn,PM2534Str);
 Val(PM2534Str,Value,k);           {der m†les over R29 i lyskassen}
 Value := Value * 1000 / R29;      {til mA}
 case SensorVer of
 93  : begin
         CurMin := CurrentMin + 5;  { /93 sensor bruger mere pga LED}
         CurMax := CurrentMax + 13;
       end;
   else
         CurMin := CurrentMin;
         CurMax := CurrentMax;
 end; { case }

 if (Value > CurMin) and (Value < CurMax) then
   CurrentTestOk := true
  else
   CurrentTestOK := false;

 if not CurrentTestOk then
  TestFailed := TRUE;

 Write(Value:4:0,' mA   (',CurMin:4:0,' -',CurMax:4:0,' mA)');
 TrueOrFalse(CurrentTestOk);
 RelayOff;
 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'LOCAL PM2534');
end;

Procedure AnalogProbeTest;
{Test af de analoge sp‘ndinger +5Va, -5Va og A/D-ref. Der testes med m†leproben
 fra lyskassen.
 M†lev‘rdier og tolerancer er givet som konstanter efter keyword CONST.}
var  PM2534Str   : str_20;
     S1,S2       : str_10;
     MeasPos,
     MeasNeg,
     MeasRef,
     MinLevel,
     Maxlevel    : real;
     k           : integer;
begin
 Analog5VTestValgt := true;
 ADRefTestValgt := true;
 AnalogPos5VTestOk := true;
 AnalogNeg5VTestOk := true;
 ADRefTestOk := true;
 Writeln('Test af analog ñ 5V og A/D-ref?    Ja/Nej');
 if TastJ <> ScanCode then
  begin
   ADRefTestValgt := FALSE;
   Analog5VTestValgt := FALSE;
   Writeln('Test af analog ñ 5V og A/D-ref ikke udf›rt!');
   Exit;
  end; {if}
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
 if not AnalogPos5VTestOk then
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
 AnalogNeg5VTestOk := (MeasNeg > MinLevel) and (MeasNeg < MaxLevel);
 Write(MeasNeg:6:2,' V     (',MinLevel:4:2,' - ',MaxLevel:4:2,' V)');
 TrueOrFalse(AnalogNeg5VTestOk);
 if not AnalogNeg5VTestOk then
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
 if not ADRefTestOk then
  TestFailed := true;

 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'LOCAL PM2534');
end;


Procedure AnalogProbeTestBarco;
{Test af 5V sp‘nding. Der testes med m†leproben fra lyskassen.
 vre og nedre m†lev‘rdi er givet som konstanter efter keyword CONST.}
var  PM2534Str   : str_20;
     MeasValue   : real;
     k           : integer;
begin

 Reset(IeIn);
 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'OUTPUT PM2534; RNG 4; MSP 3; OUT N');
 Delay(500);
 Write('M†l med probe p† regulator V1 pin 3 - tryk en tast');
 WaitForAnyKey;
 Writeln;
 Writeln(IeOut,'ENTER PM2534 LF');
 Readln(IeIn,PM2534Str);
 Val(PM2534Str,MeasValue,k);
 Sensor5VBarcoOk := (MeasValue> Min5VBarco) and (MeasValue< Max5VBarco);
 Write('5V sp‘nding: ',MeasValue:5:2,' VDC     (',Min5VBarco:4:2,' - ',Max5VBarco:4:2,')');
 TrueOrFalse(Sensor5VBarcoOk);
 if not Sensor5VBarcoOk then
  TestFailed := true;

 Writeln(IeOut,'CLEAR PM2534');
 Writeln(IeOut,'LOCAL PM2534');

 Write('Tryk en tast');
 WaitForAnyKey;
end;
