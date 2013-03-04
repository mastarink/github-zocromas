set print thread-events off
set breakpoint pending on
handle SIGPIPE nostop noprint
# break minitest.c:170
#trace minitest.c:170

# break mas_channel.c:287
# break mas_channel.c:317
run
bt
# vi: ft=gdb
