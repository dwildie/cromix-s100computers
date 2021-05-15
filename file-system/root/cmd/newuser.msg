
XPU Cromix-Plus    Release 162   Oct 07, 1987

To run Cromix-Plus, you need the following minimal hardware:

	- DPU or XPU with either 68000 or 68010 processor
	- 512K of memory
	- 16FDC or 64FDC controller
	- One floppy drive
	- One terminal hooked to FDC controller

Additional supported hardware includes

	- TUART terminals
	- Octarts and/or IOP/quadarts
	- Octart or IOP terminals
	- Parallel printers on PRI or TUART
	- 3355 printers
	- Serial printers on FDC and TUARTS, X-ON/X-OFF protocol,
	  ETX/ACK protocol, CLQ printers
	- Serial printers on Octart or IOP, X-ON/X-OFF protocol,
	  ETX/ACK protocol
	- Floating point processor
	- Nine track magnetic tape
	- CNET hardware
	- Large and small floppy drives
	- Uniform style floppies
	- RAM disks
	- STDC hard disks
	- Floppy tape
	- WDI-II hard disks
	- XMM memory management unit
	- Z80 processor on XPU/DPU and in BIART, OCTART, and IOPX boards
	- SCSI tape on ESDC
	- ESDI hard disks on ESDC


Cromix-Plus is bootable "as is". It is a minimally configured Cromix Operating
System. If you want to support additional devices, proceed as follows:

First, make a backup copy of the Cromix-Plus disk.

Make /gen the current directory, and copy the file sysdef to another file
in the same directory. (Give the new file a suitable filename.) Edit the copy
to define the drivers to be included in your Cromix-Plus Operating System.
The sysdef file has a lot of comments in it, so it should be obvious how to
edit the file. If you do not understand a particular part of the file, don't
change it. Refer to the on-line manual (help sysdef) for detailed information.

Generate a new operating system:

	crogen output_file [sysdef_file]

Typically, "output_file" should be cromix (in the /gen directory). If
"sysdef_file" is not specified, the file gen/sysdef will be used. After
verifying that the new operating system is bootable, move it to the root
directory (/) to overwrite the distributed cromix.sys.

If necessary, edit the file /etc/iostartup.cmd to ensure downloading of
Octarts, IOPs, or BIARTs (set up to download STDC).

If necessary, make a link from an existing terminal to /dev/console (set up
for tty1). Note that the raw console will stay hooked to the FDC terminal even
if you move the /dev/console to some other terminal.

The /dev/console terminal is the terminal to be used as the standard
input/output during the startup.cmd execution (date/time) and, occasionally,
by other programs. However, you can log in normally on the tty1 terminal.

Edit the /etc/ttys file to enable the necessary terminals. Presently only
tty1 is enabled.

Now, you can boot the operating system by resetting the computer. As an
alternative, you can execute the following commands from your Cromix 20 system:

	mount fd? /drive
	/drive/bin/boot68 /drive/cromix

The Cromix-Plus System should ask for the root device. When you answer all
prompts, the booting will proceed through the login message.

You can reboot the Cromix-Plus System normally. The file cromix.sys is the
Cromix system file in a form that the Boot program understands. There is no
way to boot earlier Cromix operating systems from a Cromix-Plus System.

Cromix-Plus version 31.38 contains full memory protection using the XMM
board (if one is present in the system). As long as XMM is enabled (see
Mode amem) no user program can crash the system. Any intended violation
will cause a Bus error to be generated and the user program will be
aborted.

A note of warning. Version 02.45 of paslib.obj (and earlier) has an innocent
bug which causes all programs linked with this (and earlier) library to crash
immediately. Version 02.46 of paslib.obj has this bug eliminated.


If you are using PERSCI floppy drives, you should reference them as minor
device numbers, instead of the usual 0 .. 3:

	fda     16
	fdb     17
	fdc     18
	fdd     19

When asked for the root device, respond with 1,16 (instead of the usual 1,0).
After booting is complete, use the device names dfda, dfdb, dfdc, dfdd instead
of the usual fda, fdb, fdc, and fdd. Later, you can delete the nonexisting
devices and rename existing devices.

A uniform-style floppy disk is a floppy with all tracks in the same format
and all sectors the same size. Sector size might be 128, 256, or 512 bytes.
Minor device number describes the physical characteristics of the device.
Compute the minor device number as

	unit + small + dtrack + dual + sside + sdens

where

	unit   =  0, 1, 2, or 3 for A, B, C, D
	small  =  4 if 5", zero if 8"
	dtrack =  8 if double tracked (not supported), zero otherwise
	dual   = 16 if drives in pairs (PERSCI), zero if not
	sside  = 32 if single sided, zero if double sided
	sdens  = 64 if single density, zero if double density

