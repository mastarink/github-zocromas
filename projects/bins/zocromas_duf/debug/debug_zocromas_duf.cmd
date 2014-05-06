set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
b duf_insert_mime_uni
run
bt
