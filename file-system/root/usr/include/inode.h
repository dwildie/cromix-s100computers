/*
	@(#)inode.h	1.2 (9/28/86)
	Inode definition
*/


typedef struct  _ino {
	struct  _ino   *in_back;        /* backward pointer             */
	struct  _ino   *in_forw;        /* forward pointer              */
	unsigned short  in_devn;        /* inode device number          */
	unsigned short  in_inum;        /* inode number                 */
	unsigned short  in_ucnt;        /* usage count                  */
	short           in_flags;       /* inode flags                  */

	unsigned short  in_owner;       /* file owner's user id         */
	unsigned short  in_group;       /* file owner's group id        */

	unsigned char   in_aowner;      /* owner access                 */
	unsigned char   in_agroup;      /* group access                 */
	unsigned char   in_aother;      /* other access                 */
	unsigned char   in_stat;        /* inode status                 */

	unsigned char   in_nlinks;      /* number of links to inode     */
	unsigned char   in_filler;      /* filler                       */
	unsigned long   in_size;        /* file total size in bytes     */
	unsigned short  in_inode;       /* this inode number            */

	unsigned short  in_parent;      /* parent inode (dir only)      */
	unsigned short  in_dcount;      /* number of entries in dir     */
	unsigned long   in_usage;       /* number of blocks used        */
	time            in_tcreate;     /* time created                 */
	time            in_tmodify;     /* time last modified           */
	time            in_taccess;     /* time last accessed           */
	time            in_tdumped;     /* time last dumped             */
	unsigned long   in_index[20];   /* block pointers               */
} ino;



/*
	Bit assignement for in_flags
*/

#define IN_LOCK         00001           /* inode locked                 */
#define IN_WANT         00002           /* inode wanted                 */
#define IN_MODF         00004           /* inode modified               */
#define IN_READ         00010           /* inode is read-only           */



/*
	Bit assignement for in_stat
	and values of in_stat&IN_TYPE
*/

#define IN_TYPE         0x07            /* File type (mask)             */
#define IN_STEXT        0x08            /* Shared text                  */
#define IN_ALLOC        0x80            /* Inode allocated              */

#define is_ordin        0               /* ordinary file                */
#define is_direct       1               /* directory file               */
#define is_char         2               /* character device             */
#define is_block        3               /* block device                 */
#define is_pipe         4               /* pipe file                    */



#define in_begin        in_owner        /* start of inode on disk       */
#define in_sdevn        in_dcount       /* special device number        */

/* IMPORTANT !!! Size starting from in_begin must be inodesize */
