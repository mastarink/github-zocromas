# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
set print elements 0
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

b mod_dialog.c:149
b duf_pdi_reinit.c:72

run
bt
