/*
  TTY driver

  Modification History.

  Version Who             When            What

  30.00   Nikolaj Ivancic Mar xx, 1985    Original version
  40.00   Egon Zakrajsek  Aug 13, 1985    Updated for XXU
  40.03   Egon Zakrajsek  Aug 30, 1985    Sleep event for wdg
  xx.17   Egon Zakrajsek  Mar 09, 1986    Flush both queues on abort
  xx.30   Egon Zakrajsek  Jun 13, 1986    LKILL on no ECHO
  xx.30   Damian Wildie   Jan 01, 2021    Port for S100 SIO
*/

#include "iolib.h"
#include "macros.h"
#include "char.h"
#include "queue.h"
#include "modeequ.h"
#include "error.h"
#include "event.h"
#include "signal.h"
#include "watchdog.h"
#include "sio.h"



static unsigned char intstack[512];     /* Autovector interrupt stack   */
#define STACK   (intstack + sizeof(intstack))

/*
 *      TTY driver control block
 */

static int stty_init();
static int stty_open();
static int stty_close();
static int stty_read();
static int stty_write();
static int stty_getmode();
static int stty_setmode();

chd     stty_c = {
    stty_init,
    stty_open,
    stty_close,
    stty_read,
    stty_write,
    stty_getmode,
    stty_setmode
 }; 

/*
  Header information for TTY driver
*/


typedef struct {
  unsigned char  minnum;          /* minor device number             */
  unsigned char  base;            /* base port number                */
  unsigned short tt_opencnt;      /* open count                      */
  unsigned long  tt_stat;         /* device status                   */
  cbuf           tt_inpq;         /* input queue                     */
  cbuf           tt_tahq;         /* type-ahead queue                */
  cbuf           tt_outq;         /* output queue                    */
  unsigned char  tt_palnct;       /* pause countdown                 */
  unsigned char  tt_fflnct;       /* formfeed countdown              */
  unsigned char  tt_colct;        /* column count                    */
  unsigned char  tt_klseq;        /* storage for char echo killed ln */
  unsigned short tt_klct;         /* killed line length              */
  wdg            tt_wd;           /* time waste watchdog             */
  unsigned short tt_dunit;        /* delay unit (milliseconds        */
  unsigned char  tt_mode[MODELEN];/* mode parameters                 */

} tty_str;

/* status bits on tt_stat */

#define RDWAIT       0x00000001 /* waiting for input                       */
#define WRWAIT       0x00000002 /* waiting for output queue to drain       */
#define WRITSTOP     0x00000004 /* writing stopped (screen pause)          */
#define OUTPSTOP     0x00000008 /* output interrupt stopped by the stop ch */
#define CTSWAIT      0x00000010 /* waiting for a clear to send (dial-up)   */
#define INT_DUE      0x00000020 /* an output interrupt is due              */
#define DELETION     0x00000040 /* a character deletion is processed       */
#define STXFLG       0x00000080 /* STX received (3102 function key decoding*/
#define RDXFER       0x00000100 /* transferring from type ahead to input   */
#define INPSTOP      0x00000400 /* input buffer too full (TANDEM mode)     */
#define PUTSTOP      0x00000800 /* time to output stop char (TANDEM mode)  */
#define PUTSTART     0x00001000 /* time to output start char (TANDEM mode) */
#define DISCARDED    0x00002000 /* device has been discarded on close      */
#define ECHOING      0x00004000 /* an input character is beeing echoed     */
#define INPOVFL      0x00008000 /* input (type-ahead) queue has overflowed */
#define OUTDELAY     0x00010000 /* output interrupt stopped by DELAY       */

/*
 *      Interrupt vectors
 */

#define MAXTTY   2      /* Max dimension of the s_tty array */
#define OUT_RETRY 256   /* Retrys while outputting one character */
#define PGLENGTH 24     /* Screen size (rows) */


#define HmRDWAIT 0x0100 /* various reasons for sleep      */
#define HmWRWAIT 0x0200
#define HmSCWAIT 0x0300
#define HmWDWAIT 0x0400

#define TTOHIGH  60     /* Output queue high-water mark           */
#define TTOLOW   24     /* Output queue  low-water mark           */
#define TTIHIGH  132    /* Type-ahead queue high-water mark       */
#define TTILOW   24     /* and low-water mark (TANDEM mode        */
#define PARITY   0x80   /* Parity bit mask                        */

#define BELL     'G'-'@'
#define CNTRL_C  'C'-'@'
#define X_OFF    'S'-'@'
#define X_ON     'Q'-'@'
#define CNTRL_U  'U'-'@'
#define EOFILE   'Z'-'@'
#define ESC      '['-'@'
#define STX      0x02
#define DEL      0x7F
#define RUBSEQLN 3
#define NSPEEDS  17


/*   G L O B A L   V A R I A B L E S   */

static unsigned short majdevno;         /* Major device number in high byte */
static unsigned int  tty_slpev;         /* Sleep event */
static tty_str       *sp[MAXTTY];           /* Array of pointers to structures */
static tty_str       s_tty[MAXTTY];     /* TTY data structures */
static char          rbseq[] = "\b \b";

