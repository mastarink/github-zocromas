# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint

# b mod_tree_print_uni.c:52
# b duf_evaluate_sccb
# b duf_print.c:732
# b duf_prepare_actions.c:147
# b duf_levinfo_updown.c:140
# b duf_pdi.c:180 if od==0
b duf_uni_scan.c:48

run
bt
