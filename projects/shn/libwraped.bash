shn_gvimer_plus_regfile_in ()
{
    local file=$1
    shift
#   local fuuid=${1:-"$( shn_gvimer_plus_uuid $file )"}
    local fuuid=$1
    shift
    local fline=$1
    shift
    local masedf=$1
    shift
    local typf=$1
    shift
    local filen=`basename $file`
    local resident
    local fpath=$(realpath $file)
#   shn_msg "(libedit $MSH_SHN_LIBEDIT_LOADED) editing '$file'"
    notifymas +shn "(libedit $MSH_SHN_LIBEDIT_LOADED) editing '$file'"
#   local bfil=$(shn_gvimer_plus_bin --servername "$fuuid" --remote-expr "bufnr(\"^${fpath}$\")" 2>/dev/null )
#   [[ $bfil ]] && echo "buffer = $bfil" >&2
#   echo "regfile_in fuuid: $fuuid for $fpath" 1>&2
    if [[ -f "$fpath" ]] && [[ ${fuuid} ]] ; then
        for resident in $( shn_gvimer_plus_bin --serverlist )
        do
            if [[ "$resident" == ${fuuid}* ]]; then
 	        [[ $MSH_SHN_LIBEDIT_TRACE ]] && shn_msg "regfile_in resident:$resident for $fpath"
                if [[ $fline -eq 0 ]] ; then unset fline ; fi 
                shn_gvimer_plus_resident $fpath $fuuid $fline
                return $?
            fi
        done
        if [[ -n "$typf" ]] && [[ -f "$masedf" ]]; then
            if /bin/grep --colour=auto "$filen" "$masedf" &>/dev/null; then
#	        echo "1 gvimer --servername $fuuid ${typf}.mased ($masedf)" 1>&2
#		echo "masedf: $masedf" >&2
#               shn_gvimer_plus_bin --servername "$fuuid" --cmd "set path=$(shn_gvimer_plus_vpath $typf)" --cmd "source $masedf" -c "tab drop $rfile"
		local edpath=$(shn_gvimer_plus_vpath $typf)
		shn_msg "Go $rfile : $fline ..."
                if [[ $fline -eq 0 ]] ; then unset fline ; fi 
                shn_gvimer_plus_bin \
			--servername "$fuuid" \
			${masedf:+--cmd "let masedfile=\"$masedf\""} \
			${fline:+--cmd "let masedline=\"$fline\""} \
			${edpath:+--cmd "let masedpath=\"$edpath\""} \
			${rfile:+--cmd "let maseddrop=\"$rfile\""} \
			${localvim_dir:+--cmd "let mas_localvimdir=\"$localvim_dir\""}
#		/bin/sleep 0.5
#	        echo "2 gvimer_resident $rfile $fuuid" 1>&2
#		shn_gvimer_plus_resident $rfile $fuuid
            else
              shn_errmsg "not found '${filen}' [$typf] at $masedf -- `pwd`"
                shn_gvimer_plus_fuuid $fpath $fuuid $fline
            fi
        else
            shn_gvimer_plus_fuuid $fpath $fuuid $fline
        fi
        return $?
    else
        shn_errmsg "no file:$fpath"
        return 1
    fi
    return 0
}
shn_gvimer_plus_regfile ()
{
    local nocase retcode
    if shopt nocasematch &>/dev/null; then
        nocase=1
    else
        nocase=0
    fi
    shopt -s nocasematch &>/dev/null
    shn_gvimer_plus_regfile_in $@
    retcode=$?
    if [[ "$nocase" -eq 0 ]]; then
        shopt -u nocasematch &>/dev/null
    fi
    return $retcode
}

