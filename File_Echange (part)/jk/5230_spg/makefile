
# Makefile for 5230_spg   project

CFLAGS = -q -C -BL -O -E+SPG.err
LFLAGS = 
AFLAGS =

CC = c:\ht-xa\bin\xac
LN = c:\ht-xa\bin\hlink
OH = c:\ht-xa\bin\objtohex

LIBS  = 
START = 

.c.obj:
      $(CC) $(CFLAGS) $&.c

OBJECTS = SYSTEM.OBJ \
          SYNC.OBJ \    
          LOCK.OBJ \    
          FIEL.OBJ \    
          LINE.OBJ \    
          SERI1.OBJ \   
          TABKORG.OBJ \ 
          TABKORM.OBJ \ 
          TABPOSG.OBJ \ 
          TABPOSM.OBJ \ 
          TABDIVI.OBJ \ 
          TABPYTM.OBJ \ 
          TABPYTG.OBJ \ 
          TABLETYP.OBJ \
          TABPHASE.OBJ 
          
                    
pt5230.hex: $(OBJECTS)
  $(LN) @linkrsp
  $(OH) -I C:.\l.obj SPG.hex

SYSTEM.C  :        SYSTEM.OBJ  
SYNC.C    :        SYNC.OBJ    
LOCK.C    :        LOCK.OBJ    
FIEL.C    :        FIEL.OBJ    
LINE.C    :        LINE.OBJ    
SERI1.C   :        SERI1.OBJ   
TABKORG.C :        TABKORG.OBJ 
TABKORM.C :        TABKORM.OBJ 
TABPOSG.C :        TABPOSG.OBJ 
TABPOSM.C :        TABPOSM.OBJ 
TABDIVI.C :        TABDIVI.OBJ 
TABPYTM.C :        TABPYTM.OBJ 
TABPYTG.C :        TABPYTG.OBJ 
TABLETYP.C:        TABLETYP.OBJ
TABPHASE.C:        TABPHASE.OBJ
                              
