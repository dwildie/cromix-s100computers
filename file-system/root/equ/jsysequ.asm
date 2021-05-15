	list    off

;       @(#)jsysequ.asm	1.9 (9/16/87)
;
;       Copyright (c) 1986 by Cromemco, Inc., All Rights Reserved
;        
;       This file contains declarations of all values which are
;       used during calls to the Cromix-Plus operating system.
;

stdin       equ  0      ;standard input channel
stdout      equ  1      ;standard output channel
stderr      equ  2      ;standard error channel

;
; D2 & D3 register modes for _create, _open
;
op_read     equ  0      ;read only
op_write    equ  1      ;write only
op_rdwr     equ  2      ;read and write
op_append   equ  3      ;append only
op_xread    equ  4      ;exclusive read only
op_xwrite   equ  5      ;exclusive write only
op_xrdwr    equ  6      ;exclusive read and write
op_xappend  equ  7      ;exclusive append only

op_truncf   equ  80H    ;truncate on create flag
op_condf    equ  40H    ;conditional create flag
op_force    equ  20H    ;force open of block device

;
; D2-register file position modes for .setpos
;
fwd_begin   equ    0    ;forward from the beginning of the file
fwd_current equ    1    ;forward from the current file position
fwd_end     equ    2    ;forward from the end of the file
bak_current equ   -1    ;backward from the current file position
bak_end     equ   -2    ;backward from the end of the file

;
; D2-register modes for _fstat, _cstat, _fchstat, _cchstat
;
st_all      equ  0      ;all of inode (128 bytes)
st_owner    equ  1      ;D3 = owner
st_group    equ  2      ;D3 = group
st_aowner   equ  3      ;D3 = owner access, mask
st_agroup   equ  4      ;D3 = group access, mask
st_aother   equ  5      ;D3 = other access, mask
st_ftype    equ  6      ;D3 = file type, special device #
st_size     equ  7      ;D3 = file size
st_nlinks   equ  8      ;D3 = number of links
st_inum     equ  9      ;D3 = inode number
st_device   equ  10     ;D3,D4 = device containing inode
st_tcreate  equ  11     ;D3 -> time created
st_tmodify  equ  12     ;D3 -> time last modified
st_taccess  equ  13     ;D3 -> time last accessed
st_tdumped  equ  14     ;D3 -> time last dumped
st_devno    equ  15     ;D3,D4 = device # if inode is device
st_pdevno   equ  16     ;D3,D4 = device # if inode is device
st_stext    equ  17     ;D3 = shared text attribute
st_protect  equ  18     ;D3 = delete protect attribute

;
; D2-register modes for _ustat, _chstat
;
usr_ctty     equ  0     ;D3 = controlling tty device number
usr_prior    equ  1     ;D3 = process priority
usr_parent   equ  2     ;D3 = parent process id
usr_memp     equ  3     ;D3 = address of user code
usr_mems     equ  4     ;D3 = size of code memory
usr_time     equ  5     ;D3 = process time in miliseconds
usr_ctime    equ  6     ;D3 = children time in miliseconds
usr_user     equ  7     ;D3 = effective user id
usr_group    equ  8     ;D3 = effective group id
usr_term     equ  9     ;D3 = terminal identification
usr_hdevn    equ 10     ;D3 = home directory device number
usr_hinum    equ 11     ;D3 = home directory inode number
usr_cdevn    equ 12     ;D3 = current directory device number
usr_cinum    equ 13     ;D3 = current directory inode number
usr_static   equ 14     ;D3 = programmable pointer
usr_grand    equ 15     ;D3 = grandparent process id
usr_job      equ 16     ;D3.L = job number


;
; file types for st_ftype
;
is_ordin        equ     0       ;ordinary file
is_direct       equ     1       ;directory file
is_char         equ     2       ;character device
is_block        equ     3       ;block device
is_pipe         equ     4       ;pipe file

;
; access bits for access flags
;
ac_read         equ     0       ;read access bit
ac_exec         equ     1       ;execute access bit
ac_writ         equ     2       ;write access bit
ac_apnd         equ     3       ;append access bit

;
; D2 register modes for _setuser, _getuser, _setgroup, _getgroup
;
id_effective    equ     0       ;effective id
id_login        equ     1       ;login id
id_program      equ     2       ;program id
id_d3           equ     3       ;id contained in D3 register

;
; signals
;
sigabort        equ     1       ; Abort signal
siguser         equ     2       ; User signal
sigkill         equ     3       ; Kill signal (not catchable)
sigterm         equ     4       ; Terminate signal (catchable)
sigalarm        equ     5       ; Alarm clock signal
sigpipe         equ     6       ; Broken pipe
sighangup       equ     7       ; Hang up
;               equ     8       ; Reserved

;
;
; SYSTEM CALL NUMBERS
;       system calls commented out are not implemented in current version
;
_makdev     equ  00H    ;makdev(D2,D3,D4,A0)    make device entry
_makdir     equ  01H    ;makdir(A0)             make a directory
_getdir     equ  02H    ;getdir(A0)             get current directory name
_setdir     equ  03H    ;setdir(A0)             change current directory
_mount      equ  04H    ;mount(D1,A0,A1)        mount file system
_unmount    equ  05H    ;unmount(D2,A0)         unmount file system
_delete     equ  06H    ;delete(A0)             delete file
_chkdev     equ  07H    ;chkdev(D2,D3,D4)       check for device driver
_create     equ  08H    ;D1=create(D2,D3,A0)    create & open file
_open       equ  09H    ;D1=open(D2,D3,A0)      open file
_chdup      equ  0AH    ;D2=chdup(D1)           duplicate channel
_close      equ  0BH    ;close(D1)              close file
_exchg      equ  0CH    ;exchg(D1,D2)           exchange data in files
_trunc      equ  0DH    ;trunc(D1)              truncate open file
_pipe       equ  0EH    ;D1,D2=pipe()           create a pipe
_setlev     equ  0FH    ;setlev(D1)             set interrupt level
_getpos     equ  10H    ;D3=getpos(D1)          get file position
_setpos     equ  11H    ;setpos(D1,D2,D3)       set file position
_getmode    equ  12H    ;D3=getmode(D1,D2)      get device characteristics
_setmode    equ  13H    ;D3=setmode(D1,D2,D3,D4)set device characteristics
_rdseq      equ  14H    ;D3=rdseq(D1,D3,A0)     read D3 maximum bytes
_wrseq      equ  15H    ;D3=wrseq(D1,D3,A0)     write D3 bytes
_rdbyte     equ  16H    ;D0=rdbyte(D1)          read 1 byte
_wrbyte     equ  17H    ;wrbyte(D1,D0)          write 1 byte
_rdline     equ  18H    ;D3=rdline(D1,D3,A0)    read a line
_wrline     equ  19H    ;D3=wrline(D1,A0)       write a line
_phys       equ  1AH    ;phys(A0,D3,D2)         access to phys memory
_printf     equ  1BH    ;printf(D1,A0)          print formatted string
_error      equ  1CH    ;error(D0,D1,...)       print error message
_semget     equ  1DH    ;D3=semget(D1,D2,D4)    get semaphore group id
_semop      equ  1EH    ;semop(D1,D2,A0)        semaphore operation
_semctl     equ  1FH    ;D3=semctl(D1,D2,D4,A0) semaphore control operation
_fstat      equ  20H    ;D3=fstat(D2,A0,A1)     get file status (inode)
_cstat      equ  21H    ;D3=cstat(D1,D2,A1)     get channel status (inode)
_fchstat    equ  22H    ;fchstat(D2,D3,D4,A0,A1)change file status
_cchstat    equ  23H    ;cchstat(D1,D2,D3,D4,A1)change channel status
_flink      equ  24H    ;flink(A0,A1)           link to file
_clink      equ  25H    ;clink(D1,A1)           link to open channel
_faccess    equ  26H    ;faccess(D2,A0)         test file access
_caccess    equ  27H    ;caccess(D1,D2)         test channel access
_ustat      equ  28H    ;ustat(D1,D2,D3)        get process information
_uchstat    equ  29H    ;uchstat(D1,D2,D3)      set process information
_msgget     equ  2AH    ;D3=msgget(D1,D2)       get message queue identifier
_msgsnd     equ  2BH    ;msgsnd(D1,D2,D3,A0)    send message
_msgrcv     equ  2CH    ;D3=msgrcv(D1,D2,D3,D4,A0) receive message
_msgctl     equ  2DH    ;msgctl(D1,D2,A0)       message control
_zgetmode   equ  2EH    ;                       reserved for Z80.bin
_zsetmode   equ  2FH    ;                       reserved for Z80.bin

;
_getdate    equ  30H    ;D0,D1,D2,D3=getdate()  get date
_setdate    equ  31H    ;setdate(D1,D2,D3)      set date
_gettime    equ  32H    ;D1,D2,D3=gettime()     get time
_settime    equ  33H    ;settime(D1,D2,D3)      set time
_getuser    equ  34H    ;D3=getuser(D2)         get user id
_setuser    equ  35H    ;setuser(D1,D2,D3)      set user id
_getgroup   equ  36H    ;D3=getgroup(D2)        get group id
_setgroup   equ  37H    ;setgroup(D1,D2,D3)     set group id
_getprior   equ  38H    ;D3=getprior()          get process priority
_setprior   equ  39H    ;setprior(D3)           set process priority
_getproc    equ  3AH    ;D3=getproc()           get process id
_gettable   equ  3BH    ;D3=gettable()          get system pointer
;           equ  3CH
;           equ  3DH
_lock       equ  3EH    ;lock(D2,D3,A0)         lock key
_unlock     equ  3FH    ;unlock(D2,D3,A0)       unlock key
_signal     equ  40H    ;A0=signal(D2,A0)       set up to receive a signal
_kill       equ  41H    ;kill(D2,D3)            send a signal
_sleep      equ  42H    ;sleep(D3)              sleep D3 seconds
_alarm      equ  43H    ;alarm(D3)              set alarm clock
_pause      equ  44H    ;pause()                pause for alarm clock
_wait       equ  45H    ;D1,D2,D3=wait(D1,D3)   wait for child process
_exit       equ  46H    ;exit(D3)               exit process (close files)
;           equ  47H
_fshell     equ  48H    ;D3=fshell(D1,D2,A1)    fork a shell process
_shell      equ  49H    ;shell(A1)              transfer to shell process
;           equ  4AH
_fexec      equ  4BH    ;D3=fexec(D1,D2,A0,A1)  fork and execute program
_exec       equ  4CH    ;exec(A0,A1)            execute program
;           equ  4DH
_ptrace     equ  4EH    ;ptrace(D1,D2,A0,A1,D3) trace user program
;           equ  4FH
_memory     equ  50H    ;A0=memory(A0,D1,D2,D3) allocate user memory
_indirect   equ  51H    ;indirect(D0...)        system call in D0 register
_update     equ  52H    ;update()               update disk I/O buffers
_mult       equ  53H    ;D3=D1*D2               32 bit multiply
_divd       equ  54H    ;D3,D4=D1/D2            32 bit divide
_version    equ  55H    ;D3=version()           get system version #
_boot       equ  56H    ;boot(A0,A1,D1)         boot new system
;           equ  57H
_shmget     equ  58H    ;D3=shmget(D1,D2,D4)    get shared memory identifier
_shmat      equ  59H    ;A0=shmat(D1,D2)        attach shared memory
_shmdt      equ  5AH    ;shmdt(D1,A0)           dettach shared memory
_shmctl     equ  5BH    ;shmctl(D1,D2,A0)       shared memory control

	form
;* error code definitions -- RVH  Jan 30,1979

?badchan        equ     1       ;bad channel #
?toomany        equ     2       ;channel already open
?notopen        equ     3       ;channel not open
?endfile        equ     4       ;end-of-file
?ioerror        equ     5       ;I/O error
?filtable       equ     6       ;file table exhausted
?notexist       equ     7       ;file does not exist
?badname        equ     8       ;bad file name
?diraccess      equ     9       ;directory access
?filaccess      equ     10      ;file access
?exists         equ     11      ;file already exists
?nospace        equ     12      ;no disk space left
?noinode        equ     13      ;no inodes left
?inotable       equ     14      ;inode table exhausted
?badcall        equ     15      ;illegal system call
?filsize        equ     16      ;file size too big
?mnttable       equ     17      ;mount table exhausted
?notdir         equ     18      ;not a directory
?isdir          equ     19      ;is a directory
?priv           equ     20      ;privileged system call
?notblk         equ     21      ;not a block special device
?fsbusy         equ     22      ;file system busy
?notordin       equ     23      ;not an ordinary file
?notmount       equ     24      ;device not mounted
?nochild        equ     25      ;no child processes
?nomemory       equ     26      ;not enough memory
?ovflo          equ     27      ;divide overflow
?argtable       equ     28      ;argument table exhausted
?arglist        equ     29      ;bad argument list
?numlinks       equ     30      ;too many number of links
?difdev         equ     31      ;cross-device link
?nodevice       equ     32      ;no special device
?usrtable       equ     33      ;user process table exhausted
?badvalue       equ     34      ;value out of range
?notconn        equ     35      ;I/O device not connected
?devopen        equ     36      ;device open error
?diruse         equ     37      ;directory in use (delete)
?filuse         equ     38      ;file in use (exclusive access)
?nomatch        equ     39      ;no match on ambiguous name
?chnaccess      equ     40      ;channel access
?notcromix      equ     41      ;not a cromix disk
?badfree        equ     42      ;bad free list
?badinum        equ     43      ;bad inode number
?readonly       equ     44      ;device mounted for read only
?noproc         equ     45      ;process does not exist
?signal         equ     46      ;system call was aborted
?badpipe        equ     47      ;bad call on a pipe
?locked         equ     48      ;locked
?deadlock       equ     49      ;deadlocked
?lcktable       equ     50      ;lock table exhausted
?tapeio         equ     51      ;tape I/O error
?badio          equ     52      ;bad I/O
;noz80          equ     53      ;cannot run Z80 programs
?badformat      equ     54      ;bad file format
?runaway        equ     55      ;runaway program aborted
?cdossim        equ     56      ;CDOS simmulator required
?corrupt        equ     57      ;system image corrupted
?ipcaccess      equ     58      ;IPC access error
?ipcspace       equ     59      ;IPC insufficient space
?ipcexists      equ     60      ;IPC facility already exists
?ipcnoent       equ     61      ;IPC facility not found
?ipcremove      equ     62      ;IPC facility removed
?ipcagain       equ     63      ;IPC try again
?ipc2big        equ     64      ;IPC facility too big
?ipcnomsg       equ     65      ;IPC no message waiting
?ipcrange       equ     66      ;IPC range error
?ptable         equ     67      ;Out of page tables
?nostext        equ     68      ;Out of shared text tables
?badaddress     equ     69      ;Bad address
?delprotect     equ     70      ;Protected against delete


	list    on
