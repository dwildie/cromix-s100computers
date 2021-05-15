/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K e x t . c
 *
 *  External variables for K e r m i t  File Transfer Utility.
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

struct _vheader _ver = { 0xfd, 0xed, 0xfd, 0xed, 0, 0, 0, 0,
		       0x01, 0x32,
		       "CROMIX Kermit version 1.32, Sep 1987.\n"
		     };

int     size,               /* Size of present data */
	rpsiz,              /* Maximum receive packet size */
	spsiz,              /* Maximum send packet size */
	Spad,               /* How much padding to send */
	timint,             /* Timeout for foreign host on sends */
	n,                  /* Packet number */
	oldm1,              /* old terminal modes */
	oldm2,
	oldm3,
	Omode1,             /* old line modes */
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
	Initcon,            /* TRUE if initial phase */
	Filesin,            /* Files transferred */
	Chin,               /* Characters transferred */
	Lastbaud,           /* Transfer baud rate (in system format) */
	Baud,               /* Current baud rate (in system format) */
	Sendnext,           /* Send next packet? */
	Sendall,            /* Send all of them */
	Transerr,           /* Transmission error */
	Sdelay,             /* Remote send delay */
	Infile,             /* File to get the packets from */
	Warning,            /* File deletion warning */
	Incom,              /* TRUE if we want to save incomplete files */
	Filter;             /* Is the filter on */

char    state,              /* Present state of the automaton */
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
	*filelist[MAXFILE], /* List of files to be sent */
	Filnam[MAXWORD],    /* Current file name */
	*ttyname,           /* Terminal name */
	Fileas[MAXWORD],    /* Name the file is sent as */
	recpkt[MAXPACKSIZ], /* Receive packet buffer */
	packet[MAXPACKSIZ]; /* Packet buffer */

FILE *Ft[MAXLEVELS];            /* File pointer stack for take */

char statuses[2];               /* Cromix return statuses */

char Prompt[MAXWORD] =  "Cromix-Kermit> ";      /* Kermit prompt */
char *Seeya =           "Kermit finished.";            /* Exit message */
char Logf[MAXWORD] =    INITLOG;                /* Log file */
char Lastty[MAXWORD] =  "";                     /* Last tty name */

/* n.b. whenever reverse scans take place the first entry in a group is the
 *      one found and returned.  This is done to find the help files and so
 *      the .hlp file for the command must have the name of the first listed
 */

struct sctable Commands[] = {                   /* Command table */
	"send",         SEND,
	"s",            SEND,
	"receive",      RECEIVE,                /* e.g. help file =       */
	"r",            RECEIVE,                /*            receive.hlp */
	"set",          SET,
	"=",            SET,
	"connect",      CONNECT,
	"c",            CONNECT,
	"help",         HELP,
	"?",            HELP,
	"h",            HELP,
	"quit",         QUIT,
	"exit",         QUIT,
	"ex",           QUIT,
	"stop",         QUIT,
	"shell",        SHELL,
	"sh",           SHELL,
	"!",            SHELL,
	"show",         SHOW,
	"display",      SHOW,
	"dis",          SHOW,
	"stat",         SHOW,
	"status",       SHOW,
	"take",         TAKE,
	"t",            TAKE,
	"define",       DEF,
	"def",          DEF,
	"d",            DEF,
	"transmit",     TRANS,
	"trans",        TRANS,
	"stats",        STATS,
	"statistics",   STATS,
	"server",       SERVER,
	"get",          GET,
	"g",            GET,
	"fin",          FINISH,
	"finish",       FINISH,
	"remote",       REMOTE,
	"rem",          REMOTE,
	"disc",         DISC,
	"init",         INITIAL,
	"initial",      INITIAL,
	"cwd",          CWD,
	"cd",           CWD,
	"push",         PUSH,
	"",             NOOP
};

struct sctable Options[] = {            /* Option table */
	"b",            BAUD,
	"baud",         BAUD,
	"baud-rate",    BAUD,
	"block",        BLOCK,
	"block-check",  BLOCK,
	"d",            DEBUG,
	"debug",        DEBUG,
	"debugging",    DEBUG,
	"def",          MACRO,
	"defs",         MACRO,
	"delay",        DELAY,
	"dup",          DUPLEX,
	"duplex",       DUPLEX,
	"e",            ESCAPE,
	"esc",          ESCAPE,
	"escape",       ESCAPE,
	"escchr",       ESCAPE,
	"file",         FILEP,
	"flow",         FLOW,
	"flow-control", FLOW,
	"hand",         HAND,
	"handshake",    HAND,
	"inc",          INC,
	"incomplete",   INC,
	"inq",          INQ,
	"input-quote",  INQ,
	"line",         LINE,
	"l",            LINE,
	"p",            PARITY,
	"par",          PARITY,
	"parity",       PARITY,
	"pr",           PROMPT,
	"prompt",       PROMPT,
	"r",            RECP,
	"rec",          RECP,
	"receive",      RECP,
	"rem",          REMOTE,
	"remote",       REMOTE,
	"repeat",       REPP,
	"rep",          REPP,
	"ret",          RETRY,
	"retry",        RETRY,
	"s",            SENDP,
	"send",         SENDP,
	"8-bit",        EIGHTP,
	"eight-bit",    EIGHTP,
	"8",            EIGHTP,
	"eight",        EIGHTP,
	"filter",       FILTER,
	"wild",         WILD,
	"wildcard",     WILD,
	"warn",         WARNING,
	"warning",      WARNING,
	"sync",         SYNCHRO,
	"set",          OPTI,
	"settings",     OPTI,
	"=",            OPTI,
	"o",            OPTI,
	"opt",          OPTI,
	"options",      OPTI,
	"macros",       MACRO,
	"m",            MACRO,
	"",             NOOP
};

