function shn_project_load ()
{
  local lib libt libx
  for lib in tools setup build run ; do
    libt="${MSH_SHN_DIR:-shn}/lib${lib}.bash"
    if ! [[ -f "$libt" ]] || ! . "$libt" ; then    
      shn_errmsg "loading environment $libt"
      return 1
    else
#     shn_msg "Loaded $libt"
      for libx in ${MSH_SHN_DIR:-shn}/lib${lib}_*.bash ; do
        if [[ -f "$libx" ]] ; then
	  if ! . "$libx" ; then
	    shn_errmsg "loading environment $libx"
	    return 1
#         else
#           shn_msg "Loaded $libx"
	  fi
	fi
      done
    fi
    shn_dbgmsg "loaded lib${lib}" >&2
#   shn_msg "Loaded all of $lib"
  done
}
shn_project_load $@
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
  if [[ "${MSH_SHN_PROJECTS[@]}" ]]  ; then
    local projects_realdir
    shn_dbgmsg "Have MSH_SHN_PROJECTS (${#MSH_SHN_PROJECTS[@]})"
    if [[ "$match" == '/' ]] ; then
      shn_echo "$MSH_SHN_PROJECTS_DIR"
      return 0
    fi
    for (( i=0 ; $i < ${#MSH_SHN_PROJECTS[@]} ; i++ )) ; do
      if shn_project_match ${MSH_SHN_PROJECTS[$i]} $match ; then
	match_cnt=$(( $match_cnt + 1 ))
      fi
    done
    if [[ "$match_cnt" -gt 1 ]] && [[ "$match" ]] ; then
      shn_warn "$match_cnt matches for '$match' (go first):"
      for (( i=0 ; $i < ${#MSH_SHN_PROJECTS[@]} ; i++ )) ; do
	if shn_project_match ${MSH_SHN_PROJECTS[$i]} $match ; then
	  shn_warn "${i}. ${MSH_SHN_PROJECTS[$i]}"
	fi
      done      
    fi
    for (( i=0 ; $i < ${#MSH_SHN_PROJECTS[@]} ; i++ )) ; do
      if ! [[ "$match" ]] ; then
	shn_msgn "${i}. ${MSH_SHN_PROJECTS[$i]}"
	if [[ "${MSH_SHN_PROJECTS_DIR}/${MSH_SHN_PROJECTS[$i]}" == "${MSH_SHN_PROJECT_DIR}" ]] ; then
	  shn_msg "		* ${MSH_SHN_PROJECTS_DIR}/${MSH_SHN_PROJECTS[$i]} ? ${MSH_SHN_PROJECT_DIR}"
	else
	  shn_msg
	fi
      elif shn_project_match ${MSH_SHN_PROJECTS[$i]} $match ; then
	shn_dbgmsg "Match for ${MSH_SHN_PROJECTS[$i]}"
	projects_realdir=`shn_project_dir2realpath ${MSH_SHN_PROJECTS[$i]}` || return 1
	shn_dbgmsg "dir is '$projects_realdir'"
	shn_echo "$projects_realdir"
	return 0
      fi
    done
    if [[ "$match" ]] ; then
      shn_errmsg "FAIL $FUNCNAME - not found $match"
    fi
    shn_errmsg Why
  else
    shn_errmsg "FAIL $FUNCNAME - didn't setup - MSH_SHN_PROJECTS not set"
  fi
  return 1
}
function shn_project_by_file ()
{
  local retcode=0
  local file=$1 ffile prj found prjdir
  shn_dbgmsg "$file --- $rfile"
  [[ -f "$file" ]] && rfile=`shn_realpath $file` && file=$rfile
  shn_dbgmsg "$file --- $rfile"
  if [[ "$file" ]] ; then
  # TODO : first MSH_SHN_PROJECT_DIR !
    if pushd $MSH_SHN_PROJECTS_DIR &>/dev/null ; then
      [[ -f "$file" ]] && rfile=`shn_realpath $file` && file=$rfile
      shn_dbgmsg "$file --- $rfile"
      for prj in $MSH_SHN_PROJECT_DIR ${MSH_SHN_PROJECTS[@]} ; do
	prjdir="$MSH_SHN_PROJECTS_DIR/$prj"
	if [[ -d "$prjdir" ]] ; then
	  if [[ -f "$file" ]] && [[ "$file" == $prjdir/* ]] ; then
	    found="$file"
	  elif [[ -f "$prjdir/src/$file" ]] ; then
	    found="$prjdir/src/$file"
	  elif [[ -f "$prjdir/src/inc/$file" ]] ; then
	    found="$prjdir/src/inc/$file"
	  elif [[ -f "$prjdir/inc/$file" ]] ; then
	    found="$prjdir/inc/$file"
	  fi
	  if [[ "$found" ]]
  #		  || ffile=`ls -1 $prjdir/src/*$file* 2>/dev/null` \
  #		  || ffile=`ls -1 $prjdir/src/inc/*$file* 2>/dev/null` \
  #		  || ffile=`ls -1 $prjdir/inc/*$file* 2>/dev/null`
	  then
	    echo -n $prj
	    retcode=0
	    break
	  fi
	fi
      done
      popd &>/dev/null
    fi
  else
    retcode=1
  fi
  return $retcode
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
  shn_msg "prj:$prj"
  shn_dbgmsg "$file => $prj"
  shn_project_cd "$prj"
}
# function shn_file_edit ()
# {
#   if [[ "$@" ]] ; then
#     shn_project_file_cd $@
#     gvim_caller2 $@
#     shn_project_cd -
#   else
#     gvim_caller2
#   fi
# }
function shn_project_cd ()
{
  local p pp prj
  prj=${1:-${MSH_SHN_PROJECT_NAME:-zoctypes}}
  if [[ "$prj" == '-' ]] ; then
    prj=$MSH_SHN_PREV_PROJECT_NAME
  fi
  p=`shn_project_path $prj` || return 1
# p=`shn_project_path $prj` || { cd "${MSH_SHN_PROJECT_DIR}" ; return 1 ; }
  shn_dbgmsg "cd to '$p'"
  MSH_SHN_PREV_PROJECT_NAME=$MSH_SHN_PROJECT_NAME
  pp=$PWD
  cd $p >&2 || return $?
  shn_setup_projects
  shn_dbgmsg "after setup $LINENO $FUNCNAME" >&2
# if ! [[ $p == $pp ]] ; then
#   git status .
# fi
  return 0
}
function shn_project_each_control_c ()
{
  interrupted=$(( $interrupted +1 ))
  shn_msg " -----> Ctrl-C"
# trap - INT
}
function shn_project_each ()
{
  local match=$1
  shift
  local index_from=$1
  shift
  local project_index=0 interrupted=0
  local projects_realdir prj retcode=0
  if [[ "${MSH_SHN_ENABLED_PROJECTS[@]}" ]]  ; then
    shn_dbgmsg "1 At each `pwd`"
    trap shn_project_each_control_c INT
    for (( project_index=$index_from ; $project_index < ${#MSH_SHN_ENABLED_PROJECTS[@]} ; project_index++ )) ; do
      prj=${MSH_SHN_ENABLED_PROJECTS[$project_index]}
#     wsleep
      shn_dbgmsg "--- $prj ? $match"
#     shn_msgn "+-- ${MSH_SHN_ENABLED_PROJECTS[$project_index]}		"
      if shn_project_match "$prj" "$match" ; then
	projects_realdir=`shn_project_dir2realpath ${MSH_SHN_ENABLED_PROJECTS[$project_index]}` || return 1
	pushd $projects_realdir &>/dev/null
         shn_dbgmsg "1:$1 2:$2 3:$3 4:$4 index_from:$index_from prj:$prj --- $@"
#        shn_fmsg  "--- %02d %s >> %s" $project_index ${MSH_SHN_ENABLED_PROJECTS[$project_index]} "`pwd`"
         shn_fmsg  "--- %02d %-40s"       $project_index ${MSH_SHN_ENABLED_PROJECTS[$project_index]}
	 shn_dbgmsg "EACH `pwd` -- $@"
	 if true ; then
	   $@ || { retcode=$? ; popd &>/dev/null ; break ; }
	 fi
	 shn_dbgmsg "3 At each `pwd`"
	popd &>/dev/null
	shn_dbgmsg "4 At each `pwd`"
#     else
#       shn_msg "-- NOT Match '$prj' ? '$match'"
      fi
#     shn_msgn " (+ $project_index)"
      shn_dbgmsg "5 At each `pwd`"
      if [[ "$interrupted" -gt 0 ]] ; then
        break
      fi
#     shn_msg " . $project_index"
      shn_msg
    done
    trap - SIGINT
    shn_msg " (Done $project_index of ${#MSH_SHN_ENABLED_PROJECTS[@]} ; last: $MSH_SHN_PROJECT_NAME : $MSH_SHN_PROJECT_DIR)"
    if [[ "${MSH_SHN_DIRS[status]}" ]] && [[ -d "${MSH_SHN_DIRS[status]}" ]] ; then
      if [[ $project_index -eq ${#MSH_SHN_ENABLED_PROJECTS[@]} ]] ; then
        if [[ -f "${MSH_SHN_DIRS[status]}/last" ]] ; then
	  rm "${MSH_SHN_DIRS[status]}/last"
	  unset MSH_SHN_STATUS
	fi
      else
	echo -n "$project_index $@" >> "${MSH_SHN_DIRS[status]}/last"
	MSH_SHN_STATUS="$project_index $@"
      fi
    fi
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
  if pushd . &>/dev/null ; then
    if ! [[ -L shn ]] && type -t shn_project_cd &>/dev/null ; then
      shn_project_cd
      shn_dbgmsg "after shn_project_cd $LINENO $FUNCNAME" >&2
    fi
    if [[ -f shn/libwork.bash ]] ; then
      . shn/libwork.bash
      retcode=$?
    fi
    popd &>/dev/null
  fi
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
  for k in ${!MSH_SHN_LAST_ACTION[@]} ; do
    if [[ "$k" =~ ^(.*):(.*)$ ]] ; then
      project=${BASH_REMATCH[1]}
      action=${BASH_REMATCH[2]}
      if ! [[ "$prjpat" ]] || [[ "$project" == *$prjpat* ]] ; then
	if ! [[ "$actpat" ]] || [[ "$action" == *$actpat* ]] ; then
	  echo "$project :: $action - ${MSH_SHN_LAST_ACTION[$k]}"
	fi
      fi
    fi
  done
}

MSH_SHN_PROJECT_FUNCTIONS="`shn_funlist shn_project`"
[[ "$MSH_SHN_PROJECT_FUNCTIONS" ]] && export -f $MSH_SHN_PROJECT_FUNCTIONS
# shn_msg "$(datemt) v.`shn_project_version`"
return 0
