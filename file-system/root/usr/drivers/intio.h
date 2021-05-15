/*
	Intenal IO description

	Modification History.

	Version Who             When            What

	40.00   Egon Zakrajsek  Aug 19, 1985    Original version
*/



typedef struct {
	unsigned char   m_control;              /* Memory control       */
	unsigned char   c_flush;                /* Cache flush control  */
	unsigned char   c_vi6;                  /* Clear VI 6           */
	unsigned char   l_control;              /* LED control          */
	unsigned char   port04;                 /* Unused               */
	unsigned char   port05;                 /* Unused               */
	unsigned char   port06;                 /* Unused               */
	unsigned char   port07;                 /* Unused               */
	unsigned char   port08;                 /* Unused               */
	unsigned char   port09;                 /* Unused               */
	unsigned char   port0a;                 /* Unused               */
	unsigned char   port0b;                 /* Unused               */
	unsigned char   port0c;                 /* Unused               */
	unsigned char   port0d;                 /* Unused               */
	unsigned char   port0e;                 /* Unused               */
	unsigned char   port0f;                 /* Unused               */

	unsigned char   port10;                 /* Unused               */
	unsigned char   port11;                 /* Unused               */
	unsigned char   port12;                 /* Unused               */
	unsigned char   port13;                 /* Unused               */
	unsigned char   port14;                 /* Unused               */
	unsigned char   port15;                 /* Unused               */
	unsigned char   port16;                 /* Unused               */
	unsigned char   port17;                 /* Unused               */
	unsigned char   port18;                 /* Unused               */
	unsigned char   port19;                 /* Unused               */
	unsigned char   port1a;                 /* Unused               */
	unsigned char   port1b;                 /* Unused               */
	unsigned char   port1c;                 /* Unused               */
	unsigned char   port1d;                 /* Unused               */
	unsigned char   port1e;                 /* Unused               */
	unsigned char   port1f;                 /* Unused               */

	unsigned char   c_hundred;              /* Counter - 1/100 s    */
	unsigned char   c_hour;                 /* Counter - hours      */
	unsigned char   c_minute;               /* Counter - minutes    */
	unsigned char   c_second;               /* Counter - seconds    */
	unsigned char   c_month;                /* Counter - month      */
	unsigned char   c_day;                  /* Counter - date       */
	unsigned char   c_year;                 /* Counter - year       */
	unsigned char   c_week;                 /* Counter - day of week*/
	unsigned char   a_hundred;              /* Alarm - 1/100 s      */
	unsigned char   a_hour;                 /* Alarm - hours        */
	unsigned char   a_minute;               /* Alarm - minutes      */
	unsigned char   a_second;               /* Alarm - seconds      */
	unsigned char   a_month;                /* Alarm - month        */
	unsigned char   a_day;                  /* Alarm - date         */
	unsigned char   a_year;                 /* Alarm - year         */
	unsigned char   a_week;                 /* Alarm - day of week  */

	unsigned char   c_intmask;              /* Clock - int mask     */
	unsigned char   c_command;              /* Clock - command      */
	unsigned char   port32;                 /* Unused               */
	unsigned char   port33;                 /* Unused               */
	unsigned char   port34;                 /* Unused               */
	unsigned char   port35;                 /* Unused               */
	unsigned char   port36;                 /* Unused               */
	unsigned char   port37;                 /* Unused               */
	unsigned char   port38;                 /* Unused               */
	unsigned char   port39;                 /* Unused               */
	unsigned char   port3a;                 /* Unused               */
	unsigned char   port3b;                 /* Unused               */
	unsigned char   port3c;                 /* Unused               */
	unsigned char   port3d;                 /* Unused               */
	unsigned char   port3e;                 /* Unused               */
	unsigned char   port3f;                 /* Unused               */
} intio;

#define declareio register intio *iop = (intio *) 0x80000000



#define c_intstatus c_intmask



/*
	Bit assignement for m_control
*/

#define M_FLUSH         0x01                    /* DMA flush Super or User */
#define M_FREEZE        0x02                    /* Freeze Cache         */                */
#define M_CACHE         0x04                    /* Cache enable         */
#define M_ROM           0x08                    /* ROM disable          */



/*
	Bit assignement for c_flush
*/

#define C_USER          0x01                    /* Flush user           */
#define C_SUPER         0x02                    /* Flush supervisor     */



/*
	Bit assignement for c_intmask
*/

#define M_ALARM         0x01                    /* Alarm enable         */
#define M_HUNDRED       0x02                    /* 1/100 s enable       */
#define M_TEN           0x04                    /* 1/10 s enable        */
#define M_SECOND        0x08                    /* 1 s enable           */
#define M_MINUTE        0x10                    /* 1 min enable         */
#define M_HOUR          0x20                    /* 1 hour enable        */
#define M_DAY           0x40                    /* 1 day enable         */



/*
	Bit assignement for c_intstatus
*/

#define I_ALARM         0x01                    /* Alarm interrupt      */
#define I_HUNDRED       0x02                    /* 1/100 s interrupt    */
#define I_TEN           0x04                    /* 1/10 s interrupt     */
#define I_SECOND        0x08                    /* 1 s interrupt        */
#define I_MINUTE        0x10                    /* 1 min interrupt      */
#define I_HOUR          0x20                    /* 1 hour interrupt     */
#define I_DAY           0x40                    /* 1 day interrupt      */
#define I_ENABLE        0x80                    /* Interrupt enable     */



/*
	Bit assignement for c_command
*/

#define C_FREQ          0x03                    /* Frequency mask       */
#define C_24HOUR        0x04                    /* 24 hour clock        */
#define C_RUN           0x08                    /* Run enable           */
#define C_INTENABLE     0x10                    /* Interrupt enable     */
#define C_TEST          0x20                    /* Test mode            */