struct sctable Speeds[] = {             /* Baud rate table */
	"110",          S_110,
	"150",          S_150,
	"300",          S_300,
	"1200",         S_1200,
	"2400",         S_2400,
	"4800",         S_4800,
	"9600",         S_9600,
	"19200",        S_19200,
	"auto",         Sfl_AUTO,
	"a",            Sfl_AUTO,
	"nochg",        S_NOCHG,
	"n",            S_NOCHG,
	"",             NOOP
};

struct sctable Doptions[] = {           /* Debug option table */
	"off",          OFF,
	"no",           OFF,
	"none",         OFF,
	"s",            STATES,
	"states",       STATES,
	"nostates",     NOSTAT,
	"nostat",       NOSTAT,
	"-s",           NOSTAT,
	"-states",      NOSTAT,
	"p",            PACKETS,
	"packets",      PACKETS,
	"nopack",       NOPACK,
	"nopackets",    NOPACK,
	"-packets",     NOPACK,
	"-p",           NOPACK,
	"log-file",     LOGFILE,
	"logfile",      LOGFILE,
	"log",          LOGFILE,
	"",             NOOP
};

struct sctable Yesno[] = {              /* Yes/no table */
	"yes",          YES,
	"y",            YES,
	"on",           YES,
	"no",           NO,
	"n",            NO,
	"off",          NO,
	"",             YES,
	"",             NOOP
};

struct sctable Duplex[] = {             /* Duplex table */
	"half",         HALF,
	"h",            HALF,
	"full",         FULL,
	"f",            FULL,
	"",             NOOP
};

struct sctable Filep[] = {              /* File parameters table */
	"ascii",        TEXT,
	"a",            TEXT,
	"text",         TEXT,
	"t",            TEXT,
	"binary",       BIN,
	"bin",          BIN,
	"b",            BIN,
	"image",        IMAGE,
	"i",            IMAGE,
	"",             NOOP
};

struct sctable Parity[] = {             /* Parity table */
	"none",         NONE,
	"no",           NONE,
	"n",            NONE,
	"off",          NONE,
	"-",            NONE,
	"mark",         MARK,
	"m",            MARK,
	"space",        SPACE,
	"s",            SPACE,
	"even",         EVEN,
	"e",            EVEN,
	"odd",          ODD,
	"o",            ODD,
	"",             NOOP
};

struct sctable Sendrec[] = {            /* Send/receive table */
	"eol",          EOL,
	"end-of-line",  EOL,
	"e",            EOL,
	"packet-length", PL,
	"pl",           PL,
	"pause",        PAU,
	"timeout",      TO,
	"to",           TO,
	"t",            TO,
	"padding",      PAD,
	"pad",          PAD,
	"padc",         PADC,
	"padchar",      PADC,
	"pc",           PADC,
	"quote",        QUO,
	"q",            QUO,
	"start-of-packet", SOP,
	"sop",          SOP,
	"start",        SOP,
	"s",            SOP,
	"",             NOOP
};

struct sctable Wildcards[] = {
	"all",          WILDALL,
	"a",            WILDALL,
	"one",          WILDONE,
	"o",            WILDONE,
	"",             NOOP
};

struct sctable Handshake[] = {          /* Handshake table */
	"none",         NOHS,
	"no",           NOHS,
	"n",            NOHS,
	"off",          NOHS,
	"xon",          XONH,
	"^q",           XONH,
	"xoff",         XOFFH,
	"^s",           XOFFH,
	"bell",         BELLH,
	"^g",           BELLH,
	"cr",           CRH,
	"return",       CRH,
	"ret",          CRH,
	"lf",           LFH,
	"linefeed",     LFH,
	"line",         LFH,
	"esc",          ESCH,
	"",             NOOP
};

int Impopt[] = {                /* Options that can be shown */
	DEBUG,
	INQ,
	PROMPT,
	WILD,
	LINE,
	BAUD,
	REMOTE,
	DELAY,
	DUPLEX,
	HAND,
	PARITY,
	ESCAPE,
	RETRY,
	FILEP,
	INC,
	FILTER,
	RECP,
	SENDP,
	REPP,
	EIGHTP,
	WARNING,
	SYNCHRO,
	NOOP
};

char *Delim = " \t\r\n,";               /* Shell delimiters */

char *Leapfrog[] = {
	"Leapfrog",
	"",
	NULL,
} ;

char Handchars[] = {                    /* Handshake characters */
	0,                              /* (indexed by the constant) */
	0,
	XON,
	'\021',
	'\007',
	'\r',
	'\n',
	'\033'
};

struct dtable *Defopt[MAXOPT];          /* Defined options table */

FILE    *fp,                /* File pointer for current disk file */
	*dpfp;              /* File pointer for debugging packet log file */

