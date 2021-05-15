% Cursor commands
%
if '#1' = 'clear' goto clr_screen
if '#1' = 'curson' goto curs_on
if '#1' = 'cursoff' goto curs_off
if '#1' = 'wrapon' goto wrap_on
if '#1' = 'wrapoff' goto wrap_off
if '#1' = 'clock' goto hard_clock
if '#1' = 'msg' goto disp_msg
%syntax
 echo -e 'Usage: vdt {clear curson cursoff wrapon wrapoff clock msg [message]}'
 exit 1

%clr_screen
 echo -n 'E'
 exit

%curs_off
 echo -n 'q'
 exit

%curs_on
 echo -n 'r'
 exit

%wrap_on
 echo -n '.n1'
 exit

%wrap_off
 echo -n '.n0'
 exit

%hard_clock
 echo -n '.w--'
 exit

%disp_msg
 shift
 echo -n ';' #* '1'
 exit
