/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K s h e l l . c
 *
 *  Shell for K e r m i t  File Transfer Utility.
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
 *     . Egon Zakrajsek, Cromemco Inc., Aug 1987
 *
 *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 */




#include "kermit.h"


extern char *rscan();


/*
 * g e t w o r d
 *
 * getword gets the next word, here defined as a nonempty string
 * delimited by characters in chars, starting at line and places
 * it in word; returns the pointer to the beginning of the next
 * word.
 */

char *
getword(line, word, chars)
register char *line;            /* starting position */
register char *word;            /* location to deposit the text */
char *chars;                    /* delimiter characters */
{
	register quoted;        /* are we taking everything verbatim? */

	if (debug > 1)
		printf("> (Getword) line = '%s'\n", line);

	/* advance to text; if none found, return */

	while (strchr(chars, *line) || !*line)
		if (!*line++) {
			*word = '\0';
			return (line - 1);
		}

	/* Don't quote until further instructions */

	quoted = FALSE;

	/* copy the text until we hit the delimiter or end of line */

	while ((quoted || !strchr(chars, *line)) && *line)
		if (*line == Iquote && *++line != Iquote)
			quoted = ~quoted;
		else
			*word++ = *line++;

	/* end the word */

	*word = '\0';

	/* advance past all the other delimiters */

	while (strchr(chars, *line) && *line)
		++line;

	/* and return the pointer to the next word (or end of line) */

	return (line);
}

/*
 * i n t e r
 *
 * command interpreter
 */

inter() {
	char line[MAXLINE],             /* command line */
	     args[MAXARGS][MAXWORD],    /* arguments */
	     comword[MAXWORD],          /* command word */
	    *lgets();

	register char *inline;          /* a pointer to line */

	register i;                     /* a counter */

	int command;                    /* command code */

	/* loop around forever (an exit command should be issued sometime) */

	while (Flevel >= 0) {
		/* disable cntrl-C */
		signal( sigabort, 1 ) ;    /*  D.D. */
		if (Flevel == 0 && Initcon)
			return;

		*comword = '\0';
		*line = '\0';

		if (!fileno(Ft[Flevel]))
			fputs(Prompt, stdout);

		/* read the command and skip the step if null */

		inline = getword(lgets(line), comword, Delim);

		if (*comword == '\0')
			continue;

		/* scan the command */

		command = scan(comword, Commands);

		if (command == NOOP) {
		    shellmsg( "Invalid Command." ) ;
		    continue;
		}

		for (i = 0; *inline; i++) {
			inline = getword(inline, args[i], Delim);

			if (debug > 1)
				printf("> (Inter) argument # %d is '%s'\n",
					i + 1, args[i]);
		}

		if (debug > 1)
			printf("> (Scan) Total arguments: %d\n", i);

		*args[i] = '\0';

		/* and execute it */

		parse(command, args, i);
	}
}

/*
 * s c a n
 *
 * locate the command in the command table
 */

scan(command, table)
char *command;                  /* command string */
struct sctable *table;          /* command table */
{
	register i;             /* a counter */

	/* look through the table until found, or until the end */

	if (debug > 1)
		printf("> (Scan) Looking up '%s'\n", command);

	for (i = 0; strcmp(table[i].cmd, command); i++)
		if (!*(table[i].cmd))
			return (NOOP);

	return (table[i].op);
}

/*
 * p a r s e
 *
 * perform the appropriate action for command with args as arguments
 */

