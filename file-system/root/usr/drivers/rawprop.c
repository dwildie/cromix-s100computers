/*
	16FDC or 64FDC raw console I/O
	Egon Zakrajsek -- Jan 05, 1985
*/


#include "iolib.h"
#include "macros.h"
/*#include "port.h"*/
#include "raw.h"

#define sport(x) ((unsigned char *)(0xFFFF0000 + (x)))
#define P_STAT 0x00
#define P_IO 0x01
#define stport sport(P_STAT)
#define ioport sport(P_IO)


#define RDA 0x02                        /* Receiver Data available      */
#define TBE 0x04                        /* Transmitter buffer empty     */



/************************/
/*                      */
/*      raw_prop        */
/*                      */
/************************/

/*
	Raw Propeller I/O function table
*/

static int rpp_init(), rpp_in(), rpp_out();

raw raw_prop = {
	rpp_init,
	rpp_in,
	rpp_out
};



/************************/
/*                      */
/*      rpp_init        */
/*                      */
/************************/

/*
	Initialize raw I/O on Propeller console.
*/

static int rpp_init()
{
	static int rpp_out();
	rpp_out('\r');
	return(0);
}



/************************/
/*                      */
/*      rpp_in          */
/*                      */
/************************/

/*
	Get a character from raw Propeller console.
	Wait until character ready. Then
	return character with parity bit stripped
	off.
*/

static int rpp_in()
{
	while (!(*stport & RDA)) ;
	return(*(ioport) & 0x7f);
}



/************************/
/*                      */
/*      rpp_out         */
/*                      */
/************************/

/*
	Output a character on raw Propeller console.
*/

static int rpp_out(c)
int c;
{
	while (!(*stport & TBE)) ;
	*(ioport) = c;
	while (!(*stport & TBE)) ;
	return(0);
}

