  mode amem xmm                   % Turn ON XMM (if present).
  mode timer gmt 0                % Offset (hours) from GMT time
%  mode tty1 delay 1               % To prevent C-10 missing characters
				  % There is no need to run Flush anymore
  time -s
% /etc/cron &                     % Start Cron Daemon (detached)
  init 1                          % Initial run level
  ty /etc/startup.msg
  if -r /etc/.warning ty /etc/.warning
  ren /etc/warning /etc/.warning >*/dev/null