static unsigned short delaytbl[] = {
     0,          0,       0,  40000/110,
     0,  40000/150,       0,  40000/300,
     0, 40000/1200,       0, 40000/2400,
  40000/4800, 40000/9600,       0,          0,
 40000/19200
};

static unsigned char partbl[] = {
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x80, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00,
  0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80
};

extern unsigned int  err;


/* ----------------------------------------------------------------------- */

/*
 *      Interrupt service routines and related stuff
 */

static int stty_int() {
  extern int d_printf();
  extern void sioregout();
  static ttii();
  static ttoi();
  unsigned char vector = sioregin(SIO_B_CTRL, ZSCC_RR2);
  unsigned char pending = sioregin(SIO_A_CTRL, ZSCC_RR3);

  if (pending) {
    if (ZSCC_A_TX_IP & pending) {
      ttoi(sp[PORT_A_MINOR]);
    }
    if (ZSCC_A_RX_IP & pending) {
      ttii(sp[PORT_A_MINOR]);
    }
    if (ZSCC_B_TX_IP & pending) {
      ttoi(sp[PORT_B_MINOR]);
    }
    if (ZSCC_B_RX_IP & pending) {
      ttii(sp[PORT_B_MINOR]);
    }
  } else {
      /* Just for debug */
      d_printf("E%02x", sioregin(SIO_B_CTRL, ZSCC_RR1));
      d_printf("S%02x", sioregin(SIO_A_CTRL, ZSCC_RR0));
      d_printf("D%02x", sioin(SIO_A_CTRL));
      d_printf("V%02x ", vector);
  }

  /* Issue IUS Reset command to Z8530 */
  sioregout(SIO_A_CTRL, ZSCC_RR0, ZSM_RESET_IUS);

  return 0;
}

static ttii(p)          /* Input interrupt service routine */
tty_str *p;
{
  extern int sioregin(), sioin();
  extern int sgttysig(), wakeup(), putq();
  static int ttoutq(),ttwarn(),restartout();
  static int flush(),pputinpq(),ckinhigh(),echo_c();

  int ch, rr0, rr1;

  /* Status (rr0) must be read first */
  rr0 = sioregin(p->base, ZSCC_RR0);
  rr1 = sioregin(p->base, ZSCC_RR1);
  
  if (!(rr0 & ZSCC_RDA)) return;

  if (rr1 & (ZSRR1_FE | ZSRR1_DO | ZSRR1_PE)) {
    sioregout(p->base, ZSCC_RR0, ZSM_RESET_ERR);
  }

  ch = sioin(p->base);

  if ((p->tt_mode[MD_MODE3] & (FNKEYS|BINARY)) == FNKEYS) {
      if (p->tt_stat & STXFLG) p->tt_stat &= ~STXFLG;
      else {
    if ((ch&0x7f) == STX) p->tt_stat |= STXFLG;
    else goto ttinput;
      }
      ttoutq(p, STX);
  }

ttinput:        /* Put a character into the input or type ahead queue */

  if (!(p->tt_mode[MD_MODE3] & BINARY)) {
      switch (p->tt_mode[MD_MODE1] & (EVEN|ODD)) {
      case 0:
    ch &= 0x7f; break;
      case EVEN:
    if (partbl[ch&0x7f] != (ch & PARITY)) ttwarn(p);
    ch &= 0x7f; break;
      case ODD:
    if (partbl[ch&0x7f] == (ch & PARITY)) ttwarn(p);
    ch &= 0x7f; break;
      case EVEN|ODD:
    break;
      }
      if (!(p->tt_mode[MD_MODE1] & RAW)) {
    if (ch == X_OFF) {
	p->tt_stat |= OUTPSTOP; return;
    }
    if (ch == X_ON) {
	restartout(p); return;
    }
      } /* If not RAW */
      if (ch == CNTRL_C)
    if (p->tt_mode[MD_MODE2] & ABENABLE) {
	restartout(p);
	flush(&p->tt_inpq);
	flush(&p->tt_tahq);
	sgttysig(SIGABORT, majdevno|p->minnum);
	return;
    }
      if (ch == p->tt_mode[MD_USIGNAL])
    if (p->tt_mode[MD_MODE2] & SGENABLE) {
	sgttysig(SIGUSER, majdevno|p->minnum);
	if (!(p->tt_mode[MD_MODE2] & SIGALLC)) return;
    }
  }    /*  If not BINARY */

  if (p->tt_stat & RDWAIT) {
      p->tt_palnct = p->tt_mode[MD_LENGTH];
      if (!pputinpq(p, ch)) return;
      p->tt_stat &= ~RDWAIT;
      wakeup(tty_slpev|HmRDWAIT|p->minnum);
      return;
  }
  if (putq(&p->tt_tahq, ch) == ERR) {
      ttwarn(p); return;
  }
  if (p->tt_mode[MD_MODE3] & BINARY) return;
  if ((p->tt_mode[MD_MODE2] & (SIGALLC|SGENABLE)) == SIGALLC)
      sgttysig(SIGUSER, majdevno|p->minnum);
  if (p->tt_mode[MD_MODE1] & TANDEM) ckinhigh(p);
  if (p->tt_mode[MD_MODE2] & NOTIMMECHO) return;
  if (p->tt_stat & RDXFER) return;
  ch &= 0x7f;
  if (ch == '\r')
      if (p->tt_mode[MD_MODE1] & CRDEVICE) ch = '\n';
  echo_c(p, ch);
}


