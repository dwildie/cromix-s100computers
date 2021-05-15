%  This file is used to install the enclosed software on a Cromix disk.
%  To do this, boot up Cromix version 20 as a privileged user, mount this
%  disk onto a file, change directory to that file, and type "install"

echo 68000  C  software will be loaded into the directory /usr/pkg/cccd
echo with links from appropriate files to /cmd, /usr/lib, and
echo /usr/include

makd /usr/lib /usr/pkg /usr/pkg/cccd /usr/include >* /dev/null
chowner bin /usr/lib /usr/pkg /usr/pkg/cccd /usr/include

cptree -f . /usr/pkg/cccd
d /usr/pkg/cccd
chowner bin **
maklink -f clib.obj ccterrs /usr/lib
maklink -f cc.cmd /cmd
maklink -f **.h /usr/include

echo Finished installing 68000  C  software
