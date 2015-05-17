# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

# b mod_tree_print_uni.c:52
b duf_evaluate_sccb

run
bt
