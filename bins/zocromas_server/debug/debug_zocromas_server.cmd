set print thread-events off
set breakpoint pending on
handle SIGPIPE nostop noprint
# break mas_logger.c:214
# break mas_thread_tools.c:205
#break mas_logger.c:258
# break mas_init.c:134
# break mas_in_thread_end
run
bt
# vi: ft=gdb
