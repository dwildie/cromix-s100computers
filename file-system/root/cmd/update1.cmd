% Build new root device
%

if x#1 = x goto howto
mode -pause
mount #1 /drive

move -f /drive/etc/ce_env         /drive/etc/ce_env.old          >* /dev/null
move -f /drive/etc/group          /drive/etc/group.old           >* /dev/null
move -f /drive/etc/iostartup.cmd  /drive/etc/iostartup.cmd.old   >* /dev/null
move -f /drive/etc/login.cmd      /drive/etc/login.cmd.old       >* /dev/null
move -f /drive/etc/motd           /drive/etc/motd.old            >* /dev/null
move -f /drive/etc/passwd         /drive/etc/passwd.old          >* /dev/null
move -f /drive/etc/sh_env         /drive/etc/sh_env.old          >* /dev/null
move -f /drive/etc/startup.cmd    /drive/etc/startup.cmd.old     >* /dev/null
move -f /drive/etc/startup.msg    /drive/etc/startup.msg.old     >* /dev/null
move -f /drive/etc/termcaps       /drive/etc/termcaps.old        >* /dev/null
move -f /drive/etc/ttys           /drive/etc/ttys.old            >* /dev/null
move -f /drive/etc/welcome        /drive/etc/welcome.old         >* /dev/null
move -f /drive/gen/sysdef         /drive/gen/sysdef.old          >* /dev/null
move -f /drive/usr/unix/.startup.cmd /drive/usr/unix/.startup.cmd.old >* /dev/null
d /

cptree -vf / /drive
move -f /drive/etc/startup.msg.new /drive/etc/startup.msg

boot

%howto
echo "Usage: Update1 new_root_device"
exit 1
