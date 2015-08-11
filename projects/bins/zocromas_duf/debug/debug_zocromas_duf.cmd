# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint


#b duf_path2db.c:199
#b duf_sccb_handle.c:180
# b duf_sccb_eval.c:111
# b duf_dir_scan2.c:67
# b duf_dirent_scan2.c:176
b duf_dirent_scan2.c:97
b duf_dirent_scan2.c:77
b duf_dirent_scan2.c:39
b mod_directories_uni.c:75

run
bt
