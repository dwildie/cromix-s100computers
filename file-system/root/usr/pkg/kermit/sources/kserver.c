/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K s e r v e r . c
 *
 *  Server for K e r m i t  File Transfer Utility.
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
 *     . Adrian Pickering, Southampton University, U.K., January 1987.
 *     . Dina Desai, VSL International Ltd, Switzerland, March/May 1987.
 *     . Egon Zakrajsek, Cromemco Inc., Aug 1987.
 * 
 *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 */
					 
				      

#include "kermit.h"


extern char *getword();


/* 
 * r s e r v e r
 *
 * Server idle, waiting for a message
 */

rserver()  {
	int num,                                /* Packet number */
	    len;                                /* Packet length */
	        
	int recbreak();                         /* ^c address */

	/* set up the ^c address */

	signal(sigabort, recbreak);

	/* empty the buffer */

	flushinput();

	/* No interrupt yet */

	Serve = TRUE;
        
	/* configure the terminal */

	if (remote)
		ttbin(STDIN,&Omode1,&Omode2,&Omode3);

	/* Set up the pipe if the filter is on */

	if (Filter) {
		if (!setfilter())
			return (FALSE);
	}
	else
		Infile = ttyfd;

	/* and start serving */

	Serving = TRUE;

	while (Serve) { 
        
		/* packet number and te number of tries is always 0 */

		numtry = 0;
		n = 0;

		/* get a packet and do what's required */

		switch (rpack(&len, &num, recpkt)) {
		case 'I':                       /* Initial parameters */

			/* make sure the parameters are correct */

			Eightpref = image ? FALSE : Eightc;
			Reppref = image ? FALSE : Repc;

			/* exchange them */

			rpar(recpkt);
			spar(packet);
			spack('Y', n, 10, packet);
			break;

		case 'S':                       /* Send */

			/* make sure the parameters are correct */

			Eightpref = image ? FALSE : Eightc;
			Reppref = image ? FALSE : Repc;

			/* exchange them */

			rpar(recpkt);
			spar(packet);
			spack('Y', n, 10, packet);
		        
			/* receive the file */

			if (!rservsw() && !Transerr)
				{error("Server receive failed.");
				if (fp)
					{fclose(fp);
					fp = NULL;
					}
				}
			break;

		case 'R':                       /* Receive */
			/* get the file name */

			*Fileas = '\0';
			Onfile = 0;
			filecount = getnames(filelist, recpkt, 0);
			while (gnxtfl())     

				/* and send it */

				if (!sendsw())
					{error("Server send failed.");
					if (fp)
						{fclose(fp);
						fp = NULL;
						}
					}
			break;

		case 'K':                       /* Kermit command */
			/* execute the command */

			execp(recpkt);
			break;

		case 'G':                       /* generic server command */
			switch (*recpkt) {
			case 'C':               /* change work. directory */

				/* make sure we can set it */

				if (!*(recpkt + 1)) {
					setdir("..");
					spack('Y', n, 0, 0);
				}
				else if (setdir(recpkt + 2))
					error("Can't change to %s", 
						recpkt + 2);
				else
					spack('Y', n, 0, 0);
				break;

			case 'L':                       /* logoff */
				if (dpfp)
					fclose(dpfp);

				spack('Y', n, 0, 0);

				/* set the terminal modes back - JAP */

				if (remote)
					ttres(STDIN,&Omode1,&Omode2,&Omode3);

				/* then commit suicide */

				kill (0, sigkill);
				break;

			case 'F':                       /* finish */
				spack('Y', n, 0, 0);
			        
				if (Filter)
					closefilter();

				signal(sigalarm, 1);
				Serving = FALSE;

				if (remote)
					ttres(STDIN,&Omode1,&Omode2,&Omode3);
				return;

			default:                        /* illegal type */ 
				error("Function not implemented.");
				break;
			}
			break;

		case FALSE:                             /* timeout */
			spack('N', n, 0, 0);
			flushinput() ;
			break;

		default:                                /* illegal type */
			error("Function not implemented.");
			break;
		}
	}

	Serving = FALSE;

	if (remote)
		ttres(STDIN,&Omode1,&Omode2,&Omode3);
}

/*
 * r s e r v s w
 *
 * receive server state switcher
 */

rservsw()  {

	/* initialize */

	Sendnext = TRUE;
	state = 'F';
	numtry = oldtry = 0;
	n = 1;
	Transerr = FALSE;

	*Filnam = '\0';

	while (TRUE && Serve) {
		if (debug)
			fprintf(dpfp, "State: %c\n", state);

		switch (state) {
		case 'F':               /* receive file */
			state = rfile();
			break;

		case 'D':               /* receive data */
			state = rdata();
			break;

		case 'C':               /* complete */
			return (TRUE);

		default:                /* abort or unknown */
			if (fp) {
				fclose(fp);

				if (!Incom)
					delete(Fileas);
			}

			return (FALSE);
		}
	}
}

/*
 * e x e c p    
 *
 * execute a Kermit command packet
 */

execp(packet)
char *packet;
{
	char commandname[MAXWORD],                      /* command name */
	     args[MAXARGS][MAXWORD];                    /* command arguments */

	int argno,                                      /* argument number */
	    command;                                    /* representation */    

	/* initialize */

	argno = 0;

	/* get the command name and arguments */

	packet = getword(packet, commandname, Delim);
        
	/* make sure the command is legal */

	if ((command = scan(commandname, Commands)) == NOOP)  { 
		spack('E', 0, 16, "No such command.");
		return;
	    }
	else
	    {
	     /* place the arguments into the argument list */

	     while (*packet)
		    packet = getword(packet, args[argno++], Delim);

	     /* perform the command and acknowledge */

	     parse(command, args, argno);  
	     spack( 'Y', 0, 0, 0 );
	    }
}

