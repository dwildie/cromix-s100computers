/*
	Character queue definition
	EZ  --  Mar 03, 1984
	MM  --  Mar 21, 1987 - Mod'd for 151
*/


typedef struct _cbuf {
	unsigned long   cb_cnt;                 /* number of queued chars */
	unsigned char   *cb_get;                /* first character to get */
	unsigned char   *cb_put;                /* where to put next      */
} cbuf;
