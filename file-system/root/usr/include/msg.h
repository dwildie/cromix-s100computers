/*
	@(#)msg.h	1.1 (9/29/86)
	IPC Message Facility
*/



/*
	Mode bits
*/

#define MSG_R           0000400         /* Read permission              */
#define MSG_W           0000200         /* Write permission             */

#define MSG_RWAIT       0001000         /* A reader waiting for msg     */
#define MSG_WWAIT       0002000         /* A writer waiting for msg     */

#define MSG_NOERROR     0010000         /* No error if message too big  */




/*
	Each message has one msg structure
*/

struct msg {
	struct msg      *msg_next;      /* Next message in the queue    */
	long            msg_type;       /* Message type                 */
	short           msg_ts;         /* Message text size            */
	unsigned char   *msg_ptr;       /* Pointer to message text      */
};



/*
	There is one msqid_ds structure for each queue in the system
*/

struct msqid_ds {
	struct ipc_perm msg_perm;       /* Permission structure         */
	struct msg      *msg_first;     /* Pointer to first message     */
	struct msg      *msg_last;      /* Pointer to last message      */
	unsigned long   msg_stime;      /* Last send time               */
	unsigned long   msg_rtime;      /* Last receive time            */
	unsigned long   msg_ctime;      /* Last change time             */
	unsigned short  msg_cbytes;     /* Current # of bytes on queue  */
	unsigned short  msg_qnum;       /* Current # of msg on queue    */
	unsigned short  msg_qbytes;     /* Max # of bytes on queue      */
	unsigned short  msg_lspid;      /* Pid of last msg send         */
	unsigned short  msg_lrpid;      /* Pid of last msg receive      */
};



/*
	User message template for msgsnd and msgrcv calls
*/

struct msgbuf {
	long            mtype;          /* Message type                 */
	unsigned char   mtext[1];       /* Message text                 */
};



/*
	Installation parameters
*/

struct msginfo {
	int     msgcnt;                 /* Message pool size            */
	int     msgmax;                 /* Max message size             */
	int     msgnmb;                 /* Max # of bytes on queue      */
	int     msgmni;                 /* # of msg queue identifiers   */
	int     msgtql;                 /* # of messages in system      */
};
