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


# b masxfs_levinfo_db.c:140
# b tree.c:111
#b masxfs_pathinfo_base.c:53
# b masxfs_levinfo_db_dir.c:462
b masxfs_levinfo_scan.c:257
# b masxfs_levinfo_scan.c:250
run

# display pdi.pathinfo->levinfo[d].itemname
# bt
# vi: ft=gdb
