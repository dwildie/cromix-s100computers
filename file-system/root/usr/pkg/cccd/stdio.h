/* File: stdio.h.text */
/* Date: 16-Apr-87 */

#define BUFSIZ  512
#define _NFILE  20
#ifndef FILE
extern  struct  _iobuf {
	char    *_ptr;
	int     _cnt;
	char    *_base;
	char    _flag;
	char    _file;
} _iob[_NFILE];
#endif

#define _IOREAD 01
#define _IOWRT  02
#define _IONBF  04
#define _IOMYBUF        010
#define _IOEOF  020
#define _IOERR  040
#define _IOSTRG 0100
#define _IORW   0200

#define NULL    0
#define FILE    struct _iobuf
#define EOF     (-1)

#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])
#define getc(p)         (--(p)->_cnt>=0? (*(p)->_ptr++)&0377:_filbuf(p))
#define getchar()       getc(stdin)
#define putc(x,p) (--(p)->_cnt>=0? ((int)(unsigned char)(*(p)->_ptr++=(unsigned)(x))):_flsbuf((unsigned)(x),p))
#define putchar(x)      putc(x,stdout)
#define feof(p)         (((p)->_flag&_IOEOF)!=0)
#define ferror(p)       (((p)->_flag&_IOERR)!=0)
#define fileno(p)       (p)->_file

FILE    *fopen();
FILE    *freopen();
FILE    *fdopen();
long    ftell();
char    *fgets();
char    *fputs();

#define isalpha(c) (isupper(c) || islower(c))
#define isdigit(c) ((c) <= '9' && (c) >= '0')
#define islower(c) ((c) <= 'z' && (c) >= 'a')
#define isupper(c) ((c) <= 'Z' && (c) >= 'A')
#define isspace(c) ((c)==' ' || (c)=='\n' || (c)=='\r' || (c)=='\t')
#define max(a,b)   ((a) > (b) ? (a) : (b))
#define min(a,b)   ((a) < (b) ? (a) : (b))
#define toupper(c)    (islower(c) ? (c) - 32 : (c))
#define tolower(c)    (isupper(c) ? (c) + 32 : (c))
#define abs(x) ((x) < 0 ? -(x) : (x))

double sqr();
double sqrt();
double exp();
double ln();
double pwroften();
double log10();
double sin();
double cos();
double asin();
double acos();
double atan();
double sinh();
double cosh();
double tanh();

/* The following are for CROMIX ONLY! */
#define backc(fp,c)      ungetc(c,fp)
#define ungetchar(c)     ungetc(c,stdin)
#define getline(buf,max) getl(STDIN,buf,max)
#define alloc(x)         malloc(x)
/* ---------------------------------- */

