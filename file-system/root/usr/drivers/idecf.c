/* ----------------------------------------------------------

   Block device driver for the S100computers.com ICE/CF card
     
   Copyright (C) 2020 Damian Wildie

   -------------------------------------------------------- */

#include "iolib.h"
#include "macros.h"
#include "event.h"
#include "buf.h"
#include "bufsize.h"
#include "device.h"
#include "bmodeequ.h"
#include "error.h"
#include "block.h"
#include "watchdog.h"
#include "iobuf.h"

#include "mbr.h"
#include "bios.h"
#include "disk.h"
#include "idecf.h"

static drive_info drives[IDE_DRIVES];
static bios_entry *bios = (bios_entry*)BIOS_ENTRY;
static unsigned char scratch[512];

/* --------------------------------------------------------
   idecf device function table.
   ------------------------------------------------------ */

static int init();
static int open();
static int close();
static int doio();
static int getmode();
static int setmode();

blk idecf_b = {
	0,
	0,
	init,
	open,
	close,
	doio,
	getmode,
	setmode
};

/* --------------------------------------------------------
   Init routine for IDE/CF
   Return zero. If it cannot be done return ERR.
   ------------------------------------------------------ */

static int init(majdev, cnt, val)
unsigned short majdev;
int cnt, val[];
{
  extern int d_printf();
  void print_mbr();

  int status, result, drive_id, bit;
  
  d_printf("idecf init(%ld,%d)\r\n",
	    (unsigned long)majdev, (unsigned long)cnt);

  status = bios->getStatus();
  if (status == 0) {
    status = bios->initialise();
  }
  
  for (drive_id = 0; drive_id < IDE_DRIVES; drive_id++) {
    if (status & (0x1 << drive_id)) {
      drives[drive_id].status = 0x1;
      result = bios->readBlock(drive_id, (int)0, scratch, (int)1);
      if (result) {
	d_printf("idecf disk %ld, read block 0, result = 0x%lx\r\n",
		 drive_id, result);
      } else {
	read_mbr(drive_id, scratch);
      }
    }
  }
  
  print_mbr(majdev);
  return(0);
}

/* --------------------------------------------------------
   Open routine for IDE/CF
   Return zero. In case of errors return ERR and error
   number in the buffer structure.
   ------------------------------------------------------ */

static int open(devn, mode)
unsigned short devn;
unsigned mode;
{
  extern int d_printf();
  int result;
  int drive_id = driveId(devn);
  int disk_id = diskId(devn);
  disk_info *info = &(drives[drive_id].disks[disk_id]);
  
  d_printf("idecf open(0x%lx,0x%lx)\r\n", 
	  (unsigned long)devn, (unsigned long)mode);

  if (info->dcb.dv_open == 0) {
    /* Needs to be initialised */
    result = bios->readBlock(drive_id, info->part.start, scratch, (int)1);
    read_geometry(&info->geom, scratch);

    info->dcb.dv_flag1 |= DF_CROMIX;
    info->dcb.dv_flag2 = D2_STDC;
    info->dcb.dv_flag3 = (info->dcb.dv_flag3 & D3_WRTPRO);
    if (mode & OM_MOUNT) info->dcb.dv_stat |= DS_MOUNT;
    info->dcb.dv_maxblk = info->geom.cylinders
			* info->geom.surfaces
			* info->geom.track_sectors;
  }
  info->dcb.dv_open++;
  return(0);
}

/* --------------------------------------------------------
   Close routine for IDE/CF.
   Return zero. In case of errors return ERR and error
   number in the buffer structure.
   ------------------------------------------------------ */
   
static int close(devn,mode)
unsigned short devn;
unsigned mode;
{
  extern int d_printf();
  int result;
  int drive_id = driveId(devn);
  int disk_id = diskId(devn);
  disk_info *info = &(drives[drive_id].disks[disk_id]);
    
  d_printf("idecf close(0x%lx,0x%lx)\r\n",
      (unsigned long)devn, (unsigned long)mode);

  if (info->dcb.dv_open) {
    info->dcb.dv_open--;
  }
        
  return(0);
}



