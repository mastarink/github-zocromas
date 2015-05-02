# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
b duf_sccbh_scan.c:90
b duf_dir_scan2.c:62
run
bt
