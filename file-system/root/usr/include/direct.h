/*
	@(#)direct.h	1.3 (4/8/87)
	Directory entry definition
*/


#define namsize 24                      /* Size of name in directory    */

typedef struct {
	char            dr_name[namsize];       /* File name            */
	unsigned                 : 16;          /* Reserved (zero)      */
	unsigned                 : 16;          /* Reserved (zero)      */
	short           dr_stat;                /* Directory status     */
	unsigned short  dr_inum;                /* Inode number of file */
} dir;

/*
	Bit assignement for dr_stat
*/

#define DR_ALLOC        0x8000                  /* Entry allocated      */
#define DR_PROTECT      0x4000                  /* Delete protected     */
