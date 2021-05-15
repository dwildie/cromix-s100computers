/*
	General IOLIB header file. It must be included
	in every IOLIB module in this directory.

	EZ  --  Mar 05, 1986
*/



#segment IOLIB
#option nostack
#define System 30
/*
#define HasCache
#define HasXMM
*/

#define iovers(x,y) ((x) << 8 | (y))
