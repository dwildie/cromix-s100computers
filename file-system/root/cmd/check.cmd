% Check file structures
%
if '#1' = '-s' goto salvage
 dcheck #1
 icheck #1
 exit
%salvage
 dcheck -s #2
 icheck -s #2
 boot
