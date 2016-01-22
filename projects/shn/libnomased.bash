shn_gvimer_plus_anywhere ()
{
  local file ser bfil fpath sl
  for file in $@ ; do
    sl=$(shn_gvimer_plus_bin --serverlist)
    for ser in $sl ; do
      if [[ -f $file ]] ; then
#       shn_msg "file:$file"
#	cwd=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "getcwd()" 2>/dev/null )
#       fpath="${cwd}/${file}"
	fpath=$(realpath $file)
	bfil=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "bufnr(\"^${fpath}$\")" 2>/dev/null )
#       shn_msg "1 fpath:$fpath bufnr:$bfil"
#	bfil1=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "bufnr(\"^${file}$\")" 2>/dev/null )
#       shn_msg "2 file:$file bufnr:$bfil1"
#	bfil1=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "bufnr(\"${file}$\")" 2>/dev/null )
#       shn_msg "3 file:$file bufnr:$bfil1"
	if [[ $bfil -ge 0 ]] ; then
	  echo $ser
#	  shn_msg "bufnr for '$fpath' : $bfil at server $ser"
	  return 0
	fi
      fi
#     shn_errmsg "no buffer:$file at server '$ser'"
    done
  done
  return 1
}
shn_gvimer_plus_nomased ()
{
    local file
    local fuuid0 fuuid fline
    fuuid0=$( shn_gvimer_plus_uuid $@ )
    for file in $@
    do
        fuuid=$(shn_gvimer_plus_anywhere $file)
#       shn_msg "nomased $file $fuuid (a/w)"
	[[ ${fuuid:=$fuuid0} ]]

        shn_gvimer_plus_regfile $file $fuuid ${fline:-0} || return $?
    done
    return 0
}

