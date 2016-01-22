shn_gvimer_plus_uuid ()
{
    local fuuid
    local file
    local string
    local rp
    for file in $@
    do
        if [[ -n "$typf" ]] && [[ "$typf" == shn ]]; then
            rp=`realpath $file`
        else
            rp=`realpath -s $file`
        fi
        if [[ -n "$string" ]]; then
            string+=";$rp"
        else
            string="$rp"
        fi
    done
    echo -n 'shn_gvimer_plus-'
    echo -n "$( echo $string | md5sum | /bin/cut -b-32 )_"
}
