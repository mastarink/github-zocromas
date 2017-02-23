function shn_project_load ()
{
  local lib libt libx
  for lib in tools setup build run ; do
    libt="${MSH_SHN_DIR:-shn}/lib${lib}.bash"
    if ! [[ -f "$libt" ]] || ! . "$libt" ; then
      shn_errmsg "Error loading environment $libt"
      return 1
    else
#     shn_msg "Loaded $libt"
      for libx in ${MSH_SHN_DIR:-shn}/lib${lib}_*.bash ; do
        if [[ -f "$libx" ]] ; then
	  if ! . "$libx" ; then
	    shn_errmsg "loading environment $libx"
	    return 1
#         else
#           shn_msg "loaded sub: $libx"
	  fi
	fi
      done
    fi
#   shn_msg "loaded lib${lib}" >&2
#   shn_msg "Loaded all of $lib"
  done
}
shn_project_load $@ || return 1
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
  local force=$1
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
    if [[ "$match_cnt" -gt 1 ]] && [[ "$match" ]] && ! [[ "$force" ]] ; then
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
	projects_realdir=`shn_project_dir2realpath ${MSH_SHN_PROJECTS[$i]} $force` || return 1
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
  local retcode=1
  local file=$1 ffile prj found prjdir
# shn_dbgmsg "$file --- $rfile"
  [[ -f "$file" ]] && rfile=`shn_realpath $file` && file=$rfile
# shn_dbgmsg "$file --- $rfile"
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
	    shn_msg "prj by file: $file => $prj"
	    retcode=0
	    break
