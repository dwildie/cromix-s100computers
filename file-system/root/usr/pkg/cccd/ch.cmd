/usr/pkg/cccd/c #1
if -err exit
/usr/pkg/cccd/code #1.i #1.obj
if -err exit
del #1.i #1.scr
/usr/pkg/cccd/crolinker #1 /usr/pkg/cccd/clib
del #1.obj