static ttoi(p)          /* Output interrupt service routine */
tty_str *p;
{
  extern void sioout();
  extern int getq();
  static int wrwake(),nttdelay();

  int ch, rb_off;

  if (p->tt_stat & (PUTSTOP | PUTSTART)) {  /* Input stopped */
    if (p->tt_stat & PUTSTART)
      ch = X_ON;
    else
      ch = X_OFF;
    p->tt_stat &= ~(PUTSTOP | PUTSTART);
  } else {
    if (p->tt_stat & (OUTPSTOP|OUTDELAY)) {     /* Output stopped */
done:
      sioregout(p->base, ZSCC_RR0, ZSM_RESET_TXINT);
      p->tt_stat &= ~INT_DUE;
      wrwake(p);
      return;
    } else {
      if (p->tt_klct) {           /* Erasing killed line */
	if ((rb_off = --p->tt_klseq) == 0) {
	  p->tt_klseq = RUBSEQLN;
	  p->tt_klct--;
	}
	ch = rbseq[rb_off];
      } else {                      /* Here we grab the character */
	if ((ch = getq(&p->tt_outq)) != ERR) {
      if (!(p->tt_mode[MD_MODE3] & BINARY))
	  switch (p->tt_mode[MD_MODE1] & (EVEN|ODD)) {
	  case 0:
	ch &= 0x7f; break;
	  case EVEN:
	if (partbl[ch&0x7f] != (ch & PARITY))
	    ch ^= PARITY;
	break;
	  case ODD:
	if (partbl[ch&0x7f] == (ch & PARITY))
	    ch ^= PARITY;
	break;
	  case EVEN|ODD:
	break;
	  }
	}
	else goto done;
    }
      }
  }
  sioout(p->base, ch);
  p->tt_stat |= INT_DUE;
  wrwake(p);

  switch ((char) (ch & 0x7f)) {
  case '\n' :
      nttdelay(p,NLDELAY,0); break;
  case '\t' :
      nttdelay(p,TABDELAY,2); break;
  case '\r' :
      nttdelay(p,CRDELAY,4); break;
  case '\f' :
      nttdelay(p,FFDELAY,1); break;
  case '\b' :
      nttdelay(p,BSDELAY,7); break;
  }
}


static int nttdelay(p,mask,cnt)
tty_str *p; int mask, cnt;
{
  extern callwd();

  if (cnt = (p->tt_mode[MD_MODED] & mask) >> cnt) {
      p->tt_stat |= OUTDELAY;
      callwd(&p->tt_wd,p->tt_dunit*cnt);
  }
}


static int wrwake(p)
tty_str *p;
{
  extern int wakeup();

  if ( (p->tt_stat & WRWAIT) && ((p->tt_outq.cb_cnt == TTOLOW)
     || (p->tt_outq.cb_cnt == 0)) ) {
      p->tt_stat &= ~WRWAIT;
      wakeup (tty_slpev | HmWRWAIT | p->minnum);
  }
}


/* ----------------------------------------------------------------------- */

/*
 *      Initialize TTY device
 */

static int stty_init(majdev, cnt, val)
unsigned char majdev;
int cnt, val[];
{
  extern void sioinit();
  extern int addauto();
  static int def_mode(),wd_restart();

  int i, m, b_port;
  tty_str *p;

  majdevno  = majdev << 8;
  tty_slpev = CHARDEV | majdev << 16;

  d_printf("stty init(%ld,%d)\r\n",
	      (unsigned long)majdev, (unsigned long)cnt);

  p = s_tty;
  for (i = 0; i < cnt; i++) {  /* Initialize all declared devices  */
    m = val[i] & 0x0F;
    if (m > PORT_A_MINOR) {
      d_printf("Invalid minor number, ignoring %ld\r\n", m);
      continue;
    }
    if (i >= MAXTTY) {
      d_printf("Too many devices, ignoring %ld\r\n", m);
      continue;
    }
      
    b_port = SIO_BASE_PORT + m;       /* Base port = 0xA0 + minor   */

    d_printf("stty init minor %ld, port 0x%lx\r\n", m, b_port);
    
    sp[m] = p;
    p->base = b_port;
    p->minnum = m;
    p->tt_inpq.cb_cnt = 0;
    p->tt_tahq.cb_cnt = 0;
    p->tt_outq.cb_cnt = 0;
    def_mode(p);
    p->tt_stat |= DISCARDED;
    p->tt_wd.func = wd_restart;
    p->tt_wd.arg  = (int) p;
    p->tt_dunit   = delaytbl[S_9600];
    p++;
  }

  /* Install interrupt handler */
  if (addauto(SIO_IRQ_LVL, stty_int, STACK)) return(ERR);

  /* Intitialise the SIO SCC */
  sioinit();

  return(0);
}