parse(command, args, numargs)
int command;                    /* the define value for the command */
char args[MAXARGS][MAXWORD];    /* arguments */
int numargs;
{
	if (debug > 1)
		printf("> (Parse) Command = %d\n", command);

	/* command ? is equivalent to help command */

	if (**args == '?') {
		helpout(command);
		return;
	}

	/* and make sure the command is executed */

	switch (command) {
	case QUIT:      /* quit */

		/* if at level 0, it means exit Kermit */

		if (!Flevel) {

			/* if we are not in remote, restore the line */

			if (ttyfd)
				ttres(ttyfd, &oldm1, &oldm2, &oldm3);

			/* and indicate that we are exiting */

			if (!Initcon)
				puts(Seeya);
		}

		/* decrease the level of execution */

		--Flevel;
		return;

	case CONNECT:   /* connect to a remote */

		/* if there are arguments, they are set arguments */

		if (numargs)
			set(args, numargs);

		connect();
		return;

	case HELP:      /* help */
		help(args, numargs);
		return;

	case SEND:      /* send a file or files */
		if (Serve)
		    spack( 'Y', 0, 0, 0 ) ;
		send(args, numargs);
		return;

	case RECEIVE:   /* receive a file or files */
		if (Serve)
		    spack( 'Y', 0, 0, 0 ) ;
		receive(args, numargs);
		return;

	case SHOW:      /* show the values of the parameters */
		show(args, numargs);
		return;

	case SET:       /* set the values of parameters */
		set(args, numargs);
		return;

	case SHELL:     /* execute a shell command line */
		nshell(args, numargs);
		return;

	case TAKE:      /* execute a file or files */
		take(args, numargs);
		return;

	case DEF:       /* define command */
		defopt(args, numargs);
		return;

	case TRANS:     /* transmit command */
		transmit(args, numargs);
		return;

	case STATS:     /* statistics */
		stats();
		break;

	case CWD:       /* change directory */

		/* if no arguments, go back to the home directory */

		if (!numargs)
			strcpy(args[0], "..");

		/* and change to that directory */

		if (setdir(args[0]))
			shellmsg("Can't change to %s", args[0]);
		break;

	case SERVER:    /* server */
		rserver();
		break;

	case GET:       /* get */
		getfiles(args, numargs);
		break;

	case FINISH:    /* cause the connecting server to shut down */
		sendgen('F');
		break;

	case DISC:      /* cause the connecting line to log itself off */
		sendgen('L');
		break;

	case INITIAL:   /* exchange parameters */
		isend();
		break;

	case REMOTEC:   /* remote command */
		sendrem(args, numargs);
		break;

	case PUSH:      /* enter a Kermit subshell */
		if (Flevel == MAXLEVELS - 1)
			shellmsg("Too many levels of execution");
		else
			Ft[++Flevel] = stdin;
		break;

	default:        /* system error */
		shellmsg("Invalid command -- Kermit error");
		return;
	}
}

nshell(args, numargs)
char args[MAXARGS][MAXWORD];
int numargs;
{
	char *shargs[MAXARGS];  /* arguments passed to the shell process */
	int sigufun, sigafun;
	register i;             /* a counter */

	/* disable signals */

	sigafun = signal(sigabort, 1);
	sigufun = signal(siguser, 1);

	/* the zeroth argument should be sh */

	shargs[0] = "sh";

	/* if there are arguments, create the argument list */

	if (numargs) {
		shargs[1] = "-p";

		for (i = 0; *(shargs[i + 2] = args[i]); i++)
			if (debug > 1)
				printf("> (Nshell) Arg #%d is '%s'\n", i + 1,
					args[i]);

		shargs[i + 2] = 0;
	}
	else
		shargs[1] = 0;

	/* and execute the command */

	wait(FALSE, fshell(shargs, siguser|sigabort, 0), statuses);
	putchar('\n');

	/* don't forget to return sigs when you are done */

	signal(sigabort,sigafun);
	signal(siguser,sigufun);
	}

/*
 * h e l p
 *
 * prints the appropriate help message
 */

help(args, numargs)
char args[MAXARGS][MAXWORD];
int numargs;
{
	register i;             /* a counter */

	/* Call helpout with every argument */

	if (!numargs)
		helpout(NOOP);
	else
		for (i = 0; *args[i]; i++)
			helpout(scan(args[i], Commands));
}

/*
 * h e l p o u t
 *
 * Output the help file associated with a command
 */

helpout(command)
register command;
{
	int status[2];
	int sigafun, sigufun;
	static char *Help[5] = {
		"help",
		"-d",
		KERMITDIR,
		"",                     /* topic name */
		NULL
		};

	if (debug > 1)
		printf("> (Helpout) Helping with #%d\n", command);

	/* make sure the command is legal */

	if (command == MAXCOM)          /* sign on */
		{ printf(_ver.logon) ;
		  printf("\nFor help type ? or help\n\n");
		return;
		}
	if (command == NOOP)
		Help[3] = NULL;  /* give user topic directory */
	else
		Help[3] = rscan(command,Commands);

	/* make sure signals will not get us out of Kermit */

	sigafun = signal(sigabort, 1);
	sigufun = signal(siguser, 1);

	/* copy the help file to the screen */

	wait(FALSE, fexec(CHELP, Help, siguser|sigabort, 0), status);

	signal(sigabort,sigafun);
	signal(siguser,sigufun);
}

/*
 * l g e t s
 *
 * lgets gets a line and converts all uppercases to lowercases
 */

char *
lgets(line)
register char *line;
{
	register char *inline;          /* a pointer to line */

	register low;                   /* lowercase everything? */

	/* initialize the variables */

	inline = line - 1;
	low = TRUE;

	/* repeat until the end of the line */

	do {

		/* get a character */

		*++inline = getc(INF);

		/*
		 *  if it's a quote, we either enter or exit lowercase zone
		 *  note that two quotes together represent one quote
		 */

		if (*inline == Iquote && (*++inline = getc(INF)) != Iquote)
				low =  ~low;

		/* if lowercase is on, do it if necessary */

		if (*inline >= 'A' && *inline <= 'Z' && low)
			*inline += 'a' - 'A';
	} while (*inline != '\n' && *inline != EOF);

	/* if end-of-file, it's an exit */

	if (*inline == EOF)
		strcpy(line, Flevel || Initcon?"exit":"");
	else {

		/* if necessary, add a quote at the end */

		if (~low)
			*inline++ = Iquote;

		*inline = '\0';
	}

	if (debug > 1)
		printf("> (Lgets) Line = '%s'\n", line);

	return (line);
}

