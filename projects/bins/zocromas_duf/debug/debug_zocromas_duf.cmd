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


watch global_status.pdilist
watch global_status.pdilist->next


# commands 1
# print duf_config->scn.pdi->pathinfo.levinfo[duf_config->scn.pdi->pathinfo.depth].itemname[0]
# end


# display &(duf_config->scn.pdi->pathinfo.levinfo[duf_config->scn.pdi->pathinfo.depth].itemname)
# display duf_config->scn.pdi->num_idstatements
# display &(duf_config->scn.pdi->idstatements[duf_config->scn.pdi->num_idstatements-1])



run

# display pdi.pathinfo->levinfo[d].itemname
# bt
# vi: ft=gdb
