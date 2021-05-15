/*
	@(#)grp.h	1.2 (9/28/86)
	Passwd record description
*/



struct group {
	char        *gr_name;               /* Name of the group        */
	char        *gr_passwd;             /* Encoded group password   */
	int         gr_gid;                 /* Numeric group id number  */
	char        **gr_mem;               /* Pointers to member names */
};
