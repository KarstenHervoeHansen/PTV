program aes_lock;
uses crt,iic_lpt;


var n : byte;
 ackn : boolean;
 ch   : char;

procedure soundselect (sys, lev , audio_on: byte; var ack : boolean);
begin
  write8574(64,sys*4+lev+audio_on*64,ack);
end;


begin
  soundselect(8,1,1,ackn); { set sound system }
  soundselect(8,1,0,ackn); { set sound system }

repeat
  n := 0+$f8; write8574(66,n,ackn);
until keypressed;

end.
  n := 1+$f8; write8574(66,n,ackn);
  n := 3+$f8; write8574(66,n,ackn);
  n := 5+$f8; write8574(66,n,ackn);
  n := 2+$f8; write8574(66,n,ackn);
  n := 4+$f8; write8574(66,n,ackn);


  n := 1; write8574(66,n,ackn);  write8574(66,n+8,ackn);  delay(1); write8574(66,n,ackn);

  n := 3; write8574(66,n,ackn);
  write8574(66,n+8,ackn);  delay(1); write8574(66,n,ackn);
  n := 5; write8574(66,n,ackn);
  write8574(66,n+8,ackn);  delay(1); write8574(66,n,ackn);
  n := 2; write8574(66,n,ackn);
  write8574(66,n+8,ackn);  delay(1); write8574(66,n,ackn);
  n := 4; write8574(66,n,ackn);
  write8574(66,n+8,ackn);  delay(1); write8574(66,n,ackn);
