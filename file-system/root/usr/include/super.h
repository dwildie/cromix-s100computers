/*
	@(#)super.h	1.1 (9/29/86)
	Super block definition
*/


#define frbcount 80                     /* Free block list size         */
#define fricount 80                     /* Free inode list count        */
#define sbfillsize 2                    /* Superblock filler            */
#define frbsize (frbcount * sizeof(unsigned long) + sizeof(unsigned short))
#define frisize (fricount * sizeof(unsigned short) + sizeof(unsigned short))


typedef struct  {
	unsigned short  sb_version;     /* CROMIX version number      2 */
	char            sb_cromix[6];   /* "cromix"                   6 */
	unsigned short  sb_istart;      /* First inode block          2 */
	unsigned short  sb_isize;       /* Number of inodes           2 */
	unsigned long   sb_fsize;       /* Number of blocks           4 */
	time            sb_time;        /* Last modified time         6 */
	unsigned long   sb_bksize;      /* Block size                 4 */
	char            sb_filler[sbfillsize]; /*                     2 */
	unsigned short  sb_nfree;       /* Free block count           2 */
	unsigned long   sb_free[frbcount]; /* Free list             320 */
	unsigned short  sb_ninode;      /* Free inode count           2 */
	unsigned short  sb_inode[fricount]; /* Free inodes          160 */
} sbk;                                  /*                  total = 512 */
