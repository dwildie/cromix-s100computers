/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K e r m i t . h
 *
 *  Definitions for K e r m i t  File Transfer Utility.
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



#include <stdio.h>          /* Standard UNIX definitions */
#include <jsysequ.h>        /* For CROMIX ..*/
#include <modeequ.h>

#define KERMITDIR "/usr/pkg/kermit"     /* further installation dependents */
#define LEAPFROG "/usr/pkg/kermit/leapfrog.bin"
#define CHELP "/bin/help.bin"
#define INITMANY "/etc/kermit.ini"
#define INITMINE "kermit.ini"
#define INITLOG ""

/* Symbol Definitions */

#define MAXLINE     127     /* Maximum input line length */
#define MAXWORD     30      /* Maximum word length */
#define MAXARGS     15      /* Maximum number of arguments */
#define MAXFILE     50      /* Maximum number of files to send */
#define MAXCOM      25      /* Maximum number of commands */
#define MAXLEVELS   15      /* Maximum number of levels of execution */
#define MAXOPT      35      /* Maximum defined options allowed */

#define NOTFOUND    -1      /* Not found */

#define MAXPACKSIZ  94      /* Maximum packet size */
#define SOH         1       /* Start of header */
#define CR          015     /* ASCII Carriage Return */
#define SP          040     /* ASCII space */
#define DEL         127     /* Delete (rubout) */
#define XON         17      /* ASCII XON */

#define MAXTRY      10      /* Times to retry a packet */
#define MAXREPC     94      /* The last legal repeat count */
#define MYQUOTE     '#'     /* Quote character I will use */
#define MYEIGHT     '&'     /* 8-bit prefix I will use */
#define MYREPEAT    '~'     /* Repeat character I will use */
#define MYPAD       0       /* Number of padding characters I will need */
#define MYPCHAR     0       /* Padding character I need (NULL) */
#define MYATT       010    /* Attributes mask */

#define MYTIME      10      /* Seconds after which I should be timed out */
#define MAXTIM      60      /* Maximum timeout interval */
#define MINTIM      2       /* Minumum timeout interval */

#define TRUE        -1      /* Boolean constants */
#define FALSE       0

#define INITDEBUG   FALSE  /* initial debug specification */
#define INPUTQUOTE  '"'     /* initial input quote */
#define INITALL     '*'     /* initial string wildcard */
#define INITONE     '?'     /* initial character wildcard */
#define QUOTLENGTH  10      /* maximum quoted sequence length */
			    /* 2 * (5 char sequences) */
        
#define AUTOSET     FALSE   /* automatic setting of baud rate and parity */

#define DEFESC      '~'     /* Default escape character for CONNECT (CCALL's)*/
#define DEFTRN      FALSE   /* Default turn around */
#define DEFLCH      FALSE   /* Default local echo */
#define DEFPAR      FALSE   /* Default parity */
#define DEFIM       FALSE   /* Default image mode */
#define DEFFILEP    FALSE   /* Default file type */
#define DEFFNC      TRUE    /* Default file name conversion */
#define MYEOL       '\n'    /* End-Of-Line character I need */

#define INF     Ft[Flevel]  /* Input file */

#define NOOP    0       /* Invalid command */
#define SET     1       /* Set an option */
#define CONNECT 2       /* Connect to a remote line */
#define SEND    3       /* Send a file */
#define RECEIVE 4       /* Receive a file */
#define HELP    5       /* Print a help message */
#define QUIT    6       /* Exit Kermit */
#define SHELL   7       /* Enter a shell command or a subshell */
#define SHOW    8       /* Show the value of a parameter */
#define TAKE    9       /* Execute a Kermit command file */
#define DEF     10      /* Define a set option */
#define TRANS   11      /* Transmit a file without protocol */
#define STATS   12      /* Statistics */
#define SERVER  14      /* Server */
#define GET     15      /* Get */
#define FINISH  16      /* Finish */
#define CWD     17      /* Change directory */
#define REMOTEC 18      /* Remote command */
#define DISC    19      /* Disconnect command */
#define INITIAL 20      /* Exchange parameters */
#define PUSH    21      /* Get into a new Kermit shell */

