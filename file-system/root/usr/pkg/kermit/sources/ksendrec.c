/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K s e n d r e c . c
 *
 *  The send and receive commands
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
 *  s e n d s w
 *
 *  Sendsw is the state table switcher for sending files.  It loops until
 *  either it finishes, or an error is encountered.  The routines called
 *  by sendsw are responsible for changing the state.
 *
 */

sendsw()
{
    char sinit(),       /* send functions */
	 sfile(),
	 sattr(),
	 sdata(),
	 seof(),
	 sbreak();

    int recbreak(),     /* ^C signal address */
	rectime() ;

    /* initialize state, message number, number of tries, input file  */

    if (remote)
       {
	 signal( sigabort, 1 ) ;
	 signal( sigalarm, rectime ) ;
       }
    state = 'S';
    n = 0;
    numtry = 0;

    if (Filter)  {
	if (!setfilter()) {
		printf("Couldn't set the filter.\n");

		return (FALSE);
	}
    }
    else
	Infile = ttyfd;

    /* Initialize statistical entries */

    Filesin = Chin = 0;
    Lastbaud = getmode(ttyfd, MD_ISPEED);
    strcpy(Lastty, ttyname ? ttyname : "");

    /* prepare a location to go on ^C */
      signal( sigabort, recbreak ) ;
    /* If remote and not serving, give the guy some time to get ready */

    if (remote&&!(Serving))
	sleep(Sdelay);

    /* Prepare the reverse pyramid */

    Sendnext = TRUE;

    /* No error yet */

    Transerr = FALSE;

    /* Repeat this until an exit state is reached */

    while (TRUE) {
	if (debug)
		printf( dpfp, "State: %c\n", state);

	switch (state) {
	    case 'S':           /* Send init */
			  if (remote)
			    signal( sigabort, recbreak ) ;
			  state = sinit();
			break;

	    case 'F':           /* Send file header */
			state = sfile();
			break;


	    case 'T':           /* Send attributes */
			state = sattr();
			break;

	    case 'D':           /* Send data */
			if (!(remote||debug))
			   {               /* wiggle cursor */
			     if (!(n&1)||numtry)
				 printf(" ");
			     else
				 printf("\010");
			   }
			state = sdata();
			signal( sigabort, recbreak ) ;
			break;

	    case 'Z':           /* EOF */
			if (!remote)
			    printf("\n") ;
			state = seof();
			break;

	    case 'B':           /* Send break (end of transmission) */
			state = sbreak();
			break;

	    case 'C':           /* Complete */
        
			if (!remote)
			    printf("\n");
			else
			    Sendagain = FALSE ;
			if (Filter)
				closefilter();

			return (TRUE);

	    case FALSE :          /* timeout */
	    case 'A' :            /* Abort */

			/* send an error packet if necessary */

			if (!remote && !Transerr)
				spack('E', n, 0, 0);
			if (!remote)
			    printf("\n");
			else
			    Sendagain = FALSE ;
			if (Filter)
				closefilter();

			return (FALSE);
	}
    }
}


 /*
 *  s i n i t
 *
 *  Send Initiate: send this host's parameters and get other side's back.
 */

char sinit()
{
    int num,                    /* Packet number */
	len;                    /* Packet number, length */

    /* Initialize the prefix status and value */

    Eightpref = image ? FALSE : Eightc;
    Reppref = image ? FALSE : Repc;

    /* If too many tries, give up */

    if (numtry++ > Retry) {
	error("Too many tries.");

	return ('A');
    }

    /* Fill up init info packet */

    spar(packet);

    /* Send an S packet and process the reply */

    if (Sendnext || Sendall)
	spack('S',n,10,packet);

    Sendnext = TRUE;

    switch (rpack(&len,&num,recpkt)) {
	case 'R':       /* Receive, skip it */
	case 'N':       /* NAK, try it again */
		Sendnext = numtry % 2;
		return (state);

	case 'Y':                       /* ACK */

	    /* Make sure the packet number matches */

	    if (n != num)
		return (state);

	    /* Get other side's init info */

	    rpar(recpkt, len);

	    /* Check and set defaults */

	    if (Seol == 0)
		Seol = '\n';

	    if (Rquote == 0)
		Rquote = '#';

	    /* Reset numtry, increment packet count and go to Send file */

	    numtry = 0;
	    n = (n+1)%64;
	    return ('F');

	case 'E':                       /* Error packet received */

	    /* print it out and abort */

	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Timeout, try again */
	    return (state);

	default:                        /* Abort on anything else */
	    error("Illegal packet.");
	    return ('A');
   }
 }


