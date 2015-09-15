# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
set print elements 0
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

b mod_dumplet.c:148
# b duf_ufilter_bind.c:66

run
bt
