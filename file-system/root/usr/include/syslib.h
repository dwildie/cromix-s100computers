/* 
   @(#)syslib.h	1.1 (7/3/87)
   Copyright (c) 1987 by Cromemco, Inc., All Rights Reserved

   This file contains declarations of all types which are
   used in with syslib system calls library.
*/



struct st_time {                        /* date and time in status      */
		char    year;           /* Year number (0 .. 99)        */
		char    month;          /* Month number (1 .. 12)       */
		char    day;            /* Day number (1 .. 31)         */
		char    hour;           /* Hour number (0 .. 23)        */
		char    minute;         /* Minute number (0 .. 59)      */
		char    second;         /* Second number (0 .. 59)      */
	};

struct sys_date {                       /* system date                  */
		char    month;          /* Month (1 .. 12)              */
		char    day;            /* Day (1 .. 31)                */
		char    year;           /* Year (0 .. 99)               */
		char    wday;           /* Day of the week (1 .. 7)     */
					/*      1 -- Sunday             */
					/*      2 -- Monday             */
					/*      ...........             */
					/*      7 -- Saturday           */
	};

struct sys_time {                       /* system time                  */
		char    hour;           /* Hours (0 .. 23)              */
		char    minute;         /* Minutes (0 .. 59)            */
		char    second;         /* Seconds (0 .. 59)            */
	};

struct sys_reg {                        /* registers for indirect call  */
		int     reg_D0;         /* only register actualy used   */
		int     reg_D1;         /* are defined                  */
		int     reg_D2;
		int     reg_D3;
		int     reg_D4;
		char    * reg_A0;
		char    * reg_A1;
	};

extern double dz80to68();
extern double fz80to68();

#define ERR     (-1)
