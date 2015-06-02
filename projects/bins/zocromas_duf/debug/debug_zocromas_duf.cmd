# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

watch duf_config->cli.output.out
b duf_set_file_special
b duf_main
# b duf_sccb_handle.c:126

run
bt
