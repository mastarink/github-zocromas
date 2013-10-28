function shn_echo ()
{
# echo $*
# echo $@
  builtin echo "$*"
# echo "$@"
}
function shn_msg ()
{
  shn_echo "$*"  >&2
# shn_echo $@  >&2
# shn_echo "$*"  >&2
# shn_echo "$@"  >&2
}
function shn_fmsg ()
{
  local fmt=$1
  shift
  builtin printf "$fmt" $*  >&2
}
function shn_warn ()
{
  shn_echo "$*"  >&2
}
function shn_dbgmsg ()
{
  [[ "$MAS_SHN_DEBUG" ]] && shn_msg "$*"
}
function shn_errmsg ()
{
  shn_msg ERROR "$*"
  return 1
}
function shn_project_dir2realpath ()
{
  if [[ "$1" ]] && [[ "$MAS_SHN_PROJECTS_DIR" ]] && [[ -d "$MAS_SHN_PROJECTS_DIR" ]] && [[ -d "$MAS_SHN_PROJECTS_DIR/$1" ]] ; then
    shn_echo "$MAS_SHN_PROJECTS_DIR/$1"
    return 0
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
  /bin/rm $@
}
function shn_mkdir ()
{
  /bin/mkdir $@
}
function shn_cat ()
{
  /bin/cat $@
}
function shn_sed ()
{
  /bin/sed $@
}
function shn_dirname ()
{
  shn_dbgmsg "dirname $@" >&2
  /bin/dirname "$@"
}
function shn_basename ()
{
  shn_dbgmsg "basename $@"
  /bin/basename "$@"
}
function shn_realpath ()
{
  shn_dbgmsg "realpath $@"
  /usr/bin/realpath "$@"
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
  unset MAS_SHN_DIRS MAS_SHN_FLAVOUR MAS_SHN_PROJECTS MAS_SHN_PROJECTS_DIR MAS_SHN_PROJECT_DIR MAS_SHN_PROJECT_FUNCTIONS MAS_SHN_REAL_THIS MAS_SHN_SETUP_FUNCTIONS MAS_SHN_WORK_FUNCTIONS
}
function shn_show_errors ()
{
  local ername=$1 l
  if [[ "$ername" ]] && [[ -s "$ername" ]] && ! [[ "$shn_ignore_error" ]]; then
    l=`shn_cat $ername | /bin/wc -l`
    if [[ "$l" && "$l" -gt 0 && "$l" -lt 12 ]] ; then
      shn_msg "--------------------------------------------------"
      shn_cat -n "$ername" >&2
      shn_msg "--------------------------------------------------"
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
