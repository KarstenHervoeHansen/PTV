/* FILE:    sum.c
   DATE:    28.06.2002
   AUTHOR:  JK
   PURPOSE: This program calculates checksum of a binary file


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
  printf("sum: calculates checksum of binary file.\n");
  printf("Invocation: sum input_file \n");
  printf("or\nsum input_file ? to calculate and display checksum.\n");
  exit(3);
  }

int main(int argc, char *argv[])
  {
  FILE *    fptr;
  UI        sum;
  int       i;
  UL        file_len;

  _fmode = O_BINARY;

  if ((argc != 2) && (argc != 3))
    {
    give_info();
    }

  if ((fptr = fopen(argv[1], "r+b")) == NULL)
    {
    printf("Cannot open input file: %s\n", argv[1]);
    exit(1);
    }
#if 1
/* move pointer to the end and write FF until file len is FFF */
  fseek(fptr, 0, SEEK_END); /* move to end of file */
  file_len = ftell(fptr);
  while (file_len < 0x1000)
    {
    fputc(0xFF, fptr);
    file_len++;
    }
  fseek(fptr, 0, SEEK_SET); /* move to end of file */
#endif

  sum = 0;
  while ((i = fgetc(fptr)) != EOF)
    sum += (UI) i;

  printf("Checksum = %x\n", sum);
  fclose(fptr);
  exit(0);
  }


