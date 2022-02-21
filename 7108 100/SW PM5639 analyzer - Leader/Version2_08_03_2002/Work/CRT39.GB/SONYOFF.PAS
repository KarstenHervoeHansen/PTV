Program SonyOff;
(*
  Peter Frederiksen, november 1992.

  Sony monitorer = off
  Vidiwall monitor = off
  Lamper i lyskasse = off

  Rettelser:
  950329:    Lampe = off ‘ndret idet styresignal er inverteret.

*)
uses Crt, Unit_IIC;

const  I2CIOAddr = $300;
var St,
    IICPort64,
    IICPort66,
    IICPort68,
    IICPort70   : BYTE;


PROCEDURE LampOff;
begin
 IICPort64 := IICPort64 or $01;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(5);
 IICPort68 := IICPort68 AND $1B;         {Remote = ON}
 Trm8574(I2CIOAddr,68,IICPort68,St);
 Delay(5);
 IICPort68 := IICPort68 AND $1F;         {Lampe i 5639/91 lyskasse = off}
 Trm8574(I2CIOAddr,68,IICPort68,St);
 Delay(5);
end;

PROCEDURE EBUBlack;
begin
 IICPort66 := IICPort66 AND $F8;
 Trm8574(I2CIOAddr,66,IICPort66,St);
 Delay(10);
end;

PROCEDURE SMPTEBlack;
BEGIN
 IICPort66 := IICPort66 AND $C7;
 Trm8574(I2CIOAddr,66,IICPort66,St);
 Delay(10);
END;

PROCEDURE VidiwallOff;
BEGIN
 IICPort70 := IICPort70 AND $78;       {Remote = ON}
 Trm8574(I2CIOAddr,70,IICPort70,St);
 Delay(10);
END;

BEGIN
 InitIIC(I2CIOAddr,St);
 IF St <> 0 THEN
  BEGIN
   Writeln(#7,'IIC-bus error !');
   Exit;
  END;
 IICPort64 := $FF;
 IICPort66 := $FF;
 IICPort68 := $FF;
 IICPort70 := $FF;
 Trm8574(I2CIOAddr,68,$FF,St);
 Trm8574(I2CIOAddr,66,$FF,St);
 delay(1000);
 LampOff;
 SMPTEBlack;
 EBUBlack;
 VidiwallOFF;
 Writeln('SONY monitors = OFF');
 Writeln('Vidiwall monitor = OFF');
 Writeln('Lamper = OFF');
END.