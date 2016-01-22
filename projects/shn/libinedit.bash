MSH_SHN_LIBEDIT_LOADED=$(datemt)


shn_gvimer_plus_filtyp ()
{
    local typf
    local filef=$1
    shift
    local dirn=$1
    shift
    if [[ "$filef" == *.c ]] || [[ "$filef" == *.h ]]; then
       typf="src"
    elif [[ "$filef" == *.def ]]; then
       typf="cdef"
    elif [[ "$filef" == *.cmds ]] || [[ "$filef" == *.duf ]]; then
       typf="dufcmds"
    elif [[ "$filef" == *.sh ]] || [[ "$filef" == *.bash ]] ; then
       typf="shn"
    elif [[ "$dirn" == shn ]] && ( [[ "$filef" == *.sh ]] || [[ "$filef" == *.bash ]] ); then
       typf="shn"
    elif [[ "$filef" == *.ac ]] || [[ "$filef" == *.am ]]; then
       typf="ac"
    elif [[ "$filef" == *.mased.vim ]] ; then
       typf="mased_vim"
    elif [[ "$filef" == *.vim ]] || [[ "$filef" == gvim* ]] || [[ "$filef" == vim* ]] ; then
       typf="vimstd"
    elif [[ "$filef" =~ ^.*\.([a-z]+)$ ]] ; then
       typf="${BASH_REMATCH[1]}"
    else
      shn_errmsg "can't set typf for '$filef'${dirn:+ at '$dirn'}"
    fi
    [[ $MSH_SHN_LIBEDIT_TRACE ]] && shn_msg "typf: $typf"
    echo $typf
}
shn_gvimer_plus_find ()
{
    local file=$1
    shift
    local typf=$1
    shift
    local paths
    case $typf in
        src)		paths='./src/ ./mod/ ./inc/'	;;
        cdef)		paths='./src/'	;;
        ac)		paths='./'		;;
        shn)		paths='./shn/'		;;
        vimstd)		paths="$localvim_dir/"	;;
        dufcmds)	paths="./test/duf/"	;;
        mased_vim)	paths="./$mased_dir/"	;;
        *)		paths='./'		;;
    esac
#   eval "/usr/bin/find -L $paths -type f -name $file 2>/dev/null" | head -1
    filef=`eval "/usr/bin/find -L $paths -type f -name $file 2>/dev/null" | head -1`
#   shn_msg "found: '$filef'"
    echo $filef
}
shn_gvimer_plus_vpath ()
{
    local typf=$1
    shift
    local paths
    case $typf in
        src)		paths='src/,mod/,src/inc/,mod/inc/,inc/'	;;
        cdef)		paths="src/,src/inc/"		;;
        ac)		paths='.'			;;
        shn)		paths='shn/'			;;
        vimstd)		paths="$localvim_dir"		;;
        dufcmds)	paths="test/duf/tests20151006/"		;;
        mased_vim)	paths="$mased_dir"		;;
        *)		paths='.'			;;
    esac
    echo "${paths:-.}"
}

