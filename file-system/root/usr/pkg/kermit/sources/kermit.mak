kext.obj : kext.c kermit.h kext.c
 cc kext
kmain.obj : kmain.c kermit.h
 cc kmain
ksendrec.obj : ksendrec.c kermit.h
 cc ksendrec
kutil.obj : kutil.c kermit.h
 cc kutil
kwild.obj : kwild.c kermit.h
 cc kwild
kconnect.obj : kconnect.c kermit.h
 cc kconnect
kserver.obj : kserver.c kermit.h
 cc kserver
kshell.obj : kshell.c kermit.h
 cc kshell
library.obj : library.c kermit.h
 cc library
kermit.bin : kmain.obj kext.obj ksendrec.obj kutil.obj kwild.obj
kermit.bin : kconnect.obj kserver.obj kshell.obj library.obj
 link kmain kext ksendrec kutil kwild kconnect kserver kshell library
 move -f kmain.bin kermit.bin
 version -c kermit.bin
 access rewa.e.e kermit.bin
 chowner bin kermit.bin
 maklink -f kermit.bin ^

leapfrog.bin : leapfrog.c kermit.h
 cc leapfrog
 link leapfrog
 access rewa.e.e leapfrog.bin
 chowner bin leapfrog.bin
 maklink -f leapfrog.bin ^
