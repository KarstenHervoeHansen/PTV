Program MenuGen;
{
Menuprogram for start af testprogrammer for PM5639/82/83 generator.
Peter Frederiksen, 940921

Dato    Rettelse


}

{$M $2000,0,100000}
{$D+}     {debug kontrol}
{$R-}     {intervalkontrol}
{$S-}     {stack check}
{$B-}     {short boolean}

uses  Crt,Dos,PepUtil,BgiDriv,BgiFont,Graph,Graph39;

var  Slut   : BOOLEAN;
     FKey,
     DosErr : BYTE;

Procedure GraphAbort(Msg:STRING);
begin
 Writeln(Msg,': ',GraphErrorMsg(GraphResult));
 Halt(1);
end;

Procedure CRTTextMode;
begin
 CloseGraph;
 TextMode(CO80);
 TextColor(LightGray); TextBackground(Black);
 ClrScr;
end;




begin              { ---->>>> main <<<<---- }

{    IF RegisterBGIDriver(@HercDriverProc)  < 0 THEN GraphAbort('Herc');
 IF RegisterBGIDriver(@CGADriverProc)   < 0 THEN GraphAbort('CGA'); }
 IF RegisterBGIDriver(@EGAVGADriverProc) < 0 THEN GraphAbort('EGA/VGA');

 IF RegisterBGIFont(@TriplexFontProc)    < 0 THEN GraphAbort('TriplexFont');
 IF RegisterBGIFont(@SansSerifFontProc)  < 0 THEN GraphAbort('SansSerifFont');
 IF RegisterBGIFont(@SmallFontProc)     < 0 THEN GraphAbort('SmallFont');
{   IF RegisterBGIFont(@GothicFontProc)    < 0 THEN GraphAbort('GothicFont');}


 REPEAT
   Slut := FALSE;
   InitialiseGraphics;
   HovedmenuGen;
   MakeTestMenuGen;

     REPEAT
        EmptyKeyboardBuffer;
        FKey := ScanCode;
          CASE FKey OF
           F1: Exec('\TEST\GEN39.EXE','PAL');        {/83 PAL  }

           F2: Exec('\TEST\GEN39.EXE','NTSC');       {/83 NTSC }

           F3: Exec('\TEST\GEN39.EXE','625');        {/82 G625 }

           F4: Exec('\TEST\GEN39.EXE','525');       {/82  M525 }

           F9: Exec('\TEST\COM82.EXE','');

          F10: begin
                CRTTextMode;
                Exec('\BATF\SHUTDOWN.COM','');
               end;

       Escape: Slut := TRUE;

          end; {case}
     DosErr := DosError;
     until FKey IN [F1..F4,F9,F10,Escape];

 until Slut;

 CRTTextMode;
end.


