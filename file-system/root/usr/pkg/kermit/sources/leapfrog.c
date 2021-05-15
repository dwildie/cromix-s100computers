/* a comment */

#include <stdio.h>

#define TRUE 1

main(argc, argv)
int argc;
char **argv;
{
	FILE *ttyfile,
	     *fopen();

	ttyfile = fopen(argv[1], "r");

	while(TRUE) 
		putchar(getc(ttyfile));  
}
