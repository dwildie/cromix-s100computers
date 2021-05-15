/*
	@(#)jsysequ.h	1.9 (9/16/87)

	Copyright (c) 1986 by Cromemco, Inc., All Rights Reserved
	 
	This file contains declarations of all values which are
	used during calls to the Cromix-Plus operating system.
*/



/*
   Standard channel numbers
*/

#define STDIN           0       /* Standard input channel       */
#define STDOUT          1       /* Standard output channel      */
#define STDERR          2       /* Standard error channel       */



/*
   Access modes for create
*/

#define op_read         0       /* Read only                    */
#define op_write        1       /* Write only                   */
#define op_rdwr         2       /* Read and write               */
#define op_append       3       /* Append only                  */
#define op_xread        4       /* Exclusive read only          */
#define op_xwrite       5       /* Exclusive write only         */
#define op_xrdwr        6       /* Exclusive read and write     */
#define op_xappend      7       /* Exclusive append only        */

#define op_truncf       0x80    /* Truncate on create flag      */
#define op_condf        0x40    /* Conditional create flag      */
#define op_force        0x20    /* Force open on block device   */



/*
   Modes for setpos system call
*/

#define fwd_begin       0       /* Forward from the beginning of the file  */
#define fwd_current     1       /* Forward from the current position       */
#define fwd_end         2       /* Forward from the end of the file        */
#define bak_current     -1      /* Backward from the current file position */
#define bak_end         -2      /* Backward from the end of the file       */



/*
   Status types for _fstat, _cstat, _fchstat, _cchstat
*/

#define st_all          0       /* All of inode (128 bytes)             */
#define st_owner        1       /* Owner                                */
#define st_group        2       /* Group                                */
#define st_aowner       3       /* Owner access, mask                   */
#define st_agroup       4       /* Group access, mask                   */
#define st_aother       5       /* Other access, mask                   */
#define st_ftype        6       /* File type, special device #          */
#define st_size         7       /* File size                            */
#define st_nlinks       8       /* Number of links                      */
#define st_inum         9       /* Inode number                         */
#define st_device       10      /* Device containing inode              */
#define st_tcreate      11      /* Time created                         */
#define st_tmodify      12      /* Time last modified                   */
#define st_taccess      13      /* Time last accessed                   */
#define st_tdumped      14      /* Time last dumped                     */
#define st_devno        15      /* Device number if inode is a device   */
#define st_pdevno       16      /* Phys device # if inode is a device   */
#define st_stext        17      /* Shared text attribute                */
#define st_protect      18      /* Delete protect attribute             */



/*
   Status types for _ustat, _uchstat
*/

#define usr_ctty        0       /* Controlling tty device number        */
#define usr_prior       1       /* Process priority                     */
#define usr_parent      2       /* Parent process id                    */
#define usr_memp        3       /* Address of user code                 */
#define usr_mems        4       /* Size of code memory                  */
#define usr_time        5       /* Process time in miliseconds          */
#define usr_ctime       6       /* Children time in miliseconds         */
#define usr_user        7       /* Effective user id                    */
#define usr_group       8       /* Effective group id                   */
#define usr_term        9       /* Terminal identification              */
#define usr_hdevn      10       /* Home directory device number         */
#define usr_hinum      11       /* Home directory inode number          */
#define usr_cdevn      12       /* Current directory device number      */
#define usr_cinum      13       /* Current directory inode number       */
#define usr_static     14       /* Programmable pointer                 */
#define usr_grand      15       /* Grandparent process id               */
#define usr_job        16       /* Job number                           */



/*
   File types for st_ftype
*/

#define is_ordin        0       /* Ordinary file                        */
#define is_direct       1       /* Directory file                       */
#define is_char         2       /* Character device                     */
#define is_block        3       /* Block device                         */
#define is_pipe         4       /* Pipe file                            */



/*
   Mask values for file access flags
*/

#define ac_read         0x01    /* Read access bit                      */
#define ac_exec         0x02    /* Execute access bit                   */
#define ac_writ         0x04    /* Write access bit                     */
#define ac_apnd         0x08    /* Append access bit                    */



/*
   Id types and values for _setuser, _getuser, _setgroup, _getgroup
*/

#define id_effective    0       /* Effective id                         */
#define id_login        1       /* Login id                             */
#define id_program      2       /* Program id                           */
#define id_number       3       /* Id contained in idnumber             */



/*
   Signal types
*/

#define sigabort        1       /* Control-C key                        */
#define siguser         2       /* User specifiable key                 */
#define sigkill         3       /* Kill signal                          */
#define sigterm         4       /* Terminate                            */
#define sigalarm        5       /* Alarm                                */
#define sigpipe         6       /* Broken pipe signal                   */
#define sighangup       7       /* Modem hang up                        */
				/* Reserved                             */

