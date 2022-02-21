

The purpose of this project is to modify standard PT8633 picture data and 
software in order to have patterns W10%, W15% and W20% cover full screen
instead of being windows.

The modification was requested by MEDIAPLAZA in Germany.

Modification of standard PT8633 data is done with makelogo.c.  It consists 
of replacing lines used normally for W10%, W15% and W20% with data covering 
whole screen.  This is accomplished by copying data from middle of lines
used for windows to begining and end of lines.  The boundaries are the same
used for lines 22 and 23, which are normally used for displaying White100%
full screen pattern.  Funcion shuffle_data() does that.  Affected lines are:

            PATT          W10    W15    W20 
          ______________________________________
     
             Line         204     26     28
             Line+1       205     27     29

To display full screen patterns, two lines are necessary, whereas to display
windowed patterns several other lines are necessary (marker lines, etc).

To run makelogo you must have channelx.dat files with standard PT8633
picture data in the current directory.

Changes in standard PT8633 software are necessary due to fact, that the 
software uses different algorithms for displaying windowed patterns and
full screen patterns.

Affected files are:
                       D1PATTG.C 
and                    D1PATT.H

where #ifdef MEDIAPLAZA defines line structures used for this project and 
for standard software (in D1PATT.H).


18.01.2005
:JK
***
