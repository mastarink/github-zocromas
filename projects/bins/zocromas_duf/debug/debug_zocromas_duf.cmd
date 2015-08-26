# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

b duf_option_typed.c:533
b duf_ufilter.c:37

run
bt