/*
 * s e n d
 *
 * send a file or files to the other Kermit
 */

send(args, numargs)
char args[MAXARGS][MAXWORD];
int numargs;
{
	register i;             /* a counter */

	/* clear stacked NAKs */
	flushinput() ;
	if (!numargs)  {
		printf("Send: ");
		gets(Filnam);
		printf("as: ");
		gets(Fileas);

		/* and send */

		if (!sendsw())
			shellmsg("Send failed.");
		else
			shellmsg("Done.");
	}
	else {
		/* we are going to be sending the files as themselves */

		*Fileas = '\0';

		/* convert the argument list to the file list format */

		filecount = 0;

		for (i = 0; i < numargs; i++)
			filecount = getnames(filelist, args[i], filecount);

		Onfile = 0;

		if (filecount == 0) {
			shellmsg("Can't find any of those.");
			return;
		}

		/* and get the first file's name */

		gnxtfl();

		/* configure the terminal */

		if (remote)
			ttbin(STDIN,&Omode1,&Omode2,&Omode3);

		/* and see what we've got */

		if (sendsw() == FALSE)
			shellmsg("Send failed.");
		else
			shellmsg("Done.");

		/* restore the terminal */

		if (remote)
			ttres(STDIN,&Omode1,&Omode2,&Omode3);
	}

	/* close the file pointer if we have to */

	if (fp) {
		fclose(fp);
		fp = NULL;
	}
}

/*
 * r e c e i v e
 *
 * receive a file from the other Kermit
 */

receive(args)
char args[MAXARGS][MAXWORD];
{
	/* clear up any stacked NAKs or whatever */

	flushinput();

	/* get the filename to receive if different than the send filename */

	strcpy(Filnam, args[0]);

	/* configure the terminal */

	if (remote)
		ttbin(STDIN,&Omode1,&Omode2,&Omode3);

	/* see what we get if we try to receive */

	if (recsw())
		shellmsg("Done.");
	else
		shellmsg("Receive failed.");

	/* restore the terminal */

	if (remote)
		ttres(STDIN,&Omode1,&Omode2,&Omode3);

	/* see if we have to close the file pointer */

	if (fp) {
		fclose(fp);
		fp = NULL;
	}
}

/*
 * s e t
 *
 * set an option
 */

set(args, numargs)
char args[MAXARGS][MAXWORD];
int numargs;
{
	register i,             /* an index to the argument list */
		 option;        /* internal option representation */

	/* make sure there are arguments */

	if (numargs == 0)
		shellmsg("No arguments given for a set command.");

	/* and evaluate them */

	for (i = 0; i < numargs; i++) {

		/* see what they are */

		option = scan(args[i], Options);

		/* if not a predefined option, see if it is a macro */

		if (option == NOOP)
			defscan(args[i]);

		/* else, set it */

		else
			i = setopt(option, args, i);
	}
}

/*
 * s e t o p t
 *
 * actually set an option
 */