The /dev directory contains no predefined device names for uniform-style
floppies. Use initflop to initialize uniform-style floppies.

Most new utilities are true 68000 programs. Their version number is always
30.xx. They do not need to execute under Cromix 20 or Cromix 11 series. New
utilities are, as a rule, used in the same way as old ones. There are,
however, some exceptions (refer to the Help utility).

New utilities use options according to the proposed standard. Notable
highlights are:

	- Options are one character
	- Unknown options mean errors
	- Options without an argument can be grouped together
	- If there is no input file given stdin is taken instead
		(if applicable)
	- Isolated dash means explicit stdin (if applicable)
	- Options must be written before other arguments




Important notes.

1. Cromix-Plus cannot use the STDC unless it contains 01.20 firmware. If your
   STDC does not have this version of firmware you should boot in two steps:

	Step 1. Make floppy the root device to boot Cromix-Plus on floppy.
		The iostartup.cmd will download the correct version of
		STDC firmware.
	Step 2. Reboot Cromix-Plus to hard disk.


2. Cromix-Plus has a number of important improvements. In particular, it does
   not write out system buffers to the disk as often as before, thereby
resulting in greater speed. Specifically, the system buffers are written to the
disk only when

	- Sync Shell command executes _update system call, or
	- Some user program executes _update system call, or
	- The system is idle for at least 4 seconds
	  (this number can be changed using Mode timer), or
	- Flush utility executes _update system call, or
	- Some user logs out, or
	- A device is unmounted (only that device), or
	- A device is closed by a utility (e.g. dcheck), or
	- During shutdown

As the kernel will flush buffers after four seconds of inactivity
there is no need to run the Flush utility in the backgroud.

If the system is reset, intentionaly or unintentionaly, without the system
buffers being flushed, the file structure may not be up to date or is even
incorrect.


3. The management of system buffers was greatly reorganized. If the user has
   enough memory available the number of system buffers should be increased
(with 600 buffers the system performance is just great). This will result in
greater speed (and bigger damage if the buffers are not flushed at reset or
power failure).


4. Do not pull a floppy out of the device without unmounting it. If you
   happened to do it nevertheless, put the same floppy back and with some
luck no harm will be done.


5. Shell retype capability is always enabled. It has been greatly reorganized.
   Each interactive Shell now keeps a 4K buffer to store the commands that
   were executed. The CNTRL-R key retypes the last command. If an integer
   is typed before pressing CNTRL-R the command with that number will
   overwrite the command number. The History command will write out all
   stored commands. The Shell prompt is now the Login name followed by the
   command number in brackets.



Release 144 brought new character drivers. These are

	utty        FDC and TUART terminal driver
	ulpt        Parallel printer driver for PRI and TUART
	uslpt       Serial printer driver for FDC and TUART
	otty        Octart terminal driver
	oslpt       Octart serial printer driver
        
The old drivers, tty, lpt, slpt, qtty, and qslpt, are still provided.
The new drivers are compatible with old ones except that Octart
minor device numbers are contiguous:

	 0 ..  7    Octart 1
	 8 .. 15    Octart 2
	16 .. 23    Octart 3

and so on.


CLQ printers are supported by slpt, uslpt, and oslpt drivers. To use the
slpt or uslpt driver, the CLQ signal cable must be built as follows:

	TUART or PRI        CLQ
        
	      2 <-----------> 4
	      3 <-----------> 3
	      7 <-----------> 7
	      
See the comments in the /gen/sysdef file for the minor device numbers
to be used with CLQ printers run from slpt and uslpt drivers.

The oslpt (but not qslpt) driver can also support CLQ printers. To use
a CLQ printer from oslpt driver the signal cable must be constructed
as follows:

	OCTART          CLQ
        
	  3 <-----------> 3
	  4 <-----------> 4
	  7 <-----------> 7

With such a signal cable a CLQ printer can be used as an XON/XOFF
printer provided the following modes are selected for the CLQ device:

	hand            to let the octart listen to the handshaking signals
	ff              to let the driver expand FF character
	crignore        to allow for unusual handling of CR by CLQ


New printer drivers (ulpt, uslpt, oslpt) support mode PAUSE to handle
single sheet paper. With mode PAUSE set, the printer will stop after each
page (LENGTH lines). You can insert a new sheet of paper and issue the

	mode prt resume

command to resume printing.


As some terminals do not want to send the XON character if the terminal
did not send the XOFF character beforehand, character drivers have been
modified so that any character will restart output after the terminal
output was stopped bue to mode PAUSE.