static int def_mode(p)              /* set default modes for the device */
tty_str *p;
{
  static int tuttout();

  p->tt_mode[MD_ISPEED]  = S_UNINIT;
  p->tt_mode[MD_MODE1]   = ECHO | CRDEVICE;
  p->tt_mode[MD_MODE2]   = PAUSE | ABENABLE;
  p->tt_mode[MD_DELECHO] = 'R';
  p->tt_mode[MD_LKILL]   = CNTRL_U;
  p->tt_mode[MD_LENGTH]  = PGLENGTH;
  p->tt_palnct           = PGLENGTH;
  p->tt_fflnct           = PGLENGTH;
}

/* ----------------------------------------------------------------------- */

/*
 *      Open TTY device
 */

static int stty_open(dev)
unsigned short dev;
{
  extern void sioopen();

  tty_str *p;
  int status, oldlev;

  d_printf("stty open(0x%lx)\r\n", (unsigned long)dev);

  if (p = sp[minor(dev) & 0xF]) {
    if (p->tt_opencnt++ == 0) {
      oldlev = setlev(SIO_IRQ_LVL);
      sioopen(p->base);
      sioenable(p->base, ZSCC_RDA|ZSCC_TBE);
      p->tt_stat &= ~DISCARDED;
      reslev(oldlev);
    }
    status = 0;
  }
  else {
    err = _nodevice;
    status = ERR;
  }
  return(status);
}



/* ----------------------------------------------------------------------- */

/*
 *      Close TTY device
 */

static int stty_close(dev)
unsigned short dev;
{
  extern void sioclose();
  static int ttflush(),tusshut();

  int oldlev;
  tty_str *p;

  d_printf("stty close(0x%lx)\r\n", (unsigned long)dev);

  p = sp[minor(dev) & 0xF];
  if (--p->tt_opencnt == 0 && (p->tt_mode[MD_MODE3] & DISCARD)) {
    oldlev = setlev(SIO_IRQ_LVL);
    ttflush(p);
    tusshut(p);
    sioclose(p->base);
    p->tt_stat |= DISCARDED;
    reslev(oldlev);
  }
  return(0);
}


static int ttflush(p) /* Flush all buffers (input, type-ahead and output) */
tty_str *p;
{
  static int flush();

  flush(&p->tt_inpq);
  flush(&p->tt_tahq);
  flush(&p->tt_outq);
}


static int flush(cbp)
cbuf *cbp;
{
  static int ngetq();

  while (ngetq(cbp) != ERR) ;
}


static int tusshut(p)
tty_str *p;
{
  extern int siodisable();

  siodisable(p->base, ZSCC_RDA|ZSCC_TBE);
}


/* ----------------------------------------------------------------------- */

/*
 *      Read from TTY device
 */

static int stty_read(dev, arg)
unsigned short dev;
char *arg;
{
  extern int sleep(),getq(),ioputc();
  static int ckinlow(),pputinpq(),ngetq();

  tty_str *p;
  int ch, oldlev, delim;

/*
  d_printf("stty_read(0x%lx)\r\n", dev);
*/
  
  p = sp[minor(dev) & 0xF];
  oldlev = setlev(SIO_IRQ_LVL);
  for (;;) {
    while (p->tt_stat&RDWAIT)
    if (sleep(tty_slpev|HmRDWAIT|p->minnum)) goto sigerr;
      if (p->tt_inpq.cb_cnt) break;
      do
    if ((ch = getq(&p->tt_tahq)) != ERR) {
	if (!(p->tt_mode[MD_MODE3] & BINARY) &&
      p->tt_stat & INPSTOP) ckinlow(p);
	p->tt_stat |= RDXFER;
	reslev(oldlev);
      delim = pputinpq (p, ch);
	setlev(SIO_IRQ_LVL);
	p->tt_stat &= ~RDXFER;
    }
    else {
	p->tt_stat |= RDWAIT;
	if (sleep(tty_slpev|HmRDWAIT|p->minnum))
      goto sigerr;
	break; /* goto beginning of the main loop */
    }
      while (!delim);

  }       /* main loop */
  reslev(oldlev);

  /* Emptying the input queue */
  while ((ch = ngetq(&p->tt_inpq)) != ERR) {
      if (! ((p->tt_mode[MD_MODE3] & (BINARY|CBREAK))
    || (p->tt_mode[MD_MODE1] & RAW)) && (ch == EOFILE)) break;
      if (ioputc(ch, arg)) break;
  }
  return(0);
sigerr:
  reslev(oldlev);
  d_printf("stty_read(0x%lx) return error 0x%lx\r\n", dev, _ssignal);
  err = _ssignal; return(ERR);
}


static int pputinpq(p, ch)              /* Returns TRUE if a delimiter */
	  /* is entered into queue */
