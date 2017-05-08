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
# shn_msg "setup zero '$MSH_SHN_PROJECT_NAME' $MSH_SHN_PROJECT_DIR $(pwd)"
  shn_create_links || return 1
  if false ; then
    if ! [[ -f "configure.ac" ]] ; then
      shn_errmsg "not found: configure.ac"
      return 1
    fi
  fi
  if ! [[ -f "shn/libsetup.bash" ]] ; then
    shn_errmsg "not found: shn/libsetup.bash"
    return 1
  fi
  MSH_SHN_REAL_THIS=$( shn_realpath $this ) || return 1

  shn_dbgmsg "here_dir:$here_dir"
  shn_dbgmsg "0:$0"
  shn_dbgmsg "this:$this"
  pushd $this_dir &>/dev/null
    MSH_SHN_PROJECT_RDIR=$( shn_dirname $PWD )
#   shn_msg "MSH_SHN_PROJECT_RDIR: $MSH_SHN_PROJECT_RDIR"
    MSH_SHN_PROJECT_DIR=$( shn_realpath $MSH_SHN_PROJECT_RDIR )
    shn_dbgmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
  popd &>/dev/null
  project_ac="$MSH_SHN_PROJECT_DIR/configure.ac"
  if [[ -f "$project_ac" ]] ; then
    MSH_SHN_PROJECT_NAME=$( shn_basename $MSH_SHN_PROJECT_DIR )
    project_ac=$( shn_realpath $project_ac )
  else
    shn_errmsg "not found: $project_ac"
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
	shn_dbgmsg "created dir ${MSH_SHN_DIRS[$id]} (global)"
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
# shn_msg "setup project dirs"
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
	shn_dbgmsg "created dir ${MSH_SHN_DIRS[$id]}"
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
function shn_get_projects_list_nt ()
{
  declare -p MSH_SHN_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_PROJECTS
  local i prj plf prjl
  local -a work
  work=( $( find $MSH_SHN_PROJECTS_DIR -name mas.project -exec dirname "{}"  \; | sort ) )

  plf="$MSH_SHN_PROJECTS_DIR/new_projects.list"
  if [[ $plf ]] ; then rm -f $plf ; fi
  MSH_SHN_PROJECTS=()
  for (( i=0; $i<${#work[@]}; i++ )) ; do
    local w=${work[$i]}
    if [[ $w =~ ^$MSH_SHN_PROJECTS_DIR/(.*)$  ]] ; then
      prjl=${BASH_REMATCH[1]}
      echo $prjl >> $plf
      MSH_SHN_PROJECTS+=( $prjl )
    fi
  done  
}
function shn_get_projects_list ()
{
  local i prj
  declare -p MSH_SHN_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_PROJECTS
  local projects_file_name=projects.list
  local projects_file=$MSH_SHN_PROJECTS_DIR/$projects_file_name
  MSH_SHN_PROJECTS=()
  if [[ "$projects_file" ]] && [[ -f "$projects_file" ]] ; then
    readarray -t MSH_SHN_PROJECTS < $projects_file || return 1
#   shn_msg "OK projects: $(basename $projects_file)"
  else
    shn_errmsg "FAIL projects_file: $projects_file"
    return 1
  fi
  return 0
}
function shn_prepare_projects_list ()
{
  local i prj
  declare -p MSH_SHN_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_PROJECTS
  for (( i=0 ; $i < ${#MSH_SHN_PROJECTS[@]} ; i++ )) ; do
    prj=${MSH_SHN_PROJECTS[$i]}
    prjdir=${MSH_SHN_PROJECTS_DIR}/${prj}
    prjname=$( shn_basename $prjdir )
    prjmark="${prjdir}/mas.project"
#   shn_msg "$prj" 
    if [[ $prjdir ]] && [[ -d $prjdir ]] && [[ $prjmark ]] && ! [[ -f $prjmark ]] ; then
      echo "$prjname" >  $prjmark
    fi
#   shn_msg "project $prjdir"
    shn_dbgmsg "-${i}. [$prj]"
    MSH_SHN_HASH_PROJECTS[$prj]=1
  done
  shn_get_disabled_projects_list
  unset MSH_SHN_ENABLED_PROJECTS
  for k in ${MSH_SHN_PROJECTS[@]} ; do
    if [[ "$k" ]] && [[ "${MSH_SHN_HASH_PROJECTS[$k]}" ]] ; then
####        shn_dbgmsg "-${i}. ENABLE [$k]"
      MSH_SHN_ENABLED_PROJECTS[${#MSH_SHN_ENABLED_PROJECTS[@]}]=$k
    fi
  done
}
function shn_get_disabled_projects_list ()
{
  declare -p MSH_SHN_DISABLED_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_DISABLED_PROJECTS
  local i
  local projects_disabled_file_name=projects_disabled.list
  local projects_disables_file=$projects_dir/$projects_disabled_file_name
  if [[ "$projects_disables_file" ]] && [[ -f "$projects_disables_file" ]] ; then
    readarray -t MSH_SHN_DISABLED_PROJECTS < $projects_disables_file || return 1
#   shn_msg "OK disabled projects: $(basename $projects_disables_file)"
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
}
function shn_setup_projects ()
{
  local retcode=0
  local i
# shn_msg "setup projects"
# unset MSH_SHN_DIRS
  declare -p MSH_SHN_DIRS &>/dev/null || declare -gx -A MSH_SHN_DIRS
  declare -p MSH_SHN_FILES &>/dev/null || declare -gx -A MSH_SHN_FILES
  declare -p MSH_SHN_ENABLED_PROJECTS &>/dev/null || declare -gx -a MSH_SHN_ENABLED_PROJECTS
  declare -p MSH_SHN_HASH_PROJECTS &>/dev/null || declare -gx -A MSH_SHN_HASH_PROJECTS
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
	MSH_SHN_HASH_PROJECTS=()
 	shn_get_projects_list_nt
# 	shn_get_projects_list
	shn_prepare_projects_list

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
	    if read MSH_SHN_FLAVOUR < "$MSH_SHN_PROJECTS_DIR/projects.flavour" ; then
              :
#	      shn_msg "OK flavour: projects.flavour"
	    else
	      shn_errmsg "can't read MSH_SHN_FLAVOUR"
	    fi
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
          shn_dbgmsg "updating mased for typf='$typf'"
	  if [[ -L  $link ]] && ! [[ -r  $link ]] ; then
	    shn_rm $link || return 1
	  fi
	  if ! [[ -e $link ]] ; then
	    shn_dbgmsg "$file -> $link"
	    shn_ln -s $file $link || { retval=$? ; break ; }
	    shn_dbgmsg "created link $link (mased)"
	  fi
	fi
      else
	shn_errmsg "\${MSH_SHN_DIRS[files]}/mased/\$fn: ${MSH_SHN_DIRS[files]}/mased/$fn"
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
    shn_dbgmsg "created .gitignore"
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
function shn_create_links_01 ()
{
  local fn file link
  for fn in shn m4zoc ; do # sh removed 20140429
    if [[ "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR/$fn" ]] ; then
      file=`shn_realpath --relative-to=. ${MSH_SHN_PROJECTS_DIR}/$fn`
      link=$( shn_basename $file )
      if ! [[ -L $link ]] && ! [[ -f $link ]] ; then
	shn_dbgmsg "$file -> $link"
	shn_ln -s $file $link || return 1
	shn_dbgmsg "created link $link (01)"
      fi
    else
      shn_errmsg "$FUNCNAME \${MSH_SHN_PROJECTS_DIR}/\$fn: ${MSH_SHN_PROJECTS_DIR}/$fn"
      return 1
    fi
  done
  return 0
}
function shn_create_links_02 ()
{
  for fn in .localrc ; do
    if [[ "$MSH_SHN_PROJECTS_DIR" ]] && [[ -d "$MSH_SHN_PROJECTS_DIR" ]] && [[ -f "$MSH_SHN_PROJECTS_DIR/$fn" ]] ; then
      file=`shn_realpath --relative-to=. ${MSH_SHN_PROJECTS_DIR}/$fn`
      link=$( shn_basename $file )
      if ! [[ -e $link ]] ; then
	shn_dbgmsg "$file -> $link"
	shn_ln -s $file $link || return 1
	shn_dbgmsg "created link $link (02)"
      fi
    else
      shn_errmsg "$FUNCNAME \${MSH_SHN_PROJECTS_DIR}/\$fn: ${MSH_SHN_PROJECTS_DIR}/$fn"
      return 1
    fi
  done
  return 0
}
function shn_create_links ()
{
  shn_create_links_01 || return 1
  shn_create_links_02 || return 1
  return 0
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
#	      shn_msg "making $fn from $srcf at $PWD"
#	      shn_msg "making $fn from $srcf"
	      shn_dbgmsg "$file -> $link"
	      shn_ln -s $file $link || return 1
	      shn_dbgmsg "created link $link (additional)"
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
  shn_create_links || return 1
  shn_initial_mased_vim
  shn_initial_debug
  return 0
}
function shn_clone_project ()
{
  local newprj=$1 newprjdir project_index prjdir cprjdir cprjid prjidbase newprjid ifile k newprjname needed
  local src dst ac_config_srcdir_file ac_config_dstdir_file cprj
  if [[ ${MSH_SHN_PROJECTS[@]} ]] ; then
    if false && shn_project_dir_exists "$newprj" ; then
      shn_errmsg "project '$newprj' exists!"
      return 0 # 1???
    fi

    cprjdir=$MSH_SHN_PROJECT_DIR
#   shn_msg "cprjdir: $cprjdir; MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    if [[ "$cprjdir" ]] && [[ -d "$cprjdir" ]] && [[ "$cprjdir" =~ ^$MSH_SHN_PROJECTS_DIR/(.*/)?([^/]+)$ ]] ; then
      prjidbase=${BASH_REMATCH[1]}
      cprjid=${BASH_REMATCH[2]}
      newprjid="${prjidbase}${newprj}"
      if ! shn_project_dir_exists "$newprj" ; then
        MSH_SHN_ENABLED_PROJECTS+=($newprjid)
      fi
      if ! shn_project_enabled "$newprj" ; then
        MSH_SHN_PROJECTS+=($newprjid)
      fi
      newprjdir=$(shn_project_path $newprj 1)
#     shn_msg "prjidbase: $prjidbase; cprjid: $cprjid; newprjid: $newprjid; newprjdir:$newprjdir;"
      if [[ -d "$newprjdir" ]] ; then
        if false ; then
	  return 1
	fi
      else
        mkdir $newprjdir || return 1
      fi
      if [[ $newprjdir ]] && [[ -d $newprjdir ]] ; then
        shn_save_projects || return 1
	local -A need_dirs=(['src']='src' ['src/inc']='src/inc' ['debug']='debug')
	for k in "${!need_dirs[@]}" ; do
	  needed=${need_dirs[$k]}
	  src="$cprjdir/$k"
	  dst="$newprjdir/$needed"
	  if ! [[ -d $dst ]] && ! [[ -f $dst ]] ; then mkdir $dst ; fi
	done
	# AC_CONFIG_SRCDIR
	newprjname=$( shn_basename $newprjdir )
	local -A need_files=([configure.ac]='configure.ac' [${MSH_SHN_PROJECT_NAME}.pc.in]="${newprjname}.pc.in" ['debug/debug_mastest.cmd']='debug/debug_mastest.cmd')
	for k in "${!need_files[@]}" ; do
	  needed=${need_files[$k]}
	  src="$cprjdir/$k"
	  dst="$newprjdir/$needed"
	  if [[ "$src" ]] && [[ -f "$src" ]] && [[ "$dst" ]] && ! [[ -f "$dst" ]] ; then
	    cp -a $src $dst
	  fi
	done
	ac_config_srcdir_file=$(sed -n 's@^[[:blank:]]*AC_CONFIG_SRCDIR(\[\(.*\)\])[[:blank:]]*$@\1@p'  configure.ac)
	ac_config_dstdir_file="src/mastest.c"
	

	shn_create_ac_config_dir_file "$newprjdir" "$ac_config_dstdir_file" "${newprjname}"
	shn_create_mainsrc_file "$newprjdir"  "${newprjname}"
	shn_create_maininc_file "$newprjdir"  "${newprjname}"
	shn_create_top_makefile_file "$newprjdir"  "$ac_config_dstdir_file"
	shn_create_src_makefile_file "$newprjdir"  "$ac_config_dstdir_file"
	shn_create_inc_makefile_file "$newprjdir"  "$ac_config_dstdir_file"

	sed --in-place='.bak' 's@^\([[:blank:]]*AC_CONFIG_SRCDIR[[:blank:]]*([[:blank:]]*\[\)\('$ac_config_srcdir_file'\)\([[:blank:]]*\][[:blank:]]*)[[:blank:]]*\)$@\1'$ac_config_dstdir_file'\3@'  $newprjdir/configure.ac || return 1
	shn_dbgmsg "ac_config_srcdir_file: $ac_config_srcdir_file"
	if [[ -f $newprjdir/configure.ac.bak ]] && [[ -f $newprjdir/configure.ac ]] && diff $newprjdir/configure.ac $newprjdir/configure.ac.bak ; then
	  rm $newprjdir/configure.ac
	  mv $newprjdir/configure.ac.bak $newprjdir/configure.ac
	else
	  shn_errmsg "@@@@@@@ why?"
	  if [[  -f $newprjdir/configure.ac.bak ]] ; then shn_errmsg "bak present" ; fi
	  if [[  -f $newprjdir/configure.ac ]] ; then shn_errmsg "ac present" ; fi
	  if diff $newprjdir/configure.ac $newprjdir/configure.ac.bak ; then shn_errmsg "not diff" ; fi
	fi
	cprj=${MSH_SHN_PROJECT_NAME}
        if ! [[ -f $newprjdir/configure ]] || ! [[ -f $newprjdir/Makefile.in ]] \
			|| ! [[ -f $newprjdir/src/Makefile.in ]] || ! [[ -f $newprjdir/src/inc/Makefile.in ]] \
			|| ! [[ -d $newprjdir/.auxdir ]]
	then
	  shn_project_cd "${newprjname}" || return 1
	  shn_msg "autoreconf"
	  if ! shn_build_autoreconf ; then
	    shn_msg "autoreconf error" 
	    return 1
	  fi
	  shn_msg "autoreconf'ing done; back to $cprj"
	  shn_project_cd "$cprj"
	else
	  shn_msg "no need autoreconf'ing"
	fi	
	if true ; then
	  if ! [[ -f $newprjdir/.auxdir/.build/config.status ]] ; then
	    shn_project_cd "${newprjname}" || return 1
	    shn_msg "configure"
	    if ! shn_build_configure ; then
	      shn_msg "configure error"
	      return 1
	    fi
	    shn_msg "configure'ing done; back to $cprj"
	    shn_project_cd "$cprj"
	  else
	    shn_msg "no need configure'ing"
#	    ls -l $newprjdir/.auxdir
	  fi
	  if [[ -f $newprjdir/.auxdir/.build/config.status ]] ; then
	    shn_project_cd "${newprjname}" || return 1
	    shn_msg "make"
	    if ! shn_build_make ; then
	      shn_msg "make error"
	      return 1
	    fi
	    shn_msg "make'ing done; back to $cprj"
	    shn_project_cd "$cprj"
	  else
	    shn_msg "not make'ing"
	  fi
	fi
      fi
    else
      shn_errmsg "MSH_SHN_PROJECT_DIR is wrong: '$MSH_SHN_PROJECT_DIR'; MSH_SHN_PROJECTS_DIR: '$MSH_SHN_PROJECTS_DIR'"
    fi
  else
    shn_errmsg "MSH_SHN_PROJECTS not set"
  fi
  shn_msg "clone done to ${newprjname}"
  shn_project_cd "${newprjname}" || return 1
  return 0
}
function shn_save_projects ()
{
  local projects_file_name=projects.list prj project_index prj
  local projects_file=$MSH_SHN_PROJECTS_DIR/$projects_file_name
  local newprojects_file="$MSH_SHN_PROJECTS_DIR/$projects_file_name.$(datemt)" renamedprojects_file="$MSH_SHN_PROJECTS_DIR/$projects_file_name.old.$(datemt)"
  if [[ -f $projects_file ]] && ! [[ -f $newprojects_file ]] ; then
    for (( project_index=0 ; $project_index < ${#MSH_SHN_PROJECTS[@]} ; project_index++ )) ; do
      prj=${MSH_SHN_PROJECTS[$project_index]}
      echo $prj
    done | sort | uniq > $newprojects_file || return 1
    if diff $projects_file $newprojects_file >/dev/null ; then
      [[ "$newprojects_file" ]] && [[ -f $newprojects_file ]] && rm $newprojects_file
    else
      if [[ -s $newprojects_file ]] && [[ -s $projects_file ]] && [[ "$renamedprojects_file" ]] && ! [[ -f "$renamedprojects_file" ]] \
	  && mv $projects_file $renamedprojects_file && mv $newprojects_file $projects_file ; then
	shn_msg "projects_file: $projects_file"
	shn_msg "newprojects_file: $newprojects_file"
	shn_msg "renamedprojects_file: $renamedprojects_file"
      else
	return 1
      fi
    fi
  fi
  return 0
}

shn_dbgmsg before setup
shn_setup_projects
MSH_SHN_SETUP_FUNCTIONS="`shn_funlist shn_setup`"
# Don't: [[ "$MSH_SHN_SETUP_FUNCTIONS" ]] && export -f $MSH_SHN_SETUP_FUNCTIONS

return 0
