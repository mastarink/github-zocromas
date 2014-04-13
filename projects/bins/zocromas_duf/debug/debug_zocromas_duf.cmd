set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
delete breakpoints
b duf_sql.c:50
run
bt
