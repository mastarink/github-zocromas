declare -gx MSH_SHN_PROJECTS_DIR MSH_SHN_PRJTOP_DIR MSH_SHN_PROJECT_NAME MSH_SHN_PREV_PROJECT_NAME 
declare -gx MSH_SHN_PROJECT_FULLNAME MSH_SHN_PROJECT_DIR MSH_SHN_PROJECT_RDIR MSH_SHN_BUILD_DIR
declare MSH_SHN_DEBUG
declare -gx MSH_SHN_FLAVOUR
declare -gx MSH_SHN_STATUS

# MSH_SHN_DEBUG=yes

function shn_setup_zero ()
{
  declare -gx MSH_SHN_REAL_THIS
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
  MSH_SHN_REAL_THIS=$( shn_realpath $this ) || return 1
  
  shn_dbgmsg "here_dir:$here_dir"
  shn_dbgmsg "0:$0"
  shn_dbgmsg "this:$this"
  pushd $this_dir &>/dev/null
    MSH_SHN_PROJECT_RDIR=$( shn_dirname $PWD )
    MSH_SHN_PROJECT_DIR=$( shn_realpath $MSH_SHN_PROJECT_RDIR )
    shn_dbgmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
  popd &>/dev/null
  project_ac="$MSH_SHN_PROJECT_DIR/configure.ac"
  if [[ -f "$project_ac" ]] ; then
    MSH_SHN_PROJECT_NAME=$( shn_basename $MSH_SHN_PROJECT_DIR )
    project_ac=$( shn_realpath $project_ac )
  else
    shn_errmsg "absent project $project_ac"
    return 1
  fi
  return 0
}


if ! . ${MSH_SHN_DIR:-shn}/libtools.bash ; then
  shn_errmsg "loading environment ${MSH_SHN_DIR:-shn}/libtools.bash"
  return 1
