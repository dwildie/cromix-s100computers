% Shutdown system
%
msg -a </cmd/.shutdown.msg
ren /etc/.warning /etc/warning >*/dev/null
echo -n 'Time until system shutdown: 5 '; sleep 1
echo -n '4 '; sleep 1
echo -n '3 '; sleep 1
echo -n '2 '; sleep 1
echo -n '1 '; sleep 1
echo -n '0 '; sleep 1
echo; kill -2 1
