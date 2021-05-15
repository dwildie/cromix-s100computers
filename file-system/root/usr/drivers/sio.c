/* ----------------------------------------------------------

   Low level tty routines for the S100computers.com 2 SIO card

   Copyright (C) 2020 Damian Wildie
	   
   -------------------------------------------------------- */
	      
	      
#include "iolib.h"
#include "modeequ.h"
#include "macros.h"
#include "sio.h"

#define msec_del 11     /* Multiplicative constant for delay loop */
#define min(p)          ((p) - SIO_BASE_PORT)

static int irq_status[2] = {0,0};
static unsigned short baud_tbl[] = {
  0x0000, /* 0  - S_HANGUP */
  0x0000, /* 1  - 50 baud */
  0x0000, /* 2  - 75 baud */
  0x0000, /* 3  - 110 baud */
  0x0000, /* 4  - 134.5 baud */
  0x03FE, /* 5  - 150 baud */
  0x0000, /* 6  - 200 baud */
  0x01FE, /* 7  - 300 baud */
  0x00FE, /* 8  - 600 baud */
  0x007E, /* 9  - 1200 baud */
  0x0000, /* 10 - 1800 baud */
  0x003E, /* 11 - 2400 baud */
  0x001E, /* 12 - 4800 baud */
  0x000E, /* 13 - 9600 baud */
  0x0000, /* 14 - External A */
  0x0000, /* 15 - External B */
  0x0006, /* 16 - 19200 baud */
  0x0002, /* 17 - 38400 baud */
};

/* --------------------------------------------------------
   Reset the Z8530, initialise each channel & enable irqs
   ------------------------------------------------------ */
int sioinit() {
  void initport();
  void sioregout();
  void i8255_init();
  
  i8255_init();

  sioregout(SIO_A_CTRL, ZSCC_WR9, ZSWR9_HARD_RESET);

  initport(SIO_A_CTRL);
  initport(SIO_B_CTRL);

  sioregout(SIO_A_CTRL, ZSCC_WR9, ZSWR9_MASTER_IE | ZSWR9_VEC_INCL_STAT);

  return 0;
}

void i8255_init() {
  *port(I8255_CTRL) = I8255_CONFIG;
  *port(I8255_C) = 0x0;
}

/* --------------------------------------------------------
   Initialise a Z8530 channel, Tx & Rx are disabled, 9600,N,8,1
   ------------------------------------------------------ */
static void initport(ctrl_port)
unsigned char ctrl_port;
{
  sioregout(ctrl_port, ZSCC_WR3,  ZSWR3_RX_8);
  sioregout(ctrl_port, ZSCC_WR5,  ZSWR5_TX_8);
  sioregout(ctrl_port, ZSCC_WR4,  ZSWR4_CLK_X16 | ZSWR4_ONESB);
  sioregout(ctrl_port, ZSCC_WR11, ZSWR11_RXCLK_BAUD | ZSWR11_TXCLK_BAUD);
  siospeed(ctrl_port, S_9600);
  sioregout(ctrl_port, ZSCC_WR14, ZSWR14_BAUD_ENA);
  sioregout(ctrl_port, ZSCC_WR1,  ZSWR1_RIE | ZSWR1_TIE);
}

/* --------------------------------------------------------
   Open the port, enable Tx & Rx in the Z8530 channel
   ------------------------------------------------------ */
void sioopen(ctrl_port)
unsigned char ctrl_port;
{
  void sioregout();

  sioregout(ctrl_port, ZSCC_WR3, (ZSWR3_RX_8 | ZSWR3_RX_ENABLE));
  sioregout(ctrl_port, ZSCC_WR5, (ZSWR5_DTR | ZSWR5_TX_8 | ZSWR5_TX_ENABLE | ZSWR5_RTS));
}

/* --------------------------------------------------------
   Close the port, disable Tx & Rx in the Z8530 channel
   ------------------------------------------------------ */
void sioclose(ctrl_port)
unsigned char ctrl_port;
{
  void sioregout();

  sioregout(ctrl_port, ZSCC_WR3,  ZSWR3_RX_8);
  sioregout(ctrl_port, ZSCC_WR5,  ZSWR5_TX_8);
}

/* --------------------------------------------------------

  ------------------------------------------------------ */
int sioenable(ctrl_port, mask)
unsigned char ctrl_port, mask;
{
/*
  if (mask & ZSCC_RDA)
    irq_status[min(ctrl_port)] &= 0x10;
  if (mask & ZSCC_TBE)
    irq_status[min(ctrl_port)] &= 0x02;
  sioregout(ctrl_port, ZSCC_WR_INT, irq_status[min(ctrl_port)]);
*/
}

/* --------------------------------------------------------

  ------------------------------------------------------ */
int siodisable(ctrl_port, mask)
unsigned char ctrl_port, mask;
{
/*
  if (mask & ZSCC_RDA)
    irq_status[min(ctrl_port)] &= ~0x10;
  if (mask & ZSCC_TBE)
    irq_status[min(ctrl_port)] &= ~0x02;
  sioregout(ctrl_port, ZSCC_WR_INT, irq_status[min(ctrl_port)]);
*/
}

/* --------------------------------------------------------
   Read from the specified Z8530 register
  ------------------------------------------------------ */
int sioregin(ctrl_port, reg)
unsigned char ctrl_port;
int reg;
{
  int d = *port(ctrl_port);
  if (!reg) return d;
  *port(ctrl_port) = reg & 0xf;
  return *port(ctrl_port);
}

/* --------------------------------------------------------
  Write to the specifed Z8530 register
  ------------------------------------------------------ */
void sioregout(ctrl_port, reg, value)
unsigned char ctrl_port;
int reg;
unsigned char value;
{
  if (reg) *port(ctrl_port) = reg & 0xf;
  *port(ctrl_port) = value & 0xff;
}

/* --------------------------------------------------------
   Set the port's baud rate
  ------------------------------------------------------ */
int siospeed(ctrl_port, code)
unsigned char ctrl_port;
int code;
{
  int baud;
  if (code == S_NOCHG) {
	d_printf("siospeed 0x%02x %d, no change\r\n", ctrl_port);
	return code;
  }
  if (code <= S_38400) {
	baud = baud_tbl[code];
	if (baud != 0) {
	  sioregout(ctrl_port, ZSCC_WR12, baud & 0xFF);
	  sioregout(ctrl_port, ZSCC_WR13, (baud >> 8) &0xFF);
	  d_printf("siospeed 0x%02X, %d, hi 0x%02X, lo 0x%02X\r\n",
	      ctrl_port,code, (baud >> 8) &0xFF, baud & 0xFF);
	  return code;
	}
  }
  d_printf("siospeed 0x%02X, %d, error\r\n", ctrl_port);
  return ERR;
}

/* --------------------------------------------------------
   Send a break
  ------------------------------------------------------ */
void siobreak(duration)
int duration;
{
  static void delay_lp();
  /* TODO send break Bit D4 of WR 1 */
  delay_lp(duration);
  /* TODO clear break */
}

/* --------------------------------------------------------
   Input a character from the data port
  ------------------------------------------------------ */
int sioin(ctrl_port)
unsigned char ctrl_port;
{
  return *port(ctrl_port+2);
}

/* --------------------------------------------------------
   Output a character to the data port
  ------------------------------------------------------ */
void sioout(ctrl_port, ch)
unsigned char ctrl_port;
char ch;
{
  *port(ctrl_port+2) = ch;
}


static void delay_lp(n)
int n;
{
  int i;
  for (i = 0; i < n * msec_del; i++) ;
}

