/*
	@(#)ipc.h	1.2 (9/28/86)
	Interprocess communication
*/


struct ipc_perm {
	unsigned short  uid;                    /* Owner's user id      */
	unsigned short  gid;                    /* Owner's group id     */
	unsigned short  cuid;                   /* Creator's user id    */
	unsigned short  cgid;                   /* Creator's group id   */
	unsigned short  mode;                   /* Access mode          */
	unsigned short  seq;                    /* Slot sequence number */
	int             key;                    /* Key                  */
};


/*
	Common mode bits
*/

#define IPC_ALLOC       0100000         /* Entry currently allocated    */
#define IPC_CREAT       0001000         /* Create if key does not exist */
#define IPC_EXCL        0002000         /* Fail if key exists           */
#define IPC_NOWAIT      0004000         /* Error instead of wait        */


/*
	Keys
*/

#define IPC_PRIVATE     0               /* Private key                  */


/*
	Common control commands
*/

#define IPC_RMID        0               /* Remove identifier            */
#define IPC_SET         1               /* Set options                  */
#define IPC_STAT        2               /* Get options                  */

