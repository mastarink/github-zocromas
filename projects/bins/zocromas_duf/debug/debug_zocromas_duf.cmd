set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
delete breakpoints
b src/duf_dir_scan.c:150
run
bt
