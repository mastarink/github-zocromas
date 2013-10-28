for lib in tools setup build run ; do
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
      shn_errmsg "FAIL $FUNCNAME - not found $match"
    fi
  else
    shn_errmsg "FAIL $FUNCNAME - didn't setup"
  fi
  return 1
}
function shn_project_by_file ()
{
  local file=$1 ffile prj found prjdir
  if [[ "$file" ]] ; then
    for prj in ${MAS_SHN_PROJECTS[@]} ; do
      prjdir="$MAS_SHN_PROJECTS_DIR/$prj"
      if [[ -d "$prjdir" ]] ; then
	if [[ -f "$prjdir/src/$file" ]] ; then
	  found="$prjdir/src/$file"
	elif [[ -f "$prjdir/src/inc/$file" ]] ; then
	  found="$prjdir/src/inc/$file"
	elif [[ -f "$prjdir/inc/$file" ]] ; then
	  found="$prjdir/inc/$file"
	fi
	if [[ "$found" ]] \
		  || ffile=`ls -1 $prjdir/src/*$file* 2>/dev/null` \
		  || ffile=`ls -1 $prjdir/src/inc/*$file* 2>/dev/null` \
		  || ffile=`ls -1 $prjdir/inc/*$file* 2>/dev/null`
	then
	  echo -n $prj
	  return 0
	fi
      fi
    done
  else
    return 1
  fi
  return 1
}
function shn_project_files ()
{
  local name=$1
  local cmd='ls -l'
  if [[ "$name" ]] ; then
    shn_project_cd $name || shn_project_file_cd $name || return 1
  fi
  if [[ -f "configure.ac" ]] && [[ -d src ]] ; then
    cmd="$cmd src/*.c"
    if [[ -d src/inc ]] ; then
      cmd="$cmd src/inc/*.h"
    fi
  fi
  if [[ -d inc ]] ; then
    cmd="$cmd inc/*.h"
  fi
  eval $cmd
}
function shn_project_file_cd ()
{
  local prj file=$1
  prj=`shn_project_by_file $@` || return $?
# shn_msg "$file => $prj"
  shn_project_cd "$prj"
}
function shn_file_edit ()
{
  shn_project_file_cd $@
  gvim_caller2 $@
}
function shn_project_cd ()
{
  local p prj
  prj=$1
  if [[ "$prj" == '-' ]] ; then
    prj=$MAS_SHN_PREV_PROJECT_NAME
  fi
  p=`shn_project_path $prj` || return 1
  shn_dbgmsg "cd to '$p'"
  MAS_SHN_PREV_PROJECT_NAME=$MAS_SHN_PROJECT_NAME
  cd $p >&2 || return $?
  shn_setup_projects
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
  local retcode=0
  pushd &>/dev/null
  if ! [[ -L shn ]] && type -t shn_project_cd &>/dev/null ; then
    shn_project_cd zoctypes
  fi
  if [[ -f shn/libwork.bash ]] ; then
    . shn/libwork.bash
    retcode=$?
  fi
  popd &>/dev/null
  return $retcode
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

MAS_SHN_PROJECT_FUNCTIONS="`shn_funlist shn_project`"
[[ "$MAS_SHN_PROJECT_FUNCTIONS" ]] && export -f $MAS_SHN_PROJECT_FUNCTIONS

return 0
