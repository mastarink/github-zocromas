# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
# b duf_sqlite.c:380
# b duf_service.c:394
# b mod_dir_print_uni.c:183
b duf_begfin.c:61
run
bt