/* --------------------------------------------------------
   Read/write routine for IDE/CF.
   Return zero. In case of errors return error number or
   zero and error number in the buffer structure.
   ------------------------------------------------------ */

static int doio(bp)
buf *bp;
{
  extern int d_printf();
  extern iodone();
  int lba, result;
  int drive_id = driveId(bp->bf_devn);
  int disk_id = diskId(bp->bf_devn);
  disk_info *info = &(drives[drive_id].disks[disk_id]);

/*
  d_printf("idecf doio() dev=0x%04lx, block=0x%lx\r\n",
	      (int)bp->bf_devn, (int)bp->bf_block);
*/
	      
  lba = info->part.start
	+ info->geom.start_cylinder
	  * info->geom.track_sectors
	  * info->geom.surfaces
	+ bp->bf_block;

  if (bp->bf_stat & BF_READ) {
/*    d_printf("idecf doio() read from 0x%lx\r\n", lba); */
    result = bios->readBlock(drive_id, lba, bp->bf_addr, (int)1);
  } else {
/*    d_printf("idecf doio() write to 0x%lx\r\n", lba); */
    result = bios->writeBlock(drive_id, lba, bp->bf_addr, (int)1);
  }

  iodone(bp);
  return(0);
}

/* --------------------------------------------------------
   Get mode routine for IDE/CF.
   Return Z80 indicator how to interprete
   results:
	  1       reg_D   = value & 0xff
	  2       reg_DE  = value & 0xffff
	  4       reg_DEHL= value
   In case of errors return ERR and error number in err.
   ------------------------------------------------------ */

static int getmode(devn, mode, val)
unsigned short devn;
int mode, *val;
{
  extern int d_printf();
  int retval = 2;
  int drive_id = driveId(devn);
  int disk_id = diskId(devn);
  disk_info *info = &(drives[drive_id].disks[disk_id]);
  dcb *dev = &info->dcb;
  
  switch(mode & 0xffff) {
  case BMD_VERSION:
    break;
    
  default:
    retval = bu_getmode(dev, mode, val);
    break;
  }
  
  return(retval);
}


/* --------------------------------------------------------
   Set mode routine for IDE/CF.
   Return Z80 indicator how to interprete
   results:
	  1       reg_D   = value & 0xff
	  2       reg_DE  = value & 0xffff
	  4       reg_DEHL= value
   In case of errors return ERR and error number in err.
   ------------------------------------------------------ */

static int setmode(devn,mode,val,mask)
unsigned short devn;
int mode, *val, mask;
{
  extern int d_printf();
  int retval = 2;
  int drive_id = driveId(devn);
  int disk_id = diskId(devn);
  disk_info *info = &(drives[drive_id].disks[disk_id]);
  dcb *dev = &info->dcb;
  
  d_printf("idecf setmode() %lx %lx\r\n", mode, *val);

  switch (mode & 0xffff) {
  default:
    retval = bu_setmode(dev, mode, val, mask);
    break;
  }
  
  return(retval);
}

/* --------------------------------------------------------
   Read the STDC disk geometry
   ------------------------------------------------------ */
