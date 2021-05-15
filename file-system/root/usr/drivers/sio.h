#define ZSCC_B_EXT_IP  0x01
#define ZSCC_B_TX_IP   0x02
#define ZSCC_B_RX_IP   0x04
#define ZSCC_A_EXT_IP  0x08
#define ZSCC_A_TX_IP   0x10
#define ZSCC_A_RX_IP   0x20

#define ZSCC_RDA       0x01
#define ZSCC_TBE       0x04
#define ZSCC_DCD       0x08
#define ZSCC_CTS       0x20

/*
 * Bits in Read Register 0 (`Transmit/Receive Buffer Status and External
 * Status').
 */
#define ZSRR0_BREAK             0x80    /* break/abort detected */
#define ZSRR0_TXUNDER           0x40    /* transmit underrun/EOM (sync) */
#define ZSRR0_CTS               0x20    /* clear to send */
#define ZSRR0_SYNC_HUNT         0x10    /* sync/hunt (sync mode) */
#define ZSRR0_DCD               0x08    /* data carrier detect */
#define ZSRR0_TX_READY          0x04    /* transmit buffer empty */
#define ZSRR0_ZERO_COUNT        0x02    /* zero count in baud clock */
#define ZSRR0_RX_READY          0x01    /* received character ready */

/*
 * Bits in Read Register 1 (the Zilog book does not name this one).
 */
#define ZSRR1_EOF               0x80    /* end of frame (SDLC mode) */
#define ZSRR1_FE                0x40    /* CRC/framing error */
#define ZSRR1_DO                0x20    /* data (receiver) overrun */
#define ZSRR1_PE                0x10    /* parity error */
#define ZSRR1_RC0               0x08    /* residue code 0 (SDLC mode) */
#define ZSRR1_RC1               0x04    /* residue code 1 (SDLC mode) */
#define ZSRR1_RC2               0x02    /* residue code 2 (SDLC mode) */
#define ZSRR1_ALL_SENT          0x01    /* all chars out of xmitter (async) */

/*
 * Bits in Read Register 3 (`Interrupt Pending').  Only channel A
 * has an RR3.
 */
#define ZSRR3_IP_A_RX           0x20    /* channel A recv int pending */
#define ZSRR3_IP_A_TX           0x10    /* channel A xmit int pending */
#define ZSRR3_IP_A_STAT         0x08    /* channel A status int pending */
#define ZSRR3_IP_B_RX           0x04    /* channel B recv int pending */
#define ZSRR3_IP_B_TX           0x02    /* channel B xmit int pending */
#define ZSRR3_IP_B_STAT         0x01    /* channel B status int pending */

/*
 * Bits in Write Register 1 (`Transmit/Receive Interrupt and Data
 * Transfer Mode Definition').  Note that bits 3 and 4 are taken together
 * as a single unit, and bits 5 and 6 are useful only if bit 7 is set.
 */
#define ZSWR1_REQ_WAIT          0x80    /* WAIT*-REQ* pin gives WAIT* */
#define ZSWR1_REQ_REQ           0xc0    /* WAIT*-REQ* pin gives REQ* */
#define ZSWR1_REQ_TX            0x00    /* WAIT*-REQ* pin follows xmit buf */
#define ZSWR1_REQ_RX            0x20    /* WAIT*-REQ* pin follows recv buf */
#define ZSWR1_RIE_NONE          0x00    /* disable rxint entirely */
#define ZSWR1_RIE_FIRST         0x08    /* rxint on first char & on S.C. */
#define ZSWR1_RIE               0x10    /* rxint per char & on S.C. */
#define ZSWR1_RIE_SPECIAL_ONLY  0x18    /* rxint on S.C. only */
#define ZSWR1_PE_SC             0x04    /* parity error is special condition */
#define ZSWR1_TIE               0x02    /* transmit interrupt enable */
#define ZSWR1_SIE               0x01    /* external/status interrupt enable */

/*
 * Bits in Write Register 3 (`Receive Parameters and Control').
 * Bits 7 and 6 are taken as a unit.  Note that the receive bits
 * per character ordering is insane.
 *
 * Here `hardware flow control' means CTS enables the transmitter
 * and DCD enables the receiver.  The latter is neither interesting
 * nor useful, and gets in our way, making it almost unusable.
 */
