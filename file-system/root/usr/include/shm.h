/*
	@(#)shm.h	1.1 (9/29/86)
	IPC Message Facility
*/



/*
	Mode bits
*/

#define SHM_R           0000400         /* Read permission              */
#define SHM_W           0000200         /* Write permission             */

#define SHM_CLEAR       0001000         /* Zap on first attach          */
#define SHM_DEST        0002000         /* Destroy segment when #att = 0*/

#define SHM_RDONLY      0010000         /* Attach read only             */




/*
	There is one shmid_ds structure for each shared memory segment
*/

struct shmid_ds {
	struct ipc_perm shm_perm;       /* Permission structure         */
	unsigned long   shm_segsz;      /* Segment size in bytes        */
	unsigned char   *shm_first;     /* Address of memory segment    */
	unsigned long   shm_atime;      /* Last attach time             */
	unsigned long   shm_dtime;      /* Last dettach time            */
	unsigned long   shm_ctime;      /* Last change time             */
	unsigned short  shm_lpid;       /* Pid of last shmop            */
	unsigned short  shm_cpid;       /* Pid of creator               */
	unsigned short  shm_nattch;     /* Current # attached           */
};



/*
	Installation parameters
*/

struct shminfo {
	int     shmmax;                 /* Max memory segment size      */
	int     shmmni;                 /* Number of shm identifiers    */
	int     shmseg;                 /* Max # of segments/process    */
	int     shmall;                 /* Total shared memory size     */
};
