/* FILE:    combi.c
   DATE:    02.07.2002
   AUTHOR:  JK
   PURPOSE: This program is used to create binary program file
            for 8603 910 version.  The output from compiler is a hex file.
            It is then converted to binary by hex2bin.
            this binary file must then be combined with sound
            data, which is contained in 
            D1TSTSND DAT       524,288  04-04-97  2:03p
            The original compilation note contained in readme.now
            file among 8603 sources states that:
            1.D1TSTSND DAT file is to be loaded into SPRINT programer
            2.file pt8603.hex is to be loaded next
            3.resulting contents are to be saved into a binary file
              which then is used to program 8603 prom.

            This is incorrect, as after loading pt8603.hex all 
            subsequent locations are changed to 0xFF.

            This program does:

            1.Copies contents of D1TSTSND DAT into a new binary file
            2.Moves pointer to beginning of file
            3.Copies PT8603.bin into binary file
            4.Names the binary file as PT9603.910

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
  printf("combi: combines sound and program binaries \n");
  printf("for PT8603.  Resulting combination is named PT8603.910\n");
  printf("Invocation: combi 8603_binary_input_file.\n");
  exit(3);
  }

int main(int argc, char *argv[])
  {
  FILE *    pt8603_fptr;
  FILE *    sound_fptr;
  FILE *    output_fptr;
  int       c;

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

  if ((sound_fptr = fopen("D1TSTSND.DAT", "r+b")) == NULL)
    {
    printf("Cannot open binary sound file: %s\n", "D1TSTSND.DAT");
    exit(1);
    }
  
  if ((output_fptr = fopen("PT8603.910", "wb")) == NULL)
    {
    printf("Cannot open binary output file: %s\n", "PT8603.910");
    exit(1);
    }

  /* copy D1TSTSND.DAT into PT8603.910 */
  while ((c = fgetc(sound_fptr)) != EOF)
    fputc(c, output_fptr);

  /* move pointer to beginning of PT8603.910 */
  fseek(output_fptr, 0, SEEK_SET);

  /* copy input file into PT8603.910 */
  while ((c = fgetc(pt8603_fptr)) != EOF)
    fputc(c, output_fptr);
 
  exit(0);
  }