/*
 *  s f i l e
 *
 *  Send File Header.
 */

char sfile()
{
    int num,                            /* Packet number */
	len;                            /* Packet length */

    char filnam1[50],                   /* Converted file name */
	 *newfilnam,                    /* Pointer to file name to send */
	 *cp;                           /* char pointer */

    /* If too many tries, give up */

    if (numtry++ > Retry) {
	error("Too many tries.");

	return ('A');
    }

    if (Sendnext || Sendall) {

	/* If the file hasn't been opened yet, do so now */

	if (fp == NULL) {
	    fp = fopen(Filnam,"r");

	    if (fp == NULL)     {
		error("Cannot open file %s",Filnam);

		return ('A');
	    }
	}

	/* Copy file name */

	strcpy(filnam1, Filnam);
	cp = filnam1;

	/* See which name the file is to be sent */

	if (*Fileas && !remote)
		newfilnam = Fileas;
	else  {
		newfilnam = cp;

	       /* Strip of all leading directory names (up to the last /) */
	    while (*cp)
		    if (*cp++ == '/')
			newfilnam = cp;
	}

	/* Convert lower case to upper */

	if (filnamcnv)
	    for (cp = newfilnam; *cp != '\0'; cp++)
		if (islower(*cp))
			 *cp = toupper(*cp);

	/* Compute the length of the new filename */

	len = cp - newfilnam;

	if (!remote)
	    printf("Sending %s as %s ",Filnam,newfilnam); /* no newline */

	/* Send the F packet and check the reply */

	spack('F',n,len,newfilnam);
    }

    Sendnext = TRUE;

    switch (rpack(&len,&num,recpkt)) {
	case 'N':                       /* NAK */
	    /* just stay in this state, unless it's for the next packet */

	    num = (--num<0 ? 63:num);

	    /* if it is, it's just like an ACK for this packet */

	    if (n != num) {
		Sendnext = (numtry - 1) % 2;
		return (state);
	    }

	case 'Y':                       /* ACK */

	    /* Make sure it's the right ACK */

	    if (n != num) {
		Sendnext = (numtry - 1) % 2;
		return (state);
	    }

	    /* Reset try counter, increment packet count and start sending */

	    numtry = 0;
	    n = (n+1)%64;
	    size = bufill(packet);

	    /* but maybe we first have to send an attributes packet */

	    if (Atacc)
		return ('T');
	    else
		return ('D');

	case 'E':                       /* Error packet received */

	    /* print it out and abort */

	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Timeout, stay in this state */
	    return (state);

	default:                        /* Something else, just abort */
	    error("Illegal packet.");
	    return ('A');
    }
}

/*
 * s a t t r
 *
 * send file attribute (so far, one packet)
 */

char sattr()
{
	int num,                /* packet number */
	    len;                /* packet length */

	static char typeatt[] = "\"A";  /* attribute packet */

	/* make sure the tries aren't too many */

	if (numtry++ > Retry) {
		error("Too many tries.");

		return ('A');
	}

	if (Sendnext || Sendall) {

		/* prepare the type attribute packet for sending */

		if (image)
		    typeatt[1] = 'I';
		else if (Filebin)
			typeatt[1] = 'B';
		     else
			typeatt[1] = 'A';

		if (debug)
			printf( dpfp, "> (Sattr) Try # %d\n", numtry);

		/* and send the packet */

		spack('A', n, 2, typeatt);

		/* and check and see what we got back */
	}

	Sendnext = TRUE;

	switch (rpack(&len, &num, recpkt)) {
	case 'N':

		/* make sure it's the NAK for this packet and do it again */

		if (n == num || n != (num - 1) % 64) {
			Sendnext = (numtry - 1) % 2;
			return (state);
		}

		/* else rig it so that it seems that it was an ACK */

		n = (n + 1) % 64;
		numtry = 0;
		return ('D');

	case 'Y':

		/* make sure it's for this packet and that they agree */

		if (n != num) {
			Sendnext = (numtry - 1) % 2;
			return (state);
		}

		if (*recpkt == 'N')
			return ('A');
	        
		numtry = 0;

		n = (n + 1) % 64;

		return ('D');

	default:
		error("Illegal packet.");
		return ('A');
	}
}


