/*
	@(#)tmodeequ.h	1.5 (5/11/87)

	This file contains declarations of all values which are
	used in the getmode and setmode Cromix system calls, for 
	TP tape devices.
*/


#define TPABORT   196           /* Re-initialize tape driver (9 track)  */
#define TPFMARK   198           /* Write file mark                      */
#define TPSECURE  199           /* Security erase                       */
#define TPREWIND  200           /* Rewind                               */
#define TPUNLOAD  201           /* Rewind and unload                    */
#define TPMODE    202           /* Mode bits                            */
#define TPFILNO   203           /* File number                          */
#define TPBLKNO   204           /* Block number                         */
#define TPOBLKLN  205           /* Block length for next block written  */
				/* (9 track)                            */
#define TPIBLKLN  206           /* Block length of first block read     */
				/* (9 track)                            */
#define TPOBLKS   207           /* Number of blocks written             */
				/* (9 track)                            */
#define TPSTAT    208           /* Get status                           */
#define TPLOAD    209           /* Load tape (SCSI)                     */
#define TPAPPEND  210           /* Seek to end of recorded data (SCSI)  */
#define TPLASTERR 211           /* Last error (SCSI)                    */
#define TPSOFT    212           /* Soft error count (SCSI)              */
#define TPFIRM    213           /* Download firmware (SCSI)             */
#define TPVERSION 214           /* Get version number                   */



/*      TPMODE bits     */

#define EOFCLOSE 0x0080         /* Write EOF to tape when device closes */



/*      TPSTAT status bits                                              */

#define TPDRVBUSY       0x0080  /* Drive busy                           */
#define TPWRRDY         0x0040  /* FIFO ready for input (used for write)*/
#define TPRDRDY         0x0020  /* FIFO output ready (used for read)    */
#define TPLOADPT        0x0010  /* Load point                           */
#define TPFBUSY         0x0008  /* Formatter busy                       */
#define TPONLINE        0x0004  /* On line                              */
#define TPIDENT         0x0002  /* Ident                                */
#define TPRDY           0x0001  /* Ready                                */

#define TPHISPEED       0x8000  /* High speed status                    */
#define TPHARDERR       0x2000  /* Hard error                           */
#define TPFLMARK        0x1000  /* File mark                            */
#define TPCORERR        0x0800  /* Correctable error                    */
#define TPWRPROT        0x0400  /* Tape write-protected                 */
#define TPEOT           0x0200  /* End of tape                          */
#define TPRWINDING      0x0100  /* Rewinding                            */

/*      Mode value for TPFIRM must be a pointer to struct tp_firm       */

struct tp_firm {
	int             stat;       /* Return status                    */
	unsigned long   size;       /* Size of firmware                 */
	unsigned char   *buf;       /* Address of firmware              */
};