setopt(option, args, argno)
int option;
char args[MAXARGS][MAXWORD];
register argno;
{
  register mode,                  /* temporary locations */
	   temp;

  FILE *fopen();

  if (Serve)
      {
       switch (option)
	   {

	    case DELAY:             /* set the send delay */
		if (!*args[++argno])
			shellmsg("No delay specified.");
		else
			Sdelay = atoi(args[argno]);
		break;

	    case FILEP:             /* file attribute setting */
		switch (scan(args[++argno], Filep)) {
		    case TEXT:
			Filebin = FALSE;
			image = FALSE;
			break;

		    case BIN:
			Filebin = TRUE;
			image = FALSE;
			break;

		    case IMAGE:
			Filebin = TRUE;
			image = TRUE;
			break;

		    default:
			shellmsg("Illegal file attribute.");
			break;
		}
		break;

	    case RETRY:             /* retry count */
		Retry = atoi(args[++argno]);
		break;

	    case SENDP:             /* send parameters */
		switch (scan(args[++argno], Sendrec)) {
		    case EOL:               /* end-of-packet character */
			Seol = *args[++argno];
			break;

		    case PL:                /* page length */
			temp = atoi(args[++argno]);
			if (temp < 10 || temp > 94)
				shellmsg("Illegal packet size.");
			else
				spsiz = temp;
			break;

		    case TO:                /* timeout interval */
			timint = atoi(args[++argno]);
			break;

		    case PAU:
			shellmsg("Pause is not implemented yet.");
			break;

		    case PAD:               /* padding */
			Spad = atoi(args[++argno]);
			break;

		    case PADC:              /* padding character */
			Spadchar = *args[++argno];
			break;

		    case QUO:               /* control quote */
			Squote = *args[++argno];
			break;

		    case SOP:               /* start-of-packet */

			Ssop = *args[++argno];
			break;

		    default:
			shellmsg("Illegal send option.");
			break;
		}
		break;

	    case RECP:              /* receive parameters */
		switch (scan(args[++argno], Sendrec)) {
		    case EOL:
			Reol = *args[++argno];
			break;

		    case PL:
			temp = atoi(args[++argno]);
			if (temp < 10 || temp > 94)
				shellmsg("Illegal packet size.");
			else
				Maxpack = temp;
			break;

		    case TO:
			Timeout = atoi(args[++argno]);
			break;

		    case PAU:
			shellmsg("Pause is not implemented yet.");
			break;

		    case PAD:
			Rpad = atoi(args[++argno]);
			break;

		    case PADC:
			Rpadchar = *args[++argno];
			break;

		    case QUO:
			Rquote = *args[++argno];
			break;

		    case SOP:
			Rsop = *args[++argno];
			break;

		    default:
			shellmsg("Illegal receive option.");
			break;
		}
		break;

	    case REPP:                      /* repeat prefix */
		Repc = *args[++argno];
		if (Repc == 'n')
			Repc = FALSE;
		break;

	    case EIGHTP:                    /* eight-bit prefix */
		Eightc = *args[++argno];
		if (Eightc == 'n')
			Eightc = FALSE;
		break;

	    default:
		shellmsg( "Can't set parameter for remote host");
		spack( 'E', 0, 33, "Can't set parameter for remote host");

	   }
       }
   else

	switch (option) {
	case BAUD:              /* change the baud rate */
		/* if a question mark or nothing, help */

		if (*args[++argno] == '?' || !*args[argno])
			printf("Enter a legal baud rate from 300 to 19200\n");
		else {
			/* get the baud rate */

			mode = scan(args[argno], Speeds);

			/* see if it's found */

			if (mode == NOOP)
				shellmsg("Illegal speed.");
			else {

				/* make sure we can set it */

				if (isconsole(ttyfd) ||
				    setmode(ttyfd, MD_ISPEED, mode, 0) < 0 )
					shellmsg("Attempt failed.");
				else
					Baud = mode;
			}
		}
		break;

	case BLOCK:             /* block check */
		shellmsg("Block check setting is not yet implemented.");
		argno++;
		break;

	case DEBUG:             /* debug options */
		switch (scan(args[++argno], Doptions)) {
		case STATES:    /* states debug */
			debug++;
			break;

		case PACKETS:   /* packet debug */

			/* if it isn't on yet, open the log file */

			if (!pktdeb) {
				if (*Logf)
					dpfp = fopen(Logf, "w");
				else
					dpfp = stdout;
			}

			pktdeb = TRUE;
			break;

		case NOSTAT:    /* turn states debug off */
			debug--;
			break;

		case NOPACK:    /* turn packet debug off */
			pktdeb = FALSE;
			if ( dpfp != stdout )
			    fclose( dpfp ) ;
			break;

		case OFF:       /* turn all debugging off */
			debug = FALSE;
			pktdeb = FALSE;
			break;

		case LOGFILE:   /* log file */

			/* close whatever log file is open */

			if (dpfp != stdout)
				fclose(dpfp);

			/* if no/off argument, it's now the tty */

			if (!*args[++argno] || !strcmp(args[argno], "off")) {
				*Logf = '\0';
				dpfp = stdout;
			}
			else
				strcpy(Logf, args[argno]);

			if (*Logf && pktdeb && !(dpfp = fopen(Logf, "w")))
				shellmsg("Can't open %s", Logf);
			break;

		default:        /* no such option */
			shellmsg("Illegal debug specification.");
			break;
		}
		break;

	case DELAY:             /* set the send delay */
		if (!*args[++argno])
			shellmsg("No delay specified.");
		else
			Sdelay = atoi(args[argno]);
		break;

	case DUPLEX:            /* full or half */
		switch (scan(args[++argno], Duplex)) {
		case HALF:
			lecho = TRUE;
			break;

		case FULL:
			lecho = FALSE;
			break;

		default:
			shellmsg("Illegal duplex setting.");
			break;
		}
		break;

	case ESCAPE:            /* escape character specification */
		if (!*args[++argno])
			shellmsg("No escape character specified.");
		else
			escchr = *args[argno];
		break;

	case FILEP:             /* file attribute setting */
		switch (scan(args[++argno], Filep)) {
		case TEXT:
			Filebin = FALSE;
			image = FALSE;
			break;

		case BIN:
			Filebin = TRUE;
			image = FALSE;
			break;

		case IMAGE:
			Filebin = TRUE;
			image = TRUE;
			break;

		default:
			shellmsg("Illegal file attribute.");
			break;
		}
		break;

	case FLOW:              /* flow control */
		shellmsg("Flow control is not implemented yet.");
		++argno;
		break;

	case HAND:              /* handshake */
		mode = scan(args[++argno], Handshake);

		if (mode == NOOP)
			shellmsg("Illegal handshake specification.");
		else {
			turn = (mode != NOHS);
			Hand = Handchars[mode];
		}
		break;

	case INC:               /* incomplete files */
		switch (scan(args[++argno], Yesno))  {
		case YES:
			Incom = TRUE;
			break;

		case NO:
			Incom = FALSE;
			break;

		default:
			shellmsg("Illegal incomplete specification.");
			break;
		}
		break;

	case INQ:               /* input quote */
		Iquote = *args[++argno];
		break;

	case LINE:              /* communication line setting */
		if ((mode = open(args[++argno], 2)) < 0)
			shellmsg("Can't open %s", args[argno]);
		else if (isconsole(mode)) {
			shellmsg("Can't set line to %s", args[argno]);
			(void) close(mode);
		}
		else {

			/* restore the original line setting */

			if (ttyfd) {

				if (debug > 1)
					printf("> (Parse) restoring line %d\n",
								      ttyfd);

				ttres(ttyfd, &oldm1, &oldm2, &oldm3);
				close(ttyfd);
			}

			/* get the terminal name */

			ttyname = (char *) malloc(strlen(args[argno]) + 1);
			strcpy(ttyname, args[argno]);

			/* set the terminal mode */

			if (debug > 1)
				printf("> (Parse) setting %d\n", ttyfd);

			ttyfd = mode;
			ttbin(ttyfd, &oldm1, &oldm2, &oldm3);

			if (AUTOSET) {
				setmode(ttyfd, MD_MODE1,
					 (parflg == ODD | parflg == EVEN) ?
					    parflg : ODD | EVEN, ODD | EVEN);
				setmode(ttyfd, MD_ISPEED, Baud, 0);
			}

			oldtty = 0;
			remote = FALSE;
		}
		break;

	case PARITY:            /* parity setting */
		mode = ODD | EVEN;

		switch (temp = scan(args[++argno], Parity)) {
		case NONE:
		case MARK:
		case SPACE:
			break;

		case ODD:
			mode = ODD;
			break;

		case EVEN:
			mode = EVEN;
			break;

		default:
			shellmsg("Illegal parity setting.");
			return;
		}

		parflg = temp;

		if (ttyfd)
			setmode(ttyfd, MD_MODE1, mode, ODD | EVEN);
		break;

	case PROMPT:            /* prompt */
		strcpy(Prompt, args[++argno]);
		break;

	case RETRY:             /* retry count */
		Retry = atoi(args[++argno]);
		break;

	case SENDP:             /* send parameters */
		switch (scan(args[++argno], Sendrec)) {
		case EOL:               /* end-of-packet character */
			Seol = *args[++argno];
			break;

		case PL:                /* page length */
			temp = atoi(args[++argno]);
			if (temp < 10 || temp > 94)
				shellmsg("Illegal packet size.");
			else
				spsiz = temp;
			break;

		case TO:                /* timeout interval */
			timint = atoi(args[++argno]);
			break;

		case PAU:
			shellmsg("Pause is not implemented yet.");
			break;

		case PAD:               /* padding */
			Spad = atoi(args[++argno]);
			break;

		case PADC:              /* padding character */
			Spadchar = *args[++argno];
			break;

		case QUO:               /* control quote */
			Squote = *args[++argno];
			break;

		case SOP:               /* start-of-packet */

			Ssop = *args[++argno];
			break;

		default:
			shellmsg("Illegal send option.");
			break;
		}
		break;

	case RECP:              /* receive parameters */
		switch (scan(args[++argno], Sendrec)) {
		case EOL:
			Reol = *args[++argno];
			break;

		case PL:
			temp = atoi(args[++argno]);
			if (temp < 10 || temp > 94)
				shellmsg("Illegal packet size.");
			else
				Maxpack = temp;
			break;

		case TO:
			Timeout = atoi(args[++argno]);
			break;

		case PAU:
			shellmsg("Pause is not implemented yet.");
			break;

		case PAD:
			Rpad = atoi(args[++argno]);
			break;

		case PADC:
			Rpadchar = *args[++argno];
			break;

		case QUO:
			Rquote = *args[++argno];
			break;

		case SOP:
			Rsop = *args[++argno];
			break;

		default:
			shellmsg("Illegal receive option.");
			break;
		}
		break;

	case REMOTE:                    /* make the terminal the line */
		switch (scan(args[++argno], Yesno)) {
		case YES:
			if (ttyfd)
				oldtty = ttyfd;

			remote = TRUE;
			ttyfd = 0;
			break;

		case NO:
			if (!ttyfd)
				ttyfd = oldtty;
			remote = FALSE;
			break;

		default:
			shellmsg("Illegal remote setting.");
			break;
		}
		break;

	case REPP:                      /* repeat prefix */
		Repc = *args[++argno];
		if (Repc == 'n')
			Repc = FALSE;
		break;

	case EIGHTP:                    /* eight-bit prefix */
		Eightc = *args[++argno];
		if (Eightc == 'n')
			Eightc = FALSE;
		break;

	case FILTER:                    /* IBM filter */
		switch (scan(args[++argno], Yesno)) {
		case YES: Filter = TRUE ;
			  break ;
		case NO:  Filter = FALSE ;
			  break ;
		default:
			shellmsg("No Filter present");
			break;
		}
		break;

	case WILD:                      /* Wildcard characters */
		switch (scan(args[++argno], Wildcards)) {
		case WILDALL:
			Allwild = *args[++argno];
			break;

		case WILDONE:
			Onewild = *args[++argno];
			break;

		default:
			shellmsg("Illegal wildcard specification.");
			break;
		}
		break;

	case WARNING:                   /* File deletion warning */
		switch(scan(args[++argno], Yesno))  {
		case YES:
			Warning = TRUE;
			break;

		case NO:
			Warning = FALSE;
			break;

		default:
			shellmsg("Illegal warning setting.");
			break;
		}
		break;

	case SYNCHRO:                   /* Synchronization */
		switch (scan(args[++argno], Yesno))  {
		case YES:
			Sendall = FALSE;
			break;

		case NO:
			Sendall = TRUE;
			break;

		default:
			shellmsg("Illegal synchronization setting.");
			break;
		}
		break;

	default:
		shellmsg("Illegal option.");
		break;
	}
	return (argno);
}