#define ZSWR3_RX_5              0x00    /* receive 5 bits per char */
#define ZSWR3_RX_7              0x40    /* receive 7 bits per char */
#define ZSWR3_RX_6              0x80    /* receive 6 bits per char */
#define ZSWR3_RX_8              0xc0    /* receive 8 bits per char */
#define ZSWR3_RXSIZE            0xc0    /* receive char size mask */
#define ZSWR3_HFC               0x20    /* hardware flow control */
#define ZSWR3_HUNT              0x10    /* enter hunt mode */
#define ZSWR3_RXCRC_ENABLE      0x08    /* enable recv crc calculation */
#define ZSWR3_ADDR_SEARCH_MODE  0x04    /* address search mode (SDLC only) */
#define ZSWR3_SDLC_SHORT_ADDR   0x02    /* short address mode (SDLC only) */
#define ZSWR3_SYNC_LOAD_INH     0x02    /* sync character load inhibit */
#define ZSWR3_RX_ENABLE         0x01    /* receiver enable */

/*
 * Bits in Write Register 4 (`Transmit/Receive Miscellaneous Parameters
 * and Modes').  Bits 7&6, 5&4, and 3&2 are taken as units.
 */
#define ZSWR4_CLK_X1            0x00    /* clock divisor = 1 */
#define ZSWR4_CLK_X16           0x40    /* clock divisor = 16 */
#define ZSWR4_CLK_X32           0x80    /* clock divisor = 32 */
#define ZSWR4_CLK_X64           0xc0    /* clock divisor = 64 */
#define ZSWR4_CLK_MASK          0xc0    /* clock divisor mask */
#define ZSWR4_SYNCMODE          0x00    /* no stop bit (sync mode only) */
#define ZSWR4_ONESB             0x04    /* 1 stop bit */
#define ZSWR4_1P5SB             0x08    /* 1.5 stop bits (clk cannot be 1x) */
#define ZSWR4_TWOSB             0x0c    /* 2 stop bits */
#define ZSWR4_SBMASK            0x0c    /* mask of all stop bits */
#define ZSWR4_EVENP             0x02    /* check for even parity */
#define ZSWR4_PARENB            0x01    /* enable parity checking */
#define ZSWR4_PARMASK           0x03    /* mask of all parity bits */

/*
 * Bits in Write Register 5 (`Transmit Parameter and Controls').
 * Bits 6 and 5 are taken as a unit; the ordering is, as with RX
 * bits per char, not sensible.
 */
#define ZSWR5_DTR               0x80    /* assert (set to -12V) DTR */
#define ZSWR5_TX_5              0x00    /* transmit 5 or fewer bits */
#define ZSWR5_TX_7              0x20    /* transmit 7 bits */
#define ZSWR5_TX_6              0x40    /* transmit 6 bits */
#define ZSWR5_TX_8              0x60    /* transmit 8 bits */
#define ZSWR5_TXSIZE            0x60    /* transmit char size mask */
#define ZSWR5_BREAK             0x10    /* send break (continuous 0s) */
#define ZSWR5_TX_ENABLE         0x08    /* enable transmitter */
#define ZSWR5_CRC16             0x04    /* use CRC16 (off => use SDLC) */
#define ZSWR5_RTS               0x02    /* assert RTS */
#define ZSWR5_TXCRC_ENABLE      0x01    /* enable xmit crc calculation */

/*
 * Bits in Write Register 9 (`Master Interrupt Control').  Bits 7 & 6
 * are taken as a unit and indicate the type of reset; 00 means no reset
 * (and is not defined here).
 */
#define ZSWR9_HARD_RESET        0xc0    /* force hardware reset */
#define ZSWR9_A_RESET           0x80    /* reset channel A (0) */
#define ZSWR9_B_RESET           0x40    /* reset channel B (1) */
#define ZSWR9_SOFT_INTAC        0x20    /* Not in NMOS version */
#define ZSWR9_STATUS_HIGH       0x10    /* status in high bits of intr vec */
#define ZSWR9_MASTER_IE         0x08    /* master interrupt enable */
#define ZSWR9_DLC               0x04    /* disable lower chain */
#define ZSWR9_NO_VECTOR         0x02    /* no vector */
#define ZSWR9_VEC_INCL_STAT     0x01    /* vector includes status */

/*
 * Bits in Write Register 11 (`Clock Mode Control').  Bits 6&5, 4&3, and
 * 1&0 are taken as units.  Various bits depend on other bits in complex
 * ways; see the Zilog manual.
 */
#define ZSWR11_XTAL             0x80    /* have xtal between RTxC* and SYNC* */
					/* (else have TTL oscil. on RTxC*) */
