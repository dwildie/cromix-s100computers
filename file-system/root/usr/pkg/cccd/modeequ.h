/*
	@(#)modeequ.h	1.9 (4/23/87)
	This file contains declarations of all values which are
	used in the getmode and setmode Cromix system calls.
*/


#define MD_ISPEED       0               /* input speed                  */
#define MD_OSPEED       1               /* output speed                 */
#define MD_MODE1        2               /* flags: RAW, ECHO, etc.       */
#define MD_MODED        3               /* delays for NL, CR, etc.      */
#define MD_MODE2        4               /* flags: PAUSE, XFF, etc.      */
#define MD_MODE3        5               /* flags: ESCRETN               */
#define MD_ERASE        6               /* auxilliary erase character   */
#define MD_DELECHO      7               /* erasure echo character       */
#define MD_LKILL        8               /* line kill character          */
#define MD_USIGNAL      9               /* user signal key              */
#define MD_LENGTH       10              /* page length (lines)          */
#define MD_WIDTH        11              /* page width (columns)         */
#define MD_BMARGIN      12              /* bottom margin (lines)        */
#define MD_MODE4        13              /* flags: TDELAY, HALFD, INL    */
#define MODELEN         (MD_MODE4+1)    /* number of byte mode values   */

#define MD_FORMS        254             /* printer forms number         */
#define MD_IDENT        255             /* device identification        */


/* the following are for SLPT only                                      */

#define SLPT_BSIZE      MD_ERASE        /* ETX/ACK block size           */

/* the following are for TYP only                                       */

#define TYP_CWIDTH      64              /* character width in 1/120 in  */
#define TYP_LHEIGHT     65              /* line height in 1/48 in       */
#define TYP_LMARGIN     66              /* left margin in columns (1/10)*/

/* the following are commands, not displacements in the device structure */

#define MD_RESUME       157             /* Resume paused output         */
#define MD_STATUS       156             /* flag: character is in one    */
					/*      of the  input queues    */
#define MD_IFLUSH       155             /* flush input queues           */
#define MD_FNKEYS       152             /* turn function keys on or off */
#define MD_PSIGHUP      151             /* signal current process if hang up */
#define MD_MODEM        148             /* (QTTY and MTTYs only)        */
#define MD_TYP          147             /* (TYPs only)                  */

/* contents of D3-register for MD_ISPEED calls to change the baudrate   */
/* not all listed baud rates are supported by all devices               */

#define S_HANGUP        0       /* hang up dataphone    */
#define S_50            1       /* 50 baud              */
#define S_75            2       /* 75 baud              */
#define S_110           3       /* 110 baud             */
#define S_134           4       /* 134.5 baud           */
#define S_150           5       /* 150 baud             */
#define S_200           6       /* 200 baud             */
#define S_300           7       /* 300 baud             */
#define S_600           8       /* 600 baud             */
#define S_1200          9       /* 1,200 baud           */
#define S_1800          10      /* 1,800 baud           */
#define S_2400          11      /* 2,400 baud           */
#define S_4800          12      /* 4,800 baud           */
#define S_9600          13      /* 9,600 baud           */
/*                      14         External A           */
/*                      15         External B           */
#define S_19200         16      /* 19,200 baud          */
#define S_38400         17      /* 38,400 baud          */
#define S_CTSWAIT       125     /* wait for clear to send       */
#define S_NOCHG         126     /* no change of baudrate        */
#define S_UNINIT        127     /* uninitialized baudrate       */
#define Sfl_AUTO        0x80    /* (bit 7): input CRs from keyboard to  */
				/*      set baud                        */


/* contents of the D3-register & D4-register for MD_MODE1 calls         */

#define TANDEM          0x01
#define XTAB            0x02    /* expand TABs                          */
#define LCASE           0x04    /* convert alphabetics to lower case    */
#define ECHO            0x08    /* echo input                           */
#define CRDEVICE        0x10    /* on input, map CR into NL             */
				/* on output, echo LF or CR as CRLF     */
#define RAW             0x20    /* on input, return after each          */
				/* character                            */
				/* and treat ^C, ^S, ^Q as regular      */
				/* input                                */
#define ODD             0x40    /* parity function bits                 */
#define EVEN            0x80

/* contents of the D3-register & D4-register for MD_MODE2 calls         */

#define PAUSE           0x01    /* wait for CNTRL-Q after a page        */
				/*         is output                    */
#define NOTIMMECHO      0x02    /* do not echo characters               */
				/*         typed-ahead                  */
#define NOECNL          0x04    /* do not echo NLs                      */
#define SGENABLE        0x08    /* user-specifiable key signal enable   */
#define ABENABLE        0x10    /* CNTRL-C key signal enable            */
#define XFF             0x20    /* expand FFs                           */
#define WRAP            0x40    /* wrap-around if page width is exceeded */
#define SIGALLC         0x80    /* send siguser signal for each key pushed */

