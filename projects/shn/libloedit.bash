shn_gvimer_plus_bin ()
{
    local a c='-o'
    for a in "$@" ; do c+=" '$a'" ; done
    [[ $MSH_SHN_LIBEDIT_TRACE ]] && shn_msg ${MSHCMD_GVIMC:=/usr/bin/gvim} $c
#   echo "{${MSHCMD_GVIMC:=/usr/bin/gvim} $c}"
    eval ${MSHCMD_GVIMC:=/usr/bin/gvim} $c
}
shn_gvimer_plus_fuuid ()
{
    local file=$1
    shift
    local fuuid=$1
    shift
    local fline=$1
    shift
    if [[ $fline -eq 0 ]] ; then unset fline ; fi 
#20160118.112059    shn_msg "@@" shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
    shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
}
shn_gvimer_plus_resident ()
{
    local file=$1
    shift
    local fuuid=$1
    shift
    local fline=$1
    shift
    if [[ $fline -eq 0 ]] ; then unset fline ; fi 
#20160118.112059    shn_msg "@@" shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
    shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
}