int read_geometry(geom, sector)
disk_geometry *geom;
unsigned char *sector;
{
  geom->cylinders        = ((sector[0x68] & 0xFF) << 8) + (sector[0x69] & 0xFF);
  geom->track_sectors    = sector[0x6d] & 0xFF;
  geom->surfaces         = sector[0x6c] & 0xFF;
  geom->sector_bytes     = ((sector[0x6E] & 0xFF) << 8) + (sector[0x6F] & 0xFF);
  geom->start_cylinder   = ((sector[0x72] & 0xFF) << 8) + (sector[0x73] & 0xFF);
  geom->start_part_table = ((sector[0x76] & 0xFF) << 8) + (sector[0x77] & 0xFF);
  geom->start_alt_table  = ((sector[0x70] & 0xFF) << 8) + (sector[0x71] & 0xFF);
  geom->alt_track_cyl    = ((sector[0x74] & 0xFF) << 8) + (sector[0x75] & 0xFF);
  geom->alt_tracks       = ((sector[0x6A] & 0xFF) << 8) + (sector[0x6B] & 0xFF);
  

  d_printf("cylinders:      %ld\r\n", geom->cylinders);
  d_printf("sectors/track:  %ld\r\n", geom->track_sectors);
  d_printf("surfaces:       %ld\r\n", geom->surfaces);
  d_printf("bytes/sector:   %ld\r\n", geom->sector_bytes);
  d_printf("start cylinder: %ld\r\n", geom->start_cylinder);
  return(0);
}

/* --------------------------------------------------------
   Read the MBR partition table
   ------------------------------------------------------ */
int read_mbr(drive_id, sector)
int drive_id;
unsigned char *sector;
{
  int disk_id;
  for (disk_id = 0; disk_id < MBR_PARTITIONS; disk_id++) {
    int po = MBR_OFFSET + disk_id * MBR_ENTRY_SIZE;

    disk_info *info = &(drives[drive_id].disks[disk_id]);

    info->part.status  = sector[po + 0];
    info->part.type    = sector[po + 4];
    info->part.start   = le4(sector, po + 8);
    info->part.sectors = le4(sector, po + 12);
    info->dcb.dv_unit  = disk_id;
    info->dcb.dv_stat  = DS_BFSTEP | DS_VERIFY;
  }
  return(0);
}

/* --------------------------------------------------------
   ------------------------------------------------------ */
unsigned int abs_lba(drive_id, disk_id, block_id)
unsigned short drive_id;
unsigned short disk_id;
unsigned int block_id;
{
  partition *p  = &(drives[drive_id].disks[disk_id].part);
  return p->start + block_id;
}

/* --------------------------------------------------------
   ------------------------------------------------------ */
void print_mbr(majdev)
unsigned short majdev;
{
  char *getType();
  int drive_id, disk_id;

  d_printf("\r\nIDE/CF Drives:\r\n");
  
  for (drive_id = 0; drive_id < IDE_DRIVES; drive_id++) {
    if (drives[drive_id].status) {
      d_printf(" Drive %s   Start      Size       Type\r\n",
	       drive_id == 0 ? "A" : "B");
      
      for (disk_id = 0; disk_id < MBR_PARTITIONS; disk_id++) {
	partition *p  = &(drives[drive_id].disks[disk_id].part);
	if (p->type) {
	  d_printf("    [%ld,%ld] 0x%08lx 0x%08lx ",
		(unsigned long)majdev, disk_id + drive_id * 4,
		p->start, p->sectors);
	  d_printf(getType(p->type));
	  d_printf("\r\n");
	}
      }
    }
  }
  d_printf("\r\n");
}