#define SIGABORT        (1 << sigabort - 1)
#define SIGUSER         (1 << siguser  - 1)
#define SIGKILL         (1 << sigkill  - 1)
#define SIGTERM         (1 << sigterm  - 1)
#define SIGALARM        (1 << sigalarm - 1)
#define SIGPIPE         (1 << sigpipe  - 1)
#define SIGHANGUP       (1 << sighangup- 1)



/*
   Cromix-Plus System Call Numbers
*/

#define _makdev         0x00    /* Make device entry                    */
#define _makdir         0x01    /* Make a directory                     */
#define _getdir         0x02    /* Get current directory name           */
#define _setdir         0x03    /* Change current directory             */
#define _mount          0x04    /* Mount file system                    */
#define _unmount        0x05    /* Unmount file system                  */
#define _delete         0x06    /* Delete file                          */
#define _chkdev         0x07    /* Check for device driver              */
#define _create         0x08    /* Create & open file                   */
#define _open           0x09    /* Open file                            */
#define _chdup          0x0A    /* Duplicate channel                    */
#define _close          0x0B    /* Close file                           */
#define _exchg          0x0C    /* Exchange the contents of two inodes  */
#define _trunc          0x0D    /* Truncate open file                   */
#define _pipe           0x0E    /* Generate a pipe                      */
#define _setlev         0x0F    /* Set interrupt level                  */
#define _getpos         0x10    /* Get file position                    */
#define _setpos         0x11    /* Set file position                    */
#define _getmode        0x12    /* Get device characteristics           */
#define _setmode        0x13    /* Set device characteristics           */
#define _rdseq          0x14    /* Read n bytes                         */
#define _wrseq          0x15    /* Write n bytes                        */
#define _rdbyte         0x16    /* Read 1 byte                          */
#define _wrbyte         0x17    /* Write 1 byte                         */
#define _rdline         0x18    /* Read a line                          */
#define _wrline         0x19    /* Write a line                         */
#define _phys           0x1A    /* Access physical addresses            */
#define _printf         0x1B    /* Print formatted string               */
#define _error          0x1C    /* Print error message                  */
#define _semget         0x1D    /* Get semaphore identifier             */
#define _semop          0x1E    /* Semaphore operation                  */
#define _semctl         0x1F    /* Semaphore control                    */
#define _fstat          0x20    /* Get file status (inode)              */
#define _cstat          0x21    /* Get channel status (inode)           */
#define _fchstat        0x22    /* Change file status                   */
#define _cchstat        0x23    /* Change channel status                */
#define _flink          0x24    /* Link to file                         */
#define _clink          0x25    /* Link to open channel                 */
#define _faccess        0x26    /* Test file access                     */
#define _caccess        0x27    /* Test channel access                  */
#define _ustat          0x28    /* Get process table information        */
#define _uchstat        0x29    /* Change process table information     */
#define _msgget         0x2A    /* Get message queue                    */
#define _msgsnd         0x2B    /* Send message                         */
#define _msgrcv         0x2C    /* Receive message                      */
#define _msgctl         0x2D    /* Message control                      */
#define _zgetmode       0x2E    /* Reserved for Z80.bin                 */
#define _zsetmode       0x2F    /* Reserved for Z80.bin                 */

#define _getdate        0x30    /* Get date                             */
#define _setdate        0x31    /* Set date                             */
#define _gettime        0x32    /* Get time                             */
#define _settime        0x33    /* Set time                             */
#define _getuser        0x34    /* Get user id                          */
#define _setuser        0x35    /* Set user id                          */
#define _getgroup       0x36    /* Get group id                         */
#define _setgroup       0x37    /* Set group id                         */
#define _getprior       0x38    /* Get the current process priority     */
#define _setprior       0x39    /* Set the current process priority     */
#define _getproc        0x3A    /* Get process id                       */
#define _gettable       0x3B    /* Get system pointer (Shell, ...)      */

#define _lock           0x3E    /* Lock key                             */
#define _unlock         0x3F    /* Unlock key                           */
#define _signal         0x40    /* Set up to receive a signal           */
#define _kill           0x41    /* Send a signal                        */
#define _sleep          0x42    /* Sleep for specified number of secs   */
#define _alarm          0x43    /* Set alarm clock                      */
#define _pause          0x44    /* Pause for alarm clock                */
#define _wait           0x45    /* Wait for child process               */
#define _exit           0x46    /* Exit process (close files)           */

#define _fshell         0x48    /* Fork a shell process                 */
#define _shell          0x49    /* Transfer to shell process            */

