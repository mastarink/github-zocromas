set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
b src/duf_sqlite.c:164
b src/duf_action.c:70
run
bt
