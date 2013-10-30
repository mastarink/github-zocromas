declare -gx MAS_SHN_PROJECTS_DIR MAS_SHN_PRJTOP_DIR MAS_SHN_PROJECT_NAME MAS_SHN_PREV_PROJECT_NAME 
declare -gx MAS_SHN_PROJECT_FULLNAME MAS_SHN_PROJECT_DIR MAS_SHN_PROJECT_RDIR
declare -A MAS_SHN_DIR
declare MAS_SHN_DEBUG
declare -gx MAS_SHN_FLAVOUR MAS_SHN_REAL_THIS
declare -a MAS_SHN_PROJECTS
declare -a MAS_SHN_DISABLED_PROJECTS
declare -a MAS_SHN_ENABLED_PROJECTS
declare -A MAS_SHN_HASH_PROJECTS
declare -A MAS_SHN_LAST_ACTION

# MAS_SHN_DEBUG=yes

function shn_setup_zero ()
{
  local here_dir=$PWD
  local this=${BASH_SOURCE[0]}
  local this_dir=$( shn_dirname $this )
  local project_ac
  if ! [[ -f "configure.ac" ]] ; then
    return 1
  fi
  if ! [[ -f "shn/libsetup.bash" ]] ; then 
    return 1
  fi
  MAS_SHN_REAL_THIS=$( shn_realpath $this ) || return 1
  
  shn_dbgmsg "here_dir:$here_dir"
  shn_dbgmsg "0:$0"
  shn_dbgmsg "this:$this"
  pushd $this_dir &>/dev/null
    MAS_SHN_PROJECT_RDIR=$( shn_dirname $PWD )
    MAS_SHN_PROJECT_DIR=$( shn_realpath $MAS_SHN_PROJECT_RDIR )
    shn_dbgmsg "MAS_SHN_PROJECT_DIR:$MAS_SHN_PROJECT_DIR"
  popd &>/dev/null
  project_ac="$MAS_SHN_PROJECT_DIR/configure.ac"
  if [[ -f "$project_ac" ]] ; then
    MAS_SHN_PROJECT_NAME=$( shn_basename $MAS_SHN_PROJECT_DIR )
    project_ac=$( shn_realpath $project_ac )
  else
    shn_errmsg "absent project $project_ac"
    return 1
  fi
  return 0
}


if ! . ${MAS_SHN_DIR:-shn}/libtools.bash ; then
  shn_errmsg "loading environment ${MAS_SHN_DIR:-shn}/libtools.bash"
  return 1
