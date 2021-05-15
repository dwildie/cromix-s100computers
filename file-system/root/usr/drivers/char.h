/*
	character device definition
	EZ  --  Jul 24, 1986
*/


#define cdevcnt 16                      /* Number of character drivers  */
					/* supported                    */

typedef struct {
	int             (*ch_init)();   /* startup init routine */
	int             (*ch_open)();   /* open routine         */
	int             (*ch_close)();  /* close routine        */
	int             (*ch_read)();   /* read routine         */
	int             (*ch_write)();  /* write routine        */
	int             (*ch_getmode)();/* getmode routine      */
	int             (*ch_setmode)();/* setmode routine      */
} chd;
