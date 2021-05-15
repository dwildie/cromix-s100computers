%This command file rebuilds the super block on a Cromix disk
%call by:	fixsb devname

if '#1' = '' go howto
makfs -r #1
icheck -s #1
boot
exit

%howto
echo Call by: fixsb devname
exit 1
