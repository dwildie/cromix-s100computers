% Generate Cromix.sys file
%
% Usage:
%
%       crogen [-dm] system_file [sysdef_file]
%

if x#1 = x-d goto option
if x#1 = x-m goto option
goto nooption

%option
 shift
%nooption
if x#1 = x goto howto

if "#2" = "" goto default
 sysdef config.o68 #2
 if -err exit 1
 goto linkit
%default
 sysdef config.o68 sysdef
 if -err exit 1
%linkit

rewind
if x#1 = x-d goto debug
if x#1 = x-m goto map

%nodebug
 link68 -r -qn -o #1.sys -y 0 crolib config -s /usr/drivers/s100lib -s iolib
 del #1.map
 goto common

%map
 shift
 link68 -r -q -o #1.sys -y 0 crolib config -s /usr/drivers/s100lib -s iolib
 goto common

%debug
 shift
 link68 -r -q -o #1.sys -y 0 crolib config -s /usr/drivers/s100lib -s debug -s iolib
 goto common

%common
 del config.o68
 access rewa #1.sys
 chowner bin #1.sys
 version -c #1.sys
 exit

%howto
 echo "Usage: crogen [-d] [-m] system_file [sysdef_file]"
 exit
