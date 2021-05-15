#include "iolib.h"
#include "macros.h"
#include "char.h"
#include "intio.h"
#include "error.h"
#include "modeequ.h"
#include "time.h"

static unsigned char intstack[512];     /* Autovector interrupt stack   */
#define STACK   (intstack + sizeof(intstack))

static int     clockadjust = 0;
static int     gmtoffset = 0;

static int tim_init();
static int tim_open();
static int tim_close();
static int tim_read();
static int tim_write();
static int tim_gmode();
static int tim_smode();

chd stime_c = {
	tim_init,
	tim_open,
	tim_close,
	tim_read,
	tim_write,
	tim_gmode,
	tim_smode
};

static int tim_init(majdev)
unsigned majdev;
{
  extern int addauto();
  extern int addint();
  static int tmstick();
  static int dummy();
  
  d_printf("tim_init(%ld)\r\n", majdev);

  /* Install IRQ handler */
  if (addauto(0x03, tmstick, STACK)) return(ERR);

  /* Add dummy hadler for FDC timer interrupts */
  addint(0xDF, dummy);
  addint(0xFF, dummy);
      
  return(0);
}

static int tim_open(devn)
unsigned devn;
{
  extern unsigned err;

  if (minor(devn) == 0) return(0);
  err = _nodevice;
  return(ERR);
}

static int tim_close(devn)
unsigned devn;
{
  return(0);
}

static int tim_read(devn,arg)
unsigned devn;
{
  extern unsigned err;

  err = _chnaccess;
  return(ERR);
}

static int tim_write(devn,arg)
unsigned devn;
{
  extern unsigned err;

  err = _chnaccess;
  return(ERR);
}

static int tim_gmode(devn,mode,val)
unsigned devn;
int mode, *val;
{
  extern unsigned err;
  int retval;

  switch (mode & 0xffff) {
  case 0:         *val = clockadjust, retval = 2; break;
  case 1:         *val = gmtoffset/3600, retval = 2; break;
  case MD_IDENT:  *val = ID_TIMER, retval = 1; break;
  default:        err = _badvalue, retval = ERR; break;
  }
  return(retval);
}

static int tim_smode(devn,mode,val,mask)
unsigned devn;
int mode, *val, mask;
{
  extern chd timer_c;
  extern unsigned err;
  int newval = *val;
        
  if (mode & 0xffff0000) newval >>= 16;
  switch (mode & 0xffff) {
  case 0:
    *val = clockadjust;
    clockadjust = newval;
    return timer_c.ch_setmode(devn,mode,val,mask);

  case 1:
    *val = gmtoffset/3600;
    gmtoffset = newval*3600;
    return timer_c.ch_setmode(devn,mode,val,mask);

  default:
    err = _badvalue;
    return(ERR);
  }
}

static int tmrtick()
{
  extern unsigned long curtime;
  static long mmsec = 0;
  static int adjustcount = 0;
 
  int old_adjust = adjustcount;
  
  mmsec += 500000;     /* Timer rate = 500,000 microsec */
  adjustcount += 500000;
  if (adjustcount >= 8640000) {
    adjustcount -= 8640000;
    mmsec += clockadjust;
  }
  if (mmsec < 1000000)
    return 0;
    
  mmsec -= 1000000;  /* Subtract one scond */
  curtime += 1;      /* increment curtime */

  return 0;
}

static int tmstick()
{
  static unsigned int tmrCount = 0;
  extern int tim_sched();   /* Kernel function        */

  /* 500 millisecond timer for clock */
  if (++tmrCount >= 30) {
    tmrtick();
    tmrCount = 0;
  }
  
  /* 16.667 ms timer for scheduler */
  return(tim_sched(tmrCount % 3 ? 17 : 16));
}

static int dummy() {
  return(0);
}
