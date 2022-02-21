Unit Timer;
{ -------------------------------------------------------------------------
  Unit for use built-in PC-Timer ticks and watch.
  The unit does not change the clock.
  The unit initializes it self, when activated.
  Following procedures & functions are offered :

     Set_Timer (Time_100mSec) : Sets the count down to Time_100mSec and
                                starts the timer. No action is done when
                                count down is reached, but user can test
                                the timer status by using Timer_Timeout
                                function.

     Stop_Timer               : Stop the timer whether it is running or not.

     Timer_Timeout            : Returns True if the timer has run out
                                else False.

     Set_Timer_Procedure(ProcVec, Cycle_time) :
                                Tells the timer unit what procedure the
                                user wants to be called every timer tick.
                                A timer tick is generated every 50mSec.
                                The procedure MUST be compiled with
                                far option (F+). Remember that In/Out
                                activities on interrupt may caused problems
                                if your program while interrupted, was doing
                                some Input/Output. One solution is to have
                                procedures/function that does input/output
                                with interrupt disabled. Ex :

                                Procedure User_Write(S:String);
                                Begin
                                   Inline($fa);  (Disables interrupt)
                                   Write(S);     (Write stuff);
                                   Inline($fb);  (Enables interrupt)
                                End;
                                Another solution is to write your own
                                Input/Output procedures in assmebler.

     Stop_Timer_Procedure     : Stops the user defined timer procedure.

     Set_Watch(X,Y)           : Establish x,y coordinates for watch.

     Enable_Watch             : Enables watch on x,y coordinates.

     Disable_Watch            : Disables watch.

     Disable_Timer_Interrupt  : Resets Timer-interrupt to origin.

     Enable_Timer_Interrupt   : Save origin timer-interrupt and
                                sets own timer-interrupt.
  ------------------------------------------------------------------------- }

Interface
uses Dos,Crt;

Procedure Set_Timer(Time_100mSec: Integer);
Procedure Stop_Timer;
Function  Timer_Timeout : Boolean;
Procedure Set_Timer_Procedure (ProcVec:Pointer;Cycle_Time:Integer);
Procedure Stop_Timer_Procedure;
Procedure Set_Watch(X,Y:Byte);
Procedure Enable_Watch;
Procedure Disable_Watch;
Procedure Disable_Timer_Interrupt;
Procedure Enable_Timer_Interrupt;


implementation

{ ----------------------------
  Private Variables
  ---------------------------- }
Const
   Timer_Int_Type : Byte = $1c;
   Pic_Adr        : Byte = $20;  {Programmable Interrupt Controller address.}
   Eoi            : Byte = $20;  {Bit In Interrupt Controller. To Be Set}
                                 {At The End Of A Interrupt Routine}
   Watch_Countout : Byte = 20;   {Approx. 1 Sec}

Var
   Timer_Enabled,
   Timer_Out        : Boolean;
   Old_TimerInt_Vec : Pointer;
   Timer_Time,
   Timer_Timeout_Ctr: Integer;
   Timer_Procedure_Schedule,
   Timer_Procedure_Recycle : Integer;
   Timer_Procedure  : Pointer;
   Exit_Timer_Unit  : Pointer;
   Watch_Ctr        : Byte;
   Watch_Initiated,
   Watch_Enabled    : Boolean;

{ ----------------------------
  Private procedure
  ---------------------------- }
{$F+}
Procedure Watch_Procedure; external;
Procedure Init_Watch_Procedure(Videopos:Word); external;
{$F-}
{$L watchasm.obj}

procedure FarCall(ProcAddr : Pointer);
  {-ProcAddr is the address of a routine to be called far. Can be used to
    implement jump tables if procedures take no parameters.}
  inline(
    $89/$E3/                 {mov bx,sp}
    $36/$FF/$1F/             {call far dword ptr ss:[bx]}
    $81/$C4/$04/$00);        {add sp,4}

{$R-,I-,S-,V-}
Procedure Trans_Port (TPort,T_Chr:Word);
{ This Procedure sends T_Chr to TPort.
  Used to send EOI to Programmable Interrupt Controller }
Inline ($58/            { POP Ax ; Charecter to AL }
        $5A/            { POP DX ; Port Nr }
        $EE);           { OUT DX,AL ; Transmit charecter }
{$R+,I+,S+,V+}

