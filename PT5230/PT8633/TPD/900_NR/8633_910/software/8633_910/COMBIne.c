/* FILE:    combine.c
   DATE:    19.09.2002
   AUTHOR:  JK
   PURPOSE: This program is used to create binary program file
            for 8633 9xx version.  The output from compiler is a hex file.
            It is then converted to binary by hex2bin.
            this binary file must then be combined with sound and moving
            logo data, which are contained in 
            TSTSND15 DAT       524,288  03-24-98  9:17a
            TDKLOGOA DAT        45,056  01-12-98  9:41a
            These files are in .\include directory.
            This program does:

            1.Copies contents of TSTSND15 DAT into a new binary file
            2.Moves pointer to beginning of file
            3.Copies PT8633.bin into binary file
            4.Moves pointer to 0xB000, where logo data is to be
            5.Copies 45,056 bytes of contents of TDKLOGOA DAT 
            6.Names the binary file as PT9633.9xx

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
  printf("combine: combines sound, logo data and program binaries \n");
  printf("for PT8633.  Resulting combination is named PT8633.9xx\n");
  printf("Logo data is loaded @ 40000.\n");
  printf("Invocation: combine 8633_binary_input_file.\n");
  exit(3);
  }

int main(int argc, char *argv[])
  {
  FILE *    pt8603_fptr;
  FILE *    sound_fptr;
  FILE *    output_fptr;
  FILE *    logo_fptr;
  int       c;
  int       i;

  _fmode = O_BINARY;

  if ((argc != 2))
    {
    give_info();
    }

  if ((pt8603_fptr = fopen(argv[1], "r+b")) == NULL)
    {
    printf("Cannot open input binary file: %s\n", argv[1]);
    exit(1);
    }

  if ((sound_fptr = fopen(".\\include\\TSTSND15.DAT", "r+b")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\include\\TSTSND15.DAT");
    exit(1);
    }

  if ((logo_fptr = fopen(".\\include\\TDKLOGOA.DAT", "r+b")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", ".\\include\\TDKLOGOA.DAT");
    exit(1);
    }
  
  if ((output_fptr = fopen("PT8633.9xx", "wb")) == NULL)
    {
    printf("Cannot open binary output file: %s\n", "PT8633.9xx");
    exit(1);
    }

  /* copy .\include\TSTSND15 DAT into PT8633.9xx */
  /* sound data contains 0xFF from beginning (0) to 0xCA00 */
  while ((c = fgetc(sound_fptr)) != EOF)
    fputc(c, output_fptr);

  /* move pointer to beginning of PT8633.9xx */
  fseek(output_fptr, 0, SEEK_SET);

  /* copy 8633 program file file into PT8633.9xx */
  while ((c = fgetc(pt8603_fptr)) != EOF)
    fputc(c, output_fptr);
 
  /* move pointer to 0x0x40000 in PT8633.9xx */
  /* Previously this 64 k was occupied by 15 dB sound data */
  fseek(output_fptr, 0x40000, SEEK_SET);

  while ((c = fgetc(logo_fptr)) != EOF)
    fputc(c, output_fptr);

  close(sound_fptr);
  close(pt8603_fptr);
  close(logo_fptr);
  exit(0);
  }


