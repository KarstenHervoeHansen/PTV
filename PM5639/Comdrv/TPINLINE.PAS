{$S-,R-,V-,I-,B-,F-}

{$IFNDEF Ver40}
  {$I OMINUS.INC}
{$ENDIF}

{*********************************************************}
{*                  TPINLINE.PAS 5.08                    *}
{*        Copyright (c) TurboPower Software 1987.        *}
{* Portions copyright (c) Sunny Hill Software 1985, 1986 *}
{*     and used under license to TurboPower Software     *}
{*                 All rights reserved.                  *}
{*********************************************************}

unit TpInline;
  {-Assorted inline macros}

interface

type
  JumpRecord =
    record
      SpReg, BpReg : Word;
      JmpPt : Pointer;
    end;

procedure SetJump(var JumpDest : JumpRecord);
  {-Save current SP, BP, and a jump destination}
  inline(
    $5F/                     {pop di           ;di = Ofs(JmpDest)}
    $07/                     {pop es           ;es = Seg(JmpDest)}
    $26/$89/$25/             {mov es:[di],sp   ;save sp}
    $26/$89/$6D/$02/         {mov es:[di+2],bp ;save bp}
    $E8/$00/$00/             {call null        ;push IP onto stack}
                             {null:}
    $58/                     {pop ax           ;pop into ax}
    $05/$0C/$00/             {add ax,12        ;point to "next:"}
    $26/$89/$45/$04/         {mov es:[di+4],ax ;save jump offset}
    $26/$8C/$4D/$06);        {mov es:[di+6],cs ;save jump segment}
                             {next:}

procedure LongJump(var JumpDest : JumpRecord);
  {-Restore SP, BP, and jump to JumpDest.JmpPt}
  inline(
    $5F/                     {pop di            ;di = Ofs(JumpDest)}
    $07/                     {pop es            ;es = Seg(JumpDest)}
    $26/$8B/$25/             {mov sp,es:[di]    ;restore sp}
    $26/$8B/$6D/$02/         {mov bp,es:[di+2]  ;restore bp}
    $26/$FF/$6D/$04);        {jmp far es:[di+4] ;jump far to JumpDest.JmpPt}

procedure FarCall(ProcAddr : Pointer);
  {-ProcAddr is the address of a routine to be called far. Can be used to
    implement jump tables if procedures take no parameters.}
  inline(
    $89/$E3/                 {mov bx,sp}
    $36/$FF/$1F/             {call far dword ptr ss:[bx]}
    $81/$C4/$04/$00);        {add sp,4}

procedure NearCall(ProcOfs : Word);
  {-ProcOfs is the offset of a routine to be called near.}
  inline(
    $5B/                     {pop bx}
    $FF/$D3);                {call bx}

procedure JumpToOldIsr(OldIsr : Pointer);
  {-Jump to previous ISR from an interrupt procedure.}
  inline(
    $5B/                     {pop bx          ;BX = Ofs(OldIsr)}
    $58/                     {pop ax          ;AX = Seg(OldIsr)}
    $87/$5E/$0E/             {xchg bx,[bp+14] ;Switch old BX and Ofs(OldIsr)}
    $87/$46/$10/             {xchg ax,[bp+16] ;Switch old AX and Seg(OldIsr)}
    $89/$EC/                 {mov sp,bp       ;Restore SP}
    $5D/                     {pop bp          ;Restore BP}
    $07/                     {pop es          ;Restore ES}
    $1F/                     {pop ds          ;Restore DS}
    $5F/                     {pop di          ;Restore DI}
    $5E/                     {pop si          ;Restore SI}
    $5A/                     {pop dx          ;Restore DX}
    $59/                     {pop cx          ;Restore CX}
                             {;BX and AX restored earlier; their places on stack}
                             {;now have OldIsr, which is where return will go}
    $CB);                    {retf            ;Chain to OldIsr}

procedure CallOldIsr(OldIsr : Pointer);
  {-Call previous ISR from an interrupt procedure. Destroys BX.}
  inline(
    $89/$E3/                 {mov bx,sp        ;set up stack frame}
    $9C/                     {pushf            ;push flags to simulate int}
    $36/$FF/$1F/             {call far ss:[bx] ;call OldIsr}
    $81/$C4/$04/$00);        {add sp,4         ;get rid of OldIsr}

