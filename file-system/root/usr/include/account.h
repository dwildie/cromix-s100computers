/*
	@(#)account.h	1.5 (9/18/87)
	Description of account file
*/



#define AC_FILE     "/etc/acc"

#define AC_BOOT     0                   /* Boot record                  */
#define AC_LOGIN    1                   /* Login record                 */
#define AC_LOGOUT   2                   /* Logout record                */
#define AC_EXIT     3                   /* Exit record                  */
#define AC_FULL     4                   /* Full record (no jobid)       */
					/* Values 5 .. 127 reserved     */
					/* Other values for user accnt  */
				        


struct accrec {
	char            terminal[16];   /* Which terminal               */
	char            username[16];   /* User name of login           */

	time            actime;         /* Time of transaction          */
	unsigned short  userid;         /* User id number               */
	unsigned short  groupid;        /* Group id number              */
	unsigned short  devno;          /* Terminal device number       */
	int             jobid;          /* Job identification           */

	int             actype;         /* Type of account record       */
	unsigned long   timeused;       /* Time used (miliseconds)      */
	int             reserved1;
	int             reserved2;
};
