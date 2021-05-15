/*
	Time structure
	EZ -- Mar 03, 1984
*/


typedef struct  {
	unsigned char   year;           /* year 0 .. 99         */
	unsigned char   month;          /* month 1 .. 12        */
	unsigned char   day;            /* day 1 .. 31          */
	unsigned char   hour;           /* hour 0 .. 23         */
	unsigned char   minute;         /* minute 0 .. 59       */
	unsigned char   second;         /* second 0 .. 59       */
} time;