fi
function shn_setup_global_dirs
{
  if [[ "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR" ]] ; then
# echo " B `declare -p MSH_SHN_DIRS`" >&2
    MSH_SHN_DIRS[top]=`shn_realpath "$MSH_SHN_PROJECTS_DIR/.."`
    MSH_SHN_DIRS[admin]="${MSH_SHN_DIRS[top]}/admin"
    MSH_SHN_DIRS[status]="${MSH_SHN_DIRS[admin]}/status"
    MSH_SHN_DIRS[save]="${MSH_SHN_DIRS[admin]}/saved"
    MSH_SHN_DIRS[savedist]="${MSH_SHN_DIRS[save]}/dist"
    MSH_SHN_DIRS[savegentoo]="${MSH_SHN_DIRS[save]}/gentoo"
    MSH_SHN_DIRS[ebuilds]="$MSH_SHN_PROJECTS_DIR/gentoo/ebuilds"
    MSH_SHN_DIRS[ebuild]="${MSH_SHN_DIRS[ebuilds]}/mas-tar"
    MSH_SHN_DIRS[install]="${MSH_SHN_DIRS[admin]}/install"
    MSH_SHN_DIRS[flavour]="${MSH_SHN_DIRS[install]}/${MSH_SHN_FLAVOUR:-default}"
    MSH_SHN_DIRS[error]="/tmp"
    MSH_SHN_DIRS[files]="$MSH_SHN_PROJECTS_DIR/files"

    for id in admin 'status' save savedist savegentoo ebuilds ebuild install flavour error files ; do
      if [[ "${MSH_SHN_DIRS[$id]}" ]] && ! [[ -d "${MSH_SHN_DIRS[$id]}" ]] ; then
	shn_mkdir "${MSH_SHN_DIRS[$id]}" || return 1
	shn_msg created ${MSH_SHN_DIRS[$id]}
      fi
    done

    shn_dbgmsg "MSH_SHN_PROJECTS_DIR:$MSH_SHN_PROJECTS_DIR"
    shn_dbgmsg "MSH_SHN_FLAVOUR:$MSH_SHN_FLAVOUR"
    shn_dbgmsg "top dir:${MSH_SHN_DIRS[top]}"
    shn_dbgmsg "admin dir:${MSH_SHN_DIRS[admin]}"
    shn_dbgmsg "install dir:${MSH_SHN_DIRS[install]}"
  else
    shn_errmsg "can't set dir's"
    return 1
  fi
  return 0
}
function shn_setup_project_dirs
{
  if [[ "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$MSH_SHN_PROJECT_DIR" ]] ; then
    MSH_SHN_DIRS[configure]=$MSH_SHN_PROJECT_DIR
    MSH_SHN_DIRS[aux]=$MSH_SHN_PROJECT_DIR/.auxdir
    MSH_SHN_DIRS[marks]=$MSH_SHN_PROJECT_DIR/.marks
    MSH_SHN_DIRS[zocvid]=$MSH_SHN_PROJECT_DIR
    MSH_SHN_DIRS[relediting]=editing
    MSH_SHN_DIRS[editing]=$MSH_SHN_PROJECT_DIR/${MSH_SHN_DIRS[relediting]}
    
    MSH_SHN_DIRS[old__relmased]=mased
    MSH_SHN_DIRS[old__masedid]=$MSH_SHN_PROJECT_DIR/${MSH_SHN_DIRS[old__relmased]}
    MSH_SHN_DIRS[oldlinks__vimid]=$MSH_SHN_PROJECT_DIR
#   MSH_SHN_DIRS[oldlinks__zocvid]=$MSH_SHN_PROJECT_DIR

    MSH_SHN_DIRS[relmased]=${MSH_SHN_DIRS[relediting]}/mased
    MSH_SHN_DIRS[masedid]=$MSH_SHN_PROJECT_DIR/${MSH_SHN_DIRS[relmased]}
    
    MSH_SHN_DIRS[relvimid]=${MSH_SHN_DIRS[relediting]}
    MSH_SHN_DIRS[vimid]=$MSH_SHN_PROJECT_DIR/${MSH_SHN_DIRS[relvimid]}


    MSH_SHN_DIRS[debug]=$MSH_SHN_PROJECT_DIR/debug
    MSH_SHN_DIRS[build]="${MSH_SHN_DIRS[aux]}/.build"
    MSH_SHN_BUILD_DIR=${MSH_SHN_DIRS[build]}
    MSH_SHN_DIRS[buildsrc]="${MSH_SHN_DIRS[build]}/src"
    MSH_SHN_DIRS[m4]="${MSH_SHN_DIRS[aux]}/m4"
    MSH_SHN_DIRS[error]="/tmp"
#  echo " C `declare -p MSH_SHN_DIRS`" >&2
    
    # $( find ${MSH_SHN_DIRS[files]}/*.vim -printf '%f' )
    MSH_SHN_FILES[vimid]="gvim-bufenter.vim gvim-bufreadpost.vim gvim-bufwinenter.vim gvim-filetype.vim gvim-funcs.vim gvim-guienter.vim gvimrc-mastar.vim gvim-tabenter.vim gvim-vimenter.vim gvim-winenter.vim vimrc-mastar.vim"
    MSH_SHN_FILES[localrc]=".localrc"
    MSH_SHN_FILES[sh]="sh"
    MSH_SHN_FILES[zocvid]="zocversion.txt zocvdate.txt"
    shn_dbgmsg "aux:${MSH_SHN_DIRS[aux]}"
    shn_dbgmsg "build:${MSH_SHN_DIRS[build]}"

 
    for id in aux build m4 editing vimid masedid debug ; do
      if [[ "${MSH_SHN_DIRS[$id]}" ]] &&
      		  [[ "${MSH_SHN_DIRS[old__$id]}" ]] &&
      		! [[ "${MSH_SHN_DIRS[$id]}" == "${MSH_SHN_DIRS[old__$id]}" ]] &&
		  [[ -d ${MSH_SHN_DIRS[old__$id]} ]] &&
		! [[ -d ${MSH_SHN_DIRS[$id]} ]]
      then
	shn_msg moving ${MSH_SHN_DIRS[old__$id]}  ${MSH_SHN_DIRS[$id]}
        git mv ${MSH_SHN_DIRS[old__$id]} ${MSH_SHN_DIRS[$id]} || mv ${MSH_SHN_DIRS[old__$id]} ${MSH_SHN_DIRS[$id]} || return 1
	shn_msg moved ${MSH_SHN_DIRS[old__$id]}  ${MSH_SHN_DIRS[$id]}
      elif [[ "${MSH_SHN_DIRS[$id]}" ]] && ! [[ -d "${MSH_SHN_DIRS[$id]}" ]] ; then
	shn_mkdir "${MSH_SHN_DIRS[$id]}" || return 1
	shn_msg created ${MSH_SHN_DIRS[$id]}
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
    echo -n "$MSH_SHN_PROJECT_NAME"
  else
    return 1
  fi
}
function shn_project_version ()
{
  local vseq vdate
  if [[ -L shn ]] && [[ -f shn/libwork.bash ]] ; then
    if [[ "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$MSH_SHN_PROJECT_DIR" ]] ; then
      if pushd "$MSH_SHN_PROJECT_DIR" &>/dev/null && [[ -f "zocversion.txt" ]] ; then
	read vseq < zocversion.txt
	if [[ -f zocvdate.txt ]] ; then
	  read vdate < zocvdate.txt
	fi
	if type -t datem 2>&1 >/dev/null ; then
	  echo -n "${vseq}.${vdate:-$( datem )}"
	elif [[ "$vdate" ]] ; then
	  echo -n "${vseq}.${vdate}"
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
export shn_project_version
function shn_setup_projects ()
{
  local retcode=0
  local i
# unset MSH_SHN_DIRS
  declare -p MSH_SHN_DIRS &>/dev/null || declare -gx -A MSH_SHN_DIRS
  declare -p MSH_SHN_FILES &>/dev/null || declare -gx -A MSH_SHN_FILES
  declare -p MSH_SHN_ENABLED_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_ENABLED_PROJECTS
  declare -p MSH_SHN_DISABLED_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_DISABLED_PROJECTS
  declare -p MSH_SHN_HASH_PROJECTS &>/dev/null || declare -gx -A MSH_SHN_HASH_PROJECTS
  declare -p MSH_SHN_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_PROJECTS
  if [[ "$shn_dont_setup" ]]  ; then return 0 ; fi
  shn_dbgmsg "$FUNCNAME"
  shn_dbgmsg "S1 `pwd`" >&2
  shn_setup_zero || return 1
  shn_setup_project_dirs || return 1
  shn_dbgmsg "S2 `pwd`" >&2
  shn_dbgmsg "MSH_SHN_REAL_THIS:$MSH_SHN_REAL_THIS"
  if ! [[ "`shn_basename $MSH_SHN_REAL_THIS`" == "libsetup.bash" ]] ; then
    shn_errmsg "BAD MSH_SHN_REAL_THIS:$MSH_SHN_REAL_THIS"
    return 1
  elif [[ -f "$MSH_SHN_REAL_THIS" ]] && [[ "`shn_basename $MSH_SHN_REAL_THIS`" == "libsetup.bash" ]] ; then
    local shn_dir=$( shn_dirname $MSH_SHN_REAL_THIS ) || return 1
    if [[ "$shn_dir" ]] && [[ -d "$shn_dir" ]] ; then
      shn_dbgmsg "shn_dir:$shn_dir"
      local projects_dir=$( shn_realpath $shn_dir/.. ) || return 1
      if [[ "$projects_dir" ]] && [[ -d "$projects_dir" ]] ; then
	shn_dbgmsg "projects_dir:$projects_dir"
	MSH_SHN_PROJECTS_DIR=$projects_dir
	MSH_SHN_PRJTOP_DIR=`shn_realpath "$MSH_SHN_PROJECTS_DIR/.."`
	shn_setup_global_dirs || return 1
	local projects_file_name=projects.list
	local projects_file=$projects_dir/$projects_file_name
	if [[ "$projects_file" ]] && [[ -f "$projects_file" ]] ; then
	  shn_dbgmsg "OK projects_file:$projects_file"
	  readarray -t MSH_SHN_PROJECTS < $projects_file || return 1
	  for (( i=0 ; $i < ${#MSH_SHN_PROJECTS[@]} ; i++ )) ; do
	    shn_dbgmsg "-${i}. [${MSH_SHN_PROJECTS[$i]}]"
	    MSH_SHN_HASH_PROJECTS[${MSH_SHN_PROJECTS[$i]}]=1
	  done
	else
	  shn_errmsg "FAIL projects_file:$projects_file"
	  return 1
	fi
        local projects_disabled_file_name=projects_disabled.list
        local projects_disables_file=$projects_dir/$projects_disabled_file_name
        if [[ "$projects_disables_file" ]] && [[ -f "$projects_disables_file" ]] ; then
          shn_dbgmsg "OK projects_disables_file:$projects_disables_file"
          readarray -t MSH_SHN_DISABLED_PROJECTS < $projects_disables_file || return 1
          for (( i=0 ; $i < ${#MSH_SHN_DISABLED_PROJECTS[@]} ; i++ )) ; do
            shn_dbgmsg "-${i}. DISABLED [${MSH_SHN_DISABLED_PROJECTS[$i]}]"
            if [[ "${MSH_SHN_DISABLED_PROJECTS[$i]}" ]] ; then
	      unset MSH_SHN_HASH_PROJECTS[${MSH_SHN_DISABLED_PROJECTS[$i]}]
	    fi
          done
        else
          shn_errmsg "FAIL projects_disables_file:$projects_disables_file"
          return 1
        fi
        unset MSH_SHN_ENABLED_PROJECTS
        for k in ${MSH_SHN_PROJECTS[@]} ; do
          if [[ "$k" ]] && [[ "${MSH_SHN_HASH_PROJECTS[$k]}" ]] ; then
####        shn_dbgmsg "-${i}. ENABLE [$k]"
            MSH_SHN_ENABLED_PROJECTS[${#MSH_SHN_ENABLED_PROJECTS[@]}]=$k
          fi
        done
         
####    for (( i=0 ; $i < ${#MSH_SHN_ENABLED_PROJECTS[@]} ; i++ )) ; do
####      shn_dbgmsg "-${i}. ENABLED [${MSH_SHN_ENABLED_PROJECTS[$i]}]"
####    done
	
	retcode=0
	
	shn_dbgmsg "@ at project $MSH_SHN_PROJECT_DIR"
	shn_dbgmsg "S3 `pwd`" >&2
	if MSH_SHN_PROJECT_FULLNAME="${MSH_SHN_PROJECT_NAME}-`shn_project_version`" ; then
	  shn_dbgmsg "S4 `pwd`" >&2
	  shn_setup_additional || { shn_errmsg additional ; retcode=$? ; }

  #	MSH_SHN_FLAVOUR=${MSH_SHN_FLAVOUR:-`cat $projects_dir/projects.flavour`}
	  if [[ "$MSH_SHN_PROJECTS_DIR" ]] && [[ -f "$MSH_SHN_PROJECTS_DIR/projects.flavour" ]] ; then
	    read MSH_SHN_FLAVOUR < "$MSH_SHN_PROJECTS_DIR/projects.flavour"
	  else
	    shn_errmsg "can't set MSH_SHN_FLAVOUR"
	    retcode=1
	  fi
	else
	  shn_errmsg "can't set MSH_SHN_PROJECT_FULLNAME"
	  retcode=1
	fi
      else
	shn_errmsg "FAIL projects_dir:$projects_dir"
	shn_errmsg "shn_dir:$shn_dir"
	shn_errmsg "MSH_SHN_REAL_THIS:$MSH_SHN_REAL_THIS"
	retcode=1
      fi
    else
      shn_errmsg "FAIL shn_dir:$shn_dir"
      shn_errmsg "MSH_SHN_REAL_THIS:$MSH_SHN_REAL_THIS"
      retcode=1
    fi
  else
    shn_errmsg "FAIL MSH_SHN_REAL_THIS:$MSH_SHN_REAL_THIS"
    retcode=1
  fi
  shn_dbgmsg "S5 ($retcode) `pwd`" >&2
  return $retcode
}
function shn_initial_src_mased_vim  ()
{
  local gs fname fn fnn
  gs=`grep '^AC_CONFIG_SRCDIR' $MSH_SHN_PROJECT_DIR/configure.ac` || return $?
  if [[ "$gs" ]] && [[ "$gs" =~ AC_CONFIG_SRCDIR\(\[src/(.*)\.c\]\) ]] ; then
    echo "find ${BASH_REMATCH[1]}.c" >> src.mased.vim || return $?
    echo "sfind ${BASH_REMATCH[1]}.h" >> src.mased.vim || return $?
    echo  >> src.mased.vim || return $?
    echo  >> src.mased.vim || return $?
    for fname in $MSH_SHN_PROJECT_DIR/src/*.c ; do
      fn=$( shn_basename $fname )
      if [[ "$fn" =~ ^(.*)\.c$ ]] ; then
	fnn=${BASH_REMATCH[1]}
	if [[ -f "$MSH_SHN_PROJECT_DIR/src/inc/${fnn}.h" ]] ; then
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
  if pushd ${MSH_SHN_DIRS[masedid]} &>/dev/null ; then
     for typf in sh shn ac vimrc ; do
# echo " D `declare -p MSH_SHN_DIRS`" >&2
      fn="${typf}.mased.vim"
      if [[ -f "${MSH_SHN_DIRS[files]}/mased/$fn" ]] ; then
	file=`shn_realpath --relative-to=. ${MSH_SHN_DIRS[files]}/mased/$fn` || { retval=$? ; break ; }
	link=$( shn_basename $file ) || { retval=$? ; break ; }
	if [[ -f $file ]] && ! [[ -f $link ]] ; then
          shn_msg "updating mased for typf='$typf'"
	  if [[ -L  $link ]] && ! [[ -r  $link ]] ; then
	    shn_rm $link || return 1
	  fi
	  if ! [[ -e $link ]] ; then
	    shn_dbgmsg "$file -> $link"
	    shn_ln -s $file $link || { retval=$? ; break ; }
	    shn_msg created link $link
	  fi
	fi
      else
	shn_errmsg ${MSH_SHN_DIRS[files]}/mased/$fn
	retval=1
	break
      fi
    done        
    if ! [[ -f "src.mased.vim" ]] ; then
      shn_initial_src_mased_vim || { retval=$? ; break ; }
    fi
    popd &>/dev/null
  else
    shn_errmsg "not exists mased dir : ${MSH_SHN_DIRS[masedid]}"
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
  local fn fnf file  link oldlink dir olddir srcf srcdir
  if ! [[ -f "configure.ac" ]] ; then 
    shn_errmsg setup additional - no configure.ac
    return 1
  fi
# echo " E `declare -p MSH_SHN_DIRS`" >&2
  srcdir=${MSH_SHN_DIRS[files]}
  if [[ $srcdir ]] && [[ -d $srcdir ]] ; then 
  #  mased/sh.mased.vim
    for dirid in vimid zocvid ; do
      dir=${MSH_SHN_DIRS[$dirid]}
      olddir=${MSH_SHN_DIRS[oldlinks__$dirid]}
      if [[ $dir ]] && pushd $dir &>/dev/null ; then
	for fn in ${MSH_SHN_FILES[$dirid]} ; do
	  srcf="$srcdir/$fn"
	  if [[ -f $srcf ]] ; then
	    file=$( shn_realpath --relative-to=. $srcf ) || return 1
	    link=$fn
	    if [[ $olddir ]] && ! [[ $olddir == $dir ]] ; then
	      if [[ -d $olddir ]] ; then
		oldlink="$olddir/$fn"
		if [[ -L $oldlink ]] ; then
		  shn_msg "removing $oldlink"
		  shn_rm $oldlink
		fi
	      else
		shn_errmsg "no olddir '$olddir' for $dirid"
	      fi
	    fi
	    if [[ -L $link ]] && ! [[ -r $link ]] ; then
	      shn_rm $link
	    fi
	    if ! [[ -e $link ]] ; then
	      shn_msg "making $fn from $srcf at $PWD"
#	      shn_msg "making $fn from $srcf"
	      shn_dbgmsg "$file -> $link"
	      shn_ln -s $file $link || return 1
	      shn_msg created link $link
	    fi
	  else
	    shn_errmsg "no file '$srcf'"
	    return 1
	  fi	
	done
	popd &>/dev/null
      fi
    done
  else
    shn_errmsg "shn_setup_additional - $srcdir"
    return 1
  fi
  for fn in shn m4zoc ; do # sh removed 20140429
    if [[ -d "$MSH_SHN_PROJECTS_DIR/$fn" ]] ; then
      file=`shn_realpath --relative-to=. ${MSH_SHN_PROJECTS_DIR}/$fn`
      link=$( shn_basename $file )
      if ! [[ -L $link ]] && ! [[ -f $link ]] ; then
	shn_dbgmsg "$file -> $link"
	shn_ln -s $file $link || return 1
	shn_msg created link $link
      fi
    else
      shn_errmsg ${MSH_SHN_PROJECTS_DIR}/$fn
      return 1
    fi
  done
  for fn in .localrc ; do
    if [[ -f "$MSH_SHN_PROJECTS_DIR/$fn" ]] ; then
      file=`shn_realpath --relative-to=. ${MSH_SHN_PROJECTS_DIR}/$fn`
      link=$( shn_basename $file )
      if ! [[ -e $link ]] ; then
	shn_dbgmsg "$file -> $link"
	shn_ln -s $file $link || return 1
	shn_msg created link $link
      fi
    else
      shn_errmsg ${MSH_SHN_PROJECTS_DIR}/$fn
      return 1
    fi
  done    
  shn_initial_mased_vim
  shn_initial_debug
  return 0
}

shn_dbgmsg before setup
shn_setup_projects
MSH_SHN_SETUP_FUNCTIONS="`shn_funlist shn_setup`"
# Don't: [[ "$MSH_SHN_SETUP_FUNCTIONS" ]] && export -f $MSH_SHN_SETUP_FUNCTIONS

return 0
