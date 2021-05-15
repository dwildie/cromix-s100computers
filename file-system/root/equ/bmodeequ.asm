	list    off, noxref
;
;       Cromemco Inc.
;       @(#)bmodeequ.asm	1.2 (7/7/87)
;

; Mode definitions for block devices
;

; D2-register values for _GETMODE and _SETMODE system calls
;
BMD_STATUS      equ     0       ; Get/set status byte
BMD_FLG1        equ     1       ; Get/set flag1 byte
BMD_FLG2        equ     2       ; Get/set flag2 byte
BMD_FLG3        equ     3       ; Get/set flag3 byte
BMD_SIZE        equ     4       ; Get number of bytes on device
BMD_SEEK        equ     5       ; Seek
BMD_INIT        equ     6       ; Initialize track
BMD_PRDWRT      equ     7       ; Primitive read/write
BMD_RDWRT       equ     8       ; Special read/write
BMD_RPM         equ     9       ; Get RPM
BMD_VERSION     equ     10      ; Version number
BMD_PHYCHAR     equ     11      ; Physical characteristics
BMD_LDFIRM      equ     12      ; Load firmware
BMD_SOFT        equ     13      ; Accumulated number of retries
BMD_HARD        equ     14      ; Accumulated number of hard errors
BMD_RETRY       equ     15      ; Number of retries before hard error
BMD_GETALT      equ     16      ; Get alternate track table
BMD_SETALT      equ     17      ; Set alternate track table
BMD_GETPART     equ     18      ; Get partition table
BMD_SETPART     equ     19      ; Set partition table
				;
				; Values 20 .. 63 reserved
				; Values 64 .. 127 special device modes
				; Values 128 .. 255 reserved for user
				; supplied drivers

; Floppy tape special numbers
;

BMD_RETEN       equ     64      ; Number of tape repositions before
				;     a retension


; IMI disk special numbers
;

BMD_IMITYPE     equ     64      ; Get type of IMI drive

; Values returned by BMD_IMITYPE
;

IM_50070        equ     01H     ; IMI model 50070
IM_5007W        equ     02H     ; IMI model 5007W
IM_5018H        equ     03H     ; IMI model 5018H
IM_7710A        equ     04H     ; IMI model 7710A
IM_7710B        equ     05H     ; IMI model 7710B


; Memory driver special numbers
;

BMD_TMEM        equ     64      ; Total memory
BMD_SMEM        equ     65      ; System memory
BMD_HMEM        equ     66      ; Shared memory
BMD_FMEM        equ     67      ; Free memory
BMD_MMEM        equ     68      ; Maximal free memory
BMD_CACR        equ     69      ; CACR register
				; Reserved
BMD_XMM         equ     71      ; XMM usage


; D3-register & D4-register bits for BMD_STATUS calls
;
DS_BUSY         equ     0       ; Device Busy (in use)
DS_WANT         equ     1       ; Device Wanted (do wakeup)
DS_READ         equ     2       ; Read-only device
DS_MODF         equ     3       ; Super-block modified
DS_MOUNT        equ     4       ; Device mounted
DS_HOME         equ     5       ; Device has been homed
DS_BFSTEP       equ     6       ; Buffered step flag
DS_VERIFY       equ     7       ; Verify after write

; D3-register & D4-register bits for BMD_FLG1 calls
;
DF_SMALL        equ     0       ; 1=small floppy 0=large floppy
DF_DSIDE        equ     1       ; Double sided
DF_DDENS        equ     2       ; Double density
DF_DTRACK       equ     3       ; Double tracked
DF_CROMIX       equ     4       ; Cromix format disk
DF_CDOS         equ     5       ; Cdos format disk
DF_BACKUP       equ     6       ; Backup format disk
DF_VOICE        equ     7       ; 0=step 1=voice coil

; D3-register values for BMD_FLG2 calls
;
D2_SMALL        equ     0       ; Small floppy
D2_LARGE        equ     1       ; Large floppy
D2_STDC         equ     2       ; STDC hard disk
D2_FSMD         equ     3       ; Fixed part of SMD hard disk
D2_RSMD         equ     4       ; Removable part of SMD hard disk
D2_UNIFORM      equ     5       ; Uniform floppy
D2_MEMORY       equ     6       ; Processor memory
D2_RAM          equ     7       ; RAM disk
D2_FTAPE        equ     8       ; Floppy tape
D2_IMI          equ     9       ; IMI hard disk

; D3-register & D4-register bits for BMD_FLG3 calls
;
D3_WRTPRO       equ     0       ; Device is write protected
D3_INTRPT       equ     1       ; Device interrupts
D3_DUAL         equ     2       ; Dual drive

; Floppy minor device number bits
;
FDENSITY        equ     6       ; 0 = double density
FSIDES          equ     5       ; 0 = double sided
FDUAL           equ     4       ; 1 = dual drive (PERSCI)
FDTRACK         equ     3       ; 1 = double tracked
FSIZE           equ     2       ; 0 = 8", 1 = 5"
FUNIT           equ     03H     ; Mask for unit number

; SMD minor device number bits
;
CONTROLLER      equ     7       ; Controller number
DRIVE           equ     6       ; Drive number
FIXED           equ     5       ; Fixed flag
PARTITION       equ     1FH     ; Partition number mask


; Data structure for BMD_INIT call
;
	struct  0
in_flags        ds.w    1       ; Density from minor device number
in_side         ds.w    1       ; Side to initialize
in_track        ds.w    1       ; Track to initialize
in_buf          ds.l    1       ; Pointer to track image
in_size         ds      0       ; Size of structure
	mend

; Data structure for BMD_SEEK call
;
	struct  0
sk_status       ds.b    1       ; Return status
sk_terror       ds.b    1       ; Fatal error number
sk_serror       ds.b    1       ; System error number
sk_verify       ds.b    1       ; Verify seek flag
sk_side         ds.w    1       ; Side
sk_track        ds.w    1       ; Track
sk_size         ds      0       ; Size of structure
	mend

; Data structure for BMD_RDWRT call
;
	struct  0
rw_read         ds.w    1       ; Read/write flag
rw_buf          ds.l    1       ; Buffer pointer
rw_number       ds.l    1       ; Number of blocks
rw_blknr        ds.l    1       ; Starting block number
rw_size         ds      0       ; Size of structure
	mend

; Data structure for BMD_PRDWRT call
;
	struct  0
prw_status      ds.b    1       ; Controller status
prw_terror      ds.b    1       ; Fatal error number
prw_serror      ds.b    1       ; System error number
prw_read        ds.b    1       ; Read write flag
prw_buf         ds.l    1       ; Buffer pointer
prw_number      ds.w    1       ; Number of sectors to do
prw_sector      ds.w    1       ; Starting sector number
prw_surface     ds.w    1       ; Surface number to read/write
prw_cylinder    ds.w    1       ; Cylinder number to read/write
prw_size        ds      0       ; Size of structure
	mend

; Status bits for primitive operations
;
; STDC
;
sts_ioerror     equ     0       ; IO error
sts_nioerror    equ     1       ; Not IO error
sts_select      equ     2       ; Error on select
sts_seek        equ     3       ; Error on seek
sts_prd         equ     4       ; Error on primitive read
sts_pwr         equ     5       ; Error on primitive write
sts_ptx         equ     6       ; Error on transfer
;
; Floppy
;
fls_select      equ     1       ; Error on select
fls_home        equ     2       ; Error on home
fls_rdadd       equ     3       ; Error on read address
fls_seek        equ     4       ; Error on seek
fls_pread       equ     5       ; Error on preread
fls_read        equ     6       ; Error on read
fls_write       equ     7       ; Error on write
fls_wtrk        equ     8       ; Error on write track
;
; SMD
;
sms_select      equ     1       ; Error on select
sms_home        equ     2       ; Error on home
sms_seek        equ     3       ; Error on seek
sms_read        equ     4       ; Error on read
sms_write       equ     5       ; Error on write
sms_head        equ     6       ; Error on select head
sms_pread       equ     7       ; Error on preread

; Data structure for BMD_PHYCHAR call
;

	struct  0
phy_surface     ds.w    1               ; Number of surfaces on device
phy_cylinder    ds.w    1               ; Number of cylinders on device
phy_sector      ds.w    1               ; Number of sectors/track
phy_secsiz      ds.w    1               ; Number of bytes/sector
phy_size        ds      0               ; Size of structure
	mend



; Data structure for BMD_LDFIRM call
;

	struct  0
ldf_flags       ds.w    1               ; Flags (see bellow)
ldf_count       ds.w    1               ; Number of bytes
ldf_buf         ds.l    1               ; Pointer to firmware
ldf_size        ds      0               ; Size of structure
	mend

LDFRM_DEBUG     equ     8DH             ; Load debugger firmware
LDFRM_FIRM      equ     8FH             ; Load regular firmware


; Data structure for BMD_ALTTRK call
;

	struct  0
trk_badtrk      ds.w    1               ; Bad head & cyl (4 & 12 bits)
trk_alttrk      ds.w    1               ; Alt head & cyl (4 & 12 bits)
trk_size        ds      0               ; Size of structure
	mend

	struct  0
alt_altptr      ds.l    1               ; Pointer to alternate entries
alt_numalt      ds.w    1               ; Number of alternate entries 
alt_maxalt      ds.w    1               ; Maximum numalt              
alt_alttrk      ds.w    1               ; Start of alternate tracks   
alt_size        ds      0               ; Size of structure
	mend


; Data structure for BMD_PARTITION call
;

	struct  0
ent_fstblock    ds.l    1               ; Starting block number
ent_numblock    ds.l    1               ; Number of blocks     
ent_size        ds      0               ; Size of strructure   
	mend

	struct  0
prt_partptr     ds.l    1               ; Pointer to partition entries
prt_numpart     ds.w    1               ; Number of partition entries 
prt_size        ds      0               ; Size of structure
	mend

	list xref, on