tty_str *p;
int ch;
{
  static int ppidel(),ppikill(),nputq();
  static int ttwarn(),echo_c();

  int lch;

  if (!(p->tt_mode[MD_MODE3] & BINARY)) {
      lch = ch & ~PARITY;
      if (!((p->tt_mode[MD_MODE1]&RAW)||(p->tt_mode[MD_MODE3]&CBREAK))) {
    if ((lch==DEL)||(lch=='\b')||(lch==p->tt_mode[MD_ERASE])) {
	ppidel(p);  goto no_delim;
    }
    if (lch == p->tt_mode[MD_LKILL]) {
	 ppikill(p); goto no_delim;
    }
      }
      if ((lch == '\r') && (p->tt_mode[MD_MODE1] & CRDEVICE)) ch = '\n';
      if (p->tt_mode[MD_MODE1] & LCASE) ch = tolower(ch);
  }
  if (nputq(&p->tt_inpq, ch)) {   /* Queue is full */
      p->tt_stat |= INPOVFL;
      ttwarn(p);
      goto delim;                 /* Say, delimiter */
  }
  echo_c(p, ch);
  if ((p->tt_mode[MD_MODE1] & RAW) ||
      (p->tt_mode[MD_MODE3] & (BINARY | CBREAK))) goto delim;
  ch &= ~PARITY;
  if ((ch == '\n') || (ch == EOFILE) ||
      ((ch == ESC) && (p->tt_mode[MD_MODE3] & ESCRETN))) goto delim;

no_delim:
  return(0);
delim:
  return(1);
}


static int ppidel(p)
tty_str *p;
{
  static int nunputq(),ttwarn(),echo_c();

  int ch;

  if ((ch = nunputq(&p->tt_inpq)) == ERR) ttwarn(p);
  else {
      p->tt_stat |= DELETION;
      echo_c(p, ch);
      p->tt_stat &= ~DELETION;
  }
}



static int ppikill(p)
tty_str *p;
{
  static int nunputq(),echo_c(),echooutq();

  int cnt = 0;
  int ch;

  while ((ch = nunputq(&p->tt_inpq)) != ERR) {
      ++cnt;
      if (ch < ' ') ++cnt;
  }
  if (p->tt_mode[MD_MODE1] & ECHO) {
      if (p->tt_mode[MD_DELECHO] == 'R') {
    p->tt_klct = cnt;
    p->tt_klseq = RUBSEQLN;
    ch = 0;
      }
      else {
    echo_c(p, p->tt_mode[MD_LKILL]);
    ch = '\n';
      }
      echooutq(p, ch);
  }
  return(0);
}


static int ckinlow(p)   /* Check whether to send start character */
      /* (TANDEM mode) */
tty_str *p;
{
  static int ckoutput();

  if ((p->tt_tahq.cb_cnt == TTILOW) || (p->tt_tahq.cb_cnt == 0)) {
      p->tt_stat &= ~INPSTOP;
      p->tt_stat |= PUTSTART;
      ckoutput(p);
  }
}


static int ckinhigh(p)
tty_str *p;
{
  static int ckoutput();

  if (p->tt_tahq.cb_cnt >= TTIHIGH)
      if (!(p->tt_stat & INPSTOP)) {
    p->tt_stat |= (INPSTOP|PUTSTOP);
    ckoutput(p);
      }
}



static int echo_c(p, ch)
tty_str *p;
int ch;
{
  static int echoeras(),echooutq(),putecho();

  if (!(p->tt_mode[MD_MODE1] & ECHO)   ||
       (p->tt_mode[MD_MODE3] & BINARY) ||
       ((ch &= ~PARITY) == 0) )           return(0);

  if ((ch == DEL) || (ch == '\b') || (ch == p->tt_mode[MD_ERASE]))
      echoeras(p);
  else if (ch == '\n') {
      if (p->tt_mode[MD_MODE2] & NOECNL) return(0);
      echooutq(p, '\r');
      putecho(p, '\n');
  }
  else if (ch == '\r')
      echooutq(p, '\r');
  else if (ch < ' ') {
      putecho(p, '^');
      putecho(p, ch+'@');
  }
  else putecho(p, ch);

  return(0);
}

static int echooutq(p, ch)     /* Process & put an echo character */
	     /* into output queue */
tty_str *p;
int ch;
{
  static int pputoutq();

  p->tt_stat |= ECHOING;
  pputoutq(p, ch);
  p->tt_stat &= ~ECHOING;
}


static int echoeras(p)
tty_str *p;
{
  static int echooutq();

  if (p->tt_mode[MD_DELECHO] != 'R')
      echooutq(p, p->tt_mode[MD_DELECHO]);
  else {
      echooutq(p, '\b');
      echooutq(p, ' ');
      echooutq(p, '\b');
  }
}



static int putecho(p, ch)
tty_str *p;
int ch;
{
  static int echoeras(),putecho();

  if (p->tt_stat & DELETION)
      echoeras(p);
  else
      echooutq(p, ch);
}



/* ----------------------------------------------------------------------- */

/*
 *      Write to TTY device
 */

