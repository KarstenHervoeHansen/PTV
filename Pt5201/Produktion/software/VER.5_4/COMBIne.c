/* FILE:    combine.c
   DATE:    18.03.2003
   AUTHOR:  JK
   PURPOSE: This program is used to create hex program file
            for PT5201.  The output from compiler is a hex file.
            This hex file is combined with hex files containing
            video data, sound data (with -20 and -18 dB sound) and
            with SPG program (v. 1.1).
        
            20DBFS   HEX        22,206  06-26-01  3:14p 20DBFS.HEX
            DATA04   HEX       473,152  10-17-03 10:25a DATA04.HEX
            AES_SPG  HEX        73,770  07-17-00  1:03p AES_SPG.HEX
        
            These files are in .\temp directory.
            File containing Pt5201 main software is input under invocation.

            Output file containig all hex files is named 520154.hex
            This program does:

           1. Creates text file 520154.hex.
           2. Copies input file into it.
           3. Appends AES_SPG.hex into it.
           4. Appends 20DBFS.hex into it.
           5. Appends DATA04.hex into it.
           6. Closes 520154.hex file.

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
  printf("for LT428.  Resulting combination is named 520154.hex\n");
  printf("PLD data is loaded   @ 40000.\n");
  printf("Sound data is loaded @ 4A000.\n");
  printf("Video data is loaded @ 50000.\n");
  printf("Invocation: combine .\\temp\\pt5201.hex.\n");
  exit(3);
  }


#define StartOfPROMCode				0x00000L
#define SizeOfPROMCode 				0x40000L

#define StartOfPLDCode				0x40000L
#define SizeOfPLDCode					0x0A000L

#define StartOfEmbAudioCode		0x4A000L
#define SizeOfEmbAudioCode		0x02400L

#define StartOfCBVideoCode		0x50000L
#define SizeOfCBVideoCode			0x30000L

int main(int argc, char *argv[])
  {
  FILE *    pt5201_fptr;
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

  if ((pt5201_fptr = fopen(argv[1], "r+t")) == NULL)
    {
    printf("Cannot open input hex file: %s\n", argv[1]);
    exit(1);
    }

  if ((sound_fptr = fopen(".\\temp\\20DBFS.hex", "r+t")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\temp\\20DBFS.hex");
    exit(1);
    }

  if ((video_fptr = fopen(".\\temp\\DATA04.hex", "r+t")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\temp\\DATA04.hex");
    exit(1);
    }
  if ((pld_fptr = fopen(".\\temp\\AES_SPG.hex", "r+t")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\temp\\AES_SPG.hex");
    exit(1);
    }
  
  if ((output_fptr = fopen(".\\temp\\520154.hex", "wt")) == NULL)
    {
    printf("Cannot open hex output file: %s\n", ".\\temp\\520154.hex");
    exit(1);
    }


  /* move pointer to beginning of 520154.hex */
  fseek(output_fptr, 0, SEEK_SET);

  /* copy input file into 520154.hex */
  while ((c = fgetc(pt5201_fptr)) != EOF)
    fputc(c, output_fptr);

  /* append SPG file */
  while ((c = fgetc(pld_fptr)) != EOF)
    fputc(c, output_fptr);

  /* append sound file */
  while ((c = fgetc(sound_fptr)) != EOF)
    fputc(c, output_fptr);

  /* append video file */
  while ((c = fgetc(video_fptr)) != EOF)
    fputc(c, output_fptr);


  close(pt5201_fptr);
  close(video_fptr);
  close(sound_fptr);
  close(pld_fptr);
  close(output_fptr);
  printf("Combined correctly.\n");
  exit(0);
  }


