/* 
   Structs.h:  Cromemco 68000 C I/O header file
   Copyright (c) 1983 by Cromemco, Inc., All Rights Reserved

   This file contains declarations of all values which are
   used to reference the inode in the Cromix operating system.

*/

/*
	super block definitions
*/

#define frbcount 	80		/* free block list size		*/
#define fricount	80		/* free inode list size		*/

struct	_sblock {
	unsigned char	version[2];	/* version number	*/
	unsigned char	cromix[6];	/*  'cromix'		*/
	unsigned short	istart;		/* first inode block	*/
	unsigned short	isize;		/* number of inodes	*/
	int		fsize;		/* max block number	*/
	char		time[6];	/* last modified time	*/
	char		dummy0 [512-(4*frbcount+2)-(2*fricount+2)];
	unsigned short	nfree;		/* free block count	*/
	int		free[frbcount];	/* free list address	*/
	unsigned short	ninode;		/* free inode count	*/
	unsigned short	inode[fricount];/* free inodes		*/
};
#define ilist	ninode;			/* i-list address	*/



/*
	inode buffer definitions
*/

struct _inode {
	unsigned short	owner;		/* file owner's user id		*/
	unsigned short	group;		/* file owner's group id	*/
	char		aowner;		/* owner access			*/
	char		agroup;		/* group access			*/
	char		aother;		/* other access			*/
	char		stat;		/* file status			*/
	unsigned char	nlinks;		/* number of links to inode	*/
	char		dummy3;		/*				*/
	int		size;		/* file total size (in bytes)	*/
	unsigned short	inode;		/* this inode number		*/
	unsigned short	parent;		/* parent inode number		*/
					/* (for directories only)	*/
	unsigned short	dcount;		/* number entries in a directory*/
	int		usage;		/* num blocks actually used in file*/
	char 		tcreate[6];	/* time created			*/
	char		tmodify[6];	/* time last modified		*/
	char		taccess[6];	/* time last accessed		*/
	char		tdumped[6];	/* time last dumped (backed up) */
	int		index[20];	/* block pointers		*/
};	

#define sdevn dcount			/* special device major & minor	*/
					/* numbers			*/
 
 
#define	is_type		0x07		/* file type mask (stat)	*/
#define	is_ordin	0		/* ordinary file		*/
#define	is_direct	1		/* directory file		*/
#define	is_char		2		/* character device		*/
#define	is_block	3		/* block device			*/
#define	is_pipe		4		/* pipe file			*/
#define	is_alloc	0x80		/* inode allocated (bit in stat) */

#define	if_lock		0x01		/* inode locked (in use by a process)*/
#define	if_want		0x02		/* inode wanted by another process */
#define	if_modf		0x04		/* inode has to be written out	*/
#define	if_read		0x08		/* i-node from read-only system	*/

#define	ac_read		0x01		/* read access bit		*/
#define	ac_exec		0x02		/* execute access bit		*/
#define	ac_writ		0x04		/* write access bit		*/
#define	ac_apnd		0x08		/* append access bit		*/



/*
	directory format definitions
*/


#define	namsiz	24			/* Max directory name size	*/

struct _dir	
{
	char		name[namsiz];	/* name of entry		*/
	char		reserved[4];	/* reserved			*/
	short		stat;		/* status & flags		*/
	unsigned short	inum;		/* inode number of file		*/
};	
#define	dirsize	sizeof(struct _dir)	/* directory entry size (32 bytes) */

#define ds_alloc	0x8000		/* entry allocated bit */