#define _fexec          0x4B    /* Fork and execute program             */
#define _exec           0x4C    /* Execute program                      */
				/* Reserved                             */
#define _ptrace         0x4E    /* Debug system call                    */
#define _memory         0x50    /* Allocate user memory                 */
#define _indirect       0x51    /* System call in D0-register           */
#define _update         0x52    /* Update disk I/O buffers              */
#define _mult           0x53    /* Multiply                             */
#define _divd           0x54    /* Divide                               */
#define _version        0x55    /* Get system version #                 */
#define _boot           0x56    /* Boot new operating system            */

#define _shmget         0x58    /* Get shared memory identifier         */
#define _shmat          0x59    /* Attach shared memory                 */
#define _shmdt          0x5A    /* Dettach shared memory                */
#define _shmctl         0x5B    /* Shared memory control                */


/*
   Cromix-Plus error numbers
*/

#define _badchan        1       /* Bad channel #                        */
#define _toomany        2       /* Channel already open                 */
#define _notopen        3       /* Channel not open                     */
#define _endfile        4       /* End-of-file                          */
#define _ioerror        5       /* I/O error                            */
#define _filtable       6       /* File table exhausted                 */
#define _notexist       7       /* File does not exist                  */
#define _badname        8       /* Bad file name                        */
#define _diraccess      9       /* Directory access                     */
#define _filaccess      10      /* File access                          */
#define _exists         11      /* File already exists                  */
#define _nospace        12      /* No disk space left                   */
#define _noinode        13      /* No inodes left                       */
#define _inotable       14      /* Inode table exhausted                */
#define _badcall        15      /* Illegal system call                  */
#define _filsize        16      /* File size too big                    */
#define _mnttable       17      /* Mount table exhausted                */
#define _notdir         18      /* Not a directory                      */
#define _isdir          19      /* Is a directory                       */
#define _priv           20      /* Privileged system call               */
#define _notblk         21      /* Not a block special device           */
#define _fsbusy         22      /* File system busy                     */
#define _notordin       23      /* Not an ordinary file                 */
#define _notmount       24      /* Device not mounted                   */
#define _nochild        25      /* No child processes                   */
#define _nomemory       26      /* Not enough memory                    */
#define _ovflo          27      /* Divide overflow                      */
#define _argtable       28      /* Argument table exhausted             */
#define _arglist        29      /* Arg list too big                     */
#define _numlinks       30      /* Too many number of links             */
#define _difdev         31      /* Cross-device link                    */
#define _nodevice       32      /* No special device                    */
#define _usrtable       33      /* User process table exhausted         */
#define _badvalue       34      /* Value out of range                   */
#define _notconn        35      /* I/O device not connected             */
#define _devopen        36      /* Device open error                    */
#define _diruse         37      /* Directory in use (delete)            */
#define _filuse         38      /* File in use (exclusive access)       */
#define _nomatch        39      /* No match on ambiguous name           */
#define _chnaccess      40      /* Channel access                       */
#define _notcromix      41      /* Not a cromix disk                    */
#define _badfree        42      /* Bad free list                        */
#define _badinum        43      /* Bad inode number                     */
#define _readonly       44      /* Device mounted for read only         */
#define _noproc         45      /* Process does not exist               */
#define _ssignal        46      /* System call was aborted              */
#define _badpipe        47      /* Bad call on pipe                     */
#define _locked         48      /* Locked                               */
#define _deadlock       49      /* Deadlocked                           */
#define _lcktable       50      /* Lock table exhausted                 */
#define _tapeio         51      /* Tape I/O error                       */
#define _badio          52      /* I/O error                            */
#define _noz80          53      /* Cannot run Z80 programs              */
#define _badformat      54      /* Bad file format                      */
#define _runaway        55      /* Runaway program aborted              */
#define _cdossim        56      /* CDOS simulator required              */
#define _corrupt        57      /* System image corrupted               */
#define _ipcaccess      58      /* IPC access error                     */
#define _ipcspace       59      /* IPC insufficient space               */
#define _ipcexists      60      /* IPC facility already exists          */
#define _ipcnoent       61      /* IPC facility not found               */
#define _ipcremove      62      /* IPC facility removed                 */
#define _ipcagain       63      /* IPC try again                        */
#define _ipc2big        64      /* IPC facility too big                 */
#define _ipcnomsg       65      /* IPC no message waiting               */
#define _ipcrange       66      /* IPC range error                      */
#define _ptable         67      /* Out of page tables                   */
#define _nostext        68      /* Out of shared text tables            */
#define _badaddress     69      /* Bad address                          */
#define _delprotect     70      /* Protect against delete               */
