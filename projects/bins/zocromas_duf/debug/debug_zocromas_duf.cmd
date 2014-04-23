set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
bt
