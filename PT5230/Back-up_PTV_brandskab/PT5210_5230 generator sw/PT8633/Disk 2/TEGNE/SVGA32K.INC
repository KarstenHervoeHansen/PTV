(************************************************)
(* 						*)
(*	 SuperVGA 32768 BGI driver defines	*)
(*		Copyright (c) 1991		*)
(*	    Jordan Hargraphix Software		*)
(*						*)
(************************************************)

(* These are the currently supported modes *)
const
  SVGA320x200x32768	= 0;	(* 320x200x32768 HiColor VGA *)
  SVGA640x350x32768	= 1;	(* 640x350x32768 HiColor VGA *)
  SVGA640x400x32768	= 2;	(* 640x400x32768 HiColor VGA *)
  SVGA640x480x32768	= 3;	(* 640x480x32768 HiColor VGA *)
  SVGA800x600x32768	= 4;	(* 800x600x32768 HiColor VGA *)

function RGB(R,G,B : Word) : Word;
begin
  RGB := ((R and 31)SHL 10) OR ((G and 31)SHL 5) OR (B and 31);
end;

function RealDrawColor(Color : Word) : Word;
begin
  if (GetMaxColor > 256) then
    SetRgbPalette(1024,(Color SHR 10) AND 31,(Color SHR 5) AND 31,Color AND 31);
  RealDrawColor := Color;
end;

function RealFillColor(Color : Word) : Word;
begin
  if (GetMaxColor > 256) then
    SetRgbPalette(1025,(Color SHR 10) AND 31,(Color SHR 5)AND 31,Color AND 31);
  RealFillColor := Color;
end;

function RealColor(Color : Word) : Word;
begin
  if (GetMaxColor > 256) then
    SetRgbPalette(1026,(Color SHR 10) AND 31,(Color SHR 5)AND 31,Color AND 31);
  RealColor := Color;
end;
