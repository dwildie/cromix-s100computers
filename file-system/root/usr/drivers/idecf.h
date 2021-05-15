/*
	S100 IDE Controller structures and defines
	14 December, 2020
*/


#define STDCINTR        0x10            /* interrupt on done            */

#define NSTDC           2               /* number of drives/controller  */

#define STDCUNIT(drive,part)  ((drive) << 5 | (part))

#define MAXCTL          1               /* Maximum number of controllers */

struct stdc_dma {
    unsigned char       *portnum;               /* STDC command port            */
};

#define driveId(devn) (((devn) >> 2) & 0x1)
#define diskId(devn)  ((devn) & 0x3)
