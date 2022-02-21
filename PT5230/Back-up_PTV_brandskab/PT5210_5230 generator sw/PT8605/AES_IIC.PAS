program aes_iic;
uses crt,iic_lpt;


var n : byte;
 ackn : boolean;
 ch   : char;

procedure soundselect (sys, lev : byte; var ack : boolean);
begin
  write8574(64,sys*4+lev,ack);
end;


begin

soundselect (0, 0, ackn);
soundselect (0, 1, ackn);
soundselect (0, 2, ackn);
soundselect (0, 3, ackn);
soundselect (1, 1, ackn);
soundselect (2, 1, ackn);
soundselect (3, 1, ackn);
soundselect (3, 2, ackn);
soundselect (8, 0, ackn);
soundselect (8, 1, ackn);
soundselect (9, 0, ackn);
soundselect (9, 1, ackn);
soundselect (9, 2, ackn);
soundselect (9, 3, ackn);
if ackn  then writeln('ok');
repeat
until keypressed;

ch := readkey;

end.

soundselect (1, 1);
soundselect (2, 1);
soundselect (3, 1);
soundselect (4, 1);
soundselect (8, 1);
soundselect (9, 1);
