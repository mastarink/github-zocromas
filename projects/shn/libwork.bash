# unset MSH_SHN_PROJECTS_DIR
# unset MSH_SHN_PROJ_FUNCTIONS
# unset MSH_SHN_DIR
# unalias j &>/dev/null
# unset command_not_found_handle 
# export MSH_SHN_LIBWORK_LOADED=`datemt`

for lib in tools project shn zocserver zocgrep ; do
  if ! [[ -L shn ]] || ! [[ -f ${MSH_SHN_DIR:-shn}/lib${lib}.bash ]] || ! . ${MSH_SHN_DIR:-shn}/lib${lib}.bash ; then
    shn_errmsg "loading environment ${MSH_SHN_DIR:-shn}/lib${lib}.bash"
    shn_errmsg lib not found $lib
    return 1
  fi
  shn_dbgmsg "loaded lib${lib}" >&2
done



# type -t shn_unset_functions &>/dev/null && shn_unset_functions shn

alias j='shn_project_cd'
alias jf='shn_project_file_cd'
alias lc='shn_project_files'
alias qf='shn_project_by_file'
alias e='shn_file_edit'
alias run='shn m i r'
if [[ "$MSH_SHLIB" ]] && [[ -d "$MSH_SHLIB" ]] && [[ -f "$MSH_SHLIB/libed2.bash" ]] ; then
  . $MSH_SHLIB/libed2.bash
  alias eu='gvim_caller2'
fi

# alias lshn='source shn/libwork.bash'
alias lshn='shn_load'

MSH_SHN_WORK_FUNCTIONS="`shn_funlist shn_work`" || return $?
[[ "$MSH_SHN_WORK_FUNCTIONS" ]] && export -f $MSH_SHN_WORK_FUNCTIONS
# shn_msg 'loaded working shell libs (shn)'
return 0