/*
 * s h o w
 *
 * display a parameter
 */

show(args, argno)
char args[MAXARGS][MAXWORD];
int argno;
{
	register i;             /* a counter */

	/* if there are arguments, go through them, else show everything */

	if (argno)
		for (i = 0; i < argno; i++)
			i = display(scan(args[i], Options), args, i);
	else  {
		disset(args);
		dismac();
	}
}

/*
 * d i s s e t
 *
 * displays all the settings
 */

disset(args)
char args[MAXARGS][MAXWORD];
{
	register i;                     /* a counter */

	puts("Settings:");

	for (i = 0; Impopt[i] != NOOP; i++)
		display(Impopt[i], args, -1);
}

/*
 * d i s o p t
 *
 * display all options
 */

dismac()
{
	register i;

	puts("Macros:");

	for (i = 0; i < Nextopt; i++)
		if (Defopt[i])
			showdef(i);
}

/*
 * d i s p l a y
 *
 * display the status of an option
 */

display(option, args, argno)
int option;
char args[MAXARGS][MAXWORD];
int argno;
{
	register mode;

	char *cstring();

	switch (option) {
	case BAUD:              /* baud rate */
		printf("Baud rate: %s\n", rscan(Baud, Speeds));
		break;

	case DEBUG:             /* debugging level */
		printf("Debugging: Packet %s, States = %d, Log file = %s\n",
				pktdeb?"on":"off", debug, *Logf ? Logf : "tty");
		break;

	case DELAY:             /* remote send delay setting */
		printf("Send delay (only if remote): %d\n", Sdelay);
		break;

	case DUPLEX:            /* remote duplex setting */
		printf("Duplex: %s\n", lecho ? "half" : "full");
		break;

	case ESCAPE:            /* escape character */
		printf("Escape character = '%s' (%o)\n", cstring(escchr),
							   escchr);
		break;

	case EIGHTP:
		if (Eightc)
			printf("Eight-bit prefix is %c\n", Eightc);
		else
			puts("Eight-bit prefixing is off");
		break;

	case REPP:
		if (Repc)
			printf("Repetition prefix is %c\n", Repc);
		else
			puts("Repetition prefixing is off");
		break;

	case FILEP:
		printf("File type is ");
		if (image)
			puts("image");
		else if (Filebin)
			puts("binary");
		else
			puts("text");
		break;

	case HAND:
		printf("Handshake is %s\n", turn ? cstring(Hand) : "off");
		break;

	case INC:               /* incomplete file handling */
		printf("Incomplete file save is %s\n", Incom ? "on" : "off");
		break;

	case INQ:               /* input quote */
		printf("Input quote = '%s'\n", cstring(Iquote));
		break;

	case LINE:              /* line */
		if (!ttyname)
			printf("No line specified.\n");
		else
			printf("Line = %s, fd = %o\n", ttyname, ttyfd);
		break;

	case PARITY:            /* parity */
		printf("Parity: %s\n", rscan(parflg, Parity));
		break;

	case PROMPT:            /* prompt */
		printf("Prompt = '%s'\n", Prompt);
		break;

	case RETRY:             /* retry count */
		printf("Retry count = %d\n", Retry);
		break;

	case SENDP:             /* send parameters */

		/* you can see all of them */

		if (argno < 0 || !*args[++argno]) {
			srshow("Send", Seol, spsiz, timint, Spad, Spadchar,
				       Squote, Ssop);
			return (argno);
		}

		/* or a particular one */

		switch (scan(args[argno], Sendrec)) {
		case EOL:
			printf("Send end of line = %s\n", cstring(Seol));
			break;

		case PL:
			printf("Maximum send packet length = %d\n", spsiz);
			break;

		case TO:
			printf("Send timeout = %ds\n", timint);
			break;

		case PAD:
			printf("Padding = %d\n", Spad);
			break;

		case PADC:
			printf("Padding char = %s\n", cstring(Spadchar));
			break;

		case QUO:
			printf("Send quote = %s\n", cstring(Squote));
			break;

		case SOP:
			printf("Send start-of-packet = %s\n", cstring(Ssop));
			break;

		default:
			shellmsg("I can't show %s", args[argno]);
			break;
		}
		break;

	case RECP:              /* receive parameters */
		if (argno < 0 || !*args[++argno]) {
			srshow("Receive", Reol, Maxpack, Timeout, Rpad,
					   Rpadchar, Rquote, Rsop);
			return (argno);
		}

		switch (scan(args[argno], Sendrec)) {
		case EOL:
			printf("Receive end of line = %s\n", cstring(Reol));
			break;

		case PL:
			printf("Receive packet length = %d\n", Maxpack);
			break;

		case TO:
			printf("Receive timeout = %d\n", Timeout);
			break;

		case PAD:
			printf("Receive padding = %d\n", Rpad);
			break;

		case PADC:
			printf("Receive pad char = %s\n", cstring(Rpadchar));
			break;

		case QUO:
			printf("Receive quote = %s\n", cstring(Rquote));
			break;

		case SOP:
			printf("Receive SOP = %s\n", cstring(Rsop));
			break;

		default:
			printf("Cannot show %s\n", args[argno]);
			break;
		}
		break;

	case REMOTE:            /* remote */
		printf("This terminal is %s\n", remote ? "remote" : "local");
		break;

	case FILTER:            /* Filter */
		/* printf("Filter is %s\n", Filter ? "on" : "off");
		*/
		break;

	case WILD:              /* Wildcard */
		printf("Wild: all = %s, ", cstring(Allwild));
		printf("one = %s\n", cstring(Onewild));
		break;

	case WARNING:           /* Warning */
		printf("Warning is %s\n", Warning ? "on" : "off");
		break;

	case SYNCHRO:           /* Synchronization */
		printf("Synchronization is %s\n", Sendall ? "off" : "on");
		break;

	case OPTI:              /* all options */
		disset(args);
		break;

	case MACRO:             /* all macros */
		dismac();
		break;

	default:                /* see if it is an option; if not, complain */
		if ((mode = deffind(args[argno])) == NOTFOUND)
			shellmsg("Cannot show %s", args[argno]);
		else
			showdef(mode);
		break;
	}
	return (argno);
}
/*
 * r s c a n
 *
 * reverse scan -- find the ASCII representation by the external representation
 */

