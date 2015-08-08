# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

b duf_sccb_eval.c:105
b duf_sccb_begfin.c:21

run
bt