#define ZSWR11_RXCLK_RTXC       0x00    /* recv clock taken from RTxC* pin */
#define ZSWR11_RXCLK_TRXC       0x20    /* recv clock taken from TRxC* pin */
#define ZSWR11_RXCLK_BAUD       0x40    /* recv clock taken from BRG */
#define ZSWR11_RXCLK_DPLL       0x60    /* recv clock taken from DPLL */
#define ZSWR11_TXCLK_RTXC       0x00    /* xmit clock taken from RTxC* pin */
#define ZSWR11_TXCLK_TRXC       0x08    /* xmit clock taken from TRxC* pin */
#define ZSWR11_TXCLK_BAUD       0x10    /* xmit clock taken from BRG */
#define ZSWR11_TXCLK_DPLL       0x18    /* xmit clock taken from DPLL */
#define ZSWR11_TRXC_OUT_ENA     0x04    /* TRxC* pin will be an output */
					/* (unless it is being used above) */
#define ZSWR11_TRXC_XTAL        0x00    /* TRxC output from xtal oscillator */
#define ZSWR11_TRXC_XMIT        0x01    /* TRxC output from xmit clock */
#define ZSWR11_TRXC_BAUD        0x02    /* TRxC output from BRG */
#define ZSWR11_TRXC_DPLL        0x03    /* TRxC output from DPLL */

/*
 * Bits in Write Register 14 (`Miscellaneous Control Bits').
 * Bits 7 through 5 are taken as a unit and make up a `DPLL command'.
 */
#define ZSWR14_DPLL_NOOP        0x00    /* leave DPLL alone */
#define ZSWR14_DPLL_SEARCH      0x20    /* enter search mode */
#define ZSWR14_DPLL_RESET_CM    0x40    /* reset `clock missing' in RR10 */
#define ZSWR14_DPLL_DISABLE     0x60    /* disable DPLL (continuous search) */
#define ZSWR14_DPLL_SRC_BAUD    0x80    /* set DPLL src = BRG */
#define ZSWR14_DPLL_SRC_RTXC    0xa0    /* set DPLL src = RTxC* or xtal osc */
#define ZSWR14_DPLL_FM          0xc0    /* operate in FM mode */
#define ZSWR14_DPLL_NRZI        0xe0    /* operate in NRZI mode */
#define ZSWR14_LOCAL_LOOPBACK   0x10    /* set local loopback mode */
#define ZSWR14_AUTO_ECHO        0x08    /* set auto echo mode */
#define ZSWR14_DTR_REQ          0x04    /* DTR* / REQ* pin gives REQ* */
#define ZSWR14_BAUD_FROM_PCLK   0x02    /* BRG clock taken from PCLK */
					/* (else from RTxC* pin or xtal osc) */
#define ZSWR14_BAUD_ENA         0x01    /* enable BRG countdown */


#define ZSM_RESET_IUS   0x38    /* reset interrupt under service */
#define ZSM_RESET_ERR   0x30    /* reset error cond */
#define ZSM_RESET_TXINT 0x28    /* reset xmit interrupt pending */

#define SIO_IRQ_LVL     0x5
#define SIO_BASE_PORT   0xA0
#define SIO_B_CTRL      SIO_BASE_PORT
#define SIO_A_CTRL      (SIO_BASE_PORT + 0x1)
#define SIO_B_DATA      (SIO_BASE_PORT + 0x2)
#define SIO_A_DATA      (SIO_BASE_PORT + 0x3)
#define I8255_A         (SIO_BASE_PORT + 0x8)
#define I8255_B         (SIO_BASE_PORT + 0x9)
#define I8255_C         (SIO_BASE_PORT + 0xA)
#define I8255_CTRL      (SIO_BASE_PORT + 0xB)

#define I8255_CONFIG    0x98  /* A in, B out, Clo out, Chi in */

#define ZSCC_RR0       0x0
#define ZSCC_RR1       0x1
#define ZSCC_RR2       0x2
#define ZSCC_RR3       0x3

#define ZSCC_WR0      0x0
#define ZSCC_WR1      0x1
#define ZSCC_WR2      0x2
#define ZSCC_WR3      0x3
#define ZSCC_WR4      0x4
#define ZSCC_WR5      0x5
#define ZSCC_WR9      0x9
#define ZSCC_WR11     0xb
#define ZSCC_WR12     0xc
#define ZSCC_WR13     0xd
#define ZSCC_WR14     0xe

#define ZSCC_WR_CMD     0x0
#define ZSCC_WR_INT     0x1
#define ZSCC_WR_VEC     0x2
#define ZSCC_WR_RX      0x3
#define ZSCC_WR_PAR     0x4
#define ZSCC_WR_TX      0x5
#define ZSCC_WR_MIE     0x9
#define ZSCC_WR_CLK     0xb
#define ZSCC_WR_BLO     0xc
#define ZSCC_WR_BHI     0xd
#define ZSCC_WR_BRG     0xe

#define PORT_A_MINOR    1
#define PORT_B_MINOR    0

#define port(x) ((unsigned char *)(0xffff0000 + (x)))



