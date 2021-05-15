% Set or get current run level
%
if x#1 = x goto get
   echo #1 > /etc/level
   kill -1 1
   exit
%get
   echo -n "Current run level: "
   ty /etc/level
   exit