shn_gvimer_plus_mased ()
{
  local file=$1 filef
  local typf
# local fileq
  local a b fline flinef
  shn_dbgmsg "gvimer_plus_mased 1 $@"
  if [[ $file =~ ^(.*):(.*)$ ]] ; then
    a=${BASH_REMATCH[1]}
    b=${BASH_REMATCH[2]}
#20160118.112059    shn_msg "name parts a:$a; b:$b"
    if [[ $a =~ ^[[:digit:]]+$ ]] ; then
      file=$b
      fline=$a
    elif [[ $b =~ ^[[:digit:]]+$ ]] ; then
      file=$a
      fline=$b
    fi
#20160118.112059    shn_msg "$file -- $fline"
  fi
  if ! [[ $file == *.* ]] ; then
    local -a afileq
    local -a afline
#20160118.112059    shn_msg "Looking for function '$file'"
#   fileq=$( grep -rl --inc='*.c' "^$file\>(" src/ mod/ )
    local finddirs
    if [[ -d src ]] ; then finddirs="${finddirs:+$finddirs }src/" ; fi
    if [[ -d mod ]] ; then finddirs="${finddirs:+$finddirs }mod/" ; fi
#20160118.112059shn_msg "find at $finddirs"
    if afileq=`shn_find_c_mastar_function $file $finddirs` ; then
#20160118.112059      shn_msg "Found function $file : ${afileq[@]}"
      flinef=$(   grep -n "^$file\>(" "${afileq[@]}")
      fline=${fline:-${flinef%:*}}
      file=${afileq[0]}
    elif afileq=($( grep -rl --inc='*.[ch]' "#\s*define\>\s\+$file\>" $finddirs )) ; then
#20160118.112059      shn_msg "Found define $file : ${afileq[@]}"
      afline=($(grep -n "#\s*define\>\s\+$file\>" "${afileq[@]}"))
      flinef=$( grep -n "#\s*define\>\s\+$file\>" "${afileq[@]}")
      fline=${fline:-${flinef%:*}}
      file=${afileq[0]}
    fi
#20160118.112059    shn_msg "file='$file' fline:$fline flinef:'$flinef'"
  fi
  typf=`shn_gvimer_plus_filtyp "${file:-*.c}"`
#20160118.112059  shn_msg "typf=$typf for '$file'"
  shn_dbgmsg "gvimer_plus_mased 2 typf:$typf"
  if [[ "$file" == */* ]]; then
      filef=$file
  else
      filef=`shn_gvimer_plus_find $file $typf`
      shn_dbgmsg "gvimer_plus_mased 3 filef:$filef"
      if ! [[ -n "$filef" ]]; then
	  shn_errmsg "not found '$file'${typf:+ as [typf:$typf]}"
	  return 1
      else
	  shn_msg "(libedit $MSH_SHN_LIBEDIT_LOADED) found '$filef'"
      fi
  fi
#20160118.112059  shn_msg "filef='$filef' for file='$file' fline:$fline"
#   echo "@ typf:$typf for ${file} -> $filef line $fline" >&2
  local rfile=`/usr/bin/realpath $filef`
  shn_dbgmsg "rfile:$rfile" 
  filef=`/bin/basename $rfile`
  local dir=`/bin/dirname $rfile`
  shn_dbgmsg "dir:$dir" 
  local dirn=`/bin/basename $dir`
  typf=`shn_gvimer_plus_filtyp "${filef:-*.c}" $dirn`
#   echo "2 typf:$typf" >&2
  local masedf
#   grep "^\s*\(e\|sp\|find\|sfind\|tab\s\+\(sfind\|find\|sp\)\)\s*\<${filef}\s*$" $mased_dir/*.mased.vim | head -1 >&2
  masedf=$(grep -l "^\s*\(e\|sp\|find\|sfind\|tab\s\+\(sfind\|find\|sp\)\)\s*\<${filef}\s*$" $mased_dir/*.mased.vim | head -1)
# echo "masedf:[$masedf] for $filef ($file)" >&2
####[[ ${masedf:=$mased_dir/${typf}.mased.vim} ]] # off 20140413
  [[ $MSH_SHN_LIBEDIT_TRACE ]] && shn_msg "1 masedf: '$masedf' by '$filef' at $mased_dir/"
  [[ $masedf ]] && ! [[ -f $masedf ]] && masedf=
  [[ $MSH_SHN_LIBEDIT_TRACE ]] && shn_msg "2 masedf: $masedf"
  shn_dbgmsg "gvimer_plus_mased 4 $@"
  if [[ $masedf ]] ; then
    local fuuid="$( shn_gvimer_plus_uuid $masedf )"
    shn_dbgmsg "gvimer_plus_mased 5 $@"
    [[ $MSH_SHN_LIBEDIT_TRACE ]] && shn_msg "fuuid: $fuuid"
    if ! [[ -f "$masedf" ]]; then
	if [[ "$typf" == shn ]] && [[ -d "$typf" ]]; then
	    /bin/ls --color=auto -1 shn/ | /bin/sed -e 's/^/:sfind /' > $masedf
	fi
    fi
    if [[ -f "$masedf" ]]; then
#       shn_msg "(libedit $MSH_SHN_LIBEDIT_LOADED) found masedf '$masedf'"
	notifymas +shn "(libedit $MSH_SHN_LIBEDIT_LOADED) found masedf '$masedf'"
##	echo "mased fuuid: $fuuid" 1>&2
        shn_dbgmsg "gvimer_plus_mased 6a $@"
	shn_gvimer_plus_regfile $rfile $fuuid ${fline:-0} $masedf $typf && return 0
    else
        shn_msg "(libedit $MSH_SHN_LIBEDIT_LOADED) masedf '$masedf' not found"
        shn_dbgmsg "gvimer_plus_mased 6b $@"
	shn_errmsg "not found masedf '$masedf'"
	return 1
    fi
  fi
  shn_dbgmsg "gvimer_plus_mased E $@"
  return 1
}

shn_gvimer_plus ()
{
    local file deffile mased_dir=${MSH_SHN_DIRS[relmased]} localvim_dir=${MSH_SHN_DIRS[relvimid]}
    shn_dbgmsg "gvimer $LINENO $@"
    if [[ $mased_dir ]] && [[ -d $mased_dir ]] && [[ $localvim_dir ]] && [[ -d $localvim_dir ]] ; then
        shn_dbgmsg "gvimer $LINENO $@"
        if [[ "$@" ]]; then
#20160118.112059	  shn_msg "Looking for $@"
            shn_dbgmsg "gvimer $LINENO $@"
            shn_gvimer_plus_mased $@ && return 0
            shn_gvimer_plus_nomased   $@ && return 0
            shn_errmsg "can't edit; no <mased> for '$@' at $mased_dir; try direct path"
        else
	    local xg
	    shopt -q extglob && xg=1
	    shopt -s extglob
	    local dn=${PWD##+(*([^/])/)zocromas_}.c
	    local finddirs=''
	    if [[ -d src ]] ; then finddirs="${finddirs:+$finddirs }./src" ; fi
	    if [[ -d mod ]] ; then finddirs="${finddirs:+$finddirs }./mod" ; fi
shn_msg "find at $finddirs"
	    [[ $xg ]] || shopt -u extglob
	    [[ $dn ]] && deffile=${deffile:-$( find $finddirs -type f -name "*${dn}" | head -1 )}
			 deffile=${deffile:-$( find $finddirs -type f -name '*.c'	 | head -1 )}
			 deffile=${deffile:-$( find $finddirs -type f -name '*.h'	 | head -1 )}
			 deffile=${deffile:-$( find $finddirs -type f -name '*.def'	 | head -1 )}
            if [[ -n "$deffile" ]]; then
                shn_gvimer_plus_mased $deffile && return 0
            fi
        fi
    else
      echo "no mased mode: check '$mased_dir' and '$localvim_dir'"
        shn_gvimer_plus_nomased $@ || return $?
    fi
    return 1
}
