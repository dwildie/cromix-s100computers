/*
	Watch dog record
	EZ -- Oct 29, 1984
*/


typedef struct  _wdg {
	struct _wdg     *next;          /* Next request                 */
	unsigned long   when;           /* Absolute wake up time        */
	int             (*func)();      /* Function to be called        */
	int             arg;            /* Argument to func             */
} wdg;
