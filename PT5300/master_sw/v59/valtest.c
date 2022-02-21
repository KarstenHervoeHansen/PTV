
/* file: valtest.c  a program to test put_value function */
#include <stdio.h>
#include <stdlib.h>

#define UL unsigned long
#define UC unsigned char

void put_char1(UC c);
void put_value(char delimiter, UL value);

void main(int argc, char ** argv)
  {
  UL i;

  if (argc == 1)
    {
    printf("valtest: invocation valtest num, num is UL\r\n");
    exit(1);
    }
  i = atol(argv[1]);
  put_value('\;', i);
  exit(0);
  }

void put_char1(UC c)
  {
  printf("0x%x\n", (int) c);
  }

void put_value(char delimiter, UL value)
  {
  if (value >= 1000000000)
    put_char1(value/1000000000 + '0');
  
  if (value >= 100000000)
    put_char1((value/100000000)%10 + '0');
  
  if (value >= 10000000)
    put_char1(( value/10000000)%10 + '0');
  
  if (value >= 1000000) 
    put_char1(( value/1000000)%10 + '0');
  
  if (value >= 100000)
    put_char1((value/100000)%10 + '0');
  
  if (value >= 10000)
    put_char1((value/10000)%10 + '0');
  
  if (value >= 1000) 
    put_char1((value/1000)%10 + '0');
  
  if (value >= 100)
    put_char1((value/100)%10 + '0');
  
  if (value >= 10)
    put_char1((value/10)%10 + '0');

  put_char1(value%10 + '0');
  put_char1(delimiter);
  }