char *
rscan(value, table)
int value;
struct sctable *table;
{
	while (table->op != NOOP && table->op != value)
		++table;

	if (table->op == NOOP)
		return (NULL);
	else
		return (table->cmd);
}

/*
 * c s t r i n g
 *
 * returns the character string representation
 */

char *
cstring(ch)
char ch;
{
	static char rep[3];     /* character representation */

	if (ch == 127)
		return ("<del>");

	if (ch < ' ') {
		rep[0] = '^';
		rep[1] = ctl(ch);
		rep[2] = '\0';
	}
	else {
		rep[0] = ch;
		rep[1] = '\0';
	}

	return (rep);
}

/*
 * s r s h o w
 *
 * show set or receive options
 */

srshow(string, eol, pl, to, pad, padc, quote, sop)
char *string;
char eol;
int pl;
int to;
int pad;
char padc;
char quote;
char sop;
{
	printf("%s: EOL = %s, PL = %d, ", string, cstring(eol), pl);
	printf("TO = %d, Padding = %d x %s, ", to, pad, cstring(padc));
	printf("Quote = %s, ", cstring(quote));
	printf("SOP = %s\n",  cstring(sop));
}

/*
 * t a k e
 *
 * execute a number of command files
 */

take(args, argno)
char args[MAXARGS][MAXWORD];
register argno;
{
	register i;

	if (!argno) {
		shellmsg("No file given.");
		return;
	}

	for (i = 0; i < argno; i++)
		takefile(args[i]);
}

