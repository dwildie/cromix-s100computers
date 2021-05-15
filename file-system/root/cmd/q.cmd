 d /usr/query
 if '#1' = '-s' goto s_option
 if '#1' = '' goto no_arg
%query_match
 match "#1" query_data
 shift; if '#1' != '' goto query_match
 exit

%s_option
 shift
 if '#1' = '' goto no_arg_s
%system_match
 match "#1" sys_data; echo
 match "#1" jsys_data; echo
 match "#1" mode_data; echo
 match "#1" bmode_data; echo
 match "#1" tmode_data; echo
 match "#1" query_data
 shift; if '#1' != '' goto system_match
 exit

%no_arg
 echo 'Syntax: query [-s] [string]'
 echo 'Contents of query data file:'; echo
 ty query_data
 exit

%no_arg_s
 echo 'Syntax: query [-s] [string]'
 echo 'Contents of system call and query data files:'; echo
 ty sys_data; echo; echo
 ty jsys_data; echo; echo
 ty mode_data; echo; echo
 ty bmode_data; echo; echo
 ty tmode_data; echo; echo
 ty query_data
