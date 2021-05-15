 echo "================================================================="
 echo
 echo -n "Do you want memory error correction turned on? (y/n) <n> "
 input | testinp - y >* /dev/null
 if -err goto no_ecc
   ecc on
%no_ecc
 echo
 echo -n "Do you want to download the Octart driver? (y/n) <n> "
 input | testinp - y >* /dev/null
 if -err goto no_oct
   /etc/ioload /etc/octart.iop io1
%no_oct
 echo
 echo
