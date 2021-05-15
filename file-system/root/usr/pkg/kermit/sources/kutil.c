/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K u t i l . c
 *
 *  Utility functions for K e r m i t  File Transfer Utility.
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
 *     . Dina Desai, VSL International Ltd, Switzerland, March/May 1987.
 *
 *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 */



#include "kermit.h"

/*
 * d o p a r
 *
 * do the necessary parity conversion for transmission
 */

dopar (ch)
char ch;
{
    int a;                                      /* mask */

    /* don't do anything if no parity is set */

    if (parflg == NONE)
	return (ch);

    /* otherwise, convert the character */

    ch &= 0177;

    switch (parflg) {
	case MARK:                              /* Mark means 8-bit is set */
		return (ch | 128);

	case SPACE:                             /* Space means it isn't */
		return (ch & 127);

	case ODD:                               /* Odd or Even -- count bits */
		ch |= 128;
	case EVEN:
	    a = (ch & 15) ^ ((ch >> 4) & 15);
	    a = (a & 3) ^ ((a >> 2) & 3);
	    a = (a & 1) ^ ((a >> 1) & 1);

	    /* and set the first bit according to the result */

	    return ((ch & 0177) | (a << 7));

	default:                                /* Unknown flag */
	    return (ch);
    }
}

/*
 *  s p a c k
 *
 *  Send a Packet
 */

spack(type,num,len,data)
char type,      /* packet type */
     *data;     /* packet contents */
int num,        /* packet number */
    len;        /* packet length */
{
    int i,anz;                          /* Character loop counter */

    char chksum,ch,                     /* Checksum */
	 buffer[100],                   /* Packet buffer */
	 *wbuffer;

    register char *bufp;                /* Buffer pointer */

    if (pktdeb)                         /* Display outgoing packet */
    {
	/* Prepare the string for printing */

	if (data != NULL)
	    data[len] = '\0';

	/* And print the state information */

	fprintf(dpfp, " O = <%c %d %d> (%d)", type, num, len, numtry);

	if (data)
		fprintf(dpfp, " '%s'", data);

	putc('\n', dpfp);
    }

    /* Set up the buffer pointer */

    bufp = buffer;                      /* Set up buffer pointer */

    /* Issue any padding */

    for (i=1; i<=Spad; i++)
	write(ttyfd,&Spadchar,1);

    /* Fill in the packet marker and the character count */

    ch = dopar(Ssop);
    write(ttyfd,&ch,1);


    *bufp++ = dopar(tochar(len+3));


    /* Initialize the checksum */

    chksum  = tochar(len + 3) + tochar(num);

    /* Fill in packet number and type and update the checksum */

    *bufp++ = dopar(tochar(num));


    *bufp++ = dopar(type);


    chksum += type;

    /* Loop for all data characters, updating the checksum  */

    for (i=0; i<len; i++) {
	*bufp++ = dopar(data[i]);
	chksum += data[i];
    }

    /* Compute the final checksum, put it in the packet and terminate it */

    chksum = (((chksum&0300) >> 6)+chksum)&077;

    *bufp++ = dopar(tochar(chksum));

/*    *bufp++ = dopar(Seol);*/

    /* Send the packet */

    wbuffer = buffer;
    anz = bufp - buffer;

    writef(ttyfd, buffer, bufp - buffer);

    ch = dopar(Seol);
    write(ttyfd,&ch,1);

    if (debug>1)
       for (i=0; i<anz; i++) {
	   ch = *wbuffer++;
	   fprintf(dpfp,"> (Spack) '%s'\n",cstring(ch));
       }
 }

/*
 * w r i t e f
 *
 * write filtered
 */

writef(fd, buffer, length)
int fd;
register char *buffer;
register length;
{
	register i;

	if (!Filter)
	    write( fd, buffer, length ) ;
	else  {
		for (i = 0; i < length; i++)  {
			switch (*buffer)  {
			case '[':
				write(fd, "$(", 2);
				break;

			case ']':
				write(fd, "$)", 2);
				break;

			case '|':
				write(fd, "$L", 2);
				break;

			case '\001':
				write(fd, "$A", 2);
				break;

			case '$':
				write(fd, "$$", 2);
				break;

			default:
				write(fd, buffer, 1);
				break;
			}

			++buffer;
		}
	}
}



/*
 *  r p a c k
 *
 *  Read a Packet
 */

rpack(len,num,data)
int *len,                               /* Packet length */
    *num;                               /* Packet number */
