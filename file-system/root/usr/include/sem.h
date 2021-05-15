/*
	@(#)sem.h	1.1 (9/29/86)
	IPC Message Facility
*/



/*
	Mode bits
*/

#define SEM_R           0000400         /* Read permission              */
#define SEM_A           0000200         /* Alter permission             */

#define SEM_UNDO        0010000         /* Set up adjust on exit entry  */



/*
	Semctl commands
*/

#define GETNCNT 3                       /* Get semncnt                  */
#define GETPID  4                       /* Get sempid                   */
#define GETVAL  5                       /* Get semval                   */
#define GETALL  6                       /* Get all semval's             */
#define GETZCNT 7                       /* Get semzcnt                  */
#define SETVAL  8                       /* Set semval                   */
#define SETALL  9                       /* Set all semval's             */



/*
	Other constants
*/

#define SEMVMX ((unsigned short) 32767) /* Max semaphore value          */
#define SEMAEM ((short) 16384)          /* Max adjust on exit           */


/*
	There is one sem structure for each semaphore
*/

struct sem {
	unsigned short  semval;         /* Semaphore value              */
	unsigned short  sempid;         /* PID of last operation        */
	unsigned short  semncnt;        /* # awaiting semval > cval     */
	unsigned short  semzcnt;        /* # awaiting semval == 0       */
};



/*
	There is one semid_ds structure for each set of semaphores
*/

struct semid_ds {
	struct ipc_perm sem_perm;       /* Permission structure         */
	struct sem      *sem_base;      /* Ptr to first sem in set      */
	unsigned short  sem_nsems;      /* Number of semaphores in set  */
	unsigned long   sem_otime;      /* Last semop time              */
	unsigned long   sem_ctime;      /* Last change time             */
};



/*
	There is one undo structure per process in the system
*/

struct sem_undo {
	struct sem_undo *un_np;         /* Ptr to next active undo      */
	unsigned short  un_cnt;         /* # of active entries          */
	struct undo {
	 short          un_aoe;         /* Adjust on exit value         */
	 unsigned short un_num;         /* Semaphore #                  */
	 int            un_id;          /* Semid                        */
	}               un_ent[1];      /* Undo entries (one minimum)   */
};



/*
	User semaphore template for semop system call
*/

struct sembuf {
	unsigned short  sem_num;        /* Semaphore #                  */
	short           sem_op;         /* Semaphore operation          */
	short           sem_flg;        /* Operation flags              */
};



/*
	Installation parameters
*/

struct seminfo {
	int     semcnt;                 /* Semaphore pool size (bytes)  */
	int     semmni;                 /* Number of sem identifiers    */
	int     semmsl;                 /* Max # of semaphore per id    */
	int     semopm;                 /* Max # of operations/call     */
	int     semmnu;                 /* # of undo structs/system     */
	int     semume;                 /* Max # of undo's per process  */
};