static char *getType(type)
int type;
{
  switch(type) {
    case 0x00: return "Empty";
    case 0x01: return "FAT12";
    case 0x02: return "XENIX root";
    case 0x03: return "XENIX usr";
    case 0x04: return "FAT16 <32M";
    case 0x05: return "Extended";              
    case 0x06: return "FAT16";                 
    case 0x07: return "HPFS/NTFS";             
    case 0x08: return "AIX";                   
    case 0x09: return "AIX bootable";          
    case 0x0a: return "OS/2 Boot Manager";     
    case 0x0b: return "W95 FAT32";
    case 0x0c: return "W95 FAT32 (LBA)";       
    case 0x0e: return "W95 FAT16 (LBA)";
    case 0x0f: return "W95 Ext'd (LBA)";
    case 0x10: return "OPUS";
    case 0x11: return "Hidden FAT12";
    case 0x12: return "Compaq diagnostics";
    case 0x14: return "Hidden FAT16 <32M";
    case 0x16: return "Hidden FAT16";
    case 0x17: return "Hidden HPFS/NTFS";
    case 0x18: return "AST SmartSleep";
    case 0x1b: return "Hidden W95 FAT32";
    case 0x1c: return "Hidden W95 FAT32 (LBA)";
    case 0x1e: return "Hidden W95 FAT16 (LBA)";
    case 0x24: return "NEC DOS";
    case 0x39: return "Plan 9";
    case 0x3c: return "PartitionMagic recovery";
    case 0x40: return "Venix 80286";
    case 0x41: return "PPC PReP Boot";
    case 0x42: return "SFS";
    case 0x4d: return "QNX4.x";
    case 0x4e: return "QNX4.x 2nd part";
    case 0x4f: return "QNX4.x 3rd part";
    case 0x50: return "OnTrack DM";
    case 0x51: return "OnTrack DM6 Aux1";      
    case 0x52: return "CP/M";                  
    case 0x53: return "OnTrack DM6 Aux3";
    case 0x54: return "OnTrackDM6";
    case 0x55: return "EZ-Drive";
    case 0x56: return "Golden Bow";
    case 0x5c: return "Priam Edisk";
    case 0x5f: return "Cromix (ST)";
    case 0x61: return "SpeedStor";
    case 0x63: return "GNU HURD or SysV";      
    case 0x64: return "Novell Netware 286";
    case 0x65: return "Novell Netware 386";
    case 0x70: return "DiskSecure Multi-Boot";
    case 0x75: return "PC/IX";
    case 0x80: return "Old Minix";             
    case 0x81: return "Minix / old Linux";     
    case 0x82: return "Linux swap / Solaris";
    case 0x83: return "Linux";
    case 0x84: return "OS/2 hidden C: drive";
    case 0x85: return "Linux extended";
    case 0x86: return "NTFS volume set";
    case 0x87: return "NTFS volume set";
    case 0x88: return "Linux plaintext";
    case 0x8e: return "Linux LVM";
    case 0x93: return "Amoeba";
    case 0x94: return "Amoeba BBT";            
    case 0x9f: return "BSD/OS";                
    case 0xa0: return "IBM Thinkpad hibernation";
    case 0xa5: return "FreeBSD";               
    case 0xa6: return "OpenBSD";
    case 0xa7: return "NeXTSTEP";
    case 0xa8: return "Darwin UFS";
    case 0xa9: return "NetBSD";
    case 0xab: return "Darwin boot";
    case 0xaf: return "Mac HFS/HFS+";
    case 0xb7: return "BSDI fs";
    case 0xb8: return "BSDI swap";
    case 0xbb: return "Boot Wizard hidden";
    case 0xbe: return "Solaris boot";
    case 0xbf: return "Solaris";
    case 0xc1: return "DRDOS/sec (FAT-12)";
    case 0xc4: return "DRDOS/sec (FAT-16 < 32M)";
    case 0xc6: return "DRDOS/sec (FAT-16)";
    case 0xc7: return "Syrinx";
    case 0xda: return "Non-FS data";
    case 0xdb: return "CP/M / CTOS";     
    case 0xde: return "Dell Utility";          
    case 0xdf: return "BootIt";                
    case 0xe1: return "DOS access";            
    case 0xe3: return "DOS R/O";               
    case 0xe4: return "SpeedStor";             
    case 0xeb: return "BeOS fs";
    case 0xee: return "EFI GPT";               
    case 0xef: return "EFI (FAT-12/16/32)";    
    case 0xf0: return "Linux/PA-RISC boot";    
    case 0xf1: return "SpeedStor";
    case 0xf4: return "SpeedStor";             
    case 0xf2: return "DOS secondary";         
    case 0xfd: return "Linux raid autodetect"; 
    case 0xfe: return "LANstep";               
    case 0xff: return "BBT";                   

    default: return "?";
  }
}