/*
 * t a k e f i l e
 *
 * execute a single file
 */

takefile(file)
char *file;
{
	if (Flevel + 1 > MAXLEVELS) {
		shellmsg("Too many levels of execution.");
		return;
	}

	/* increment the file level and call inter to execute it */

	if (Ft[++Flevel] = fopen(file, "r"))
		inter();
	else {
		if (!Initcon)
			shellmsg("Can't open %s", file);

		--Flevel;
	}
}

/*
 * defopt
 *
 * define an option
 */

defopt(args, numargs)
char args[MAXARGS][MAXWORD];
int numargs;
{
	register i,     /* counters */
		 j;

	if (numargs == 0) {
		shellmsg("(Define) No arguments given.");
		return;
	}

	if (debug > 1)
		printf("> (defopt) Trying to %sdefine %s\n",
			  numargs == 1 ? "un" : "", args[0]);

	/* if no arguments, undefine the option */

	if (numargs == 1) {
		if ((i = deffind(args[0])) == NOTFOUND)
			shellmsg("%s can't be undefined.", args[0]);
		else {
			free(Defopt[i]);
			Defopt[i] = NULL;
		}

		return;
	}

	/* else, define it if not already defined */

	if (deffind(args[0]) != NOTFOUND || scan(args[0], Options)) {
		shellmsg("%s already defined.", args[0]);
		return;
	}

	/* find the first empty hole in the list */

	for (i = 0; i < Nextopt; i++)
		if (!Defopt[i])
			break;

	if (i == Nextopt) {
		if (Nextopt == MAXOPT)
			shellmsg("Maximum number of options exceeded.");
		else
			++Nextopt;
	}

	Defopt[i] = (struct dtable *) malloc(sizeof (struct dtable));

	strcpy(Defopt[i]->name, args[0]);

	for (j = 1; j < numargs; j++)
		strcpy(Defopt[i]->options[j - 1], args[j]);

	Defopt[i]->numoptions = numargs - 1;
}

