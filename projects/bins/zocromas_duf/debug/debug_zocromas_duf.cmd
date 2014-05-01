set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
b duf_levinfo.c:554
run
bt
