Program Menu5639;
{
Menuprogram for start af testprogrammer for PM5639 sensorer.
Peter Frederiksen, P-DPE
911104          Menuprogram for PM5639/90 og PESA color probe

Rettelser:
921123    PM5639/91 projector tilf›jet til menu
930107    PM5639/92 tilf›jet til menu
930625    PM5639/90 serie III tilf›jet til menu
931105    VISTEK color probe tilf›jet til menu
          tilf›jet til menuen.
940622:   menu for /900 numre tilf›jet   (/916)
010108:   translated to English: JK
}

{$M $2000,0,70000}
{$D+}     {debug kontrol}
{$R-}     {intervalkontrol}
{$S-}     {stack check}
{$B-}     {short boolean}

uses  Crt,Dos,PepUtil,BgiDriv,BgiFont,Graph,Graph39;

var  Slut   : BOOLEAN;
     FKey,
     DosErr : BYTE;

PROCEDURE GraphAbort(Msg:STRING);
begin
 Writeln(Msg,': ',GraphErrorMsg(GraphResult));
 Halt(1);
end;

PROCEDURE CRTTextMode;
begin
 CloseGraph;
 TextMode(CO80);
 TextColor(LightGray); TextBackground(Black);
 ClrScr;
end;


PROCEDURE PCOff;
begin
 CRTTextMode;
 Exec('\BIN\SHUTDOWN.COM','');
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
   HovedmenuSens;
   MakeTestMenuSens;

     REPEAT
        EmptyKeyboardBuffer;
        FKey := ScanCode;
          CASE FKey OF
(*           F1: Exec('\BIN\PTV39.EXE','');        {/90 CRT sensor series I+II}*)
           F2: Exec('\bin\CRT39GB.EXE ','90 3');   {/90 CRT sensor series III}
           F3: Exec('\bin\CRT39GB.EXE ','90 4');   {/90 CRT sensor series IV}
           F4: Exec('\bin\CRT39GB.EXE ','90 5');   {/90 CRT sensor series V}

           F5: Exec('\BIN\CRT39GB.EXE ','92 1');   {/92 Ind. sensor  series I}
           F6: Exec('\BIN\CRT39GB.EXE ','92 2');   {/92 Ind. sensor  series II}
           F7: Exec('\BIN\CRT39GB.EXE ','92 3');   {/92 Ind. sensor  series III}

           F8: Exec('\BIN\CRT39GB.EXE','93 2');  {5639/93 Mini sensor series II}
           F9: Exec('\BIN\CRT39GB.EXE','93 3');  {5639/93 Mini sensor series III}

           F10: SubMenu;

       Escape: Slut := TRUE;

          end; {case}
     DosErr := DosError;
     UNTIL FKey IN [F1..F10,Escape];

 UNTIL Slut;

 CRTTextMode;
end.


