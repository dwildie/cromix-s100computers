/*
	File: bufsize.h
	Buffer size definition
	EZ  --  Jan 21, 1985
*/


#define blocksize 512                   /* Block size in characters     */

#define indexsize 128                   /* Number of index numbers per  */
					/* block                        */

#define _indexsize 7                    /* Number of bits in indexsize  */

#define inodesize 128                   /* Size of disk portion of      */
					/* inode in characters          */

#define inperblock 4                    /* Number of inodes per block   */