/*
 *  s d a t a
 *
 *  Send File Data
 */

char sdata()
{
    int num,                    /* Packet number */
	len;                    /* Packet length */

    if (numtry++ > Retry) {
	error("Too many tries.");
	return ('A');
    }

    if (Sendnext || Sendall) {

	    /* send a D packet and see what we get */

	    spack('D',n,size,packet);
    }

    Sendnext  = TRUE;

    switch (rpack(&len,&num,recpkt)) {
	case 'N':                       /* NAK, act as usual */
	    if (n != (num - 1) % 64) {
		Sendnext = (numtry - 1) % 2;
		return (state);
	    }
	    num = (num + 1) % 64;

	case 'Y':                       /* ACK, act as usual  */
	    if (n != num) {
		Sendnext = (numtry - 1) % 2;
		return (state);
	    }

	    numtry = 0;
	    n = (n+1)%64;

	    /* and return the same state unless the file is all sent */

	    if ((size = bufill(packet)) == EOF)
		return ('Z');
	    return ('D');

	case 'E':                       /* Error packet received */
	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Timeout, stay in the same state */
	    return (state);

	default:                        /* Anything else, "abort" */
	    error("Illegal packet.");
	    return ('A');
    }
}


/*
 *  s e o f
 *
 *  Send End-Of-File.
 */

char seof()
{
    int num,                    /* Packet number */
	len;                    /* Packet length */

    if (numtry++ > Retry) {
	error("Too many tries.");

	return ('A');
    }

    if (Sendnext || Sendall) {

	/* send a 'Z' packet and make sure we get an ACK, or stay here */

	spack('Z',n,0,packet);
    }

    Sendnext = TRUE;

    switch (rpack(&len,&num,recpkt)) {
	case 'N':                       /* NAK, act as usual */
	    num = (--num<0 ? 63:num);
	    if (n != num) {
		Sendnext = (numtry - 1) % 2;
		return (state);
	    }

	case 'Y':                       /* ACK, check the number */
	    if (n != num) {
		Sendnext = (numtry - 1) % 2;
		return (state);
	    }

	    numtry = 0;
	    n = (n+1)%64;

	    /* close the file */

	    fclose(fp);                 /* Close the input file */
	    fp = NULL;                  /* Set flag indicating no file open */

	    /* we got another file in */

	    ++Filesin;

	    /* get the next file; if none, prepare to break the transmission */

	    if (gnxtfl() == FALSE)
		return ('B');

	    return ('F');

	case 'E':                       /* Error packet received */
	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Timeout */
	    return (state);

	default:                        /* Something else, "abort" */
	    error("Illegal packet.\n");
	    return ('A');
    }
}


/*
 *  s b r e a k
 *
 *  Send Break (EOT)
 */

char sbreak()
{
    int num,                    /* Packet number */
	len;                    /* Packet length */

    if (numtry++ > Retry) {
	error("Too many tries.");

	return ('A');
    }

    /* Send a B packet and get an acknowledgement */

    spack('B',n,0,packet);
    switch (rpack(&len,&num,recpkt)) {
	case 'N':                       /* NAK, act as usual */
	    num = (--num<0 ? 63:num);
	    if (n != num)
		return (state);

	case 'Y':                       /* ACK, act as usual */
	    if (n != num) return (state);
	    numtry = 0;
	    n = (n+1)%64;
	    return ('C');

	case 'E':                       /* Error packet received */
	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Timeout */
	    return (state);

	default:
	    error("Illegal packet.");
	    return ('A');
   }
}


/*
 *  r e c s w
 *
 *  This is the state table switcher for receiving files.
 */

