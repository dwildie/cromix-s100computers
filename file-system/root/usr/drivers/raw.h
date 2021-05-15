/*
	primitive (raw) character device definition
	EZ  --  Mar 03, 1984
*/

typedef struct {
	int             (*rw_init)();   /* startup init routine */
	int             (*rw_in)();     /* in character routine */
	int             (*rw_out)();    /* out char routine     */
} raw;
