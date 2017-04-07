datem () 
{ 
    /bin/date '+%Y%m%d' "$@"
}
datemt () 
{ 
    /bin/date '+%Y%m%d.%H%M%S' "$@"
}
function wsleep ()
{
  local i s=$(( ${1:-1} * 10 ))
  for (( n=0 ; $n < $s ; n++ )) ; do
    for (( i=0 ; $i < 20000 ; i++ )) ; do
      :
    done
  done
}
function shn_echo ()
{
# echo $*
# echo $@
  builtin echo "$*"
# echo "$@"
}
function shn_echon ()
{
# echo $*
# echo $@
  builtin echo -n "$*"
# echo "$@"
}
function shn_msg ()
{
  local nocolor
  if ! [[ $MSH_SHN_DISABLE_MSG ]] ; then
    if [[ $# -gt 0 ]] ; then
      if [[ $TERM == dumb ]] ; then
	nocolor=yes
      fi
      if [[ $nocolor ]] || [[ $MSH_SHN_MSG_NOCOLOR ]]; then
        shn_echo "$(date '+%Y%m%d.%H%M%S.%N') ${MSH_SHN_MSG_PREFIX} $*"  >&2
      else
        shn_echo "$(date '+%Y%m%d.%H%M%S.%N') ${MSHPR_FBCYAN}${MSH_SHN_MSG_PREFIX}${MSHPR_ATTROFF}${MSH_SHN_MSG_COLOR:-${MSHPR_FBCYAN}${MSHPR_REVERSE}} $* ${MSHPR_ATTROFF}"  >&2
      fi
    else
      shn_echo >&2
    fi
  fi
}
function shn_msgn ()
{
  local nocolor
  if ! [[ $MSH_SHN_DISABLE_MSG ]] ; then
    if [[ $# -gt 0 ]] ; then
      if [[ $TERM == dumb ]] ; then
	nocolor=yes
      fi
      if [[ $nocolor ]] || [[ $MSH_SHN_MSG_NOCOLOR ]]; then
        shn_echon "$(date '+%Y%m%d.%H%M%S.%N') ${MSH_SHN_MSG_PREFIX} $*"  >&2
      else
        shn_echon "$(date '+%Y%m%d.%H%M%S.%N') ${MSHPR_FBCYAN}${MSH_SHN_MSG_PREFIX}${MSHPR_ATTROFF}${MSH_SHN_MSG_COLOR:-${MSHPR_FBCYAN}${MSHPR_REVERSE}} $* ${MSHPR_ATTROFF}"  >&2
      fi
    else
      shn_echon >&2
    fi
  fi
}
function shn_msgns ()
{
  shn_msgn "$* ○ "
}
function shn_msglist ()
{
  local i n
  n=$#
  for (( i=0 ; $i < $n ; i++ )) ; do
    shn_echo "$i: $1"  >&2
    shift
  done
}
function shn_fmsg ()
{
  local fmt=$1
  shift
  builtin printf -- "$fmt" $*  >&2
}
function shn_warn ()
{
  shn_echo "$*"  >&2
}
function shn_dbgmsg ()
{
  [[ "$MSH_SHN_DEBUG" ]] && shn_msg "dbg: $*"
}
function shn_errmsg ()
{
  local MSH_SHN_MSG_COLOR=$MSHPR_BYELLOWONRED
  local MSH_SHN_MSG_NOCOLOR
  if [[ $TERM == dumb ]] ; then
    MSH_SHN_MSG_NOCOLOR=yes
    shn_msg ERROR "$*"
  else
#   shn_msg "---------------------------------"
    shn_msg ERROR "$*"
#   shn_msg "---------------------------------"
  fi

  return 1
}
function shn_project_dir2realpath ()
{
  local dirn=$1
  shift
  local force=$1
  shift
  if [[ "$dirn" ]] && [[ "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR/$dirn" ]] ; then
    shn_echo "$MSH_SHN_PROJECTS_DIR/$dirn"
    return 0
  elif ! [[ -d "$MSH_SHN_PROJECTS_DIR" ]] ; then
    shn_errmsg "No directory : $MSH_SHN_PROJECTS_DIR"
  elif [[ $force ]] ; then
    shn_echo "$MSH_SHN_PROJECTS_DIR/$dirn"
    return 0
  elif ! [[ -d "$MSH_SHN_PROJECTS_DIR/$dirn" ]] ; then
    shn_errmsg "No directory : $MSH_SHN_PROJECTS_DIR/$dirn"
  else
    shn_errmsg "Why"
  fi
  return 1
}
function shn_ln ()
{
  /bin/ln $@
}
function shn_cp ()
{
  /bin/cp $@
}
function shn_mv ()
{
  /bin/mv $@
}
function shn_rename ()
{
  /usr/bin/rename $@
}
function shn_chmod ()
{
  /bin/chmod $@
}
function shn_rm ()
{
  [[ "$1" ]] && /bin/rm $@
}
function shn_mkdir ()
{
  [[ "$1" ]] && /bin/mkdir $@
}
function shn_cat ()
{
  [[ "$1" ]] && /bin/cat $@
}
function shn_sed ()
{
  /bin/sed $@
}
function shn_dirname ()
{
  shn_dbgmsg "dirname $@" >&2
# [[ "$1" ]] && /bin/dirname "$@"
  [[ "$1" ]] && dirname "$@"
}
function shn_basename ()
{
  shn_dbgmsg "basename $@"
# [[ "$1" ]] && /bin/basename "$@"
  [[ "$1" ]] && basename "$@"
}
function shn_realpath ()
{
  shn_dbgmsg "realpath $@"
# [[ "$1" ]] && /usr/bin/realpath "$@"
  [[ "$1" ]] && realpath "$@"
}
function shn_pwd ()
{
# shn_msg "pwd:`pwd`" 
# shn_msg "PWD:           $PWD"
# builtin pwd
  echo $PWD
# shn_msg PWD:           $PWD
}
function shn_funlist ()
{
  local prefix=$1
  if [[ "$prefix" ]] ; then
    declare -f | sed -ne "s/^\\(${prefix}_[[:alnum:]_]\\+\\)[[:blank:]]*().*$/\\1/p"
  fi
}
function shn_unset_functions ()
{
  local prefix=$1 f
  if [[ "$prefix" ]] ; then
    local fl
    fl=$( shn_funlist $prefix )
    if [[ "$fl" ]] ; then
      for f in $fl ; do
	if type -t shn_dbgmsg &>/dev/null ; then
	  shn_dbgmsg "UNSET $f"
	fi
	unset $f
      done
    fi
  fi
}
function shn_unset_all
{
  shn_unset_functions shn
  unset command_not_found_handle
  unset MSH_SHN_DIRS MSH_SHN_FLAVOUR MSH_SHN_PROJECTS MSH_SHN_PROJECTS_DIR MSH_SHN_PROJECT_DIR MSH_SHN_PROJECT_FUNCTIONS MSH_SHN_REAL_THIS MSH_SHN_SETUP_FUNCTIONS MSH_SHN_WORK_FUNCTIONS
}
function shn_filter_errors ()
{
  grep -v '^\(\s*CC\|cc1\|Making\s\+\(install\s\+in\s\+\(inc\|src\)\)\)\>' $* | sed -e "s@^$MSH_SHN_PROJECT_DIR/*@@"  | head -30 | cat -n >&2
}
function shn_show_errors ()
{
  local MSH_SHN_MSG_NOCOLOR
  local ername=$1 l
  if [[ "$ername" ]] && [[ -s "$ername" ]] && ! [[ "$shn_ignore_error" ]]; then
    l=`shn_filter_errors $ername 2>&1 | /bin/wc -l`
    if [[ "$l" && "$l" -gt 0 && "$l" -lt 300 ]] ; then
      if [[ $TERM == dumb ]] ; then
        shn_msg "---------- $l lines -----------------------------------------$MSH_SHN_PROJECT_FULLNAME"
        MSH_SHN_MSG_NOCOLOR=yes
	shn_filter_errors "$ername"
      else
        shn_msg "----------------------------------------------------------"
	shn_filter_errors "$ername"
	shn_msg "----- ... ------------------------------------------------"
      fi
    fi
    shn_msg "-- see $ername" 
  fi
}
function command_not_found_handle ()
{
  if type -t shn_errmsg &>/dev/null; then
    shn_errmsg "command NOT found '$@'"
  else
    echo "command NOT found '$@'" >&2
  fi
}
return 0