recsw()
{
    char rinit(),                       /* receive procedures */
	 rfile(),
	 rdata();

    int recbreak();                     /* ^C execution address */

    /* Initialize state, message number, number of tries, input file  */

    state = 'R';
    n = 0;
    numtry = 0;

    if (Filter)  {
	if (!setfilter())
		return (FALSE);

	if (debug > 1)
		printf("> (Recsw) Filter is set.\n");
    }

    else
	Infile = ttyfd;

    /* Initialize statistical entries */

    Filesin = -1;
    Chin = 0;
    Lastbaud = getmode(ttyfd, MD_ISPEED);
    strcpy(Lastty, ttyname ? ttyname : "");

    /* Prepare to trap ^C */

    signal(sigabort, recbreak);

    /* Say no errors yet */

    Transerr = FALSE;

    /* Continue as long as necessary */

    while (state) {
	if (debug)
		printf("State: %c\n", state);

	    switch (state) {
	    case 'R':                   /* Receive Init */
			state = rinit();
			break;

	    case 'F':                   /* Receive File */

			state = rfile();
			++Filesin;
			break;

	    case 'D':                   /* Receive data */
			if (!(remote||debug))
			    {               /* wiggle cursor */
			     if (!(n&1)||numtry)
				 printf(" ");
			     else
				 printf("\010");
			    }
			state = rdata();
			break;

	    case 'C':                   /* Complete state */

			/* Increment file count */
			++Filesin;
			if (Filter)
				closefilter();
			if (!remote)
				printf("\n");
			return (TRUE);

	    case 'A':                   /* "Abort" state */
			/* erase the file if incomplete and incom is off */

			if (fp) {
				fclose(fp);
				fp = NULL;
				*Filnam = '\0' ;
				if (!Incom)
					delete(Fileas);
			}

			if (!remote && !Transerr)
				spack('E', n, 0, 0);
			if (Filter)
				closefilter();
			if (!remote)
				printf("\n");
			return (FALSE);
	}
    }
}


/*
 *  r i n i t
 *
 *  Receive Initialization
 */

char rinit()
{
    int len,                    /* Packet length */
	num;                    /* Packet number */

    if (numtry++ > Retry) {
	error("Too many tries.");
	return ('A');
    }

    /* Initialize the prefixes */

    Eightpref = image ? FALSE : Eightc;
    Reppref = image ? FALSE : Repc;

    /* Get a packet */

    switch (rpack(&len,&num,packet)) {
	case 'S':                       /* Send-Init */
	    /* Get their parameters and reply with others */

	    rpar(packet, len);
	    spar(packet);
	    spack('Y',n,10,packet);


	    /* and perform the usual */

	    oldtry = numtry;
	    numtry = 0;
	    n = (n+1)%64;
	    return ('F');

	case 'E':                       /* Error packet received */
	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Didn't get packet */

	    /* Return a NAK and keep trying */

	    spack('N',n,0,0);
	    return (state);

	default:                        /* Some other packet type, "abort" */
	    error("Illegal packet.");
	    return ('A');
    }
}


/*
 *  r f i l e
 *
 *  Receive File Header
 */

char rfile()
{
    int num,                    /* Packet number */
	len;                    /* Packet length */
    char *p;

    FILE *wopen();

    char filnam1[50];           /* Holds the converted file name */

    if (numtry++ > Retry) {
	error("Too many tries.");
	return ('A');
    }

    /* Get a packet and make sure it's a file header */

    switch (rpack(&len,&num,packet)) {
	case 'S':                       /* Send-Init, maybe our ACK lost */
	    if (oldtry++ > Retry) {
		error("Too many tries.");
		return ('A');
	    }

	    /* check to see if it's the previous packet */

	    if (num == ((n==0) ? 63:n-1)) {
		spar(packet);
		spack('Y',num,10,packet);
		numtry = 0;
		return (state);
	    }

	    /* otherwise abort */

	    else {
		error("Illegal packet number.");
		return ('A');
	    }

	case 'Z':                       /* End-Of-File */
	    if (oldtry++ > Retry) {
		error("Too many tries.");
		return ('A');
	    }

	    /* Make sure it's the previous packet */

	    if (num == ((n==0) ? 63:n-1)) {
		spack('Y',num,0,0);
		numtry = 0;
		return (state);
	    }

	    /* otherwise "abort" */

	    else {
		error("Illegal packet number.");
		return ('A');
	    }

	case 'F':                       /* File Header (just what we want) */

	    /* Check the packet number */

	    if (num != n) {
		if (Filter)
			return (state);

		error("Illegal packet number.");
		return ('A');
	    }

	    /* Has the file name already been given? */

	    if (*Filnam) {
		strcpy(filnam1, Filnam);
		*Filnam = '\0';
	    }

	    /* If not, get it from the packet */

	    else
		strcpy(filnam1, packet);

	    /* Perform case conversion */

	    if (filnamcnv)
		for (p = filnam1; *p != '\0'; p++)
		    *p = tolower(*p);


	    /* Make sure we can open this file */

	    if ((fp=wopen(filnam1))==NULL) {
		error("Cannot create %s",filnam1);

		return ('A');
	    }

	    /* OK, notify the receiver */

	    else if (!remote)
		printf("Receiving %s as %s ",packet,filnam1); /* no newline */

	    /* Save the name */

	    strcpy(Fileas, filnam1);

	    /* Acknowledge the file header */

	    spack('Y',n,0,0);

	    /* and set the parameters accordingly */

	    oldtry = numtry;
	    numtry = 0;
	    n = (n+1)%64;
	    return ('D');

	case 'B':                       /* Break transmission (EOT) */
	    if (num != n) {
		error("Illegal packet number.");

		return ('A');
	    }

	    spack('Y',n,0,0);
	    return ('C');

	case 'E':                       /* Error packet received */
	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Didn't get packet */
	    spack('N',n,0,0);
	    return (state);

	default:                        /* Some other packet, "abort" */
	    error("Illegal packet.");
	    return ('A');
    }
}


