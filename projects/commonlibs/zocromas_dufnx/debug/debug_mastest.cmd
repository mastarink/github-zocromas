# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
set print elements 0
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
set breakpoint pending on

# b masxfs_pathinfo_base.c:68
# b masxfs_levinfo_scan.c:540
run

# display pdi.pathinfo->levinfo[d].itemname
# bt
# vi: ft=gdb