function HiWord(L : LongInt) : Word;
  {-Return high-order word of L}
  inline(
    $58/                     {pop ax ;ignore low word}
    $58);                    {pop ax ;pop high word into AX}

function LoWord(L : LongInt) : Word;
  {-Return low-order word of L}
  inline(
    $58/                     {pop ax ;pop low word into AX}
    $5A);                    {pop dx ;ignore high word}

function SwapNibble(B : Byte) : Byte;
  {-Swap the high and low nibbles of B: SwapNibble($F0) returns $0F.}
  inline(
    $58/                     {pop ax     ;AL = B}
    $88/$C4/                 {mov ah,al  ;AH = B}
    $B9/$04/$00/             {mov cx,4   ;4 bits/nibble}
                             {Start:}
    $D0/$CC/                 {ror ah,1   ;rotate rightmost bit of AH}
    $D0/$D8/                 {rcr al,1   ;into leftmost bit of AL}
    $E2/$FA);                {loop Start ;do it four times}

function SwapWord(L : LongInt) : LongInt;
  {-Swap low- and high-order words of L}
  inline(
    $5A/                     {pop dx ;pop low word into DX}
    $58);                    {pop ax ;pop high word into AX}

function Normalized(P : Pointer) : Pointer;
  {-Return P as a normalized pointer}
  inline(
    $58/                     {pop ax    ;pop offset into AX}
    $5A/                     {pop dx    ;pop segment into DX}
    $89/$C3/                 {mov bx,ax ;BX = Ofs(P^)}
    $B1/$04/                 {mov cl,4  ;CL = 4}
    $D3/$EB/                 {shr bx,cl ;BX = Ofs(P^) div 16}
    $01/$DA/                 {add dx,bx ;add BX to segment}
    $25/$0F/$00);            {and ax,$F ;mask out unwanted bits in offset}

procedure SetFlag(var Flags : Word; FlagMask : Word);
  {-Set the bit(s) specified by FlagMask in Flags}
  inline(
    $58/                     {pop ax        ;FlagMask into AX}
    $5F/                     {pop di}
    $07/                     {pop es        ;ES:DI => Flags}
    $26/$09/$05);            {or es:[di],ax ;Flags := Flags or FlagMask}

procedure ClearFlag(var Flags : Word; FlagMask : Word);
  {-Clear the bit(s) specified by FlagMask in Flags}
  inline(
    $58/                     {pop ax         ;FlagMask into AX}
    $5F/                     {pop di}
    $07/                     {pop es         ;ES:DI => Flags}
    $F7/$D0/                 {not ax         ;AX := not AX}
    $26/$21/$05);            {and es:[di],ax ;Flags := Flags and not FlagMask}

function FlagIsSet(Flags, FlagMask : Word) : Boolean;
  {-Returns True if the bit specified by FlagMask is set in Flags}
  inline(
    $5A/                     {pop dx    ;FlagMask into DX}
    $58/                     {pop ax    ;Flags into AX}
    $21/$D0/                 {and ax,dx ;Mask out everything not in FlagMask}
    $74/$03/                 {jz  Exit}
    $B8/$01/$00);            {mov ax,1  ;AX = Ord(True)}
                             {Exit:}

procedure ExchangeBytes(var I, J : Byte);
  {-Exchange bytes I and J. Useful in sorts}
  inline(
    $8C/$DB/                 {mov bx,ds       ;save DS}
    $5E/                     {pop si}
    $1F/                     {pop ds          ;DS:SI => J}
    $5F/                     {pop di}
    $07/                     {pop es          ;ES:DI => I}
    $8A/$04/                 {mov al,[si]     ;AL = J}
    $26/$86/$05/             {xchg al,es:[di] ;I = J, AL = I}
    $88/$04/                 {mov [si],al     ;J = I}
    $8E/$DB);                {mov ds,bx       ;restore DS}