{$F+}
Procedure Exit_timer;
{ Exit procedure. Resets interrupt vector etc. }
{$F-}
Begin
  SetIntVec(Timer_Int_Type, Old_TimerInt_Vec);
  inline($fa);
  Timer_Enabled := False;
  Timer_Procedure := Nil;
  Inline ($fb);                  {enable int}
  ExitProc := Exit_Timer_Unit;
end;


{$F+}
Procedure Timer_Int;
{ This procedure will be called every 1/20 sec,
  when the unit has been initialized. }
{$F-}
Interrupt;
Begin
   if Timer_Procedure <> nil then { Is there an user procedure to call? }
      if Timer_Procedure_Schedule = 0 Then
      Begin
         FarCall(Timer_Procedure);
         Timer_Procedure_Schedule := Timer_Procedure_Recycle;
      End;

   If Timer_Enabled then          { Is the timer started ? }
    Begin
      Inc(Timer_Time);
      If Timer_Time >= Timer_Timeout_Ctr
       Then
        Begin
          Timer_Enabled      := False;
          Timer_out          := True;
          Timer_Time         := 0;
          Timer_Timeout_Ctr  := 0;
        End;
     End;
   If Watch_Enabled Then
         Watch_Procedure;
   Trans_Port(Pic_Adr,Eoi);       {Tell Int-controller That This Is End Of Int.}
 End;

{ ----------------------------
  Public procedures
  ---------------------------- }

Procedure Set_Timer(Time_100mSec: Integer);
{ Set the timeout value and enable timer countdown }
Begin
  If Time_100mSec > 0 Then
  Begin
   Timer_Enabled := False;
   Timer_Timeout_Ctr := (Time_100mSec div 2) * 4;
   Timer_Time := 0;
   Timer_Out  := False;
   Timer_Enabled := True;
  End
  Else
   Timer_Out := True;
End;

Procedure Stop_Timer;
{ Stop the timer by clearing the enable flag }
Begin
   Timer_Enabled := False;
End;

Function Timer_Timeout : Boolean;
{ This function is used by user to see if the timer has run out }
Begin
   Timer_Timeout := Timer_Out;
end;

Procedure Set_Timer_Procedure (ProcVec:Pointer;Cycle_Time:Integer);
{ Called by user to establish a user defined procedure to be called.
  The user procedure is called every 50mSec. }
Begin
   Timer_Procedure := ProcVec;
   Timer_Procedure_Schedule := Cycle_Time;
   Timer_Procedure_Recycle := Cycle_Time;
end;

Procedure Stop_Timer_Procedure;
{ This procedure stops the calling of user defined procedure
  on timer ticks. }
Begin
   Timer_Procedure := nil;
End;

Procedure Set_Watch(X,Y:Byte);
Var
   XX,YY,YX : Word;
Begin
   XX := X-1; YY := Y - 1;
   YX := (YY Shl 8) Or XX;
   Init_Watch_Procedure(YX);
   Watch_Initiated := True;
end;

Procedure Enable_Watch;
Begin
   If Watch_Initiated then
   Begin
      Watch_Ctr := 0;
      Watch_Enabled := True;
      Watch_Procedure;
   End;
End;

Procedure Disable_Watch;
Begin
   If Watch_Initiated then
      Watch_Enabled := False;
End;

Procedure Disable_Timer_Interrupt;
Begin
  SetIntVec(Timer_Int_Type, Old_TimerInt_Vec);
End;

Procedure Enable_Timer_Interrupt;
Begin
  GetIntVec(Timer_Int_Type,Old_TimerInt_Vec);
  SetIntVec(Timer_Int_Type,@Timer_Int);
End;

{ ----------------------------------------------------------------------
   Timer Unit Initialization
  ---------------------------------------------------------------------- }
Begin
  Exit_Timer_Unit := ExitProc;
  ExitProc := @Exit_Timer;
  Timer_Enabled := False;
  Timer_Out     := False;
  Timer_Time    := 0;
  Timer_Timeout_Ctr := 0;
  Timer_Procedure := nil;
  Watch_Initiated := False;
  Watch_Enabled   := False;
  Enable_Timer_Interrupt;
end.
