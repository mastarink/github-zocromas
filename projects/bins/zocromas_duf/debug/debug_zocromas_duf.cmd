# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

b duf_pdi_credel.c:25
b duf_pdi.c:107
b duf_maindb.c:499

run
bt
