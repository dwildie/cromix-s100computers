if #1x = x goto how_to
if #1 != '-l' goto not_link
shift

%lloop
if #1x = x goto linkit
c +c #1
if -err goto error
jcode #1.i
if -err goto error
shift
goto lloop
%linkit
rew
shift
jlinker +m +a +s -m#1.map #* -lsys -lc
exit

%not_link
%loop
if #1x = x exit
c +c #1
if -err goto error
jcode #1.i
shift
goto loop

%error
echo "Error(s) in compilation"
exit 1

%how_to
echo "Usage: CC [-l] <file> [<file ...]"
echo
