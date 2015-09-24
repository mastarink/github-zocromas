# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
set print elements 0
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

# watch duf_config->targ.argc
watch duf_dbgfunlevel

run
bt
