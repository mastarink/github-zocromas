shn_gvimer_plus_bin () 
{ 
    local a c='-o'
    for a in "$@" ; do c+=" '$a'" ; done
#   echo ${MSHCMD_GVIMC:=/usr/bin/gvim} $c >&2
    eval ${MSHCMD_GVIMC:=/usr/bin/gvim} $c
}
shn_gvimer_plus_fuuid () 
{ 
    local file=$1;
    shift;
    local fuuid=$1;
    shift;
    shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file"
}
shn_gvimer_plus_resident () 
{ 
    local file=$1;
    shift;
    local fuuid=$1;
    shift;
    shn_gvimer_plus_bin --servername "$fuuid" --remote-tab-silent ${fline:++$fline} "$file" 
}
shn_gvimer_plus_uuid () 
{ 
    local fuuid;
    local file;
    local string;
    local rp;
    for file in $@;
    do
        if [[ -n "$typf" ]] && [[ "$typf" == shn ]]; then
            rp=`realpath $file`;
        else
            rp=`realpath -s $file`;
        fi;
        if [[ -n "$string" ]]; then
            string+=";$rp"
        else
            string="$rp"
        fi;
    done;
    echo -n 'shn_gvimer_plus-'
    echo -n "realpath $string" | md5sum | /bin/cut -b-32
}
shn_gvimer_plus_regfile_in () 
{ 
    local file=$1;
    shift;
    local fuuid=${1:-"$( shn_gvimer_plus_uuid $file )"};
    shift;
    local masedf=$1;
    shift;
    local typf=$1;
    shift;
    local filen=`basename $file`;
    local resident;
    local fpath=$(realpath $file)
#   local bfil=$(shn_gvimer_plus_bin --servername "$fuuid" --remote-expr "bufnr(\"^${fpath}$\")" 2>/dev/null )
#   [[ $bfil ]] && echo "buffer = $bfil" >&2
#   echo "regfile_in fuuid: $fuuid for $fpath" 1>&2;
    if [[ -f "$fpath" ]]; then
        for resident in $( shn_gvimer_plus_bin --serverlist );
        do
            if [[ "$resident" == $fuuid ]]; then
#	        echo "regfile_in resident:$resident for $fpath" 1>&2;
                shn_gvimer_plus_resident $fpath $fuuid ${fline:++$fline}
                return $?;
            fi;
        done;
        if [[ -n "$typf" ]] && [[ -f "$masedf" ]]; then
            if /bin/grep --colour=auto "$filen" "$masedf" &>/dev/null; then
#	        echo "1 gvimer --servername $fuuid ${typf}.mased ($masedf)" 1>&2;
#		echo "masedf: $masedf" >&2
#               shn_gvimer_plus_bin --servername "$fuuid" --cmd "set path=$(shn_gvimer_plus_vpath $typf)" --cmd "source $masedf" -c "tab drop $rfile"
		local edpath=$(shn_gvimer_plus_vpath $typf)
                shn_gvimer_plus_bin \
			--servername "$fuuid" \
			${masedf:+--cmd "let masedfile=\"$masedf\""} \
			${edpath:+--cmd "let masedpath=\"$edpath\""} \
			${rfile:+--cmd "let maseddrop=\"$rfile\""}
#		/bin/sleep 0.5;
#	        echo "2 gvimer_resident $rfile $fuuid" 1>&2;
#		shn_gvimer_plus_resident $rfile $fuuid;
            else
                echo "$FUNCNAME: not found '${filen}' [$typf] at $masedf -- `pwd`" 1>&2;
                shn_gvimer_plus_fuuid $fpath $fuuid;
            fi;
        else
            shn_gvimer_plus_fuuid $fpath $fuuid;
        fi;
        return $?;
    else
        echo "$FUNCNAME not found $fpath";
        return 1;
    fi;
    return 0
}
shn_gvimer_plus_regfile () 
{ 
    local nocase retcode;
    if shopt nocasematch &>/dev/null; then
        nocase=1;
    else
        nocase=0;
    fi;
    shopt -s nocasematch &>/dev/null;
    shn_gvimer_plus_regfile_in $@;
    retcode=$?;
    if [[ "$nocase" -eq 0 ]]; then
        shopt -u nocasematch &>/dev/null;
    fi;
    return $retcode
}
shn_gvimer_plus_anywhere () 
{
  local file ser bfil fpath sl
  for file in $@ ; do
    sl=$(shn_gvimer_plus_bin --serverlist)
    for ser in $sl ; do
      echo "server:$ser" >&2
      if [[ -f $file ]] ; then
        echo "file:$file" >&2
#	cwd=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "getcwd()" 2>/dev/null )
#       fpath="${cwd}/${file}"
	fpath=$(realpath $file)
	bfil=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "bufnr(\"^${fpath}$\")" 2>/dev/null )
        echo "1 fpath:$fpath bufnr:$bfil" >&2
	bfil1=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "bufnr(\"^${file}$\")" 2>/dev/null )
        echo "2 file:$file bufnr:$bfil1" >&2
	bfil1=$(shn_gvimer_plus_bin --servername "$ser" --remote-expr "bufnr(\"${file}$\")" 2>/dev/null )
        echo "3 file:$file bufnr:$bfil1" >&2
	if [[ $bfil -ge 0 ]] ; then
	  echo "bufnr for '$fpath' : $bfil" >&2
	  echo $ser
	  return 0
	fi
      else
        echo "no file:$file" >&2
      fi
    done
  done
  return 1
}
shn_gvimer_plus_nomased () 
{ 
    local file;
    local fuuid0 fuuid
    fuuid0=$( shn_gvimer_plus_uuid $@ )
    for file in $@;
    do
        fuuid=$(shn_gvimer_plus_anywhere $file)
        echo "nomased $file $fuuid (a/w)" >&2
	[[ ${fuuid:=$fuuid0} ]]
#	echo "fuuid:$fuuid" >&2
        shn_gvimer_plus_regfile $file $fuuid || return $?;
    done;
    return 0
}
shn_gvimer_plus_filtyp () 
{ 
    local typf;
    local filef=$1;
    shift;
    local dirn=$1;
    shift;
    if [[ "$filef" == *.c ]] || [[ "$filef" == *.h ]]; then
       typf="src";
    elif [[ "$filef" == *.sh ]] || [[ "$filef" == *.bash ]] ; then
       typf="shn";
    elif [[ "$dirn" == shn ]] && ( [[ "$filef" == *.sh ]] || [[ "$filef" == *.bash ]] ); then
       typf="shn";
    elif [[ "$filef" == *.ac ]] || [[ "$filef" == *.am ]]; then
       typf="ac";
    elif [[ "$filef" == *.mased.vim ]] ; then
       typf="mased_vim";
    elif [[ "$filef" == *.vim ]] || [[ "$filef" == gvim* ]] || [[ "$filef" == vim* ]] ; then
       typf="vimstd";
    fi
    echo $typf
}
shn_gvimer_plus_find () 
{ 
    local file=$1;
    shift;
    local typf=$1;
    shift;
    local paths;
    case $typf in 
        src)		paths='./src/ ./inc/ ./settling/src ./settling/src/inc'	;;
        ac)		paths='./'		;;
        shn)		paths='./shn/'		;;
        vimstd)		paths='.'		;;
        mased_vim)	paths='./mased/'	;;
        *)		paths='./'		;;
    esac;