static int stty_write(dev, arg)
unsigned short dev;
char *arg;
{
  extern int iogetc(),sleep(),sgtest();
  static int pputoutq();

  tty_str *p;
  int oldlev, ch, slpret = 0;

  p = sp[minor(dev) & 0xF];
  while ((ch = iogetc(arg)) != ERR) {
    oldlev = setlev(SIO_IRQ_LVL);
    do {
      if (p->tt_stat & (WRITSTOP|OUTPSTOP)) {
	slpret = sleep(tty_slpev|HmSCWAIT|p->minnum);
	continue;
      }
      if (p->tt_stat & OUTDELAY) {
	slpret = sleep(tty_slpev|HmWDWAIT|p->minnum);
	continue;
      }
      if (p->tt_outq.cb_cnt > TTOHIGH) {
	p->tt_stat |= WRWAIT;
	slpret = sleep(tty_slpev|HmWRWAIT|p->minnum);
	continue;
      }
      break;
    } while (slpret == 0);
    reslev(oldlev);
    if (sgtest()) goto sigerr;
    pputoutq(p, ch);
  }
  return(0);
sigerr:
  d_printf("stty_write(0x%lx) return error 0x%lx\r\n",dev, _ssignal);
  err = _ssignal; return(ERR);
}


static int pputoutq(p, ch)              /* Process character and put it
	  /* into output queue  */
tty_str *p;
int ch;
{
  static int ttoutq(),pponl(),ppotab();
  static int ppocr(),ppoff(),ppbak();

  if (ch >= ' ') {                             /* "Normal" character */
      if ((p->tt_mode[MD_MODE3] & BINARY)      /* If RAW or BINARY   */
	|| (p->tt_mode[MD_MODE1] & RAW)) {
    ttoutq(p, ch); p->tt_colct++;
      }
      else {
    if (p->tt_mode[MD_WIDTH] == 0       /* or within margins   */
      || p->tt_colct < p->tt_mode[MD_WIDTH]) {
	ttoutq(p, ch); p->tt_colct++;
    }
    else {
	if (p->tt_mode[MD_MODE2] & WRAP) {  /* or if WRAP on  */
      pponl(p); ttoutq(p, ch); p->tt_colct++;
	}
	else if (p->tt_stat & ECHOING) {   /* or if it's echo */
      ttoutq(p, ch); p->tt_colct++;  /* DO OUTPUT IT    */
	}
    }
      }
  }
  else {                                      /* "Control" character */
      switch (ch) {
    case '\n':      pponl(p);     break;    /* Newline         */
    case '\t':      ppotab(p);    break;    /* Tab             */
    case '\r':      ppocr(p);     break;    /* Carriage return */
    case '\f':      ppoff(p);     break;    /* Form Feed       */
    case '\b':      ppbak(p);     break;    /* Backspace       */
    default  :      ttoutq(p,ch); break;    /* Other CNTRL ch  */
      }
  }
}


static int pponl(p)
tty_str *p;
{
  static int do_newline(),ppoff();

  if (do_newline(p))
      if (!(p->tt_mode[MD_MODE3] & BINARY) &&
    p->tt_fflnct <= p->tt_mode[MD_BMARGIN]) ppoff(p);
}


static int ppotab(p)                    /* Process TAB character         */
tty_str *p;
{
  static int ttoutq(),pponl(),pputoutq();

  int w;

  if (p->tt_mode[MD_MODE3] & BINARY) {
      p->tt_colct++; ttoutq(p, '\t');
  }
  else {
      if ((w = p->tt_mode[MD_WIDTH]) && (p->tt_colct+8 & ~7) > w)
    if (p->tt_mode[MD_MODE2] & WRAP) pponl(p);
    else return 0;
      if (p->tt_mode[MD_MODE1] & XTAB)
    do pputoutq(p, ' '); while (p->tt_colct & 0x07);
      else p->tt_colct = p->tt_colct+8 & ~7, ttoutq(p, '\t');
  }
}


static int ppocr(p)                     /* Process Carriage Return      */
tty_str *p;
{
  static int ttoutq();

  p->tt_colct = 0; ttoutq(p, '\r');
}


static int ppoff(p)
tty_str *p;
{
  static int ttoutq(),do_newline();

  if ((p->tt_mode[MD_MODE3] & BINARY) ||
      !(p->tt_mode[MD_MODE2] & XFF)) ttoutq(p, '\f');
  else while (do_newline(p));
  p->tt_colct = 0;
  p->tt_fflnct = p->tt_mode[MD_LENGTH];
}



static int ppbak(p)
tty_str *p;
{
  static int ttoutq();

  if (p->tt_colct-- == 0) {
      if (p->tt_palnct++ == p->tt_mode[MD_LENGTH]) {
    p->tt_palnct = 1;
    p->tt_colct  = p->tt_mode[MD_WIDTH-1];
      }
  }
  ttoutq(p, '\b');
}


static int do_newline(p)
tty_str *p;
{
  extern int sleep();
  static int ppocr(),ttwarn(),ttoutq();

  if (--p->tt_palnct == 0) {
      p->tt_palnct = p->tt_mode[MD_LENGTH];
      if ((p->tt_mode[MD_MODE2] & PAUSE) &&
    !(p->tt_mode[MD_MODE3] & BINARY) &&
    !(p->tt_mode[MD_MODE1] & RAW) &&
    !(p->tt_mode[MD_BMARGIN])) {
	if (p->tt_stat & ECHOING) p->tt_palnct = 1;
	else {
      p->tt_stat |= WRITSTOP;
      ppocr(p); ttwarn(p);
      sleep(tty_slpev | HmSCWAIT | p->minnum);
	}
      }
  }
  if (!(p->tt_mode[MD_MODE3] & BINARY)
  &&  (p->tt_mode[MD_MODE1] & CRDEVICE)) ppocr(p);
  ttoutq(p, '\n');
  p->tt_colct = 0;
  if (--p->tt_fflnct) return(1);          /* TOP not reached */
  else {
      p->tt_fflnct = p->tt_mode[MD_LENGTH];
      return(0);                          /* TOP reached     */
  }
}



