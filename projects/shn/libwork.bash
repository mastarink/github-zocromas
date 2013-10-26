unset MAS_SHN_PROJECTS_DIR
unset MAS_SHN_PROJ_FUNCTIONS
unset MAS_SHN_DIR
unalias j &>/dev/null
unset command_not_found_handle 
export MAS_SHN_LIBWORK_LOADED=`datemt`

for lib in tools project ; do
  if ! [[ -f ${MAS_SHN_DIR:-shn}/lib${lib}.bash ]] || ! . ${MAS_SHN_DIR:-shn}/lib${lib}.bash ; then
    shn_errmsg "loading environment ${MAS_SHN_DIR:-shn}/lib${lib}.bash"
    return 1
  fi
  shn_dbgmsg "loaded lib${lib}" >&2
done

shn_dbgmsg "loaded libproject" >&2

# type -t shn_unset_functions &>/dev/null && shn_unset_functions shn

alias j='shn_project_cd'
alias lshn='source shn/libwork.bash'
MAS_SHN_WORK_FUNCTIONS="`shn_funlist shn_work`" || return $?
[[ "$MAS_SHN_WORK_FUNCTIONS" ]] && export -f $MAS_SHN_WORK_FUNCTIONS
return 0
