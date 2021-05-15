% Update current root from Ftar Cromix
%

if x#1 = x goto howto
d /
mode -pause
ftar -vx /dev/#1
echo "Run Update2 command on the next Ftar floppy"
echo "If this was the last floppy, please execute the Boot command"
mode pause
exit

%howto
echo "Usage: Update2 floppy_name"
exit 1