procedure ExchangeWords(var I, J : Word);
  {-Exchange words I and J. Useful in sorts}
  inline(
    $8C/$DB/                 {mov bx,ds       ;save DS}
    $5E/                     {pop si}
    $1F/                     {pop ds          ;DS:SI => J}
    $5F/                     {pop di}
    $07/                     {pop es          ;ES:DI => I}
    $8B/$04/                 {mov ax,[si]     ;AX = J}
    $26/$87/$05/             {xchg ax,es:[di] ;I = J, AX = I}
    $89/$04/                 {mov [si],ax     ;J = I}
    $8E/$DB);                {mov ds,bx       ;restore DS}

procedure ExchangeStructs(var I, J; Size : Word);
  {-Exchange structures I and J. Useful in sorts}
  inline(
    $FC/                     {cld             ;go forward} {!!.08}
    $8C/$DA/                 {mov dx,ds       ;save DS}
    $59/                     {pop cx          ;CX = Size}
    $5E/                     {pop si}
    $1F/                     {pop ds          ;DS:SI => J}
    $5F/                     {pop di}
    $07/                     {pop es          ;ES:DI => I}
    $D1/$E9/                 {shr cx,1        ;move by words}
    $E3/$0C/                 {jcxz odd}
    $9C/                     {pushf}
                             {start:}
    $89/$F3/                 {mov bx,si}
    $26/$8B/$05/             {mov ax,es:[di]  ;exchange words}
    $A5/                     {movsw}
    $89/$07/                 {mov [bx],ax}
    $E2/$F6/                 {loop start      ;again?}
    $9D/                     {popf}
                             {odd:}
    $73/$07/                 {jnc exit}
    $8A/$04/                 {mov al,[si]     ;exchange the odd bytes}
    $26/$86/$05/             {xchg al,es:[di]}
    $88/$04/                 {mov [si],al}
                             {exit:}
    $8E/$DA);                {mov ds,dx       ;restore DS}

function MinWord(A, B : Word) : Word;
  {-Returns the smaller of A and B}
  inline(
    $58/                     {pop ax}
    $5B/                     {pop bx}
    $39/$C3/                 {cmp bx,ax}
    $73/$02/                 {jae done}
    $89/$D8);                {mov ax,bx}
                             {done:}

function MaxWord(A, B : Word) : Word;
  {-Returns the greater of A and B}
  inline(
    $58/                     {pop ax}
    $5B/                     {pop bx}
    $39/$C3/                 {cmp bx,ax}
    $76/$02/                 {jbe done}
    $89/$D8);                {mov ax,bx}
                             {done:}

procedure FillWord(var Dest; Count, Filler : Word);
  {-Fill memory starting at Dest with Count instances of Filler}
  inline(
    $58/                     {pop ax    ;AX = Filler}
    $59/                     {pop cx    ;CX = Count}
    $5F/                     {pop di    ;ES:DI => Dest}
    $07/                     {pop es}
    $FC/                     {cld       ;go forward}
    $F2/$AB);                {rep stosw ;fill memory}

procedure FillStruct(var Dest; Count : Word; var Filler; FillerSize : Word);
  {-Fill memory starting at Dest with Count instances of Filler}
  inline(
    $58/                     {pop ax     ;AX = FillerSize}
    $5B/                     {pop bx     ;DX:BX => Filler}
    $5A/                     {pop dx}
    $59/                     {pop cx     ;CX = Count}
    $5F/                     {pop di     ;ES:DI => Dest}
    $07/                     {pop es}
    $E3/$11/                 {jcxz done  ;done if Count = 0}
    $FC/                     {cld        ;go forward}
    $1E/                     {push ds    ;save DS}
    $8E/$DA/                 {mov ds,dx  ;DS:BX => Filler}
                             {again:}
    $89/$CA/                 {mov dx,cx  ;save loop count}
    $89/$DE/                 {mov si,bx  ;DS:SI => Filler}
    $89/$C1/                 {mov cx,ax  ;CX = FillerSize}
    $F2/$A4/                 {rep movsb  ;fill}
    $89/$D1/                 {mov cx,dx  ;restore loop count}
    $E2/$F4/                 {loop again ;repeat}
    $1F);                    {pop ds     ;restore DS}
                             {done:}

  {==========================================================================}

implementation

end.
