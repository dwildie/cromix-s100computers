/*
	@(#)ptrace.h	1.1 (9/29/86)
	Ptrace information
*/


typedef struct  _ptc {
	unsigned long   us_D[8];        /* User data registers          */
	unsigned char  *us_A[8];        /* User address registers       */
	unsigned short  us_SR;          /* User status register         */
	unsigned short *us_PC;          /* User PC register             */
	unsigned short  us_pstat;       /* ptrace status                */
	unsigned short  us_signo;       /* user signal number           */
	short           us_tstat;       /* termination status           */
} ptc;

/*
	Ptrace commands
*/

#define P_START 0                       /* Next fexec is debugged       */
#define P_RDSEQ 1                       /* Read child memory            */
#define P_WRSEQ 2                       /* Write child memory           */
#define P_RDSTA 3                       /* Read child status            */
#define P_WRSTA 4                       /* Write child status           */
#define P_RUN   5                       /* Run child process            */
#define P_TRACE 6                       /* Trace child process          */
#define P_TERM  7                       /* Terminate child process      */

/*
	us_pstat values
*/

#define PS_RUNNING      0               /* Child running, parent asleep */
#define PS_START        1               /* Initial state                */
#define PS_BREAK        2               /* Trap #5 exception            */
#define PS_TRACE        3               /* Trace exception              */
#define PS_SIGNAL       4               /* Program aborted by signal    */
#define PS_EXIT         5               /* Program terminated           */