char *data;                             /* Packet data */
{
    int rectime();                      /* alarm signal function */

    int i,                              /* Data character number */
	done;                           /* Loop exit */

    char t,                             /* Current input character */
	 type,                          /* Packet type */
	 rchksum;                       /* Checksum received from other host */



    /* set up the timeout */

    Sendagain = TRUE;
    signal(sigalarm, rectime);
    alarm(timint);

    /* Wait for the packet header; make sure we didn't get a timeout */

    while (Sendagain && inchr() != Rsop);

    /* If we got a timeout, return */

    if (!Sendagain)
	return (FALSE);

    /* Got SOH, start looping */
    /* If we get another one, just resynchronize */

    done = FALSE;

    while (!done && Sendagain)  {
	cchksum = 0;

	if ((t = cinchr()) == Rsop)
		continue;

	if (!Sendagain)
		return (FALSE);

	/* This should be the character count */

	*len = unchar(t)-3;

	if ((t = cinchr()) == Rsop)
		continue;

	/* This should be the packet number */

	*num = unchar(t);

	if ((t = cinchr()) == Rsop)
		continue;

	if (!Sendagain)
		return (FALSE);

	/* and this, the packet type */

	type = t;

	/* Put len characters from the packet into the data buffer */
	for (i=0; i<*len && Sendagain; i++)
	   if ((data[i] = cinchr()) == Rsop)
	       break ;

	if (!Sendagain)
		return (FALSE);

	/* If Rsop, resynchronize */

	if (data[i] == Rsop)
		continue;

	/* Mark the end of the data */

	data[*len] = 0;

	if ((t = inchr()) == Rsop)
		continue;

	if (!Sendagain)
		return (FALSE);

	/* and get the checksum */

	rchksum = unchar(t);

	/* Got checksum, done */


	done = TRUE;
    }

    /* Turn the timeouts off */

    signal(sigalarm, 1);

    /* If the other guy needs a handshake, wait till he sends it */

    if (turn)
	while (inchr() != Hand);

    if (pktdeb) {

	/* Display what we've got */

	fprintf(dpfp, "I = <%c %d %d> (%d)", type, *num, *len, numtry);

	if (data)
		fprintf(dpfp, " '%s'", data);

	putc('\n', dpfp);
    }


    /* Fold in bits 7,8 to compute the checksum */

    cchksum = (((cchksum&0300) >> 6)+cchksum)&077;

    /* Echo it if it does not match and debugging is on */

    if (cchksum != rchksum) {
	if (debug)
	    fprintf(dpfp, "> (Rpack) cchksum = %d, rchksum = %d\n",
							cchksum, rchksum);

	return (FALSE);
    }

    /* and return packet type */

    return (type);
}

/*
 * r e c t i m e
 *
 * break out of the receive cycle
 */

rectime()
{
	signal(sigalarm, 1);

	Sendagain = FALSE;

	if (debug)
		fprintf(dpfp, "timeout\n");
}

/*
 * r e c b r e a k
 *
 * if received a control-C, abort alltogether
 */

recbreak() {
	signal(sigalarm, 1);
	Serve = Sendagain = FALSE;
	state = 'A';
}


/*
 * Get a parity adjusted character from the line, add it to the checksum
 * and return it.
 */

cinchr()
{
    char ch;                            /* Character we're getting */

    ch = inchr();
    cchksum += ch;
    return (ch);
}


/*
 * Get a character from the line.  Do any necessary parity stripping
 * and return the character.
 */

inchr()
{
    char ch ;

    if (remote && (Infile == 0))
     ch = getc(stdin);
     else read(Infile,&ch,1);

    /* Handle the parity if necessary */

    if (parflg != NONE)
	ch = unpar(ch);

    if (debug > 1)
	fprintf(dpfp, "> (Inchr) '%s'\n", cstring(ch));

    return (ch);
}

/*
 *  b u f i l l
 *
 *  Get a bufferful of data from the file that's being sent.
 *  Control, repeat count, and 8-bit quoting are all done
 */

