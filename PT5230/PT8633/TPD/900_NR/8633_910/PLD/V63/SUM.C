



/* FILE NAME        sum.c
 * PURPOSE          To calculate checksum of a binary file.
 *
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       19.04.1992
 * NOTES:
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>


main(argc,argv)
int argc;
char *argv[];
  {
  unsigned long csum;
  unsigned char c;
  int           in_fil_ptr;
  int           result;
  unsigned long  i;


  if (argc == 1)
    {
    printf("sum: calculates checksum of a binary file\n");
    printf("invokation: sum filename\n");
    exit(1);
    }


  if ((in_fil_ptr = open(argv[1], O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }

  csum = 0;
  c    = 0;
  i    = 0;

  while (read(in_fil_ptr, (unsigned char *)&c, sizeof(unsigned char)) != 0)
    {
    i++;
    csum += (unsigned long) c;
    }
  printf("File len = %ld  csum = %lx\n", i, csum);


  close(in_fil_ptr);
  printf ("Checksum is: %0X Hex\n", csum);
  exit (0);

  }
/* this is end of sum.c */

