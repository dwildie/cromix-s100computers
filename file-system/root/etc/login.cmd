% This command file will execute on each login
% before the user is given his Shell.
% The command file will execute with privileges
% of a privileged user.

mode -pause

if -r /etc/motd type /etc/motd
if -r /etc/cookie goto cookie
exit 0

%cookie
echo "Thought for the moment:"
echo
randline /etc/cookie