/* contents of the D3-register & D4-register for MD_MODE3 calls         */

#define ESCRETN         0x01    /* ESC causes input line to be          */
				/*         returned                     */
#define FNKEYS          0x02    /* enable response to 3102 function keys */
#define HUPENAB         0x04    /* hang up modem when device finally closed */
#define SIGHUPALL       0x08    /* send sighangup signals to all processes  */
				/* which use this tty if modem hangs up     */
#define CBREAK          0x10    /* on input, return after each character,   */
				/* no erase, linekill, or eof characters    */
#define BINARY          0x20    /* on input, return after each          */
				/* character, no erase, linekill, or    */
				/* eof characters, no output pause or   */
				/* output width truncation, treat x-off,*/
				/* x-on as regular input, no tandem mode*/
				/* (ie, no input buff ctl), no abort    */
				/* signal (^C), no user signal, no      */
				/* changing or checking parity bit, no  */
				/* delays after control chars as nls,   */
				/* no echoing, no character             */
				/* transformations, no function key     */
				/* decoding.                            */
#define CRIGNORE        0x40    /* On output, ignore CR and change LF   */
				/* to CR                                */
#define DISCARD         0x80    /* discard the device when it is no     */
				/*         longer open                  */

/* contents of the D3-register & D4-register for MD_MODE4 calls         */

#define TDELAY          0x01    /* Delay is expressed in time units     */
				/* instead of number of NULL chars      */
#define RTSCTS          0x02    /* Hardware handshake                   */
#define HALFD           0x04    /* Half duplex line                     */
#define INL             0x08    /* Translate input NL to ^X             */

/* contents of the D3-register & D4-register for MD_MODED calls         */

#define NLDELAY         0x03    /* (pairs of bits)                      */
#define TABDELAY        0x0C
#define CRDELAY         0x30
#define FFDELAY         0x40    /* (single bits)                        */
#define BSDELAY         0x80

/* bits of the D3-register for MD_STATUS calls */

#define INOTEMPTY       0x01    /* there is a character in the input    */
				/* buffer (but if not RAW mode, it won't*/
				/* be accessible until a whole line is  */
				/* entered)                             */

/* contents of the D3-register for MD_MODEM _getmode call               */

#define RXDA            0x01    /* Receiver Data Available              */
#define TXBE            0x04    /* Transmitter Buffer Empty             */
#define DCD             0x08    /* Data Carrier Detect                  */
#define CTS             0x20    /* Clear To Send                        */
#define RXBREAK         0x80    /* Receiver data line broken            */

/* contents of the D4-register for MD_MODEM _getmode call               */

#define notRI           0x40    /* Not Ringing                          */
#define notDSR          0x80    /* Data Set not Ready                   */

/* contents of the D3-register & D4-register for MD_MODEM _setmode call */

#define RTS             0x02    /* Request To Send                      */
#define TXBREAK         0x10    /* Break the Transmitter line           */
#define DTR             0x80    /* Data Terminal Ready                  */

/* contents of the D3-register for MD_TYP call                          */

#define TYPCHK          0x02    /* The 3355 printer is in check cond.   */
#define TYPPAP          0x04    /* The 3355 printer is out of paper     */
#define TYPRIB          0x08    /* The 3355 printer is out of ribbon    */
#define TYPOFL          0x10    /* The 3355 printer is off-line         */

/* contents of D3-register for MD_IDENT call                            */

#define ID_TTY           0      /* Tuart terminal                       */
#define ID_QTTY          1      /* Quadart or Octart terminal           */
#define ID_LPT           2      /* Parallel printer                     */
#define ID_TYP           3      /* Fully formed printer                 */
#define ID_SLPT          4      /* Serial printer                       */
#define ID_QSLPT         5      /* Serial printer on quadart            */
#define ID_CNET          6      /* CNET driver                          */
#define ID_FFP           7      /* FFP processor driver                 */
#define ID_SYSTEM        8      /* System device                        */
#define ID_TIMER         9      /* Timer device                         */
#define ID_TAPE         10      /* Half inch tape drive                 */
#define ID_SCC          11      /* SCC terminal                         */
#define ID_MAX          12      /* Maximizer                            */
#define ID_NTTY         13      /* New Tuart terminal                   */
#define ID_NOCT         14      /* New Octart terminal                  */
#define ID_PLPT         15      /* Parallel printer                     */
#define ID_USLPT        16      /* UART serial printer                  */
#define ID_OSLPT        17      /* OCTART serial printer                */
#define ID_SCTP         18      /* SCSI Tape                            */
				/* Values 19 .. 127 reserved            */
				/* Values 128 .. 255 reserved for user  */
				/* defined drivers and devices          */
