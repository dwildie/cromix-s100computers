/*
	@(#)pwd.h	1.1 (9/29/86)
	Passwd record description
*/



struct passwd {
	char        *pw_name;               /* Login name               */
	char        *pw_passwd;             /* Encoded password         */
	int         pw_uid;                 /* Numeric user id number   */
	int         pw_gid;                 /* Numeric group id number  */
	char        *pw_dir;                /* Home directory pathname  */
	char        *pw_shell;              /* Initial program          */
	char        *pw_prompt;             /* Prompt string            */
};