static int ttwarn(p)    /* Ring the bell, unless */
      /* -ECHO, TANDEM, RAW or BINARY */
tty_str *p;
{
  static int echooutq();

  if ((p->tt_mode[MD_MODE1]&(ECHO|TANDEM|RAW)) == ECHO &&
      !(p->tt_mode[MD_MODE3] & BINARY)) echooutq(p, BELL);
}



static int ttoutq(p, ch)
tty_str *p;
int ch;
{
  static int nputq(),ckoutput();

  int r = OUT_RETRY;

  while (r--) {
      if (nputq(&(p->tt_outq), ch) == 0) break;
      if (p->tt_stat & ECHOING) break;
  }
  if (!(p->tt_stat & INT_DUE)) ckoutput(p);
}


static int restartout(p)
tty_str *p;
{
  extern int wakeup();
  static int ckoutput();

  if (p->tt_stat & (WRITSTOP|OUTPSTOP)) {
      p->tt_stat &= ~(WRITSTOP|OUTPSTOP);
      wakeup (tty_slpev | HmSCWAIT | p->minnum);
  }
  ckoutput(p);
}


static int wd_restart(p)
tty_str *p;
{
  extern int wakeup();
  static int ckoutput();

  d_printf("Watchdog has expired\r\n");
  if (p->tt_stat & OUTDELAY) {
      p->tt_stat &= ~OUTDELAY;
      wakeup (tty_slpev | HmWDWAIT | p->minnum);
  }
  ckoutput(p);
}


static int ckoutput(p)  /* Output a character from the output queue, */
      /* unless an output interrupt is due         */
tty_str *p;             /* Called only by TTOUTQ,                    */
      /* RESTARTOUT, CKINHIGH, CKINLOW             */
{
  static int ttoi();

  int oldlev;

  oldlev = setlev(SIO_IRQ_LVL);
  if (!(p->tt_stat & INT_DUE)) ttoi(p);
  reslev(oldlev);
}



/* ----------------------------------------------------------------------- */

/*
 *      Set mode on TTY device
 *      Store old value in *val (if applicable)
 *      Return number of bytes for Z80 or ERR.
 */

static int stty_setmode(dev, mode, val, mask)
unsigned short dev, mask;
int mode, *val;
{
  static int flush(),ttsetspeed(),ttfnkeys();
  static int restartout();

  tty_str *p;
  int retval, oldlev, bval, bbval;

  p = sp[minor(dev) & 0xF];
  bval = bbval = *val;
  if (mode & 0xFFFF0000) bval >>= 24, bbval >>= 16;
  mode &= 0xffff;
  switch (mode) {
  case MD_IFLUSH:
      flush(&p->tt_inpq);
      flush(&p->tt_tahq);
      *val = bval;
      retval = 1;
      break;
  case MD_ISPEED:
  case MD_OSPEED:
    retval = ttsetspeed(p,&bval);
    if (retval >= 0) *val = bval;
    break;
  case MD_FNKEYS:
      retval = ttfnkeys(p,&bval);
      if (retval >= 0) *val = bval;
      break;
  case MD_MODE1:
  case MD_MODE2:
  case MD_MODE3:
  case MD_MODE4:
      *val = p->tt_mode[mode];
      p->tt_mode[mode] = (~mask & *val) | (mask & bval);
      oldlev = setlev(SIO_IRQ_LVL);
      restartout(p);
      reslev(oldlev);
      retval = 1;
      break;
  case MD_MODED:
  case MD_ERASE:
  case MD_DELECHO:
  case MD_LKILL:
  case MD_USIGNAL:
  case MD_LENGTH:
  case MD_WIDTH:
  case MD_BMARGIN:
    *val = p->tt_mode[mode];
    p->tt_mode[mode] = bval;
    retval = 1;
    break;
  default:
    d_printf("stty setmode unknown mode %lx\r\n", mode);
    err = _badvalue;
    retval = ERR;
    break;
  }
  return(retval);
}



static int ttfnkeys(p, val)
tty_str *p;
int *val;
{
  static int ttoutq();

  int oldlev, oval;

  oval = p->tt_mode[MD_MODE3] & FNKEYS ? 0xff : 0;
  oldlev = setlev(SIO_IRQ_LVL);
  ttoutq(p, ESC); ttoutq(p, '.');
  if (*val) ttoutq(p,'9'), p->tt_mode[MD_MODE3] |= FNKEYS;
  else ttoutq(p,'8'), p->tt_mode[MD_MODE3] &= ~FNKEYS;
  reslev(oldlev);
  *val = oval;
  return(1);
}


