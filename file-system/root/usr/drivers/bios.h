/* ----------------------------------------------------------

   Definitions for calling the BIOS routines
        
   Copyright (C) 2020 Damian Wildie
	   
   -------------------------------------------------------- */

#define BIOS_ENTRY 0xFC0010

typedef struct {
  int (*initialise)();
  int (*getStatus)();
  int (*readBlock)();
  int (*writeBlock)();
  int (*initConsole)();
  int (*outChar)();
  int (*inChar)();
} bios_entry;