fi
function shn_setup_global_dirs
{
  if [[ "$MAS_SHN_PROJECTS_DIR" ]] && [[ -d "$MAS_SHN_PROJECTS_DIR" ]] ; then
    MAS_SHN_DIR[top]=`shn_realpath "$MAS_SHN_PROJECTS_DIR/.."`
    MAS_SHN_DIR[admin]="${MAS_SHN_DIR[top]}/admin"
    MAS_SHN_DIR[save]="${MAS_SHN_DIR[admin]}/saved"
    MAS_SHN_DIR[savedist]="${MAS_SHN_DIR[save]}/dist"
    MAS_SHN_DIR[savegentoo]="${MAS_SHN_DIR[save]}/gentoo"
    MAS_SHN_DIR[ebuilds]="$MAS_SHN_PROJECTS_DIR/ebuilds"
    MAS_SHN_DIR[ebuild]="${MAS_SHN_DIR[ebuilds]}/mas-tar"
    MAS_SHN_DIR[install]="${MAS_SHN_DIR[admin]}/install"
    MAS_SHN_DIR[flavour]="${MAS_SHN_DIR[install]}/${MAS_SHN_FLAVOUR:-default}"
    MAS_SHN_DIR[error]="/tmp"
    MAS_SHN_DIR[files]=$MAS_SHN_PROJECTS_DIR/files

    for id in admin save savedist savegentoo ebuilds ebuild install flavour error files ; do
      if [[ "${MAS_SHN_DIR[$id]}" ]] && ! [[ -d "${MAS_SHN_DIR[$id]}" ]] ; then
	shn_mkdir "${MAS_SHN_DIR[$id]}" || return 1
	shn_msg created ${MAS_SHN_DIR[$id]}
      fi
    done

    shn_dbgmsg "MAS_SHN_PROJECTS_DIR:$MAS_SHN_PROJECTS_DIR"
    shn_dbgmsg "MAS_SHN_FLAVOUR:$MAS_SHN_FLAVOUR"
    shn_dbgmsg "top dir:${MAS_SHN_DIR[top]}"
    shn_dbgmsg "admin dir:${MAS_SHN_DIR[admin]}"
    shn_dbgmsg "install dir:${MAS_SHN_DIR[install]}"
  else
    shn_errmsg "can't set dir's"
    return 1
  fi
  return 0
}
function shn_setup_project_dirs
{
  if [[ "$MAS_SHN_PROJECT_DIR" ]] && [[ -d "$MAS_SHN_PROJECT_DIR" ]] ; then
    MAS_SHN_DIR[configure]=$MAS_SHN_PROJECT_DIR
    MAS_SHN_DIR[aux]=$MAS_SHN_PROJECT_DIR/.auxdir
    MAS_SHN_DIR[mased]=$MAS_SHN_PROJECT_DIR/mased
    MAS_SHN_DIR[debug]=$MAS_SHN_PROJECT_DIR/debug
    MAS_SHN_DIR[build]="${MAS_SHN_DIR[aux]}/.build"
    MAS_SHN_DIR[buildsrc]="${MAS_SHN_DIR[build]}/src"
    MAS_SHN_DIR[m4]="${MAS_SHN_DIR[aux]}/m4"
    MAS_SHN_DIR[error]="/tmp"

    shn_dbgmsg "aux:${MAS_SHN_DIR[aux]}"
    shn_dbgmsg "build:${MAS_SHN_DIR[build]}"

    for id in aux build m4 mased debug ; do
      if [[ "${MAS_SHN_DIR[$id]}" ]] && ! [[ -d "${MAS_SHN_DIR[$id]}" ]] ; then
	shn_mkdir "${MAS_SHN_DIR[$id]}" || return 1
	shn_msg created ${MAS_SHN_DIR[$id]}
      fi
    done
  else
    shn_errmsg "can't set dir's"
    return 1
  fi
  return 0
}
function shn_project_name ()
{
  if [[ -L shn ]] && [[ -f shn/libwork.bash ]] ; then
    echo -n "$MAS_SHN_PROJECT_NAME"
  else
    return 1
  fi
}
function shn_project_version ()
{
  if [[ -L shn ]] && [[ -f shn/libwork.bash ]] ; then
    if [[ "$MAS_SHN_PROJECT_DIR" ]] && [[ -d "$MAS_SHN_PROJECT_DIR" ]] ; then
      if pushd "$MAS_SHN_PROJECT_DIR" &>/dev/null && [[ -f "zocversion.txt" ]] ; then
	vseq=$( cat zocversion.txt )
	if type -t datem 2>&1 >/dev/null ; then
	  echo -n "${vseq}.$( datem )"
	else
	  echo -n "$vseq"
	fi
	popd &>/dev/null
      else
	echo -n '0.0.1'
      fi
    else
      return 1
    fi
  else
    return 1
  fi
  return 0
}
function shn_setup_projects ()
{
  local retcode=0
  local i
  if [[ "$shn_dont_setup" ]]  ; then return 0 ; fi
  shn_dbgmsg "$FUNCNAME"
  shn_dbgmsg "S1 `pwd`" >&2
  shn_setup_zero || return 1
  shn_setup_project_dirs || return 1
  shn_dbgmsg "S2 `pwd`" >&2
  shn_dbgmsg "MAS_SHN_REAL_THIS:$MAS_SHN_REAL_THIS"
  if ! [[ "`shn_basename $MAS_SHN_REAL_THIS`" == "libsetup.bash" ]] ; then
    shn_errmsg "BAD MAS_SHN_REAL_THIS:$MAS_SHN_REAL_THIS"
    return 1
  elif [[ -f "$MAS_SHN_REAL_THIS" ]] && [[ "`shn_basename $MAS_SHN_REAL_THIS`" == "libsetup.bash" ]] ; then
    local shn_dir=$( shn_dirname $MAS_SHN_REAL_THIS ) || return 1
    if [[ "$shn_dir" ]] && [[ -d "$shn_dir" ]] ; then
      shn_dbgmsg "shn_dir:$shn_dir"
      local projects_dir=$( shn_realpath $shn_dir/.. ) || return 1
      if [[ "$projects_dir" ]] && [[ -d "$projects_dir" ]] ; then
	shn_dbgmsg "projects_dir:$projects_dir"
	MAS_SHN_PROJECTS_DIR=$projects_dir
	MAS_SHN_PRJTOP_DIR=`shn_realpath "$MAS_SHN_PROJECTS_DIR/.."`
	shn_setup_global_dirs || return 1
	local projects_file_name=projects.list
	local projects_file=$projects_dir/$projects_file_name
	if [[ "$projects_file" ]] && [[ -f "$projects_file" ]] ; then
	  shn_dbgmsg "OK projects_file:$projects_file"
	  readarray -t MAS_SHN_PROJECTS < $projects_file || return 1
	  for (( i=0 ; $i < ${#MAS_SHN_PROJECTS[@]} ; i++ )) ; do
	    shn_dbgmsg "-${i}. [${MAS_SHN_PROJECTS[$i]}]"
	    MAS_SHN_HASH_PROJECTS[${MAS_SHN_PROJECTS[$i]}]=1
	  done
	else
	  shn_errmsg "FAIL projects_file:$projects_file"
	  return 1
	fi
        local projects_disabled_file_name=projects_disabled.list
        local projects_disables_file=$projects_dir/$projects_disabled_file_name
        if [[ "$projects_disables_file" ]] && [[ -f "$projects_disables_file" ]] ; then
          shn_dbgmsg "OK projects_disables_file:$projects_disables_file"
          readarray -t MAS_SHN_DISABLED_PROJECTS < $projects_disables_file || return 1
          for (( i=0 ; $i < ${#MAS_SHN_DISABLED_PROJECTS[@]} ; i++ )) ; do
            shn_dbgmsg "-${i}. DISABLED [${MAS_SHN_DISABLED_PROJECTS[$i]}]"
            if [[ "${MAS_SHN_DISABLED_PROJECTS[$i]}" ]] ; then
	      unset MAS_SHN_HASH_PROJECTS[${MAS_SHN_DISABLED_PROJECTS[$i]}]
	    fi
          done
        else
          shn_errmsg "FAIL projects_disables_file:$projects_disables_file"
          return 1
        fi
        unset MAS_SHN_ENABLED_PROJECTS
        for k in ${MAS_SHN_PROJECTS[@]} ; do
          if [[ "$k" ]] && [[ "${MAS_SHN_HASH_PROJECTS[$k]}" ]] ; then
            shn_dbgmsg "-${i}. ENABLE [$k]"
            MAS_SHN_ENABLED_PROJECTS[${#MAS_SHN_ENABLED_PROJECTS[@]}]=$k
          fi
        done
         
        for (( i=0 ; $i < ${#MAS_SHN_ENABLED_PROJECTS[@]} ; i++ )) ; do
          shn_dbgmsg "-${i}. ENABLED [${MAS_SHN_ENABLED_PROJECTS[$i]}]"
        done
	
	retcode=0
	
	shn_dbgmsg "@ at project $MAS_SHN_PROJECT_DIR"
	shn_dbgmsg "S3 `pwd`" >&2
	if MAS_SHN_PROJECT_FULLNAME="${MAS_SHN_PROJECT_NAME}-`shn_project_version`" ; then
	  shn_dbgmsg "S4 `pwd`" >&2
	  shn_setup_additional || { shn_errmsg additional ; retcode=$? ; }

  #	MAS_SHN_FLAVOUR=${MAS_SHN_FLAVOUR:-`cat $projects_dir/projects.flavour`}
	  if [[ "$MAS_SHN_PROJECTS_DIR" ]] && [[ -f "$MAS_SHN_PROJECTS_DIR/projects.flavour" ]] ; then
	    read MAS_SHN_FLAVOUR < "$MAS_SHN_PROJECTS_DIR/projects.flavour"
	  else
	    shn_errmsg "can't set MAS_SHN_FLAVOUR"
	    retcode=1
	  fi
	else
	  shn_errmsg "can't set MAS_SHN_PROJECT_FULLNAME"
	  retcode=1
	fi
      else
	shn_errmsg "FAIL projects_dir:$projects_dir"
	shn_errmsg "shn_dir:$shn_dir"
	shn_errmsg "MAS_SHN_REAL_THIS:$MAS_SHN_REAL_THIS"
	retcode=1
      fi
    else
      shn_errmsg "FAIL shn_dir:$shn_dir"
      shn_errmsg "MAS_SHN_REAL_THIMAS_SHN_REAL_THIS:$MAS_SHN_REAL_THIS"
      retcode=1
    fi
  else
    shn_errmsg "FAIL MAS_SHN_REAL_THIS:$MAS_SHN_REAL_THIS"
    retcode=1
  fi
  shn_dbgmsg "S5 ($retcode) `pwd`" >&2
  return $retcode
}
function shn_initial_src_mased_vim  ()
{
  local gs fname fn fnn
  gs=`grep '^AC_CONFIG_SRCDIR' $MAS_SHN_PROJECT_DIR/configure.ac` || return $?
  if [[ "$gs" ]] && [[ "$gs" =~ AC_CONFIG_SRCDIR\(\[src/(.*)\.c\]\) ]] ; then
    echo "find ${BASH_REMATCH[1]}.c" >> src.mased.vim || return $?
    echo "sfind ${BASH_REMATCH[1]}.h" >> src.mased.vim || return $?
    echo  >> src.mased.vim || return $?
    echo  >> src.mased.vim || return $?
    for fname in $MAS_SHN_PROJECT_DIR/src/*.c ; do
      fn=$( shn_basename $fname )
      if [[ "$fn" =~ ^(.*)\.c$ ]] ; then
	fnn=${BASH_REMATCH[1]}
	if [[ -f "$MAS_SHN_PROJECT_DIR/src/inc/${fnn}.h" ]] ; then
	  echo "tab sfind ${fnn}.c" >> src.mased.vim || return $?
	  echo "sfind ${fnn}.h" >> src.mased.vim || return $?
	  echo  >> src.mased.vim || return $?
	fi
      fi
    done
  fi
  return 0
}
function shn_initial_mased_vim
{
  local file link fn typf retval=0
  if [[ -d mased ]] ; then
    if pushd mased &>/dev/null ; then
       for typf in sh shn ac ; do
        fn="${typf}.mased.vim"
	if [[ -f "${MAS_SHN_DIR[files]}/mased/$fn" ]] ; then
	  file=`shn_realpath --relative-to=. ${MAS_SHN_DIR[files]}/mased/$fn` || { retval=$? ; break ; }
	  link=$( shn_basename $file ) || { retval=$? ; break ; }
	  if ! [[ -L $link ]] && ! [[ -f $link ]] ; then
	    shn_dbgmsg "$file -> $link"
	    shn_ln -s $file $link || { retval=$? ; break ; }
	    shn_msg created link $link
	  fi
	else
	  shn_errmsg ${MAS_SHN_DIR[files]}/mased/$fn
	  retval=1
	  break
	fi
      done        
      if ! [[ -f "src.mased.vim" ]] ; then
        shn_initial_src_mased_vim || { retval=$? ; break ; }
      fi
      popd &>/dev/null
    fi
  fi
  return $retval
}
function shn_initial_gitignore ()
{
  if ! [[ -f ".gitignore" ]] ; then
    echo '*.tmp' >> .gitignore
    shn_msg "created .gitignore"
  fi
  return 0
}
function shn_initial_debug
{
  local file link fn typf retval=0
  if [[ -d debug ]] ; then
    if pushd debug &>/dev/null ; then
      shn_initial_gitignore || { retval=$? ; break ; }
      popd &>/dev/null
    fi
  fi
  return $retval
}
function shn_setup_additional ()
{
  local fn file link
  if ! [[ -f "configure.ac" ]] ; then 
    shn_errmsg setup additional - no configure.ac
    return 1
  fi
  if [[ "${MAS_SHN_DIR[files]}" ]] && [[ -d "${MAS_SHN_DIR[files]}" ]] ; then 
  #  mased/sh.mased.vim
    for fn in gvim-funcs.vim  gvimrc-mastar  gvim-vimenter.vim  vimrc-mastar zocversion.txt ; do
      if [[ -f "${MAS_SHN_DIR[files]}/$fn" ]] ; then
        file=`shn_realpath --relative-to=. ${MAS_SHN_DIR[files]}/$fn` || return 1
	link=$( shn_basename $file )
	if ! [[ -L $link ]] && ! [[ -f $link ]] ; then
	  shn_dbgmsg "$file -> $link"
	  shn_ln -s $file $link || return 1
	  shn_msg created link $link
	fi
      else
        shn_errmsg "no file '${MAS_SHN_DIR[files]}/$fn'"
	return 1
      fi
    done
  else
    shn_errmsg "shn_setup_additional - ${MAS_SHN_DIR[files]}"
    return 1
  fi
  for fn in sh shn ; do
    if [[ -d "$MAS_SHN_PROJECTS_DIR/$fn" ]] ; then
      file=`shn_realpath --relative-to=. ${MAS_SHN_PROJECTS_DIR}/$fn`
      link=$( shn_basename $file )
      if ! [[ -L $link ]] && ! [[ -f $link ]] ; then
	shn_dbgmsg "$file -> $link"
	shn_ln -s $file $link || return 1
	shn_msg created link $link
      fi
    else
      shn_errmsg ${MAS_SHN_PROJECTS_DIR}/$fn
      return 1
    fi
  done
  for fn in .localrc ; do
    if [[ -f "$MAS_SHN_PROJECTS_DIR/$fn" ]] ; then
      file=`shn_realpath --relative-to=. ${MAS_SHN_PROJECTS_DIR}/$fn`
      link=$( shn_basename $file )
      if ! [[ -L $link ]] && ! [[ -f $link ]] ; then
	shn_dbgmsg "$file -> $link"
	shn_ln -s $file $link || return 1
	shn_msg created link $link
      fi
    else
      shn_errmsg ${MAS_SHN_PROJECTS_DIR}/$fn
      return 1
    fi
  done    
  shn_initial_mased_vim
  shn_initial_debug
  return 0
}

shn_dbgmsg before setup
shn_setup_projects
MAS_SHN_SETUP_FUNCTIONS="`shn_funlist shn_setup`"
# Don't: [[ "$MAS_SHN_SETUP_FUNCTIONS" ]] && export -f $MAS_SHN_SETUP_FUNCTIONS

return 0