static int ttsetspeed(p, val)   /* Set speed to value. Store old        */
tty_str *p;                     /* speed in *val. Return 1 or ERR       */
int *val;
{
  extern void sioout();
  extern int siospeed();
  static int tuconnect(),autobaud();
  static int ttflush();

  int retval, newspeed, index;

  if (tuconnect(p)) {
    if (*val & Sfl_AUTO)
      newspeed = autobaud(p) | Sfl_AUTO;
    else
       newspeed = siospeed(p->base,*val);

    if (newspeed == ERR) {
      retval = ERR;
    } else {
      *val = p->tt_mode[MD_ISPEED];
      p->tt_mode[MD_ISPEED] = newspeed;
      retval = 1;
      sioout(p->base, '\r');
      ttflush(p);
    }

    if ((index = newspeed & 0x7f) <= S_19200)
      p->tt_dunit = delaytbl[index];
  }
  else {
    d_printf("stty is not connected\r\n");
    ttflush(p);
    err = _notconn;
    retval = ERR;
  }
  return(retval);
}


static int tuconnect(p)
tty_str *p;
{
  extern int sioenable(), sioregin();

  int status, oldlev;

/*
  status = sioregin(p->base, ZSCC_RR_STATUS);

  if (status & ZSCC_RDA) {
*/
      oldlev = setlev(SIO_IRQ_LVL);
      sioenable(p->base, ZSCC_RDA);
      reslev(oldlev);
      return(1);
/*
  }
  else return(0);
*/
}



static int autobaud(p)          /* Determine baud rate. */
	/* Return speed code or ERR */
tty_str *p;
{
  extern void siobreak();
  extern int siospeed();
  extern int sioenable(),siodisable();
  static int baudchar();

  int s_mode3, rtr, speed, ch, oldlev;

  s_mode3 = p->tt_mode[MD_MODE3];
  p->tt_mode[MD_MODE3] |= BINARY;
  for (;;) {
    oldlev = setlev(SIO_IRQ_LVL);
    siodisable(p->base, ZSCC_RDA);
    siobreak(p->base, 200);
    sioenable(p->base, ZSCC_RDA);
    reslev(oldlev);

    for (rtr = 0; rtr < 10; rtr++) {
      for (speed = NSPEEDS-1; speed >= 0; speed--) {
	if (siospeed(p,speed) == ERR) continue;
	ttflush(p);
	if ((ch = baudchar(p)) == ERR) goto reterr;
	if ((ch = baudchar(p)) == ERR) goto reterr;
	if (ch == '\r' || ch == '\n') goto found;
      }
    }
  }
reterr:
  speed = ERR;
found:
  p->tt_mode[MD_MODE3] = s_mode3;
  return(speed);
}


static int baudchar(p)          /* Get a character. Return it or ERR    */
tty_str *p;
{
  extern int sleep(),getq();
  static int ttinrdy();

  int oldlev, ch;

  oldlev = setlev(SIO_IRQ_LVL);
  while (!ttinrdy(p)) {
    p->tt_stat |= RDWAIT;
    if (sleep(tty_slpev|HmRDWAIT|p->minnum)) goto sigerr;
  }
  ch = getq(p->tt_inpq.cb_cnt ? &p->tt_inpq : &p->tt_tahq);
  ch &= ~PARITY;
  reslev(oldlev);
  return(ch);
sigerr:
  reslev(oldlev);
  err = _ssignal;
  return(ERR);
}

/* ----------------------------------------------------------------------- */

/*
 *      Get mode from TTY device
 */

static int stty_getmode(dev, mode, val)
unsigned short dev;
int mode, *val;
{
  static int ttinrdy();

  tty_str *p;
  int retval;

  retval = 1;
  mode &= 0xffff;
  if (p = sp[minor(dev) & 0x0f]) {
      if (mode == MD_STATUS)
    *val = ttinrdy(p);
      else if (mode == MD_IDENT) *val = ID_TTY;
      else if (mode < MODELEN) *val = p->tt_mode[mode];
      else err = _badvalue, retval = ERR;
  }
  else err = _nodevice, retval = ERR;
  return(retval);
}


static int ttinrdy(p)
tty_str *p;
{
  return (p->tt_inpq.cb_cnt || p->tt_tahq.cb_cnt ? 1 : 0);
	  /* Bug in C compiler */
}



/*
  Temporary functions to disable interrupts
*/

static int ngetq(p)
cbuf *p;
{
  extern int getq();

  int oldlev;
  int ch;

  oldlev = setlev(SIO_IRQ_LVL);
  ch = getq(p);
  reslev(oldlev);
  return(ch);
}

static int nputq(p,ch)
cbuf *p; int ch;
{
  extern int putq();

  int oldlev, retval;

  oldlev = setlev(SIO_IRQ_LVL);
  retval = putq(p,ch);
  reslev(oldlev);
  return(retval);
}

static int nunputq(p)
cbuf *p;
{
  extern int unputq();

  int oldlev;
  int ch;

  oldlev = setlev(SIO_IRQ_LVL);
  ch = unputq(p);
  reslev(oldlev);
  return(ch);
}

