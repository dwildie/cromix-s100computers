	list    off
	list    noxref

;       @(#)modeequ.asm	1.9 (4/23/87)
;

; -----------------------------------------------------------------------------
; mode definitions for terminals and printers,
; TTY, QTTY, MTTY, LPT, SLPT, QSLPT, and TYP

; D2-register values for _GETMODE and _SETMODE system calls
MD_ISPEED       equ     0               ; input speed
MD_OSPEED       equ     1               ; output speed
MD_MODE1        equ     2               ; flags: RAW, ECHO, etc.
MD_MODED        equ     3               ; delays for NL, CR, etc.
MD_MODE2        equ     4               ; flags: PAUSE, XFF, etc.
MD_MODE3        equ     5               ; flags: CBREAK, VRAW, etc.
MD_ERASE        equ     6               ; auxilliary erase character
MD_DELECHO      equ     7               ; erasure echo character
MD_LKILL        equ     8               ; line kill character
MD_USIGNAL      equ     9               ; SIGUSER signal key
MD_LENGTH       equ     10              ; page length (lines)
MD_WIDTH        equ     11              ; page width (columns)
MD_BMARGIN      equ     12              ; bottom margin (lines)
MD_MODE4        equ     13              ; flags: TDELAY, HALFD, INL
MODELEN         equ     MD_MODE4 + 1    ; Number of byte mode values

MD_FORMS        equ     254             ; printer forms number
MD_IDENT        equ     255             ; device identification

; more D2-register values for SLPT only
SLPT_BSIZE      equ     MD_ERASE        ; ETX/ACK block size

; more D2-register values for TYP only
TYP_CWIDTH      equ     64              ; character width in 1/120 in.
TYP_LHEIGHT     equ     65              ; line height in 1/48 in.
TYP_LMARGIN     equ     66              ; left margin in 1/10 in.

; more D2-register values for _GETMODE and _SETMODE system calls
MD_RESUME       equ     157             ; Resume paused output
MD_STATUS       equ     156             ; check whether input queues empty
MD_IFLUSH       equ     155             ; flush input queues
MD_FNKEYS       equ     152             ; turn function keys on or off
					;   D3-register = 1 to enable fnkeys
					;   D3-register = 0 to disable them
MD_PSIGHUP      equ     151             ; signal current process if hang up
;               equ     150             ; (this value reserved)
;               equ     149             ; output break (QTTYs only)
					;   D3-register = 1 to start break
					;   D3-register = 0 to stop break
MD_MODEM        equ     148             ; (QTTYs and MTTYs only)
MD_TYP          equ     147             ; (TYP only)


; D3-register values for MD_ISPEED baudrate calls
; not all baud rates are supported by all devices
S_HANGUP        equ     0               ; hang up phone
S_50            equ     1               ; 50 baud
S_75            equ     2               ; 75 baud
S_110           equ     3               ; 110 baud
S_134           equ     4               ; 134.5 baud
S_150           equ     5               ; 150 baud
S_200           equ     6               ; 200 baud
S_300           equ     7               ; 300 baud
S_600           equ     8               ; 600 baud
S_1200          equ     9               ; 1,200 baud
S_1800          equ     10              ; 1,800 baud
S_2400          equ     11              ; 2,400 baud
S_4800          equ     12              ; 4,800 baud
S_9600          equ     13              ; 9,600 baud
;               equ     14              ; External A
;               equ     15              ; External B
S_19200         equ     16              ; 19,200 baud
S_38400         equ     17              ; 38,400 baud

S_CTSWAIT       equ     125     ; wait for Clear To Send
S_NOCHG         equ     126     ; no change of baudrate
S_UNINIT        equ     127     ; baudrate has not been initialized yet
Sfl_AUTO        equ     7       ; (bit 7) input CRs from keyboard to set baudr

; D3-register & D4-register bits for MD_MODE1 calls
TANDEM          equ     0       ; send XOFF/XON to control filling of input buf
XTAB            equ     1       ; expand TABs
LCASE           equ     2       ; convert alphabetics to lower case
ECHO            equ     3       ; echo input
CRDEVICE        equ     4       ; on input, map CR into NL,
				; on output, change NL to CRLF.
RAW             equ     5       ; on input, return after each character,
				; no erase, linekill, or EOF characters,
				; no output PAUSE or output width truncation,
				; treat X-OFF & X-ON as regular input.
ODD             equ     6       ; parity function bits
EVEN            equ     7       ;

; special definition for the big version of the TYP printer driver
AUTOPRO         equ     RAW     ; automatic proportional spacing

; D3-register & D4-register bits for MD_MODE2 calls
PAUSE           equ     0       ; wait for CNTRL-Q after a page is output
NOTIMMECHO      equ     1       ; do not echo characters typed-ahead
NOECNL          equ     2       ; do not echo NLs
SGENABLE        equ     3       ; send SIGUSER signal if MD_USIGNAL key pushed
ABENABLE        equ     4       ; send SIGABORT signal if CNTRL-C key pushed
XFF             equ     5       ; expand FFs
WRAP            equ     6       ; wrap-around if page width is exceeded
SIGALLC         equ     7       ; send SIGUSER signal for every key pushed

; D3-register & D4-register bits for MD_MODE3 calls
ESCRETN         equ     0       ; ESC causes input line to be returned
FNKEYS          equ     1       ; response to 3102 function keys enabled
HUPENAB         equ     2       ; hang up modem when device is finally closed
SIGHUPALL       equ     3       ; send SIGHANGUP signals to all processes which
				; use this TTY device if modem hangs up
CBREAK          equ     4       ; on input, return after each character,
				; no erase, linekill, or EOF characters.
BINARY          equ     5       ; on input, return after each character,
				; no erase, linekill, or EOF characters,
				; no output PAUSE or output width truncation,
				; treat X-OFF & X-ON as regular input,
				; no tandem mode (i.e., no input buf control),
				; no abort signal (^C), no user signal,
				; no changing or checking parity bit,
				; no delays after control chars such as NLs,
				; no echoing,
				; no character transformations (i.e., ignore
				; the LCASE, CRDEV, and XTABS modes)
				; no function-key decoding.
CRIGNORE        equ     6       ; On output, ignore CR, change LF to CR
DISCARD         equ     7       ; discard the device when it is no longer open

; D3-register & D4-register bits for MD_MODE4 calls
TDELAY          equ     0       ; Delay is expressed in time units instead
				; of NULL characters
RTSCTS          equ     1       ; Hardware handhaking
HALFD           equ     2       ; Half duplex
INL             equ     3       ; Translate input NL to ^X


; D3-register & D4-register values for MD_MODED calls
NLDELAY         equ     03H     ; (pairs of bits)
TABDELAY        equ     0CH     ;
CRDELAY         equ     30H     ;
FFDELAY         equ     40H     ; (single bits)
BSDELAY         equ     80H     ;

; D3-register bits for MD_STATUS calls
INOTEMPTY       equ     0       ; there is a character in the input buffer
				; (but if not CBREAK, RAW, or BINARY mode,
				; it won't be accessible until a whole line
				; is entered)


; _GETMODE D3-register bits for MD_MODEM calls
RXDA            equ     0       ; Receiver Data Available
TXBE            equ     2       ; Transmitter Buffer Empty
DCD             equ     3       ; Data Carrier Detect
CTS             equ     5       ; Clear To Send
RXBREAK         equ     7       ; Reciver data line broken

; _GETMODE D4-register bits for MD_MODEM calls
notRI           equ     6       ; Not ringing
notDSR          equ     7       ; Data Set not Ready


; _SETMODE D3-register and D4-register bits for MD_MODEM calls
RTS             equ     1       ; Request to Send
TXBREAK         equ     4       ; Break the transmitter line
DTR             equ     7       ; Data Terminal Ready


; _GETMODE D3-register bits for MD_TYP calls
TYPCHK          equ     1       ; the 3355 printer is in a check condition
TYPPAP          equ     2       ; the 3355 printer is out of paper
TYPRIB          equ     3       ; the 3355 printer is out of ribbon
TYPOFL          equ     4       ; the 3355 printer is off-line

; _GETMODE D3-register values for MD_IDENT calls
ID_TTY          equ      0      ; Tuart terminal
ID_QTTY         equ      1      ; Quadart or Octart terminal
ID_LPT          equ      2      ; Parallel printer
ID_TYP          equ      3      ; Fully formed printer
ID_SLPT         equ      4      ; Serial printer
ID_QSLPT        equ      5      ; Serial printer on quadart
ID_CNET         equ      6      ; CNET driver
ID_FFP          equ      7      ; FFP processor driver
ID_SYSTEM       equ      8      ; System device
ID_TIMER        equ      9      ; Timer device
ID_TAPE         equ     10      ; Half inch tape drive
ID_SCC          equ     11      ; SCC terminal
ID_MAX          equ     12      ; Maximizer
ID_NTTY         equ     13      ; New Tuart terminal
ID_NOCT         equ     14      ; New Octart terminal
ID_PLPT         equ     15      ; Parallel printer
ID_USLPT        equ     16      ; UART serial printer
ID_OSLPT        equ     17      ; OCTART serial printer
ID_SCTP         equ     18      ; SCSI Tape
				; Values 19 .. 127 reserved
				; Values 128 .. 255 reserved for user
				; defined drivers and devices

	list    xref
	list    on
