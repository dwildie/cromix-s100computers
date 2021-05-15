% Compile C program
% -s means generate a single module
%

if x#1 = x-s goto single
 c #*
 if -err goto err
 jcode #1.i #1.obj
 if -err goto err
 convobj -mj #1
 goto exit
%single
 shift
 c #*
 if -err goto err
 jcode #1.i #1.obj
 if -err goto err
 convobj -smj #1
%exit
 del #1.i #1.scr #1.obj >* /dev/null
 exit 0
%err
 del #1.i #1.scr #1.obj #1.o68 >* /dev/null
 exit 1
