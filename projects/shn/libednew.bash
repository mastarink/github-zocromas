shn_gvimer_plus_bin_nt ()
{
    local a c='-o'
    for a in "$@" ; do c+=" '$a'" ; done
    [[ $MSH_SHN_LIBEDIT_TRACE ]] && shn_msg ${MSHCMD_GVIMC:=/usr/bin/gvim} $c
    echo "{${MSHCMD_GVIMC:=/usr/bin/gvim} $c}" >&2
    eval "${MSHCMD_GVIMC:=/usr/bin/gvim} $c"
}
shn_gvimer_plus_fuuid_nt ()
{
    local file=$1
    shift
    local fuuid=$1
    shift
    local fline=$1
    shift
    if [[ $fline -eq 0 ]] ; then unset fline ; fi 
#20160118.112059    shn_msg "@@" shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
#    shn_msg "$FUNCNAME: [$file]"
    shn_gvimer_plus_bin_nt --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
}
shn_gvimer_plus_resident_nt ()
{
    local file=$1
    shift
    local fuuid=$1
    shift
    local fline=$1
    shift
    if [[ $fline -eq 0 ]] ; then unset fline ; fi 
#20160118.112059    shn_msg "@@" shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
#    shn_msg "$FUNCNAME: [$file]"
    shn_gvimer_plus_bin_nt --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
}
shn_gvimer_plus_filtyp ()
{
  local filid=$1
  shift
  local typ ytyp
  for typ in ${!edfile_typs[*]} ; do
    if [[ $filid == ${edfile_typs[$typ]} ]] ; then
      ytyp=$typ
#     echo "$FUNCNAME: $typ : " ${edfile_typs[$typ]} "filid:$filid; ytyp:$ytyp" >&2
      echo -n $ytyp
      return 0
    fi
  done  
  for typ in ${!edfile_typs_re[*]} ; do
    if [[ $filid =~ ${edfile_typs_re[$typ]} ]] ; then
      ytyp=$typ
      echo "$FUNCNAME: $typ : " ${edfile_typs[$typ]} "filid:$filid; ytyp:$ytyp" >&2
      echo -n $ytyp
      return 0
    fi
  done  
  
  return 1
}
shn_gvimer_masedf_nt ()
{
  local filid=$1 ; shift
  local typf=$1 ; shift
  declare -a fpath=$@ ; shift
  local masedf
  if ! [[ -d $mased_dir ]] ; then
    :
  fi
  if [[ $mased_dir ]] && [[ -d $mased_dir ]] ; then
##  for masedf in $mased_dir/${typf}* ; do
    for masedf in $mased_dir/* ; do
      if grep -q '\<\(tab\s\+sfind\|sfind\|find\|e\)\>\s\+\<'$filid'\s*$' $masedf && [[ -f $masedf ]] ; then
	echo -n $masedf
	return 0
      fi
    done
  else
    echo "Error mased_dir: '$mased_dir'" >&2
  fi
  return 1
}
shn_gvimer_plus_mased_file_here_nt ()
{
  local res=1
  local filid=$1
  shift
  local rfile=$1
  shift
#      shn_msg "0 R : $rfile"
  local fpath typf afileq funcid fline edpath p way='undefined' resident masedf fuuid newfile rr
  local -a afpath afpath_src arfile
  if [[ $filid =~ ^([[:digit:]]+)\:(.*)$ ]] ; then
    filid=${BASH_REMATCH[2]}
    fline=${BASH_REMATCH[1]}
  elif [[ $filid =~ ^(.*)\:([[:digit:]]+)$ ]] ; then
    filid=${BASH_REMATCH[1]}
    fline=${BASH_REMATCH[2]}
  fi
#  shn_msg "1 $FUNCNAME:$LINENO: filid here: $filid"
  if ! typf=$( shn_gvimer_plus_filtyp "${filid}" )  \
  		&& afileq=`shn_find_c_mastar_function "$filid" ${edfile_dirs[src]}` ; then
    funcid=$filid
    typf=src
    rfile="$afileq"
#      shn_msg "1 R : $rfile"
    filid=$(basename $afileq)
  elif afileq=`shn_find_c_mastar_def "$filid" ${edfile_dirs[src]}` ; then
    typf=src
    rfile="$afileq"
#      shn_msg "2 R : $rfile"
    filid=$(basename $afileq)
  fi
#  shn_msg "2 $FUNCNAME:$LINENO: filid here: $filid"
  if [[ $typf ]] ; then
    eval "afpath=(${edfile_dirs[$typf]})"
#   edpath="${afpath[@]}" ; edpath=${edpath// /,}
    fpath='' ; edpath=''
    for p in ${afpath[@]} ; do
      if [[ -d $p ]] ; then
        fpath=${fpath}${fpath:+' '}$p
	edpath=${edpath}${edpath:+','}$p
      fi
    done
    if ! [[ "$rfile" ]] ; then
      if [[ -f "$filid" ]] ; then
        rfile="$filid"
#        shn_msg "3 R : $rfile"
      elif [[ $filid ]] ; then
        arfile=(`find $fpath -name "$filid"`)
	rfile=${arfile[0]}
#	shn_msg "4 R : $rfile"
      fi
    fi
    if [[ $rfile ]] && [[ -f $rfile ]] ; then
      rfile=`/usr/bin/realpath $rfile`
      filid=$(basename $rfile)
#	shn_msg "5 R : $rfile"
    else
      unset rfile
    fi
    if masedf=`shn_gvimer_masedf_nt $filid $typf $fpath` ; then
      fuuid=$( shn_gvimer_plus_uuid "$masedf" )
      way='mased'
    else
      way='nomased'
    fi
    res=0
  else
    way='notyp'
    if [[ "$filid" ]] && [[ -f "$filid" ]] ; then
      rfile=`/usr/bin/realpath "$filid"`
#	shn_msg "6 R : $rfile"
    else
      rfile=`/usr/bin/realpath "$filid"`
#      shn_msg "7 R : $rfile"
      newfile=$fuuid
    fi
#   shn_msg "3 $FUNCNAME:$LINENO: filid here: $filid"
    
    if resident=$(shn_gvimer_plus_anywhere $rfile) ; then
      unset fuuid
#     fuuid=$resident
    fi
  fi
  if [[ $fline -eq 0 ]]  ; then unset fline ; fi 
  fuuid=${fuuid:-$fuuid0}
  fuuid=${fuuid^^}
  local nn=0
  for rr in $( shn_gvimer_plus_bin_nt --serverlist )
  do
#   shn_msg "$nn: [$rr] ${rr:0:$len} ??? $fuuid"
    if [[ $fuuid ]] ; then
      len=${#fuuid}
      if [[ "${rr:0:$len}" == $fuuid ]]; then
	resident=${fuuid}
#	shn_msg "${rr:0:$len} !!! $fuuid"
	unset fuuid
      fi
    fi
    nn=$(( $nn+1 ))
  done


  local msg=$way
  
  if [[ $filid ]]        ; then msg="$msg filid: '$filid';"        ; fi
  if [[ $funcid ]]       ; then msg="$msg funcid: '$funcid';"      ; fi
  if [[ $fline ]]        ; then msg="$msg fline: '$fline';"        ; fi
  if [[ $typf ]]         ; then msg="$msg typf: [$typf];"          ; fi
  if [[ $masedf ]]       ; then msg="$msg masedf: [$masedf];"      ; fi
  if [[ $resident ]]     ; then msg="$msg resident: [$resident];"  ; fi
  if [[ $fuuid ]]        ; then msg="$msg fuuid: [$fuuid];"        ; fi
# if [[ $fuuid0 ]]       ; then msg="$msg fuuid0: [$fuuid0];"      ; fi
  if [[ $fpath ]]        ; then msg="$msg fpath: [$fpath];"        ; fi
# if [[ ${afpath[@]} ]]  ; then msg="$msg afpath: [${afpath[@]}];" ; fi
  if [[ $edpath ]]       ; then msg="$msg edpath: [$edpath];"      ; fi
  if [[ $newfile ]]      ; then msg="$msg newfile: [$newfile];"    ; fi
  if [[ $rfile ]]        ; then msg="$msg rfile: [$rfile];"    ; fi
#    shn_msg "4 $FUNCNAME:$LINENO: filid here: $filid; $rfile"
  if [[ $resident ]] ; then
#    shn_msg "A : $msg"
    shn_gvimer_plus_resident_nt $rfile $resident $fline
  elif [[ $typf ]] && [[ $masedf ]]; then
#    shn_msg "B : $msg"
    shn_gvimer_plus_bin_nt --servername "$fuuid" \
        		${masedf:+--cmd "let masedfile=\"$masedf\""} \
        		${fline:+--cmd "let masedline=\"$fline\""} \
        		${edpath:+--cmd "let masedpath=\"$edpath\""} \
        		${rfile:+--cmd "let maseddrop=\"$rfile\""} \
        		${localvim_dir:+--cmd "let mas_localvimdir=\"$localvim_dir\""}
  elif [[ $rfile ]] ; then
    # nomased || notype
#   shn_msg "C : $msg"
#    shn_msg "CF : $rfile"
    shn_gvimer_plus_fuuid_nt "$rfile" $fuuid $fline
  else 
    rfile="`pwd`/$filid"
#	shn_msg "8 R : $rfile"
#   shn_msg "D : $msg"
#    shn_msg "DF : $rfile"
    shn_gvimer_plus_fuuid_nt "$rfile" $fuuid $fline
  fi
  return $res
}
shn_gvimer_plus_mased_file_nt ()
{
  local filid=$1
  shift
  local res=1
  local trfile tdir rfile
  if [[ -f "$filid" ]] ; then
    rfile=$(realpath "$filid")
    filid=$(basename "$filid")  
    tdir=$(dirname "$rfile")
  fi
#  shn_msg "$FUNCNAME: filid: $filid; @ $tdir;"
  if ! [[ $tdir ]] ; then
    shn_gvimer_plus_mased_file_here_nt "$filid"
  elif [[ $MSH_SHN_PROJECT_DIR ]] && plen=${#MSH_SHN_PROJECT_DIR} && [[ ${tdir:0:$plen} == $MSH_SHN_PROJECT_DIR ]] ; then
    shn_gvimer_plus_mased_file_here_nt "$filid" $rfile
    res=$?
  fi
  return $res
}
shn_gvimer_plus_mased_nt ()
{
  local filid
# shn_msg "$FUNCNAME:		args: $@;"
  for filid in "$@" ; do
#    shn_msg "$FUNCNAME: filid: '$filid'"
    if [[ -f "$filid" ]] ; then
      filid=$(realpath "$filid")
    fi
    if shn_gvimer_plus_mased_file_nt "$filid" ; then
      :
#     shn_msg "$FUNCNAME:		 arg: '$filid'" >&2
#   else
#     return 1
#   else
#     shn_msg "$FUNCNAME:		 ??? arg: '$filid' => " >&2
    fi
  done
  return 0
}
shn_gvimer_plus_nt ()
{
  local arg l
  local -a dirslines
  local  mased_dir=${MSH_SHN_DIRS[relmased]} localvim_dir=${MSH_SHN_DIRS[relvimid]}
  if ! [[ $localvim_dir ]] && [[ "$auto_localvim_dir" == '[auto]' ]] ; then
    if [[ -d editing ]] ; then
      localvim_dir=editing
    fi
  fi
  if ! [[ $mased_dir ]] && [[ "$auto_mased_dir" == '[auto]' ]] ; then
    local -a amased_dir
    if [[ -d $localvim_dir ]] ; then
      amased_dir=$(find $localvim_dir -type d -name 'mased')
    else
      amased_dir=$(find -type d -name 'mased')
    fi
    if [[ ${#amased_dir[@]} -eq 1 ]] ; then
      mased_dir=${amased_dir[0]}
    fi
  fi
  local  fuuid0=$( shn_gvimer_plus_uuid "$@" )
# declare -A edfile_dirs=(['src']='src mod src/inc mod/inc' [cdef]='src/inc' [shn]='shn' [dufcmds]='test/duf/tests20151006' [mased_vim]="$mased_dir" [ac]="." )
  declare -A edfile_dirs=(['src']='src mod src/inc mod/inc' [mased_vim]="$mased_dir" )
# declare -A edfile_typs=(['src']='*.[ch]' [cdef]='*.def'  [ac]="*.a[mc]" [mased_vim]="*.mased.vim"  )
  declare -A edfile_typs=(['src']='*.[ch]'  [ac]="*.a[mc]" [mased_vim]="*.mased.vim"  )
# declare -A  edfile_typs_re=([dufcmds]='(^.*\.(cmds|duf)$)' [shn]='(^.*\.(bash|sh)$)' [vimstd]='(^(.*\.vim|(gvim|vim).*)$)' )
  declare -A  edfile_typs_re=( [vimstd]='(^(.*\.vim|(gvim|vim).*)$)' )
  if [[ ${auto_edcfg:-'[auto]'} == '[auto]' ]] ; then
    auto_edcfg="${localvim_dir}/eddirs.cfg"    
  fi
  local edcfg=${auto_edcfg:-${localvim_dir}/eddirs.cfg}
  if [[ $auto_edcfg ]] && [[ -f $auto_edcfg ]] ; then
    readarray -t dirslines < editing/eddirs.cfg
  fi
  for l in "${dirslines[@]}" ; do 
#   echo -e "@\t\t\t\t\t\t\t\t\t\t$l" >&2
    if [[ $l =~ ^[[:blank:]]*([[:alnum:]]+)[[:blank:]]*:[[:blank:]]*([^[:blank:]\+=][^\+=]*|)([[:blank:]]*=[[:blank:]]*([^[:blank:]\+=]+)|)([[:blank:]]*\+[[:blank:]]*([^[:blank:]\+=]+)|)[[:blank:]]*$ ]] ; then
#     echo -e "+ ${BASH_REMATCH[1]} \t:: ${BASH_REMATCH[2]} \t:: ${BASH_REMATCH[4]} \t:: ${BASH_REMATCH[6]}" >&2
      if [[ ${BASH_REMATCH[2]} ]] ; then edfile_dirs[${BASH_REMATCH[1]}]=${BASH_REMATCH[2]} ; fi
      if [[ ${BASH_REMATCH[4]} ]] ; then edfile_typs[${BASH_REMATCH[1]}]=${BASH_REMATCH[4]} ; fi
      if [[ ${BASH_REMATCH[6]} ]] ; then edfile_typs_re[${BASH_REMATCH[1]}]=${BASH_REMATCH[6]} ; fi
    fi
  done
#   echo '******** dirs:' >&2
#   for l in ${!edfile_dirs[@]} ; do
#     echo "$l :: " ${edfile_dirs[$l]}  >&2
#   done
#   echo '******** typs:' >&2
#   for l in ${!edfile_typs[@]} ; do
#     t="${edfile_typs[$l]}"
#     echo "========= $l"
#     cat <<RR >&2
#     $l :: $t
# RR
#   done
#   echo '******** typs_re:' >&2
#   for l in ${!edfile_typs_re[@]} ; do
#     t="${edfile_typs_re[$l]}"
#     echo "========= $l"
#     cat <<RR >&2
#     $l :: $t
# RR
#   done
  
# for arg in "$@" ; do 
#   shn_msg "$FUNCNAME:		fuuiddef: $fuuiddef;	$arg;" >&2
# done
  if [[ "$@" ]] ; then
    shn_gvimer_plus_mased_nt "$@"
  elif [[ -f configure.ac ]] ; then
    local filid
    filid=$(sed -ne 's@AC_CONFIG_SRCDIR(\[\(.*\)\])@\1@p' configure.ac )
    shn_gvimer_plus_mased_nt "$filid"
  fi
}
function shn_file_edit_simple ()
{
  if [[ "$@" ]] ; then
    shn_gvimer_plus_nt "$@"
  else
    shn_gvimer_plus_nt
  fi
}
function shn_file_edit ()
{
  
  if [[ "$@" ]] ; then
    shn_project_file_cd $@
#   for arg in "$@" ; do 
#     shn_msg "$FUNCNAME: $arg" >&2
#   done
# export MSH_SHN_DEBUG=yes
    shn_gvimer_plus_nt "$@"
# unset MSH_SHN_DEBUG
    shn_project_cd -
    shn_dbgmsg "after shn_project_cd $LINENO $FUNCNAME" >&2
  else
    shn_gvimer_plus_nt
  fi
}
