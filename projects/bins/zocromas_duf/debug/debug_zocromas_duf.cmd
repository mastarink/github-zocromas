set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
b src/duf_path.c:132
run
bt
