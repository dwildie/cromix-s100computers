/*
  Raw console driver for the S100 68030 BIOS

  Copyright (C) 2020 Damian Wildie
*/


#include "iolib.h"
#include "macros.h"
#include "raw.h"
#include "bios.h"

static bios_entry *bios = (bios_entry*)BIOS_ENTRY;

/*
	Raw Console function table
*/

static int con_init(), con_in(), con_out();

raw rawcon = {
  con_init,
  con_in,
  con_out
};

/*
  Initialize console.
*/

static int con_init()
{
  static int con_out();
  bios->initConsole();
  con_out('\r');
  return(0);
}

/*
  Get a character from the raw console.
*/

static int con_in()
{
  return bios->inChar();
}

/*
  Output a character on the raw console.
*/

static int con_out(c)
int c;
{
  bios->outChar(c & 0x7f);
  return(0);
}

