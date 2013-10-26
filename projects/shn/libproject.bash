for lib in tools setup build ; do
  if ! [[ -f ${MAS_SHN_DIR:-shn}/lib${lib}.bash ]] || ! . ${MAS_SHN_DIR:-shn}/lib${lib}.bash ; then
    shn_errmsg "loading environment ${MAS_SHN_DIR:-shn}/lib${lib}.bash"
    return 1
  fi
  shn_dbgmsg "loaded lib${lib}" >&2
done

function shn_project_match ()
{
  local project=$1
  shift
  local match=$1
  shift
  ! [[ "$match" ]] || [[ "$project" == *$match* ]]
}

function shn_project_path ()
{
  local match=$1
  shift
  local i match_cnt
  if [[ "${MAS_SHN_PROJECTS[@]}" ]]  ; then
    local projects_realdir
    shn_dbgmsg "Have MAS_SHN_PROJECTS (${#MAS_SHN_PROJECTS[@]})"
    if [[ "$match" == '/' ]] ; then
      shn_echo "$MAS_SHN_PROJECTS_DIR"
      return 0
    fi
    for (( i=0 ; $i < ${#MAS_SHN_PROJECTS[@]} ; i++ )) ; do
      if shn_project_match ${MAS_SHN_PROJECTS[$i]} $match ; then
	match_cnt=$(( $match_cnt + 1 ))
      fi
    done
    if [[ "$match_cnt" -gt 1 ]] && [[ "$match" ]] ; then
      shn_warn "$match_cnt matches for '$match' (go first):"
      for (( i=0 ; $i < ${#MAS_SHN_PROJECTS[@]} ; i++ )) ; do
	if shn_project_match ${MAS_SHN_PROJECTS[$i]} $match ; then
	  shn_warn "${i}. ${MAS_SHN_PROJECTS[$i]}"
	fi
      done      
    fi
    for (( i=0 ; $i < ${#MAS_SHN_PROJECTS[@]} ; i++ )) ; do
      if ! [[ "$match" ]] ; then
	shn_msg "${i}. ${MAS_SHN_PROJECTS[$i]}"
      elif shn_project_match ${MAS_SHN_PROJECTS[$i]} $match ; then
	shn_dbgmsg "Match for ${MAS_SHN_PROJECTS[$i]}"
	projects_realdir=`shn_project_dir2realpath ${MAS_SHN_PROJECTS[$i]}` || return 1
	shn_dbgmsg "dir is '$projects_realdir'"
	shn_echo $projects_realdir
	return 0
      fi
    done
    if [[ "$match" ]] ; then
      shn_errmsg "FAIL cdproject - not found $match"
    fi
  else
    shn_errmsg "FAIL cdproject - didn't setup"
  fi
  return 1
}
function shn_project_cd ()
{
  local p
  p=`shn_project_path $@` || return 1
  shn_dbgmsg "cd to '$p'"
  cd $p >&2
}
function shn_project_each ()
{
  local match=$1 retcode=0
  shift
  local project_index=0
  local projects_realdir
  if [[ "${MAS_SHN_ENABLED_PROJECTS[@]}" ]]  ; then
    shn_dbgmsg "1 At each `pwd`"
    for (( project_index=0 ; $project_index < ${#MAS_SHN_ENABLED_PROJECTS[@]} ; project_index++ )) ; do
      shn_dbgmsg "2 At each `pwd`"
      shn_dbgmsg "--- ${MAS_SHN_ENABLED_PROJECTS[$project_index]} ? $match"
      if shn_project_match ${MAS_SHN_ENABLED_PROJECTS[$project_index]} $match ; then
        projects_realdir=`shn_project_dir2realpath ${MAS_SHN_ENABLED_PROJECTS[$project_index]}` || return 1
	pushd $projects_realdir &>/dev/null
	 shn_dbgmsg "EACH `pwd` -- $@"
	 $@ || { retcode=$? ; popd &>/dev/null ; break ; }
	 shn_dbgmsg "3 At each `pwd`"
	popd &>/dev/null
	shn_dbgmsg "4 At each `pwd`"
      fi
      shn_dbgmsg "5 At each `pwd`"
    done
  fi
  return $retcode
}
function shn_exit ()
{
  shn_msg "shn EXIT"
}
function shn_load ()
{
  if [[ -f shn/libwork.bash ]] ; then
    . shn/libwork.bash
  fi
}
function shn_last ()
{
  local prjpat actpat
  prjpat=$1
  shift
  actpat=$1
  shift
  local project action
  local k
  for k in ${!MAS_SHN_LAST_ACTION[@]} ; do
    if [[ "$k" =~ ^(.*):(.*)$ ]] ; then
      project=${BASH_REMATCH[1]}
      action=${BASH_REMATCH[2]}
      if ! [[ "$prjpat" ]] || [[ "$project" == *$prjpat* ]] ; then
	if ! [[ "$actpat" ]] || [[ "$action" == *$actpat* ]] ; then
	  echo "$project :: $action - ${MAS_SHN_LAST_ACTION[$k]}"
	fi
      fi
    fi
  done
}
function shn ()
{
# export MAS_SHN_DEBUG=yes  
  shn_dbgmsg 1 shn
  shn_dbgmsg 2a shn
  local retcode=0
#?trap shn_exit EXIT
  shn_dbgmsg "project $MAS_SHN_PROJECT_NAME"
  shn_setup_projects || shn_project_cd "${MAS_SHN_PROJECT_NAME:-zocromas_zoctypes}" || { retcode=$? ; shn_errmsg shn setup ; return $retcode ; }
  shn_dbgmsg 3 shn
  if [[ "$1" == each ]] ; then
    shift
    shn_msg "Will install to ${MAS_SHN_DIR[flavour]}"
    shn_project_each '' shn $@
  else
    local act_list="$@"
    local shn_dont_setup=yes
    
#   local shn_ignore_error=yes
    shn_fmsg  "[--%02d----------- %30s ------------- %s]\n" ${project_index:-0} $MAS_SHN_PROJECT_NAME `datemt`
    while [[ "$1" ]] ; do
      local code=$1
      shift
      shn_dbgmsg "shn 1 -- $code"
      if [[ "$code" ]] ; then
	shn_dbgmsg "shn 2 -- $code"
	case $code in
	  l)
	    # pwd >&2 || return $?
	    MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:list]=`datemt`
	  ;;
	  c)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_configure || { retcode=$? ; shn_errmsg 2.${code} shn ;  break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;
	  C)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_clean || { retcode=$? ; shn_errmsg 2.${code} shn ;  break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;
	  D)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_distclean || { retcode=$? ; shn_errmsg 2.${code} shn ;  break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;
	  U)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_superclean || { retcode=$? ; shn_errmsg 2.${code} shn ;  break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;
	  a)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_autoconf || { retcode=$? ; shn_errmsg 2.${code} shn ; break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;	  
	  e)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_ebuild_update    || { retcode=$? ; shn_errmsg 2.${code} shn ; break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;
	  E)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_ebuild_check    || { retcode=$? ; shn_errmsg 2.${code} shn ; break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;	  
	  t)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_dist      || { retcode=$? ; shn_errmsg 2.${code} shn ; break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;
	  m)
	    shn_dbgmsg "shn 2.${code}.1"
	    shn_build_make      || { retcode=$? ; shn_errmsg 2.${code} shn ; break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.2"
	  ;;
	  i)
	    shn_dbgmsg "shn 2.${code}.1"
  #	  shn_build_make      || return $?
  #         shn_dbgmsg "shn 2.${code}.2"
	    shn_build_install || { retcode=$? ; shn_errmsg 2.${code} shn ; break ; }
	    shn_dbgmsg shn $code ok
	    shn_dbgmsg "shn 2.${code}.3"
	  ;;
	  *)
	    shn_errmsg "wrong code '$code'"
	    return 1
	  ;;
	esac
	shn_dbgmsg "shn 3 -- $code"
      fi
      shn_dbgmsg "shn 4 -- $code ; next:$1"
    done
  fi
  shn_dbgmsg "shn 5 -- $code"
  shn_dbgmsg shn "  <`datemt`> end($retcode)" $act_list -- ${MAS_SHN_PROJECT_NAME}
# shn_pwd
  return $retcode
}
MAS_SHN_PROJECT_FUNCTIONS="`shn_funlist shn_project`"
[[ "$MAS_SHN_PROJECT_FUNCTIONS" ]] && export -f $MAS_SHN_PROJECT_FUNCTIONS

return 0