/* Set options */

#define BAUD    1
#define BLOCK   2
#define DEBUG   3
#define DELAY   4
#define DUPLEX  5
#define ESCAPE  6
#define FILEP   7
#define FLOW    8
#define HAND    9
#define INC     10
#define LINE    11
#define PARITY  12
#define PROMPT  14
#define RECP    15
#define RETRY   16
#define SENDP   17
#define REMOTE  18
#define INQ     19
#define OPTI    20
#define MACRO   21
#define REPP    22
#define EIGHTP  23
#define FILTER  24
#define WILD    25
#define WARNING 26
#define SYNCHRO 27

/* Debug options */

#define OFF     1
#define NOSTAT  2
#define NOPACK  3
#define STATES  4
#define PACKETS 5
#define LOGFILE 6

/* Yes/no equivalents */

#define YES     1
#define NO      2

/* Half/full duplex */

#define HALF    1
#define FULL    2

/* File attributes */

#define TEXT 1
#define BIN 2
#define IMAGE 3

/* Parity settings */

#define NONE    1
#define MARK    2
#define SPACE   3       /* other parity settings from modeequ.h */

/* Send/receive options */

#define EOL     1
#define PL      2
#define TO      3
#define PAU     4
#define PAD     5
#define PADC    6
#define QUO     7
#define SOP     8

/* Wildcards */

#define WILDALL 1
#define WILDONE 2

/* Handshakes */

#define NOHS    1
#define XOFFH   2
#define XONH    3
#define BELLH   4
#define CRH     5
#define LFH     6
#define ESCH    7

/* Macro Definitions */

/*
 * tochar: converts a control character to a printable one by adding a space.
 *
 * unchar: undoes tochar.
 *
 * ctl:    converts between control characters and printable characters by
 *         toggling the control bit (ie. ^A becomes A and A becomes ^A).
 *
 * unpar:  turns off the parity bit.
 */

#define tochar(ch)  ((ch) + ' ')
#define unchar(ch)  ((ch) - ' ')
#define ctl(ch)     ((ch) ^ 64 )
#define unpar(ch)   ((ch) & 127)

/* getbuf gets the ith char in the buffer */

#define getbuf(str, i)  (image ? (str)[i] : (str)[i] & 0177)

/* Definitions */

struct sctable {                /* scan table */
	char *cmd;              /* command or option name */
	int op;                 /* equivalent */
};

struct dtable {                 /* define table */
	char name[MAXWORD];     /* name */
	char options[MAXARGS][MAXWORD]; /* options */
	int numoptions;                 /* number of options */
};

typedef struct _vheader {
	char            magicnos[8];
	char            version;
	char            release;
	char            logon[50];
	};

/* Global Variables */

