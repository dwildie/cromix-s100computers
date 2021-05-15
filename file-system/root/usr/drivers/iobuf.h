/*
	File: iobuf.h
	Must agree with the beginning of buf
	EZ  --  May 13, 1986
*/


typedef struct _iobuf {
	struct   _buf  *bf_back;        /* Backward pointer     */
	struct   _buf  *bf_forw;        /* Forward pointer      */
	ulong           bf_block;       /* Logical block #      */
	ushort          bf_devn;        /* Device number        */
	short           bf_stat;        /* Buffer status        */
} iobuf;
