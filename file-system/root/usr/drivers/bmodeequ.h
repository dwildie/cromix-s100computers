/*
	Mode definitions for block devices

	Cromemco Inc.
	Jun 02, 1986
*/



/*
	Mode numbers for getmode and setmode calls
*/

#define BMD_STATUS      0       /* Get/set status byte                  */
#define BMD_FLG1        1       /* Get/set flag1 byte                   */
#define BMD_FLG2        2       /* Get/set flag2 byte                   */
#define BMD_FLG3        3       /* Get/set flag3 byte                   */
#define BMD_SIZE        4       /* Get number of bytes on device        */
#define BMD_SEEK        5       /* Seek                                 */
#define BMD_INIT        6       /* Initialize track                     */
#define BMD_PRDWRT      7       /* Primitive read/write                 */
#define BMD_RDWRT       8       /* Special read/write                   */
#define BMD_RPM         9       /* Get RPM                              */
#define BMD_VERSION     10      /* Version number                       */
#define BMD_PHYCHAR     11      /* Physical Characteristics             */
#define BMD_LDFIRM      12      /* Load firmware                        */
#define BMD_SOFT        13      /* Accumulated number of retries        */
#define BMD_HARD        14      /* Accumulated number of hard errors    */
#define BMD_RETRY       15      /* Number of retries before hard error  */
				/* Values 16 .. 63 reserved             */
				/* Values 64 .. 127 special device modes*/
				/* Values 128 .. 255 reserved for user  */
				/* supplied drivers                     */




/*
	Floppy tape special numbers
*/

#define BMD_RETEN       64      /* Number of tape repositions before    */
				/*   a retension                        */

/*
	Memory driver special numbers
*/

#define BMD_TMEM        64      /* Total memory                         */
#define BMD_SMEM        65      /* System memory                        */
#define BMD_HMEM        66      /* Shared memeory                       */
#define BMD_FMEM        67      /* Free memory                          */
#define BMD_MMEM        68      /* Maximal free memory                  */
#define BMD_CACR        69      /* CACR register                        */
#define BMD_CACHE       70      /* External cache control               */
#define BMD_XMM         71      /* XMM usage                            */




/*
	Mode values and masks for BMD_STATUS calls
*/

#define DS_BUSY         0x01    /* Device Busy (in use)                 */
#define DS_WANT         0x02    /* Device Wanted (do wakeup)            */
#define DS_READ         0x04    /* Read-only device                     */
#define DS_MODF         0x08    /* Super-block modified                 */
#define DS_MOUNT        0x10    /* Device mounted                       */
#define DS_HOME         0x20    /* Device has been homed                */
#define DS_BFSTEP       0x40    /* Buffered step flag                   */
#define DS_VERIFY       0x80    /* Verify after write                   */


/*
	Mode values and masks for BMD_FLG1 calls
*/

#define DF_SMALL        0x01    /* 1=small floppy 0=large floppy        */
#define DF_DSIDE        0x02    /* Double sided                         */
#define DF_DDENS        0x04    /* Double density                       */
#define DF_DTRACK       0x08    /* Double tracked                       */
#define DF_CROMIX       0x10    /* Cromix format disk                   */
#define DF_CDOS         0x20    /* Cdos format disk                     */
#define DF_BACKUP       0x40    /* Backup format disk                   */
#define DF_VOICE        0x80    /* 0=step 1=voice coil                  */

/*
	Mode values for BMD_FLG2 calls
*/

#define D2_SMALL        0       /* Small floppy                         */
#define D2_LARGE        1       /* Large floppy                         */
#define D2_STDC         2       /* STDC Hard disk                       */
#define D2_FSMD         3       /* Fixed part of SMD hard disk          */
#define D2_RSMD         4       /* Removable part of SMD hard disk      */
#define D2_UNIFORM      5       /* Uniform floppy                       */
#define D2_MEMORY       6       /* Processor memory                     */
#define D2_RAM          7       /* RAM disk                             */
#define D2_FTAPE        8       /* Floppy tape                          */
#define D2_HD           9       /* Not used in 40 Cromix                */
				/* Values 10 .. 127 reserved            */
				/* Values 128 .. 255 reserved for user  */
				/* supplied drivers                     */

/*
	Mode values and masks for BMD_FLG3 calls
*/

#define D3_WRTPRO       0x01    /* Device is write protected            */
#define D3_INTRPT       0x02    /* Device interrupts                    */
#define D3_DUAL         0x04    /* Dual drive                           */


/*
	Floppy minor device number definition
*/

