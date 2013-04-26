set print thread-events off
set breakpoint pending on
handle SIGPIPE nostop noprint
# break mas_channel.c:192
# break mas_io.c:304
# break mas_io.c:346
# break mas_io.c:299
# break mas_io.c:327
run --nohosts -Hzocromas.mastar.lan:5005 --nodaemon
bt
# vi: ft=gdb
