%  This procedure is used to install CDOS programs on a Cromix disk
 if '#1' = '' goto argerror
 if #1 = fda goto install
 if #1 = fdb goto install
 if #1 = fdc goto install
 if #1 = fdd goto install
 if #1 = sfda goto install
 if #1 = sfdb goto install
 if #1 = sfdc goto install
 if #1 = sfdd goto install
%argerror
 echo -e Wrong number of arguments
 echo -e 'Syntax: install fdx	where fdx = fda, fdb, ..., sfda, sfdb, ...'
 exit
%install
 del install2.cmd >* /dev/null
 cdoscopy #1 install.crx
 if -r install.crx goto continue1
%noinstall
 echo -e install.crx file not present on CDOS disk
 exit
%continue1
 ren install.crx install2.cmd
 if -e /usr/pkg goto continue2
%createpkg
 makd /usr/pkg
 chowner bin /usr/pkg
%continue2
 install2 #1
 del install2.cmd
 echo CDOS software from disk #1 now installed on this Cromix disk