/*
 * d e f f i n d
 *
 * find a defined option in the table
 */

deffind(name)
char *name;
{
	register i;

	for (i = 0; i < Nextopt; i++)
		if (Defopt[i] && !strcmp(name, Defopt[i]->name))
			return (i);

	return (NOTFOUND);
}

/*
 * d e f s c a n
 *
 * scan the define option and call set with it
 */

defscan(name)
char *name;
{
	register i;

	if ((i = deffind(name)) == NOTFOUND) {
		shellmsg("(Set) %s not a valid option", name);
		return;
	}

	set(Defopt[i]->options, Defopt[i]->numoptions);
}

/*
 * s h o w d e f
 *
 * show the value of a defined option
 */

showdef(opt)
register opt;
{
	register i;

	fputs(Defopt[opt]->name, stdout);
	putchar(':');

	for (i = 0; i < Defopt[opt]->numoptions; i++) {
		putchar(' ');
		fputs(Defopt[opt]->options[i], stdout);
	}

	putchar('\n');
}

/*
 * t r a n s m i t
 *
 * transmit a file bare, without any protocol
 */

transmit(files, numfiles)
char files[MAXARGS][MAXWORD];
register numfiles;
{
	register i;

	char c;

	if (numfiles == 0) {
		shellmsg("No files to transmit.");
		return;
	}

	for (i = 0; i < numfiles; i++) {
		if (!(fp = fopen(files[i], "r")))
			shellmsg("Can't open %s", files[i]);
		else {
			while ((c = getc(fp)) != EOF)
				write(ttyfd, &c, 1);

			fclose(fp);
		}
	}
}

/*
 * e x e c l
 *
 * execute a line
 */

execl(argv, argc)
char **argv;
int argc;
{
	char args[MAXARGS][MAXWORD];            /* args in kermit format */

	int command, i, next;

	next = 1;

	while (next < argc) {
	    command = scan(argv[next], Commands);
	    next++;
	    for (i = 0;
		 next < argc && scan(argv[next], Commands) == NOOP;
		 next++, i++
	    ) strcpy(args[i], argv[next]);

	    *args[i] = '\0';

	    parse(command, args, i);

	}
}

/*
 * s t a t s
 *
 * Display the statistics for the last file transfer
 */

stats() {
	printf("Last file transfer");

	if (Chin) {
		printf(":\nFiles transmitted successfully: %d\n", Filesin);
		printf("Characters transmitted: %d\n", Chin);
		printf("Baud rate: %s\n", rscan(Lastbaud, Speeds));
		printf("Line: %s\n", *Lastty ? Lastty : "remote");
	}
	else
		puts(", if any, was unsuccessful.\n");
}

/*
 * i s c o n s o l e
 *
 * Return nonzero if the specified channel refers to console
 * (or to a device that is not a character device)
 */

int isconsole(fd)
int fd;
{
	static char tty[] = "/dev/tty";

	return (cstat(fd, st_ftype) != is_char ||
		cstat(fd, st_pdevno) == fstat(tty, st_pdevno));
}