#define FDENSITY        0x40            /* 0=double density             */
#define FSIDES          0x20            /* 0=double sided               */
#define FDUAL           0x10            /* 1=dual drive (PERSCI)        */
#define FDTRACK         0x08            /* 1=double tracked             */
#define FSIZE           0x04            /* 0=8" 1=5"                    */
#define FUNIT           0x03            /* physical unit number mask    */


/*
	SMD minor device number assignment
*/

#define CONTROLLER      0x80            /* Controller mask              */
#define DRIVE           0x40            /* Drive number mask            */
#define FIXED           0x20            /* Fixed flag mask              */
#define PARTITION       0x1f            /* Partition number             */


/*
	Data structure for BMD_INIT call
*/

typedef struct {
	unsigned short  flags;          /* flags (FDENSITY for floppy)  */
	unsigned short  side;           /* side to be initialized       */
	unsigned short  track;          /* track to be initialized      */
	unsigned char   *buf;           /* pointer to track image       */
} bm_init;


/*
	Data structure for BMD_SEEK call
*/

typedef struct {
	unsigned char   status;         /* Return status                */
	unsigned char   ferror;         /* Fatal error number           */
	unsigned char   serror;         /* System error number          */
	unsigned char   verify;         /* Verify seek flag             */
	unsigned short  side;           /* side                         */
	unsigned short  track;          /* track                        */
} bm_seek;


/*
	Data structure for BMD_RDWRT call
*/

typedef struct {
	unsigned short  read;           /* Read/write flag              */
	unsigned char   *buf;           /* buffer pointer               */
	unsigned long   number;         /* no. of blocks to read/write  */
	unsigned long   blknr;          /* starting block number        */
} bm_rdwrt;


/*
	Data structure for BMD_PRDWRT call
*/

typedef struct {
	unsigned char   status;         /* return status                */
	unsigned char   ferror;         /* fatal error number           */
	unsigned char   serror;         /* system error number          */
	unsigned char   read;           /* Read/write flag              */
	unsigned char   *buf;           /* buffer pointer               */
	unsigned short  number;         /* number of sectors to do      */
	unsigned short  sector;         /* starting sector number       */
	unsigned short  surface;        /* surface number to read/write */
	unsigned short  cylinder;       /* cylinder number to read/write*/
} bm_prdwrt;


/*
	Status bits primitive operations
*/

/*      STDC    */
#define STS_IOERROR     0x01            /* I/O error                    */
#define STS_NIOERROR    0x02            /* Non I/O error                */
#define STS_SELECT      0x04            /* Error on select              */
#define STS_SEEK        0x08            /* Error on seek                */
#define STS_PRD         0x10            /* Error on primitive read      */
#define STS_PWR         0x20            /* Error on primitive write     */
#define STS_PTX         0x40            /* Error on transfer            */

/*      Floppy  */

#define FLS_SELECT      0x01            /* Error on select              */
#define FLS_HOME        0x02            /* Error on home                */
#define FLS_RDADD       0x03            /* Error on read address        */
#define FLS_SEEK        0x04            /* Error on seek                */
#define FLS_PREAD       0x05            /* Error on preread             */
#define FLS_READ        0x06            /* Error on read                */
#define FLS_WRITE       0x07            /* Error on write               */
#define FLS_WTRK        0x08            /* Error on write track         */

/*      SMD     */

#define SMS_SELECT      0x01            /* Error on select              */
#define SMS_HOME        0x02            /* Error on home                */
#define SMS_SEEK        0x03            /* Error on seek                */
#define SMS_READ        0x04            /* Error on read                */
#define SMS_WRITE       0x05            /* Error on write               */
#define SMS_HEAD        0x06            /* Error on select head         */
#define SMS_PREAD       0x07            /* Error on preread             */

/*
	Data structure for BMD_PHYCHAR call
*/

typedef struct {
	unsigned short  surface;        /* number of surfaces on device */
	unsigned short  cylinder;       /* number of cylinders on device*/
	unsigned short  sector;         /* number of sectors/track      */
	unsigned short  secsiz;         /* number of bytes/sector       */
} bm_phy;


/*
	Data structure for BMD_LDFIRM call
*/

typedef struct {
	unsigned short  flags;          /* flags (see below)            */
	unsigned short  count;          /* number of bytes              */
	unsigned char   *buf;           /* pointer to firmware          */
} bm_ldfrm;

/*
	Flags
*/

#define LDFRM_DEBUG     0x8d            /* Load debugger firmware       */
#define LDFRM_FIRM      0x8f            /* Load Regular firmware        */
