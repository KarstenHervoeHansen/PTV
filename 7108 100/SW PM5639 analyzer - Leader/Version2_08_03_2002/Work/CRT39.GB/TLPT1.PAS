program lpt1;

uses
  dos, crt;

var
  buf : char;
  Data : byte;

procedure SetLPT1Data( RGB : char );
begin
  port[$378] := $00;
  case RGB of
    'R' : begin
            port[$378] := $01;
          end;
    'G' : begin
            port[$378] := $02;
          end;
    'B' : begin
            port[$378] := $04;
          end;
  end;
end;


begin
  writeln('Start');
  repeat
    buf := readkey;
    writeln( buf );
    case buf of
      'r','R' : SetLPT1Data( 'R' );
      'g','G' : SetLPT1Data( 'G' );
      'b','B' : SetLPT1Data( 'B' );
    end;
  until buf = #27;
  writeln('Slut');
end.