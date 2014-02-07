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
  shn_echo "$*"  >&2
# shn_echo $@  >&2
# shn_echo "$*"  >&2
# shn_echo "$@"  >&2
}
function shn_msgn ()
{
  shn_echon "$*"  >&2
# shn_echon $@  >&2
# shn_echon "$*"  >&2
# shn_echon "$@"  >&2
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
  shn_msg ERROR "$*"
  return 1
}
function shn_project_dir2realpath ()
{
  if [[ "$1" ]] && [[ "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR/$1" ]] ; then
    shn_echo "$MSH_SHN_PROJECTS_DIR/$1"
    return 0
  elif ! [[ -d "$MSH_SHN_PROJECTS_DIR/$1" ]] ; then
    shn_errmsg "No directory : $MSH_SHN_PROJECTS_DIR/$1"
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
  [[ "$1" ]] && /bin/dirname "$@"
}
function shn_basename ()
{
  shn_dbgmsg "basename $@"
  [[ "$1" ]] && /bin/basename "$@"
}
function shn_realpath ()
{
  shn_dbgmsg "realpath $@"
  [[ "$1" ]] && /usr/bin/realpath "$@"
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
function shn_show_errors ()
{
  local ername=$1 l
  if [[ "$ername" ]] && [[ -s "$ername" ]] && ! [[ "$shn_ignore_error" ]]; then
    l=`shn_cat $ername | /bin/wc -l`
    if [[ "$l" && "$l" -gt 0 && "$l" -lt 300 ]] ; then
      shn_msg "-------------------------------------------------------"
      shn_cat -n "$ername" | head -30 >&2
      shn_msg "-- ... ------------------------------------------------"
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
