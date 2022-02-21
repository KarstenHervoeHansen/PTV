/* FILE NAME        sound33.c
 * PURPOSE          To generate file with sound data for 8633 SDI .
 *                  generator.
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * TEST
 * LAST UPDT:       10.12.2002
 * NOTES:           
*/

#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

#define SAMPLE_RATE 48E3

#define OUT_FILE    "tstsnd16.dat"
#define TXT_FILE    "tstsnd16.txt"

/* PROTOS */
void open_output_file(int * , int *);
void split(long *, UC *, UI );
void makesinus(long *, double , double , int , UC *, long);
void save2file(int *, UC *, long, long);
void calculate_checksum(UC *);


void main(void)
  {
  int i;
  int b_file;                       /* handles for output files */
  int t_file;
  long sound[5 * 192 * 3 * 4];      /* buffer for sound data   11520 */
  UC    prom[5 * 192 * 3 * 4 * 2];  /* buffer for output data  23040 */
  UC   chan_status[24];             /* array holding channel status block structure */
  UC   tmp[80];
  

  open_output_file(&b_file, &t_file);
  
  sprintf(tmp, "Sound elements for 8633 SDI generator.\n");
  write(t_file, tmp, (long)strlen(tmp));

  /* fill binary file with 0x80000 bytes of 0xFF */
  for (i = 0; i < 0x4000; i++)
    prom[i] = 0xFF;

  for (i = 0; i < 32; i++)
    save2file(&b_file, prom, (long)0x4000, (long)(i * 0x4000));


  /* Silence in stereo, only the Cbit is set */
  for (i = 0; i < 24; i++)
    chan_status[i] = 0;

  chan_status[0]  = 1; /* professional */
  chan_status[1]  = 2; /* stereo       */
  chan_status[23] = 0; /* CRCC = 0     */
  calculate_checksum(chan_status);

  makesinus(sound, 1000.0, 1000.0, 192, chan_status, (long) 0);
#if 0
  split(sound, prom, (192 * 3 * 4));
  save2file(&b_file, prom, (long)(192 * 3 * 4 * 2), (long)(0xCA00));
#endif
  close(b_file);
  close(t_file);
  }





/**************************************************************************/
/* Function: void makesinus()                                             */
/* Purpose:  To generate sound data which is to be embedded into SDI
/*           video.
/*           
/* Args:     long *buff        - buffer where sound data is stored
/*           double freq_left  - frequency for left channel
/*           double freq_right - frequency for right channel
/*           int samples       - number of samples to generate
/*           UC *chan_status   - channel status block array
/*           long level        - required amplitude
/* 
/* Remarks:  This function generates 20 bit sound samples for four channels
/*           and manipulates them into 3 words X, X+1 and X+2, as required 
/*           by AES standard.  The bit distribution for the 3 X words is:
/*            
/*                 Bit          X           X+1         X+2
/*               ==============================================
/*                  b9         !b8         !b8          !b8
/*                  b8         au5         au14        Parity
/*                  b7         au4         au13          C
/*                  b6         au3         au12          U
/*                  b5         au2         au11          V
/*                  b4         au1         au10         au19
/*                  b3         au0         au9          au18
/*                  b2        ch bit1      au8          au17
/*                  b1        ch bit2      au7          au16
/*                  b0         Z bit       au6          au15
/* 
/*           U and V bits are always set to 0.  The C bit is calculated
/*           using properly initialized chan_status array.  There are 
/*           191 bits in chan_status array, with the last 8 bits being
/*           CRCC.  The CRCC is calculated over 183 bits of chan_status.
/*           For each X word, proper C bit is choosen:
/*               C = chan_status[bit_no].
/*           
/*
/* Returns:  Nothing.  On return, buff is filled with audio in the following 
/*           manner:
/*
/*              =========================== buff[0]
/*                +0   X
/*                +1   X+1   Channel 1
/*                +2   X+2
/*              -----------------------
/*                +3   X
/*                +4   X+1   Channel 2
/*                +5   X+2
/*              -----------------------
/*                +6   X
/*                +7   X+1   Channel 3
/*                +8   X+2
/*              -----------------------
/*                +9   X
/*                +10  X+1   Channel 4
/*                +11  X+2
/*              -----------------------
/*                +12  X
/*                +13  X+1   Channel 1
/*                +14  X+2
/*              -----------------------
/*                +15  X
/*                +16  X+1   Channel 2
/*                +17  X+2
/*                 .
/*                 .
/*                 .
/*                 .
/**************************************************************************/

