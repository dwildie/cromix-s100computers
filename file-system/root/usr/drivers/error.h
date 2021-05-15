/*
	Cromix error numbers
	EZ -- Jun 12, 1986
*/


#define _badchan        1       /* bad channel #                        */
#define _toomany        2       /* channel already open                 */
#define _notopen        3       /* channel not open                     */
#define _endfile        4       /* end-of-file                          */
#define _ioerror        5       /* I/O error                            */
#define _filtable       6       /* file table exhausted                 */
#define _notexist       7       /* file does not exist                  */
#define _badname        8       /* bad file name                        */
#define _diraccess      9       /* directory access                     */
#define _filaccess      10      /* file access                          */
#define _exists         11      /* file already exists                  */
#define _nospace        12      /* no disk space left                   */
#define _noinode        13      /* no inodes left                       */
#define _inotable       14      /* inode table exhausted                */
#define _badcall        15      /* illegal system call                  */
#define _filsize        16      /* file size too big                    */
#define _mnttable       17      /* mount table exhausted                */
#define _notdir         18      /* not a directory                      */
#define _isdir          19      /* is a directory                       */
#define _priv           20      /* privileged system call               */
#define _notblk         21      /* not a block special device           */
#define _fsbusy         22      /* file system busy                     */
#define _notordin       23      /* not an ordinary file                 */
#define _notmount       24      /* device not mounted                   */
#define _nochild        25      /* no child processes                   */
#define _nomemory       26      /* not enough memory                    */
#define _ovflo          27      /* divide overflow                      */
#define _argtable       28      /* argument table exhausted             */
#define _arglist        29      /* arg list too big                     */
#define _numlinks       30      /* too many number of links             */
#define _difdev         31      /* cross-device link                    */
#define _nodevice       32      /* no special device                    */
#define _usrtable       33      /* user process table exhausted         */
#define _badvalue       34      /* value out of range                   */
#define _notconn        35      /* I/O device not connected             */
#define _devopen        36      /* device open error                    */
#define _diruse         37      /* directory in use (delete)            */
#define _filuse         38      /* file in use (exclusive access)       */
#define _nomatch        39      /* no match on ambiguous name           */
#define _chnaccess      40      /* channel access                       */
#define _notcromix      41      /* not a cromix disk                    */
#define _badfree        42      /* bad free list                        */
#define _badinum        43      /* bad inode number                     */
#define _readonly       44      /* device mounted for read only         */
#define _noproc         45      /* process does not exist               */
#define _ssignal        46      /* system call was aborted              */
#define _badpipe        47      /* bad call on pipe                     */
#define _locked         48      /* locked                               */
#define _deadlock       49      /* deadlocked                           */
#define _lcktable       50      /* lock table exhausted                 */
#define _tapeio         51      /* tape I/O error                       */
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