bufill(buffer)
char buffer[];                          /* Buffer */
{
    register i,                         /* Loop index */
	     t,                         /* Char read from file */
	     prev,                      /* previous character */
	     inl,                       /* in last */
	     repc;                      /* repeat count */

    char last[QUOTLENGTH];              /* Last char read */

    char t7;                            /* 7-bit version of above */

    /* Initialize the parameters */

    i = 0;
    prev = -1;
    repc = 0;

    /* Get the next character */

    while ((t = getc(fp)) != EOF) {
	if (debug > 2)
		fprintf(dpfp, "> (Bufill) '%s'\n", cstring(t));

	/* Skip a return if the file is a text file */

	if (t == '\r' && !Filebin)
		continue;

	/* Initialize last, update the statistics */

	inl = 0;
	++Chin;

	/* check if it is a repeat */

	if (repc) {

		/* if it is the same and we can handle it, continue */

		if (t == prev && Reppref && repc < MAXREPC) {
			++repc;
			continue;
		}

		/* else, enter the last character to the buffer */

		i = enter(buffer, last, i, repc);
		inl = 0;
		prev = t;
		repc = 0;
	}

	++repc;

	/* Get the low order 7 bits */

	t7 = unpar(t);

	/* Does this char require special handling? */

	/* check to see if it's eight-bit */

	if (t > 0177 && Eightpref)
		last[inl++] = Eightpref;

	/* does it have to be quoted */

	if (t7 < SP || t7==DEL || t7==Squote || t7==Reppref || t7==Eightpref)
	{
	    /* Add a return to a newline in a text file */

	    if (t=='\n' && !Filebin)
		last[inl++] = '\r';

	    /* quote the character */

	    last[inl++] = Squote;

	    /* and uncontrollify it if it isn't a quote */

	    if (t7 != Squote && t7 != Reppref && t7 != Eightpref)
	    {
		t = ctl(t);
		t7 = ctl(t7);
	    }
	}

	/* and deposit it in last */

	if (image)
	    last[inl++] = t;
	else
	    last[inl++] = t7;

	last[inl] = '\0';

	/* if the buffer is almost full, put last into it and return */

	if (i >= spsiz-QUOTLENGTH-1)
		return (enter(buffer, last, i, repc));
    }
if (debug) fprintf(dpfp, "> (Bufill) '%s'\n", cstring(t));
    if (i==0) return (EOF);             /* Wind up here only on EOF */
    return (enter(buffer, last, i, repc));
}

/*
 * e n t e r
 *
 * enter the character into the buffer
 */

enter(buffer, ch, inbuffer, repc)
register char *buffer;
register char *ch;
register inbuffer;
register repc;
{
	if (debug > 2)
		fprintf(dpfp, "> (Enter) '%s' at %d\n", ch, inbuffer);

	/* if the first character is a return, put the representation in */

	if (*ch == '\r') {
		buffer[inbuffer++] = Squote;
		buffer[inbuffer++] = ctl(*ch++);
	}

	/* if there is a repeat count, and it's worth it, use the repeat */

	if (repc > 1) {
		if (repc == 2 && !*(ch + 1))
			buffer[inbuffer++] = *ch;
		else {
			buffer[inbuffer++] = Reppref;
			buffer[inbuffer++] = (char) (repc + SP);
		}
	}

	/* and fill the buffer */

	while (*ch)
		buffer[inbuffer++] = *ch++;

	/* Null terminate it just in case */

	buffer[inbuffer] = '\0';

	return (inbuffer);
}


/*
 *      b u f e m p
 *
 *  Put data from an incoming packet into a file.
 */

char
bufemp(buffer,len)
char  buffer[];                         /* Buffer */
int   len;                              /* Length */
{
    register i,                         /* a counter */
	     repc,                      /* repeat count */
	     eight;                     /* eight-bit mask */

    char t;                             /* Character holder */

    /* Loop through the data field */

    for (i=0; i<len; i++) {
	repc = 1;
	eight = FALSE;

	/* Get the character */

	t = getbuf(buffer, i);

	/* If it's the repeat prefix, get the count */

	if (Reppref && t == Reppref) {
		repc = getbuf(buffer, ++i) - SP;

		if (repc < 0 || repc > MAXREPC) {
			error("Illegal receive count: %d", repc);
			return ('A');
		}

		t = getbuf(buffer, ++i);
	}

	/* if it's the eight-bit prefix, set the mask to turn the 8-bit on */

	if (Eightpref && t == Eightpref) {
		t = getbuf(buffer, ++i);
		eight = 0200;
	}

	/* if it's the control quote, decontrollify the next character */

	if (t == Rquote) {
	    t = getbuf(buffer, ++i);

	    /* Unless, of course, it's a quote character itself */

	    if (t != Rquote && t != Reppref && t != Eightpref)
		t = ctl(t);
	}

	/* if it is a CR and the file is not binary, continue */

	if (t == CR && !Filebin && !eight)
		continue;

	/* Increment the character count */

	Chin += repc;

	/* and write the character into the file */

	t |= eight;

	while (repc--)
		putc(t, fp);
    }

	return ('D');
}

