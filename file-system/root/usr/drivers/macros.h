/*
	Definition of macros
	EZ  --  Feb 23, 1986
*/



#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define islower(c) ((c) >= 'a' && (c) <= 'z')
#define isupper(c) ((c) >= 'A' && (c) <= 'Z')
#define isspace(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')
#define isalpha(c) (isupper(c) || islower(c))
#define toupper(c) (islower(c) ? (c) - ' ' : (c))
#define tolower(c) (isupper(c) ? (c) + ' ' : (c))
#define devnum(major,minor) ((major) << 8 | (minor))
#define major(devn) ((devn) >> 8)
#define minor(devn) ((devn) & 0xff)
#define port(x) ((unsigned char *)(0xffffff00 + (x)))
/*
#define setlev(x)       _setlev((x) << 8 & 0x0700)
*/
#define ERR     (-1)
#define EOF     (-1)
#define NULL    0
#define BFUNIT 4

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

#define vtop(vaddr) ((uchar *) ((ulong) vaddr | 0x40000000L))
