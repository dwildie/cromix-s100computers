;       @(#)tmodeequ.asm	1.5 (5/11/87)
;
;       This file contains declarations of all values which are
;       used in the getmode and setmode Cromix system calls, for 
;       TP tape devices.
;


TPABORT         equ     196             ; Re-initialize tape driver (9 track)
TPFMARK         equ     198             ; Write file mark
TPSECURE        equ     199             ; Security erase
TPREWIND        equ     200             ; Rewind
TPUNLOAD        equ     201             ; Rewind and unload
TPMODE          equ     202             ; Mode bits
TPFILNO         equ     203             ; File number
TPBLKNO         equ     204             ; Block number
TPOBLKLN        equ     205             ; Block length for next block written
					; (9 track)
TPIBLKLN        equ     206             ; Block length of first block read
					; (9 track)
TPOBLKS         equ     207             ; Number of blocks written
					; (9 track)
TPSTAT          equ     208             ; Get status
TPLOAD          equ     209             ; Load tape (SCSI)
TPAPPEND        equ     210             ; Seek to end of recorded data (SCSI)
TPLASTERR       equ     211             ; Get last error (SCSI)
TPSOFT          equ     212             ; Soft error count (SCSI)
TPFIRM          equ     213             ; Download the firmware (SCSI)
					; (See tmodeequ.h for details)
TPVERSION       equ     214             ; Get firmware version



;       TPMODE bits

EOFCLOSE        equ     7               ; Write EOF to tape when device closes



;       TPSTAT status bits

TPDRVBUSY       equ     7               ; Drive busy
TPWRRDY         equ     6               ; FIFO ready for input (used for write)
TPRDRDY         equ     5               ; FIFO output ready (used for read)
TPLOADPT        equ     4               ; Load point
TPFBUSY         equ     3               ; Formatter busy
TPONLINE        equ     2               ; On line
TPIDENT         equ     1               ; Ident
TPRDY           equ     0               ; Ready

TPHISPEED       equ     15              ; High speed status
TPHARDERR       equ     13              ; Hard error
TPFLMARK        equ     12              ; File mark
TPCORERR        equ     11              ; Correctable error
TPWRPROT        equ     10              ; File write-protected
TPEOT           equ      9              ; End of tape
TPRWINDING      equ      8              ; Rewinding