#   eval "/usr/bin/find -L $paths -type f -name $file 2>/dev/null" | head -1
    filef=`eval "/usr/bin/find -L $paths -type f -name $file 2>/dev/null" | head -1`;
    echo $filef
}
shn_gvimer_plus_vpath () 
{ 
    local typf=$1;
    shift;
    local paths;
    case $typf in 
        src)		paths='src/,src/inc/,inc/,settling/src/,settling/src/inc'	;;
        ac)		paths='.'			;;
        shn)		paths='shn/'			;;
        vimstd)		paths='.'			;;
        mased_vim)	paths='mased/'			;;
        *)		paths='.'			;;
    esac;
    echo "${paths:-.}"
}

shn_gvimer_plus_mased () 
{ 
    local file=$1 filef 
    local typf;
    local fileq a b fline
    if [[ $file =~ ^(.*):(.*)$ ]] ; then
      a=${BASH_REMATCH[1]}
      b=${BASH_REMATCH[2]}
      if [[ $a =~ ^[[:digit:]]+$ ]] ; then
	file=$b
	fline=$a
      elif [[ $b =~ ^[[:digit:]]+$ ]] ; then
	file=$a
	fline=$b
      fi
    fi
    if ! [[ $file == *.* ]] ; then
      fileq=$( grep -rl --inc='*.c' "^$file\>(" )
      if [[ $fileq ]] ; then
        file=$fileq
      fi
    fi
    typf=`shn_gvimer_plus_filtyp "${file:-*.c}"`;
    if [[ "$file" == */* ]]; then
        filef=$file
    else
        filef=`shn_gvimer_plus_find $file $typf`;
        if ! [[ -n "$filef" ]]; then
            echo "libedit $FUNCNAME: not found '$file' as [$typf] " 1>&2;
            return 1;
	else
	    echo "libedit found $filef" 1>&2;
        fi;
    fi
#   echo "@ typf:$typf for ${file} -> $filef line $fline" >&2
    local rfile=`/usr/bin/realpath $filef`;
#   echo "rfile:$rfile" >&2
    filef=`/bin/basename $rfile`;
    local dir=`/bin/dirname $rfile`;
    local dirn=`/bin/basename $dir`;
    typf=`shn_gvimer_plus_filtyp "${filef:-*.c}" $dirn`;
#   echo "2 typf:$typf" >&2
    local masedf;
#   grep "^\s*\(e\|sp\|find\|sfind\|tab\s\+\(sfind\|find\|sp\)\)\s*\<${filef}\s*$" mased/*.mased.vim | head -1 >&2
    masedf=$(grep -l "^\s*\(e\|sp\|find\|sfind\|tab\s\+\(sfind\|find\|sp\)\)\s*\<${filef}\s*$" mased/*.mased.vim | head -1)
#   echo "masedf:[$masedf] for $filef ($file)" >&2
####[[ ${masedf:=mased/${typf}.mased.vim} ]] # off 20140413
    [[ $masedf ]] && ! [[ -f $masedf ]] && masedf=
    if [[ $masedf ]] ; then
      local fuuid="$( shn_gvimer_plus_uuid $masedf )";
      if ! [[ -f "$masedf" ]]; then
	  if [[ "$typf" == shn ]] && [[ -d "$typf" ]]; then
	      /bin/ls --color=auto -1 shn/ | /bin/sed -e 's/^/:sfind /' > $masedf;
	  fi;
      fi;
      if [[ -f "$masedf" ]]; then
#	  echo "mased fuuid: $fuuid" 1>&2;
#	  echo "mased masedf: $masedf" 1>&2;
	  shn_gvimer_plus_regfile $rfile $fuuid $masedf $typf
	  return $?;
      else
	  echo "$FUNCNAME: not found masedf '$masedf'" 1>&2;
	  return 1;
      fi;
    fi
    return 1
}
shn_gvimer_plus () 
{ 
    local file deffile
    if [[ -d mased ]]; then
        if [[ -n "$@" ]]; then
            shn_gvimer_plus_mased $@ && return $?;
            shn_gvimer_plus_nomased   $@ && return $?;
            echo "error editing $@ ; no <mased> for $@; try direct path" 1>&2;
            return 1;
        else
	    local dn=${PWD##+(*([^/])/)zocromas_}.c
	    [[ $dn ]] && deffile=${deffile:-$( find ./src -type f -name "*${dn}" | head -1 )}
			 deffile=${deffile:-$( find ./src -type f -name '*.c'	 | head -1 )}
			 deffile=${deffile:-$( find ./src -type f -name '*.h'	 | head -1 )}
            if [[ -n "$deffile" ]]; then
                shn_gvimer_plus_mased $deffile || return $?;
            else
                return 1;
            fi;
        fi;
    else
        shn_gvimer_plus_nomased $@ || return $?;
    fi
}
function shn_file_edit ()
{
  if [[ "$@" ]] ; then
    shn_project_file_cd $@
    shn_gvimer_plus $@
    shn_project_cd -
  else
    shn_gvimer_plus
  fi
}
