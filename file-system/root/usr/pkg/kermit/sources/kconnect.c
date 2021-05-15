/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K c o n n e c t . c
 *
 *  Connect command for K e r m i t  File Transfer Utility.
 *
 *  CROMIX Kermit, VSL International Ltd, 1985.
 *  Maxim Dynin.
 *
 *  Based on the UNIX version developed at Columbia University by :
 *      Bill Catchings, Bob Cattani, Chris Maio, Frank da Cruz,
 *      Alan Crosswell, Jeff Damens, as well as Jim Guyton of Rand Corporation
 *      and Walter Underwood of Ford Aerospace.
 *
 *
 *  Modifications by :
 *     . Wolfgang Boehm, Roland Kreuzer, and Eva Kreuger, University of Passau,
 *       West Germany, December 1986.
 *     . Adrian Pickering, Southampton University, U.K., January/May 1987.
 *     . Dina Desai, VSL INternational Ltd, Switzerland, March/May 1987.
 *
 *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 */



#include "kermit.h"

/*
 * c o n n e c t
 *
 * Connect routine straight from the Kermit manual
 */

connect()
{
	int pid,                /* process id */
	    mode1,              /* terminal modes */
	    mode2,
	    mode3,
	    statuses[2];        /* child termination status */
		  
	static char esc[3];            /* printable escape char */
								  
	register connected;     /* true if connected */

	static char bel = '\07';        /* sound bell if illegal sequence */

	char c;                         /* character typed in */

	char getnc();

	/* If remote, no line is specified. */

	if (remote)
		return;

	strcpy(esc, cstring(escchr));
        
	/* Make own terminal binary */

	ttbin(STDIN, &mode1, &mode2, &mode3);

	/* and allow control-C through */

	setmode(STDIN, MD_MODE2, 0, ABENABLE);

	/* Leapfrog is the process that will write to our terminal */

	Leapfrog[1] = ttyname;

	/* start it */

	flushinput();

	pid = fexec(LEAPFROG, Leapfrog, 0, 0);

	if (pid == -1) {
		 printf("Cannot open leapfrog\r\n");
		 ttres(STDIN, &mode1, &mode2, &mode3);
		 return;
	}
	/* and now we are officially connected */

	printf("\r\nConnected.\r\nTo exit, type %s followed by C\r\n",esc);

	connected = TRUE;

	/* keep reading */

	while (connected) {
		c = getnc();

		/* if it's the escape character, but not the next one ... */

		if ((c & 0177) == escchr && ((c = getnc()) & 0177) != escchr)

			/* it's a command */

			switch (c & 0177) {
			case 'c':
			case 'C':               /* close */
			    connected = FALSE;
			    printf("\r\n");
			    break;
			default:
			    if ((c & 0177) == escchr) {
				write(ttyfd, &c, 1);
				    if (lecho)
					putchar(c);
			    } else
				printf("Cromix-Kermit: %sC[lose] or %s%s%c\r\n",
						esc,esc,esc,bel);
			    break;

			}
		else {

			/* if half-duplex, echo the character */

			if (lecho)
				putchar(c);

			/* and send it */

			write(ttyfd, &c, 1);
		}
	}

	/* restore the tty mode and don't forget to stop the reverse process */

	ttres(STDIN, &mode1, &mode2, &mode3);

	printf("\r\nDisconnected.\r\n");

	kill(pid, 3);

	/* wait for child to terminate to get rid of process table */
        
	wait(0, pid, statuses);
}

/*
 * g e t n c
 *
 * get a normal character
 */

char
getnc()
{
	char c;         /* character read */

	/* we'll just read it from the terminal */

	read(0, &c, 1);

	return (c);
}

