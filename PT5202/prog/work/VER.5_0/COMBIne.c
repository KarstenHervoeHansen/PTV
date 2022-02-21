/* FILE:    combine.c
   DATE:    18.03.2003
   AUTHOR:  JK
   PURPOSE: This program is used to create hex program file
            for PT5202.  The output from compiler is a hex file.
            This hex file is combined with hex files containing
            video data, sound data (with -20 and -18 dB sound) and
            with SPG program (v. 1.1).
        
            5202SND3 HEX        33,294  03-18-03  9:30a 5202SND3.HEX
            DATA05   HEX       473,152  11-21-03 11:04a DATA05.HEX
            SPG_03   HEX        77,969  03-18-03  9:30a SPG_03.HEX
        
            These files are in .\temp directory.
            This program does:

           1. Creates text file 520250.hex.
           2. Copies PT5202.hex into it.
           3. Appends SPG_03.hex into it.
           4. Appends 5202snd3.hex into it.
           5. Appends DATA03.hex into it.
           6. Closes 520250.hex file.

*/
#include <stdio.h>
#include <alloc.h>
#include <dos.h>
#include <fcntl.h>

#define UI unsigned int
#define UC unsigned char
#define UL unsigned long

void give_info(void);
void give_info(void)
  {
  printf("combine: combines sound, PLD and video data and program hex files \n");
  printf("for PT5202.  Resulting combination is named 520250.hex\n");
  printf("PLD   data is loaded @ 30000.\n");
  printf("Sound data is loaded @ 3A000.\n");
  printf("Video data is loaded @ 40000.\n");
  printf("Invocation: combine .\\temp\\pt5202.hex.\n");
  exit(3);
  }

int main(int argc, char *argv[])
  {
  FILE *    pt5202_fptr;
  FILE *    pld_fptr;
  FILE *    sound_fptr;
  FILE *    output_fptr;
  FILE *    video_fptr;
  int       c;
  int       i;

  _fmode = O_TEXT;

  if ((argc != 2))
    {
    give_info();
    }

  if ((pt5202_fptr = fopen(argv[1], "r+t")) == NULL)
    {
    printf("Cannot open input hex file: %s\n", argv[1]);
    exit(1);
    }

  if ((sound_fptr = fopen(".\\temp\\5202SND3.hex", "r+t")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\temp\\5202SND3.hex");
    exit(1);
    }

  if ((video_fptr = fopen(".\\temp\\DATA05.hex", "r+t")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\temp\\DATA05.hex");
    exit(1);
    }
  if ((pld_fptr = fopen(".\\temp\\spg_03.hex", "r+t")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\temp\\spg_03.hex");
    exit(1);
    }
  
  if ((output_fptr = fopen(".\\temp\\520250.hex", "wt")) == NULL)
    {
    printf("Cannot open hex output file: %s\n", ".\\temp\\520250.hex");
    exit(1);
    }


  /* move pointer to beginning of 520250.hex */
  fseek(output_fptr, 0, SEEK_SET);

  /* copy input file into 520250.hex */
  while ((c = fgetc(pt5202_fptr)) != EOF)
    fputc(c, output_fptr);
  printf("Combined %s\n", argv[1]);

  /* append SPG file */
  while ((c = fgetc(pld_fptr)) != EOF)
    fputc(c, output_fptr);
  printf("Combined %s\n", ".\\temp\\spg_03.hex");

  /* append sound file */
  while ((c = fgetc(sound_fptr)) != EOF)
    fputc(c, output_fptr);
  printf("Combined %s\n", ".\\temp\\5202SND3.hex");

  /* append video file */
  while ((c = fgetc(video_fptr)) != EOF)
    fputc(c, output_fptr);
  printf("Combined %s\n", ".\\temp\\DATA05.hex");

  close(pt5202_fptr);
  close(video_fptr);
  close(sound_fptr);
  close(pld_fptr);
  close(output_fptr);
  printf("Combined correctly into: %s.\n", ".\\temp\\520250.hex");
  exit(0);
  }


