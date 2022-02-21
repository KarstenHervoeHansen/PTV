#include	<string.h>

char *
strcat(register char * to, register const char * from)
{
	register char *	cp;

	cp = to;
	while(*cp)
		cp++;
	while(*cp++ = *from++)
		continue;
	return to;
}
