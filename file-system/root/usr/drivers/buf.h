/*
	File: buf.h
	Buffer definition
	EZ  --  May 13, 1986
*/


typedef struct _buf {
	struct   _buf  *bf_back;        /* Backward pointer     */
	struct   _buf  *bf_forw;        /* Forward pointer      */
	ulong           bf_block;       /* Logical block #      */
	ushort          bf_devn;        /* Device number        */
	short           bf_stat;        /* Buffer status        */

	ushort          bf_cylinder;    /* Cylinder number      */
	uchar           bf_surface;     /* Surface number       */
	uchar           bf_sector;      /* Sector number        */
	ushort          bf_count;       /* No. of undone bytes  */
	ushort          bf_errc;        /* Error code           */
	struct   _buf  *bf_next;        /* Next buffer in hash  */
	uchar          *bf_addr;        /* Actual buffer        */
} buf;

#define bf_qnext        bf_back


/*
	Definition of bf_stat bits
*/


#define BF_BUSY         00001           /* Buffer busy          */
#define BF_WANT         00002           /* Buffer wanted        */
#define BF_DONE         00004           /* Buffer operation done*/
#define BF_MODF         00010           /* Buffer modified      */
#define BF_READ         00020           /* Buffer should be read*/
