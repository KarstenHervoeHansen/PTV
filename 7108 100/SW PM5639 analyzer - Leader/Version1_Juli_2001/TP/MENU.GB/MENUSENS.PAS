Program MenuSens;
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

}

{$M $2000,0,100000}
{$D+}     {debug kontrol}
{$R-}     {intervalkontrol}
{$S-}     {stack check}
{$B-}     {short boolean}

USES  Crt,Dos,PepUtil,BgiDriv,BgiFont,Graph,Graph39;

VAR  Slut   : BOOLEAN;
     FKey,
     DosErr : BYTE;

PROCEDURE GraphAbort(Msg:STRING);
BEGIN
 Writeln(Msg,': ',GraphErrorMsg(GraphResult));
 Halt(1);
END;

PROCEDURE CRTTextMode;
BEGIN
 CloseGraph;
 TextMode(CO80);
 TextColor(LightGray); TextBackground(Black);
 ClrScr;
END;


PROCEDURE PCOff;
BEGIN
 CRTTextMode;
 Exec('\BIN\SHUTDOWN.COM','');
END;



BEGIN              { ---->>>> main <<<<---- }

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
           F1: Exec('\BIN\PTV39.EXE','');          {/90 CRT sensor serie I}

           F2: Exec('\BIN\CRT39.EXE ','90');       {/90 CRT sensor serie II}

           F3: Exec('\BIN\PROJ39.EXE','');

           F4: Exec('\BIN\CRT39.EXE ','92');       {/92 Industrial sensor}

           F5: Exec('\BIN\PESA39.EXE','PESA');

           F6: Exec('\BIN\PESA39.EXE','VISTEK');

           F7: Menu900;

           F9: Exec('\BIN\COM5639.EXE','');

          F10: PCOff;

       Escape: Slut := TRUE;

          END; {case}
     DosErr := DosError;
     UNTIL FKey IN [F1..F7,F9,F10,Escape];

 UNTIL Slut;

 CRTTextMode;
END.


