{ The setting of the N compiler directive within this include file will }
{ determine whether your toolbox programs will use the standard Turbo   }
{ Pascal 6 byte real number or the Turbo Pascal double precision real   }
{ number. In order to use the double precision real number type, you    }
{ must have an 8087 math coprocessor installed in your computer.        }


{$N-} { Change to $N+ if you want to use the 8 byte double precision real }
      { Change to $N- if you want to use the 6 byte non-8087 real number  }
