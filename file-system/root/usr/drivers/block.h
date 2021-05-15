/*
	File: block.h
	Block device definition
	EZ  --  Jul 24, 1986
*/


#define bdevcnt 12                      /* Number of block drivers      */
					/* supported                    */

typedef struct {
	unsigned short  bk_stat;        /* controller status    */
	unsigned short  bk_errc;        /* error code           */
	int             (*bk_init)();   /* startup init routine */
	int             (*bk_open)();   /* open routine         */
	int             (*bk_close)();  /* close routine        */
	int             (*bk_doio)();   /* do I/O routine       */
	int             (*bk_getmode)();/* getmode routine      */
	int             (*bk_setmode)();/* setmode routine      */
} blk;


/*
	Open mode bits
*/

#define OM_MOUNT        0x01
#define OM_FORCE        0x02


/*
	Close mode bits
*/

#define CM_UNMOUNT      0x01
#define CM_EJECT        0x02