extern  size,               /* Size of present data */
	rpsiz,              /* Maximum receive packet size */
	spsiz,              /* Maximum send packet size */
	Spad,               /* How much padding to send */
	timint,             /* Timeout for foreign host on sends */
	n,                  /* Packet number */
	oldm1,              /* old terminal modes */
	oldm2,
	oldm3,
	Omode1,             /* old line modes - JAP */
	Omode2,
	Omode3,
	numtry,             /* Times this packet retried */
	oldtry,             /* Times previous packet retried */
	ttyfd,              /* File descriptor of tty for I/O, 0 if remote */
	oldtty,             /* Old tty I/O descriptor (if we go into remote) */
	remote,             /* -1 means we're a remote kermit */
	image,              /* -1 means 8-bit mode */
	parflg,             /* TRUE means use parity specified */
	turn,               /* TRUE means look for turnaround char (XON) */
	lecho,              /* TRUE for locally echo chars in connect mode */
	debug,              /* Indicates level of debugging output (0=none) */
	pktdeb,             /* TRUE means log all packet to a file */
	filnamcnv,          /* -1 means do file name case conversions */
	speed,              /* speed to set */
	filecount,          /* Number of files left to send */
	Onfile,             /* File number sending */
	Sendagain,          /* Timeout indicator */
	Serve,              /* Continue the server */
	Serving,            /* Server flag */
	Retry,              /* Maximum retry count */
	Nextopt,            /* Next option number */
	Maxpack,            /* Maximum packet length */
	Timeout,            /* Timeout in seconds */
	Rpad,               /* Padding in chars */
	Filebin,            /* File type: text/binary */
	Atacc,              /* Ability of the other guy to accept attributes */
	Helpmore,           /* TRUE as long as we want more help */
	Flevel,             /* File level */
	Initcon,            /* TRUE if initial connection */
	Filesin,            /* Files transferred */
	Chin,               /* Characters transferred */
	Lastbaud,           /* Transfer baud rate */
	Baud,               /* Current baud rate */
	Sendnext,           /* Send next packet? */
	Sendall,            /* Send all of them */
	Transerr,           /* Transmission error? */
	Sdelay,             /* Remote send delay */
	Filter,             /* Is the filter on */
	Warning,            /* File deletion warning */
	Incom,              /* What to do with an incomplete file */
	Infile;             /* File to get the packets from */

extern char     state,              /* Present state of the automaton */
		cchksum,            /* Our (computed) checksum */
		Spadchar,           /* Padding character to send */
		Ssop,               /* Send start of packet */
		Rsop,               /* Receive start of packet */
		Reol,               /* EOL character we'll be receiving */
		Seol,               /* End-Of-Line character to send */
		Repc,               /* Repeat character I want to use */
		Reppref,            /* Repeat prefix we will use */
		Eightc,             /* Eight-bit character I want to use */
		Eightpref,          /* Eight-bit prefix we will use */
		Hand,               /* Handshake character */
		escchr,             /* Connect command escape character */
		Rquote,             /* Quote character in incoming data */
		Iquote,             /* input quote */
		Rpadchar,           /* Padding character we want to receive */
		Squote,             /* Quoting character */
		Startc,             /* Start character */
		Allwild,            /* String wildcard */
		Onewild,            /* Character wildcard */
		*filelist[],        /* List of files to be sent */
		Filnam[],           /* Current file name */
		*ttyname,           /* Terminal name */
		*Helpfile[],        /* Helpfile text representation */
		Fileas[],           /* Name which we send the file under */
		recpkt[],           /* Receive packet buffer */
		packet[];           /* Packet buffer */

extern FILE *Ft[];                      /* command file stack */

extern char statuses[];                 /* return statuses for Cromix */

extern char Prompt[],                   /* Kermit prompt */
	    *Seeya,                     /* standard exit message */
	    Logf[],                     /* log file */
	    Lastty[];                   /* last tty used if remote */

extern struct sctable Commands[],       /* command ascii representations */
		      Options[],        /* same for set options */
		      Speeds[],         /* baud rates */
		      Doptions[],       /* debug options */
		      Yesno[],          /* yes or no equivalents */
		      Duplex[],         /* duplex */
		      Filep[],          /* file parameters */
		      Parity[],         /* parity */
		      Sendrec[],        /* send/receive options */
		      Wildcards[],      /* wildcard specifications */
		      Handshake[];      /* handshakes */

extern Impopt[];                        /* list of options to show */

extern char *Delim,                     /* delimiters in a command line */
	    *Leapfrog[],                /* leapfrog command line */
	    Handchars[];                /* handshake characters */

extern struct dtable *Defopt[MAXOPT];   /* Defined options */

extern FILE     *fp,                /* Current disk file */
		*dpfp;              /* Debugging packet log file */

extern struct _vheader _ver;            /* CROMIX version header */

