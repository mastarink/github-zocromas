# unset MAS_SHN_PROJECTS_DIR
# unset MAS_SHN_PROJ_FUNCTIONS
# unset MAS_SHN_DIR
# unalias j &>/dev/null
# unset command_not_found_handle 
# export MAS_SHN_LIBWORK_LOADED=`datemt`


for lib in tools project shn ; do
  if ! [[ -L shn ]] || ! [[ -f ${MAS_SHN_DIR:-shn}/lib${lib}.bash ]] || ! . ${MAS_SHN_DIR:-shn}/lib${lib}.bash ; then
    shn_errmsg "loading environment ${MAS_SHN_DIR:-shn}/lib${lib}.bash"
    return 1
  fi
  shn_dbgmsg "loaded lib${lib}" >&2
done



# type -t shn_unset_functions &>/dev/null && shn_unset_functions shn

alias j='shn_project_cd'
alias jf='shn_project_file_cd'
alias lc='shn_project_files'
alias qf='shn_project_by_file'
alias ef='shn_file_edit'
alias run='shn m i r'
if [[ "$MAS_SHLIB" ]] && [[ -d "$MAS_SHLIB" ]] && [[ -f "$MAS_SHLIB/libed2.bash" ]] ; then
  . $MAS_SHLIB/libed2.bash
  alias e='gvim_caller2'
fi

# alias lshn='source shn/libwork.bash'
alias lshn='shn_load'

MAS_SHN_WORK_FUNCTIONS="`shn_funlist shn_work`" || return $?
[[ "$MAS_SHN_WORK_FUNCTIONS" ]] && export -f $MAS_SHN_WORK_FUNCTIONS
# shn_msg 'loaded working shell libs (shn)'
return 0