#	  else
#	    shn_msg "not found prj by file ($file)"
	  fi
	fi
      done
      popd &>/dev/null
    fi
  else
    shn_msg "no file for prj by file"
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
  shn_msg "\$1:$1; MSH_SHN_PROJECT_NAME:$MSH_SHN_PROJECT_NAME;"
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
# echo -n "$# - "; for a in "$@" ; do echo -n "'$a' " >&2 ; done ; echo '-=-=-=-=-=-=-=-=-=-=-=-=-' >&2
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
function shn_project_enabled ()
{
  local newprj=$1 newprjdir prjdir
  local project_index
  if [[ ${MSH_SHN_ENABLED_PROJECTS[@]} ]] ; then
    for (( project_index=0 ; $project_index < ${#MSH_SHN_ENABLED_PROJECTS[@]} ; project_index++ )) ; do
      prjdir=$(shn_project_path ${MSH_SHN_ENABLED_PROJECTS[$project_index]})
      if newprjdir=$(MSH_SHN_DISABLE_MSG=yes shn_project_path $newprj 1) ; then
	if [[ "$prjdir" == "$newprjdir" ]] ; then
	  return 0
	fi
      fi
    done
  fi
  return 1
}
function shn_project_exists ()
{
  local newprj=$1 newprjdir prjdir
  local project_index
  if [[ ${MSH_SHN_PROJECTS[@]} ]] ; then
    for (( project_index=0 ; $project_index < ${#MSH_SHN_PROJECTS[@]} ; project_index++ )) ; do
      prjdir=$(shn_project_path ${MSH_SHN_PROJECTS[$project_index]})
      if newprjdir=$(MSH_SHN_DISABLE_MSG=yes shn_project_path $newprj 1) ; then
	if [[ "$prjdir" == "$newprjdir" ]] ; then
	  return 0
	fi
      fi
    done
  fi
  return 1
}

function shn_create_ac_config_dir_file ()
{
  local newprjdir=$1
  shift
  local ac_config_dstdir_file=$1
  shift
  local src=$1
  shift
  if ! [[ -f    "$newprjdir/$ac_config_dstdir_file" ]] ; then
    cat << ACC > "$newprjdir/$ac_config_dstdir_file"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include "$src.h"

int
main( int argc __attribute__(( unused )), char *argv[] __attribute__(( unused )) )
{
  $src();
  return 0;
}
ACC
  fi
}
function shn_create_mainsrc_file ()
{
  local newprjdir=$1
  shift
  local src_name=$1
  shift
  local fn="$newprjdir/src/$src_name.c"
  if ! [[ -f    $fn ]] ; then
    cat << SRC > $fn
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include "${src_name}.h"

int
$src_name( void )
{
  return 0;
}
SRC
  fi
}
function shn_create_maininc_file ()
{
  local newprjdir=$1
  shift
  local inc_name=$1
  shift
  local fn="$newprjdir/src/inc/$inc_name.h"
  if ! [[ -f    $fn ]] ; then
    cat << INC > $fn
#ifndef MAS_${inc_name^^}_H
#define MAS_${inc_name^^}_H

int $inc_name( void );

#endif
INC
  fi
}

function shn_create_top_makefile_file ()
{
  local newprjdir=$1
  shift

  local fn="$newprjdir/Makefile.am"
  if ! [[ -s $fn ]] ; then
cat << AM > $fn
SUBDIRS=src

pkgconfigdir = \$(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE_NAME@.pc
AM
  fi
}
function shn_create_src_makefile_file ()
{
  local newprjdir=$1
  shift
  local testfile=$1
  shift
  if [[ -f "$newprjdir/$testfile" ]] ; then
    local d=$(dirname $testfile)
    local testfilename=$(basename $testfile)
    local prj=$(basename $newprjdir)
    if [[ $d == 'src' ]] ; then
      if [[ $testfilename =~ ^(.*)\.(c)$ ]] ; then
	local testname=${BASH_REMATCH[1]}
	local fn="$newprjdir/src/Makefile.am"
	if ! [[ -s $fn ]] ; then
	  cat << AM > $fn
SUBDIRS= inc


lib_LTLIBRARIES = lib${prj}.la
lib${prj}_la_SOURCES = \\
 	${prj}.c

lib${prj}_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=${testname}

${testname}_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
${testname}_CFLAGS=@AM_CFLAGS@  -pthread

${testname}_SOURCES= \\
	${testfilename}
else

endif
AM
        fi
      fi
    fi
  fi
}
function shn_create_inc_makefile_file ()
{
  local newprjdir=$1
  shift
  local fn="$newprjdir/src/inc/Makefile.am"
  local prj=$(basename $newprjdir) prj_short
  if [[ $prj =~ ^(zocromas_)(.*)$ ]] ; then
    prj_short=${BASH_REMATCH[1]}
  else
    prj_short=$prj
  fi
  if ! [[ -s $fn ]] ; then
    cat << AM > $fn
lib${prj}includedir = \$(includedir)/mastar/${prj_short}
lib${prj}include_HEADERS =  \\
        ${prj}.h
#
#
# noinst_HEADERS = \\
#         dummy.h
AM
  fi
}


function shn_clone_project ()
{
  local newprj=$1 newprjdir project_index prjdir cprjdir cprjid prjidbase newprjid ifile k newprjname needed
  local src dst ac_config_srcdir_file ac_config_dstdir_file cprj
  if [[ ${MSH_SHN_PROJECTS[@]} ]] ; then
    if false && shn_project_exists "$newprj" ; then
      shn_errmsg "project '$newprj' exists!"
      return 1
    fi

    shn_msg "OK $newprj not exists"
    cprjdir=$MSH_SHN_PROJECT_DIR
    shn_msg "cprjdir: $cprjdir"
    if [[ "$cprjdir" ]] && [[ -d "$cprjdir" ]] && [[ "$cprjdir" =~ ^$MSH_SHN_PROJECTS_DIR/(.*/)([^/]+)$ ]] ; then
      prjidbase=${BASH_REMATCH[1]}
      cprjid=${BASH_REMATCH[2]}
      newprjid="${prjidbase}${newprj}"
      shn_msg "A ------------------"
      if ! shn_project_exists "$newprj" ; then
        MSH_SHN_ENABLED_PROJECTS+=($newprjid)
      fi
      shn_msg "B ------------------"
      if ! shn_project_enabled "$newprj" ; then
        MSH_SHN_PROJECTS+=($newprjid)
      fi
      shn_msg "C ------------------"
      newprjdir=$(shn_project_path $newprj 1)
      shn_msg "D ------------------"
      shn_msg "prjidbase: $prjidbase; cprjid: $cprjid; newprjid: $newprjid; newprjdir:$newprjdir;"
      if [[ -d "$newprjdir" ]] ; then
        if false ; then
	  return 1
	fi
      else
        mkdir $newprjdir || return 1
      fi
      shn_msg "E ------------------"
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

	sed --in-place='.bak' 's@^\([[:blank:]]*AC_CONFIG_SRCDIR[[:blank:]]*([[:blank:]]*\[\)\('$ac_config_srcdir_file'\)\([[:blank:]]*\][[:blank:]]*)[[:blank:]]*\)$@\1'$ac_config_dstdir_file'\3@'  $newprjdir/configure.ac
	if [[ -f $newprjdir/configure.ac.bak ]] && [[ -f $newprjdir/configure.ac ]] && diff $newprjdir/configure.ac $newprjdir/configure.ac.bak ; then
	  rm $newprjdir/configure.ac
	  mv $newprjdir/configure.ac.bak $newprjdir/configure.ac
	  shn_msg "@@@@@@@ move"
	else
	  shn_msg "@@@@@@@ why?"
	  if [[  -f $newprjdir/configure.ac.bak ]] ; then shn_msg "bak present" ; fi
	  if [[  -f $newprjdir/configure.ac ]] ; then shn_msg "ac present" ; fi
	  if diff $newprjdir/configure.ac $newprjdir/configure.ac.bak ; then shn_msg "not diff" ; fi
	fi
	shn_msg "ac_config_srcdir_file: $ac_config_srcdir_file"
	cprj=${MSH_SHN_PROJECT_NAME}
        if ! [[ -f $newprjdir/configure ]] || ! [[ -f $newprjdir/Makefile.in ]] \
			|| ! [[ -f $newprjdir/src/Makefile.in ]] || ! [[ -f $newprjdir/src/inc/Makefile.in ]] \
			|| ! [[ -d $newprjdir/.auxdir ]]
	then
	  shn_project_cd "${newprjname}"
	  shn_build_autoreconf
	  shn_project_cd "$cprj"
	  shn_msg "autoreconf'ing"
	else
	  shn_msg "not autoreconf'ing"
	fi	
	if true ; then
	  if ! [[ -f $newprjdir/.auxdir/.build/config.status ]] ; then
	    shn_project_cd "${newprjname}"
	    shn_build_configure
	    shn_project_cd "$cprj"
	    shn_msg "configure'ing"
	  else
	    shn_msg "not configure'ing"
	    ls -l $newprjdir/.auxdir
	  fi
	  if [[ -f $newprjdir/.auxdir/.build/config.status ]] ; then
	    shn_project_cd "${newprjname}"
	    shn_build_make
	    shn_project_cd "$cprj"
	    shn_msg "make'ing"
	  else
	    shn_msg "not make'ing"
	  fi
	fi
      fi
    fi
  fi
  return 0
}
MSH_SHN_PROJECT_FUNCTIONS="`shn_funlist shn_project`"
[[ "$MSH_SHN_PROJECT_FUNCTIONS" ]] && export -f $MSH_SHN_PROJECT_FUNCTIONS
# shn_msg "$(datemt) v.`shn_project_version`"
return 0