/*
 *  g n x t f l
 *
 *  Get next file in a file group
 */

gnxtfl()
{
    if (debug)
	fprintf(dpfp, "> (Gnxtfl) filelist = \"%s\"\n",filelist[Onfile]);

    strcpy(Filnam, filelist[Onfile] ? filelist[Onfile] : "");

    /* if no more, fail */

    if (++Onfile > filecount)
	return FALSE;

    /* else succeed */

    else return TRUE;
}

/*
 * w o p e n
 *
 * if Warning is off, return the file pointer or NULL if we can't open it
 * else make sure we can create it
 */

FILE *
wopen(filename)
char *filename;
{
	FILE *filpt;

	char line[MAXLINE];

	if (filpt = fopen(filename, "r"))  {
		fclose(filpt);
		if (Warning) {
		     if (!Serve) {
			   printf("Overwrite %s? ", filename );
			   gets( line );
		     }
		     if (Serve || ( *line != 'y' && *line != 'Y' ))
			   return (NULL);
		}
		delete( filename ) ;
	}

	return(fopen(filename, "w"));
}

/*
 *  s p a r
 *
 *  Fill the data array with my send-init parameters
 *  0 : Biggest packet I can receive
 *  1 : When I want to be timed out
 *  2 : How much padding I need
 *  3 : Padding character I want
 *  4 : End-of-line character I want
 *  5 : Control-Quote character I send
 *  6 : Eight-bit prefix I want
 *  7 : Checksum type
 *  8 : Repeat prefix I want
 *  9 : My ability to accept attribute packets
 */

spar(data)
char data[];
{
    data[0] = tochar(Maxpack);
    data[1] = tochar(Timeout);
    data[2] = tochar(Rpad);
    data[3] = ctl(Rpadchar);
    data[4] = tochar(Reol);
    data[5] = Squote;
    data[6] = Eightpref ? Eightpref : 'N';
    data[7] = '1';
    data[8] = Reppref ? Reppref : 'N';
    data[9] = ctl(MYATT);
}


/*  r p a r
 *
 *  Get the other host's send-init parameters
 *
 */

rpar(data, len)
char data[];
register len;
{
    spsiz = unchar(data[0]);
    timint = unchar(data[1]);
    Spad = unchar(data[2]);
    Spadchar = ctl(data[3]);
    Seol = unchar(data[4]);
    Rquote = data[5];

    /* Since these are optional, make sure we agree */

    if (len < 7 || !Eightpref || data[6] == 'N' ||
	(state == 'S' && Eightpref != data[6] && data[6] != 'Y'))
	Eightpref = FALSE;
    else if (data[6] != 'Y')
	Eightpref = data[6];

    if (len < 8 || !Reppref || data[8] >= 'A' && data[8] <= 'Z' ||
	 (state == 'S' && Reppref != data[8]))
	Reppref = FALSE;
    else
	Reppref = data[8];

    if (len > 9)
	Atacc = MYATT & ctl(data[9]);
}


/*
 *  f l u s h i n p u t
 *
 *  Dump all pending input to clear stacked up NACK's.
 */

flushinput()
{
	int rectime();          /* alarm execution address */

	char any;               /* somewhere to put a character */

	/* set the continue flag to true */

	Sendagain = TRUE;

	/* set the alarm */

	signal(sigalarm, rectime);
	alarm(1);

	/* and start reading */
	if (debug) fprintf(dpfp,">flushinput  ttyfd =%d\n",ttyfd);

	while (Sendagain)
		{
		     read(ttyfd, &any, 1);
		};
}

/*
 *  Kermit printing routines:
 *
 *  printmsg -  like printf with "Kermit: " prepended
 *  error - like printmsg if local kermit; sends a error packet if remote
 *  prerrpkt - print contents of error packet received from remote host
 */

/*
 *  p r i n t m s g
 *
 *  Print message on standard output if not remote.
 */

/*VARARGS1*/
printmsg(fmt, a1, a2, a3, a4, a5)
char *fmt;
{
    if (!remote)  {
	printf("Kermit: ");
	printf(fmt,a1,a2,a3,a4,a5);
	printf("\n");

	/* force output */

	fflush(stdout);
    }
}

/*
 * s h e l l m s g
 *
 * print a message locally unless in server mode
 */

shellmsg(fmt, a1, a2, a3, a4, a5)
char *fmt;
{
	if (Serving)
		error(fmt, a1, a2, a3, a4, a5);
	else  {
		fputs("Kermit: ", stdout);
		printf(fmt, a1, a2, a3, a4, a5);
		putchar('\n');

		/* force output */

		fflush(stdout);
	}
}