void makesinus(long *buff, double freq_left, double freq_right, 
               int samples, UC *chan_status, long level)
  {
  int i;
  int j;
  int z;
  int ndx;
  long amplitude;
  long parity;
  char cbit;
  char pbit;
  

  for (i = 0; i < samples; i++)
    {
    for (j = 0; j < 4; j++) /* for 4 channels */
      {
      if (j % 2 == 0)       /* if channel 1 */
        amplitude = level * (long) sin(2 * M_PI * freq_left  / SAMPLE_RATE * i);
      else
        amplitude = level * (long) sin(2 * M_PI * freq_right / SAMPLE_RATE * i);

      /* calculate X */
      ndx = 12 * i + 3 * j;
      buff[ndx] = (amplitude << 3) & 0x1FF + ((j >> 1) & 0x06);

      if (i % 192 == 0)      /* if this is frame 0 */
        buff[ndx] += 1;      /* Z-bit = 1          */

      if (buff[ndx] < 0x100) /*  if b8 == 0        */
        buff[ndx] += 0x200;  /* set b9 = !b8       */

      /* calculate X+1 */
      ndx++;
      buff[ndx] = (amplitude >> 6) & 0x1FF;

      if (buff[ndx] < 0x100) /*  if b8 == 0    */
        buff[ndx] += 0x200;  /* set b9 = !b8   */

      /* calculate X+2 */
      ndx++;
      buff[ndx] = (amplitude >> 15) & 0x1F;

      if (buff[ndx] < 0x100) /*  if b8 == 0    */
        buff[ndx] += 0x200;  /* set b9 = !b8   */

      /* calculate channel status bit */
      cbit = (chan_status[(i % 192) / 8 + 1] >> (i % 8)) & 0x01;

      if (cbit)
        buff[ndx] += 0x100; /* C = 1 */
      else
        buff[ndx] &= 0x37F; /* C = 0 */

      /* calculate parity */
      parity = (buff[ndx--] & 0x0FF) >> 18 +
               (buff[ndx--] & 0x1FF) >>  9 +
               (buff[ndx] & 0x1FF);

      pbit = 0;
      ndx  = 12 * i + 3 * j + 2;
      for (z = 0; z < 26; z++)
        {
        if ((parity >> z) & 0x01)
          pbit = ~pbit;

        if (pbit)
          buff[ndx] += 0x100; /* P = 1 */
        else
          buff[ndx] &= 0x2FF; /* P = 0 */

        if (buff[ndx] < 0x100) /* if  b8 == 0         */
          buff[ndx] += 0x200;  /* set b9 = !b8        */
        }
      }
    }
  }

/**************************************************************************/
/* Function: void split()                                                 */
/* Purpose:  To split 10 bit sound data in buff into 2 x 8 bit data and
/*           place them in prom.
/*           
/* Args:     long *buff   - buffer where sound data is stored
/*           UC * prom    - buffer for output data
/*           UI items     - number of data to split
/*           
/*           
/* 
/* Remarks:  
/*
/**************************************************************************/

void split(long *buff, UC *prom, UI items)
  {
  UI i;

  for (i = 0; i < items; i++)
    {
    prom[i]         =  buff[i]       & 0x0FF;
    prom[i + items] = (buff[i] >> 8) & 0x03;
    }
  }


/**************************************************************************/
/* Function: void calculate_checksum()                                    */
/* Purpose:  To calculate CRC value of channel status block
/*           
/*           
/* Args:     UC *csb - channel status block array
/*           
/*           
/*           
/* 
/* Remarks:  
              CRCC for channel staatus data block that uses bytes 0 - 22
              inclusive, i.e. bits 0 - 183.
              Generating polynomial is:
            
                G(x) = x^8 + x^4 + x^3 + x^2 + 1;
              
              with initial state of all ones.

              This function calculates checksum of channel status block
              only, not of sound data!
/*
/**************************************************************************/
void calculate_checksum(UC * csb)
  {
  UC tmp[8];
  UC i;
  UC mask;

  for (i = 0; i < 8; i++)
    tmp[i] = 1;
  
  for (i = 0; i < 184; i++)
    {
    mask = ((csb[((i % 192) / 8) + 1] >> (i % 8)) & 0x01) ^ tmp[7];
    tmp[7] = tmp[6];
    tmp[6] = tmp[5];
    tmp[5] = tmp[4];
    tmp[4] = tmp[3] ^ mask;
    tmp[3] = tmp[2] ^ mask;
    tmp[2] = tmp[1] ^ mask;
    tmp[1] = tmp[0];
    tmp[0] = mask;
    }
  if (tmp[0])
    csb[23] += 0x80;
  if (tmp[1])
    csb[23] += 0x40;
  if (tmp[2])
    csb[23] += 0x20;
  if (tmp[3])
    csb[23] += 0x10;
  if (tmp[4])
    csb[23] += 0x08;
  if (tmp[5])
    csb[23] += 0x04;
  if (tmp[6])
    csb[23] += 0x02;
  if (tmp[7])
    csb[23] += 0x01;
  }

/**************************************************************************/
/* Function: void open_output_file()                                      */
/* Purpose:  To open sound data file and text file
/*           
/*           
/* Args:     int *hand_b   - handle to binary file
/*           int *hand_t   - handle to text file
/*           
/*           
/* 
/* Remarks:  
/*
/**************************************************************************/
void open_output_file(int * hand_b, int *hand_t)
  {
  *hand_b = open(OUT_FILE, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);
  
  if (*hand_b == -1)
    {
    puts("Cant open output binary file.\n");
    exit(1);
    }

  *hand_t = open(TXT_FILE, O_RDWR | O_TEXT | O_CREAT, S_IREAD | S_IWRITE);
  
  if (*hand_t == -1)
    {
    puts("Cant open output text file.\n");
    exit(1);
    }
  }


/**************************************************************************/
/* Function: void save2file()                                             */
/* Purpose:  To to save data into binary file
/*           
/*           
/* Args:     int *hand_b  - handle to binary file
/*           UC  *buff    - buffer with data
/*           long hmany   - number of items
/*           long start   - start position for writing
/* 
/* Remarks:  
/*
/**************************************************************************/
void save2file(int *hand_b, UC *buff, long hmany, long start)
  {
  long pos;
  int  result;

  pos = start;
  lseek(hand_b, pos, SEEK_SET); 
  result = write(*hand_b, buff, hmany);
  if (result == -1)
    printf("Error writing file\n");
  }