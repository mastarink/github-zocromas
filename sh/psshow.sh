#!/bin/sh
export PS_FORMAT=tt,start,user,ppid,sid,pid,lwp,stat,s,%cpu,%mem,vsz,sz,rss,nlwp,comm,cmd
# bsdstart,tty,ni,user,ppid,pid,lwp,%cpu,%mem,stat,rss,vsz,s,sz,thcount,fname,cmd
/bin/ps ww  -L --sort -pcpu,pid -Czoclient,zocromas_server,zocchild,zocmaster | cut -b-150 | sed -ne 's/$/  .../p'

