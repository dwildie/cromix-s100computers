/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  K m a i n . c
 *
 *  Main Program for K e r m i t  File Transfer Utility.
 *
 *  CROMIX Kermit, VSL International Ltd, 1985.
 *  Maxim Dynin.                                           
 *
 *  Based on the UNIX version developed at Columbia University by :
 *      Bill Catchings, Bob Cattani, Chris Maio, Frank da Cruz,
 *      Alan Crosswell, Jeff Damens, as well as Jim Guyton of Rand Corporation
 *      and Walter Underwood of Ford Aerospace.
 *          
 *
 *  Modifications by :
 *     . Wolfgang Boehm, Roland Kreuzer, and Eva Kreuger, University of Passau,
 *       West Germany, December 1986.
 *     . Adrian Pickering, Southampton University, U.K., January 1987.
 *     . Dina Desai, VSL International Ltd, Switzerland, March/May 1987.
 * 
 *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 */



#include "kermit.h"

main(argc, argv)
int argc;
char **argv;
{
	/* do initialisations */

	init();
	takefile(INITMANY);
	takefile(INITMINE);

	if (argc > 1)   /* do command line */
		execl(argv, argc);
	else {
		helpout(MAXCOM); /* interactive mode */
		Initcon = FALSE;
		inter();
	}  
	setmode( 0, MD_MODE2, Omode2, PAUSE ) ;
}

