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


function RealDrawColor(Color : Word) : Word;
begin
  if (GetMaxColor > 256) then
    SetRgbPalette(1024,(Color SHR 10) AND 31,(Color SHR 5) AND 31,Color AND 31);
  RealDrawColor := Color;
end;