/* 
 * i s e n d
 *
 * send the init package
 */

isend()  {
	int num,                                        /* Packet number */
	    len;                                        /* Packet length */

	char type;                                      /* Packet type */

	int recbreak();                                 /* ^c address */

	/* initialize */

	Serve = TRUE;

	numtry = 0;

	signal(sigabort, recbreak);

	Eightpref = image ? FALSE : Eightc;
	Reppref = image ? FALSE : Repc;

	flushinput();

	/* set up the init packet */

	spar(packet);

	if (Filter) {
		if (!setfilter())
			return (FALSE);
	}
	else
		Infile = ttyfd;

	/* and keep sending it until and acknowledgement */

	while (numtry++ <= Retry && Serve) {
		spack('I', 0, 10, packet);

		if ((type = rpack(&len, &num, recpkt)) == 'Y' && !num) {
			rpar(recpkt);
			if (Filter)
				closefilter();
			return (TRUE);
		}
		else if (type == 'E') {
			prerrpkt(recpkt);
			if (Filter)
				closefilter();
			return (FALSE);
		}
	}

	shellmsg("Init packet not accepted.");
	if (Filter)
		closefilter();

	return (FALSE);
}

/*
 * g e t
 *
 * send a receive-initiate packet
 */

get(filename)
char *filename;
{
	int num,
	    len;

	char type;

	int recbreak();

	numtry = 0;

	Serve = TRUE;

	if (Filter) {
		if (!setfilter())
			return (FALSE);
	}
	else
		Infile = ttyfd;

	signal(sigabort, recbreak);

	flushinput();

	while (numtry++ <= Retry && Serve) {
		spack('R', 0, strlen(filename), filename);
	        
		if ('S' == (type = rpack(&len, &num, recpkt))) { 
			if (Filter)
				closefilter();
	        
			spack('N', 0, 0, 0);

			return (recsw());
		}
		else if (type == 'E') {
			prerrpkt(recpkt);
			if (Filter)
				closefilter();
			return (FALSE);
		}
	}
	if (Filter)
		closefilter();

	return (FALSE);
}

/*
 * g e t f i l e s
 *
 * get a list of files
 */

getfiles(args, argno)
char args[MAXARGS][MAXWORD];
int argno;
{
	char filenam[MAXWORD],
	     as[MAXWORD];

	int i,                  /* a counter */
	    files,              /* numbers of files gotten successfully */
	    chars;              /* number of characters transferred */

	if (remote) {
		shellmsg("The get command is for local usage only.");
		return;
	}

	files = chars = 0;

	if (!argno) {
		fputs("Receive: ", stdout);
		gets(filenam);
		fputs("As: ", stdout);
		gets(as);

		if (*as != 0)
			strcpy(Filnam, as);

		if (get(filenam)) 
			files = 1;

		chars = Chin;
	}
	else {
		for (i = 0; i < argno; i++) {
			*Filnam = '\0' ;

			if (!get(args[i]))
				shellmsg("Could not get %s\n", args[i]);
			else
				++files;
			        
			chars += Chin;
			Chin = 0;
		}
	}

	Filesin = files;
	Chin = chars;
	if (fp)
	    {
	     fclose( fp ) ;
	     fp = NULL ;
	    } ;
	if (!Filesin)                                 
		shellmsg("Get unsuccessful");
	else
		shellmsg("Done.");
}

/*
 * s e n d g e n
 * 
 * send a general server command
 */

sendgen(command)
char command;
{
	int len,                /* Packet length */
	    num;                /* Packet number */

	char type;              /* Packet type */
		     
	int recbreak();

	Serve = TRUE;

	numtry = 0;
        
	signal(sigabort, recbreak);

	flushinput();

	if (Filter) {
		if (!setfilter())
			return (FALSE);
	}
	else
		Infile = ttyfd;

	*packet = command;
	packet[1] = '\0';

	while (numtry++ <= Retry && Serve) {
	        
		spack('G', 0, 1, packet);

		if ((type = rpack(&len, &num, recpkt)) == 'Y' && !num) {
			if (Filter)
				closefilter();

			return (TRUE);
		}
		else if (type == 'E') {
			prerrpkt(recpkt);
			if (Filter)
				closefilter();
			return (FALSE);
		}
	}

	shellmsg("Too many tries.");
	if (Filter)
		closefilter();

	return (FALSE);
}
 
/*
 * s e n d r e m
 *
 * send a remote command
 */

sendrem(args, argno)
char args[MAXARGS][MAXWORD];
int argno;
{
	int i,
	    num,
	    len;

	char type, komstr[] ;
	static char helpstr[] = ' ' ;
	int recbreak(), strcat() ;

	signal(sigabort, recbreak);

	Serve = TRUE;

	if (Filter) {
		if (!setfilter())
			return (FALSE);
	}
	else
		Infile = ttyfd;

	flushinput();
	*komstr = '\0' ;
	for (i = 0; i < argno; i++) 
	     {
	      strcat( komstr, args[i] ) ;
	      strcat( komstr, helpstr ) ;
	     }                
	numtry = 0;
	while (numtry++ <= Retry && Serve) 
	       {
		spack('K', 0, strlen( komstr ), komstr );                
		if ((type = rpack(&len, &num, recpkt)) == 'Y' && !num)
		    break;
		else 
		    if (type == 'E') 
			{
			  prerrpkt(recpkt);
			  break;
			}
		}
        
	if (Filter)
		closefilter();
} 
