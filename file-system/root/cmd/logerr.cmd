% Ecc management
%
if "#1" = "" goto report
ecc on
ecc -t #1 >> /etc/msu_errs &
exit
%report
ty /etc/msu_errs