/*
 *  e r r o r
 *
 *  Print error message.
 *
 *  If local, print error message with printmsg.
 *  If remote or server, send an error packet with the message.
 */

/*VARARGS1*/
error(fmt, a1, a2, a3, a4, a5)
char *fmt;
{
    char msg[80];
    int len;

    if (remote || Serve) {

	/* put it into a string and send the error packet */

	sprintf(msg,fmt,a1,a2,a3,a4,a5);
	len = strlen(msg);
	spack('E',n,len,msg);
    }
    else
	printmsg(fmt, a1, a2, a3, a4, a5);
}

/*
 *  p r e r r p k t
 *
 *  Print contents of error packet received from remote host.
 */

prerrpkt(msg)
char *msg;
{
    printf("Kermit aborting with this error from remote host:\n%s\n",msg);

    Transerr = TRUE;
}



/*
 * t t b i n
 *
 * Get the line tty ready; save the old modes
 */

ttbin(channel, mode1, mode2, mode3)
int channel,
    *mode1,
    *mode2,
    *mode3;
{
	*mode1 = getmode(channel,MD_MODE1);
	*mode2 = getmode(channel,MD_MODE2);
	*mode3 = getmode(channel,MD_MODE3);
	if (image)
		{
		setmode(channel, MD_MODE1,
						     /* leave parity alone */
			TANDEM|                         RAW|ODD|EVEN,
			TANDEM|XTAB|LCASE|ECHO|CRDEVICE|RAW|ODD|EVEN);
		setmode(channel, MD_MODE2,
			0,                           /* no sigabort in image */
			SGENABLE|ABENABLE|XFF|WRAP|SIGALLC);
		}
	else
		{
		setmode(channel, MD_MODE1,
			TANDEM|                         RAW,
			TANDEM|XTAB|LCASE|ECHO|CRDEVICE|RAW);
		setmode(channel, MD_MODE2,
				 ABENABLE,
			SGENABLE|ABENABLE|XFF|WRAP|SIGALLC);
		}
	setmode(channel, MD_MODE3, 0,
		BINARY|FNKEYS);
}

/*
 * t t r e s
 *
 * Restore the old tty setting
 */

ttres(channel, mode1, mode2, mode3)
int channel,
    *mode1,
    *mode2,
    *mode3;
{
	setmode(channel, MD_MODE1, *mode1,
		image ? TANDEM|XTAB|LCASE|ECHO|CRDEVICE|RAW|ODD|EVEN :
			TANDEM|XTAB|LCASE|ECHO|CRDEVICE|RAW);
	setmode(channel, MD_MODE2, *mode2,
		SGENABLE|ABENABLE|XFF|WRAP|SIGALLC);
	setmode(channel, MD_MODE3, *mode3,
		BINARY|FNKEYS);
}

/*
 * i n i t
 *
 * initializes whatever it's supposed to initialize
 */

init() {
	Seol = Reol = CR;
	Ssop = Rsop = SOH;
	Squote = Rquote = MYQUOTE;
	Eightc = MYEIGHT;
	Repc = MYREPEAT;
	Rpad = Spad = 0;
	Rpadchar = Spadchar = NULL;
	*Filnam = '\0';
	ttyname = NULL;
	ttyfd = oldtty = 0;
	pktdeb = FALSE;
	turn = DEFTRN;
	lecho = DEFLCH;
	Filebin = DEFFILEP;
	image = DEFIM;
	filnamcnv = DEFFNC;
	escchr = DEFESC;
	debug = INITDEBUG;
	remote = TRUE;
	fp = NULL;
	dpfp = stdout;
	parflg = NONE;
	Sdelay = MYTIME;
	Iquote = INPUTQUOTE;
	Allwild = INITALL;
	Onewild = INITONE;
	Maxpack = spsiz = MAXPACKSIZ;
	Hand = XON;
	Flevel = 0;
	Timeout = timint = MYTIME;
	Retry = MAXTRY;
	INF = stdin;
	Filesin = Chin = 0;
	Baud = getmode(ttyfd, MD_ISPEED) & 0x7f;        /* Strip A bit  */
	Warning = TRUE;
	Filter = FALSE;
	Sendall = FALSE;
	Incom = TRUE;
	Serving = FALSE;
	Initcon = TRUE;
	Omode2 = setmode( 0, MD_MODE2, 0, PAUSE ) ;
}


setfilter()
{
}

closefilter()
{
}

