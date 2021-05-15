/*
	device control block definition
	EZ -- Jan 18, 1985
*/


typedef struct  {
	unsigned char   dv_stat;        /* Device status (bmodeequ.h)   */
	unsigned char   dv_flag1;       /* Flag byte 1 (bmodeequ.h)     */
	unsigned char   dv_flag2;       /* Flag byte 2 (bmodeequ.h)     */
	unsigned char   dv_flag3;       /* Flag byte 3 (bmodeequ.h)     */
	unsigned short  dv_open;        /* Number of opens              */
	unsigned short  dv_unit;        /* Unit number                  */
	unsigned long   dv_maxblk;      /* Maximum block number         */
	unsigned char   *dv_other;      /* Pointer to other information */
} dcb;