/*
 *  r d a t a
 *
 *  Receive Data
 */

char rdata()
{
    int num,                    /* Packet number */
	len;                    /* Packet length */

    char bufemp();

    if (numtry++ > Retry) {
	error("Too many tries.");
	return ('A');
    }

    /* Get packet */

    switch (rpack(&len,&num,recpkt)) {
	case 'D':                       /* Got Data packet */
	    /* Check the number */

	    if (num != n) {
		if (oldtry++ > Retry) {
		    error("Illegal packet number.");
		    return ('A');
		}

		/* Re-ACK it if it's the previous one */

		if (num == ((n==0) ? 63:n-1)) {
		    spack('Y', num, 0, 0);
		    numtry = 0;
		    return (state);
		}

		/* sorry, wrong number */

		else if (!Filter) {
			error("Illegal packet number.");
			return ('A');
		}

		/* if it's filter, just return */

		else
			return(state);
	    }

	    /* Got data with right packet number */

	    /* Write it out to the file, acknowledge, and reset parameters */

	    state = bufemp(recpkt,len);

	    spack('Y',n,0,0);

	    oldtry = numtry;
	    numtry = 0;
	    n = (n+1)%64;

	    return (state);

	case 'F':                       /* Got a File Header */
	    if (oldtry++ > Retry) {
		error("Too many tries.");
		return ('A');
	    }

	    /* Check packet number and abort if it isn't the last one */

	    if (num == ((n==0) ? 63:n-1)) {

		/* it is so reacknowledge it */

		spack('Y',num,0,0);
		numtry = 0;
		return (state);
	    }
	    else {
		error("Illegal packet number.");
		return ('A');
	    }

	case 'A':               /* Attributes */

	    /* Check the packet number */

	    if (num == (n - 1) % 64) {
		spack('Y', n, 0, 0);
		return (state);
	    }
	    else if (num == n) {
		spack('Y', n, 0, 0);
		numtry = 0;

		/* The only one we can accept is data type, or " */

		if (*recpkt == '"')
			switch (recpkt[1]) {
			case 'A':               /* Text */
				image = FALSE;
				Filebin = FALSE;
				break;

			case 'B':               /* Binary */
				image = FALSE;
				Filebin = TRUE;
				break;

			case 'I':               /* Image */
				image = TRUE;
				Filebin = TRUE;
				break;
			}

		/* increment the packet number */

		n = (n + 1) % 64;
	    }
	    else {
		error("Illegal packet number.");
		return ('A');
	    }
	    return (state);

	case 'Z':                       /* End-Of-File */

	    /* check the packet number */

	    if (num != n) {
		error("Illegal packet number.");
		return ('A');
	    }

	    /* OK, ACK it, close the file and reset the parameters */

	    spack('Y',n,0,0);
	    fclose(fp);
	    fp = NULL;
	    ++Filesin ;
	    *Filnam = '\0' ;
	    n = (n+1)%64;

	    /* Prepare to receive the next file */

	    return ('F');

	case 'E':                       /* Error packet received */
	    prerrpkt(recpkt);
	    return ('A');

	case FALSE:                     /* Didn't get packet */
	    spack('N',n,0,0);
	    return (state);

	default:                        /* Some other packet, abort */
	    error("Illegal packet.");
	    return ('A');
    }
}

