function shn_std_command ()
{
  local std=$1 retcode=0
  shift
  shn_dbgmsg "SC1 `pwd`" >&2
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg "SC2 `pwd`" >&2
  if [[ "${MAS_SHN_DIR[$std]}" ]] && [[ -d "${MAS_SHN_DIR[$std]}" ]] ; then
    if pushd "${MAS_SHN_DIR[$std]}" &>/dev/null ; then
      shn_dbgmsg "SC3 `pwd`" >&2
      "$@" || retcode=$?
    else
      shn_errmsg "std command at '$std' $@"
      retcode=1
    fi
    popd &>/dev/null
  else
    shn_errmsg "std command at '$std' $@"
    retcode=1
  fi
  return $retcode
}
function shn_build_command ()
{
  shn_std_command build $@
}
function shn_installed_command ()
{
  shn_dbgmsg "IC1 `pwd`" >&2
  shn_std_command flavour $@
}
function shn_build_xcommand ()
{
  local retcode=0
  local errname 
  local cmd=`which $1` ; shift
  local cmd_base=$( shn_basename $cmd )
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg 1 $FUNCNAME
  errname="${MAS_SHN_DIR[error]}/shn_build.${cmd_base}.${MAS_SHN_PROJECT_NAME}.`datemt`.result"
  if [[ "${MAS_SHN_DIR[error]}" ]] && [[ -d "${MAS_SHN_DIR[error]}" ]] ; then
    shn_dbgmsg 2 $FUNCNAME ${MAS_SHN_DIR[error]}
    if [[ "${MAS_SHN_DIR[build]}" ]] && [[ -d "${MAS_SHN_DIR[build]}" ]] ; then
      shn_dbgmsg 3 $FUNCNAME ${MAS_SHN_DIR[build]}
      if pushd "${MAS_SHN_DIR[build]}" &>/dev/null ; then
  #     shn_pwd
	shn_dbgmsg 4 $FUNCNAME ${MAS_SHN_DIR[build]}
	if [[ "$cmd" ]] && [[ "$errname" ]] && [[ "${MAS_SHN_DIR[error]}" ]] && [[ -d "${MAS_SHN_DIR[error]}" ]] && [[ -x "$cmd" ]] ; then
	  shn_dbgmsg 5 $FUNCNAME -- "cmd:$cmd"
	  if $cmd $@  >>$errname 2>&1 ; then
	    shn_dbgmsg 6 $FUNCNAME -- "cmd:$cmd"
	    shn_dbgmsg "$cmd_base $@ # [$MAS_SHN_PROJECT_NAME] v.`shn_project_version` ok"
	    shn_dbgmsg "cmd:" $cmd $@
#	  elif ! [[ "$shn_ignore_error" ]] ; then
	  elif ! [[ "$shn_no_error_message" ]] ; then
	    shn_errmsg 1 "$cmd_base $@ # [$MAS_SHN_PROJECT_NAME] v.`shn_project_version`"
	    shn_show_errors $errname
#    shn_msg "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
	    retcode=1
	  fi
	else
	  shn_errmsg 2 "$cmd_base $@ # [$MAS_SHN_PROJECT_NAME] `shn_project_version`"
	  ! [[ "$cmd" ]]                      && shn_errmsg 2.1 $cmd
	  ! [[ "$errname" ]]                  && shn_errmsg 2.2 $errname
	  ! [[ "${MAS_SHN_DIR[error]}" ]]     && shn_errmsg 2.3 ${MAS_SHN_DIR[error]}
	  ! [[ -d "${MAS_SHN_DIR[error]}" ]]  && shn_errmsg 2.4 ${MAS_SHN_DIR[error]}
	  ! [[ -x "$cmd" ]]                   && shn_errmsg 2.5 $cmd
	  retcode=1
	fi
	popd &>/dev/null
      else
	shn_errmsg 3 "${MAS_SHN_DIR[build]}"
      fi
    else
      shn_errmsg 4 "${MAS_SHN_DIR[build]}"
    fi
  else
    shn_errmsg 5 "${MAS_SHN_DIR[error]}"
  fi
  return $retcode
}
function shn_xcommand ()
{
  local retcode=0
  local errname 
  local cmd=`which $1` ; shift
  local cmd_base=$( shn_basename $cmd )
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg 1 $FUNCNAME
  errname="${MAS_SHN_DIR[error]}/shn_build.${cmd_base}.${MAS_SHN_PROJECT_NAME}.`datemt`.result"
  if [[ "${MAS_SHN_DIR[error]}" ]] && [[ -d "${MAS_SHN_DIR[error]}" ]] ; then
    shn_dbgmsg 2 $FUNCNAME ${MAS_SHN_DIR[error]}
    if [[ "${MAS_SHN_DIR[build]}" ]] && [[ -d "${MAS_SHN_DIR[build]}" ]] ; then
      shn_dbgmsg 3 $FUNCNAME ${MAS_SHN_DIR[build]}
  #     shn_pwd
      shn_dbgmsg 4 $FUNCNAME ${MAS_SHN_DIR[build]}
      if [[ "$cmd" ]] && [[ "$errname" ]] && [[ "${MAS_SHN_DIR[error]}" ]] && [[ -d "${MAS_SHN_DIR[error]}" ]] && [[ -x "$cmd" ]] ; then
	shn_dbgmsg 5 $FUNCNAME -- "cmd:$cmd"
	if $cmd $@  >>$errname 2>&1 ; then
	  shn_dbgmsg 6 $FUNCNAME -- "cmd:$cmd"
	  shn_dbgmsg "$cmd_base $@ # [$MAS_SHN_PROJECT_NAME] v.`shn_project_version` ok"
	  shn_dbgmsg "cmd:" $cmd $@
#	  elif ! [[ "$shn_ignore_error" ]] ; then
	elif ! [[ "$shn_no_error_message" ]] ; then
	  shn_errmsg 1 "$cmd_base $@ # [$MAS_SHN_PROJECT_NAME] v.`shn_project_version`"
	  shn_show_errors $errname
#    shn_msg "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
	  retcode=1
	fi
      else
	shn_errmsg 2 "$cmd_base $@ # [$MAS_SHN_PROJECT_NAME] `shn_project_version`"
	! [[ "$cmd" ]]                      && shn_errmsg 2.1 $cmd
	! [[ "$errname" ]]                  && shn_errmsg 2.2 $errname
	! [[ "${MAS_SHN_DIR[error]}" ]]     && shn_errmsg 2.3 ${MAS_SHN_DIR[error]}
	! [[ -d "${MAS_SHN_DIR[error]}" ]]  && shn_errmsg 2.4 ${MAS_SHN_DIR[error]}
	! [[ -x "$cmd" ]]                   && shn_errmsg 2.5 $cmd
	retcode=1
      fi
    else
      shn_errmsg build "${MAS_SHN_DIR[build]}"
    fi
  else
    shn_errmsg "${MAS_SHN_DIR[error]}"
  fi
  return $retcode
}
function shn_std_list ()
{
  local std dir f
  std=$1
  shift
  dir=${1:-.}
  shift
  shn_dbgmsg "SL1 `pwd` -- $dir" >&2
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg "SL2 `pwd`" >&2
  shn_std_command $std pwd
  if [[ -d "$dir" ]] ; then
    if [[ "$@" ]] ; then
      for f in $@ ; do
#	shn_std_command $std find "$dir" -maxdepth 1 -name "$f" -ls
        shn_std_command $std ls --color=auto -ltAr --time-style='+%b %d %Y %H:%M:%S' $dir/$f || return $?
      done
    else
#     shn_std_command $std find "$dir" -maxdepth 1 -ls
      shn_std_command $std ls --color=auto -ltAr --time-style='+%b %d %Y %H:%M:%S' $dir || return $?
    fi
  fi
  return 0
}
function shn_build_list ()
{
  shn_dbgmsg "BL1 `pwd`" >&2
  shn_std_list build $@
}
function shn_installed_list ()
{
  shn_dbgmsg "IL1 `pwd`" >&2
  shn_std_list flavour $@
}
function shn_build_common_make ()
{
  shn_build_xcommand make -s $@ && shn_msg common make $@ ok || return $?
}
function shn_build_autoconf ()
{
  local retcode=0
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg start maintainer-clean -- $MAS_SHN_PROJECT_NAME -- $MAS_SHN_PROJECT_DIR
  local shn_no_error_message=yes
  shn_build_common_make maintainer-clean # no error
  unset shn_no_error_message
  shn_dbgmsg end maintainer-clean
  if [[ "$MAS_SHN_PROJECT_DIR" ]] && [[ -d "$MAS_SHN_PROJECT_DIR" ]] ; then
    if pushd $MAS_SHN_PROJECT_DIR &>/dev/null ; then
      shn_dbgmsg start autoreconf -i
      autoreconf -i && shn_msg autoconf ok || { retcode=$? ; }
      popd  &>/dev/null
      shn_dbgmsg end autoreconf -i
    fi
  else
    shn_errmsg  "$MAS_SHN_PROJECT_DIR"
  fi
# shn_msg autoconf "$retcode"
  return $retcode
}
function shn_build_configure ()
{
  local global_opts_file=$MAS_SHN_PROJECTS_DIR/configure_opts.${MAS_SHN_FLAVOUR:-default}
  local project_opts_file=$MAS_SHN_PROJECT_DIR/configure_opts.${MAS_SHN_FLAVOUR:-default}
  local configscript=${MAS_SHN_DIR[configure]}/configure
  declare -a MAS_SHN_GLOBAL_CONFIGURE_OPTS
  declare -a MAS_SHN_PROJECT_CONFIGURE_OPTS
  declare -a MAS_SHN_ADD_CONFIGURE_OPTS
  export PKG_CONFIG_PATH="${MAS_SHN_DIR[flavour]}/lib/pkgconfig"
  local configure_opts
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:configure]=`datemt`
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  if [[ "$global_opts_file" ]] && [[ -f "$global_opts_file" ]] ; then
    readarray -t MAS_SHN_GLOBAL_CONFIGURE_OPTS < $global_opts_file
    shn_dbgmsg "global opts from $global_opts_file"
    shn_dbgmsg "global opts : ${MAS_SHN_GLOBAL_CONFIGURE_OPTS[*]}"
  fi
  if [[ "$project_opts_file" ]] && [[ -f "$project_opts_file" ]] ; then
    readarray -t MAS_SHN_PROJECT_CONFIGURE_OPTS < $project_opts_file
    shn_dbgmsg "project opts from $project_opts_file"
    shn_dbgmsg "project opts : ${MAS_SHN_PROJECT_CONFIGURE_OPTS[*]}"
  fi
  MAS_SHN_ADD_CONFIGURE_OPTS[${#MAS_SHN_ADD_CONFIGURE_OPTS[@]}]="--prefix=${MAS_SHN_DIR[flavour]}"
  configure_opts="${MAS_SHN_GLOBAL_CONFIGURE_OPTS[*]} ${MAS_SHN_PROJECT_CONFIGURE_OPTS[*]} ${MAS_SHN_ADD_CONFIGURE_OPTS[*]}"
  shn_dbgmsg "configure_opts : $configure_opts"
  shn_dbgmsg "configure [$MAS_SHN_PROJECT_NAME] `shn_project_version`"
  shn_build_xcommand $configscript  $configure_opts && shn_msg configure ok || return $?
  shn_dbgmsg "C1 `pwd`" >&2
# shn_build_list . config.status config.log config.h
  return 0
}
function shn_build_make ()
{
  shn_dbgmsg "$FUNCNAME 1"
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:make]=`datemt`
  shn_build_common_make && shn_msg make ok || return $?
  shn_dbgmsg "$FUNCNAME 2"
# shn_build_list src || shn_build_list inc
  return 0
}
function shn_build_superclean ()
{
  local f d l
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:superclean]=`datemt`
  for f in configure Makefile.in src/inc/Makefile.in src/Makefile.in inc/Makefile.in aclocal.m4 ; do
    if [[ "$f" ]] && [[ -f "$f" ]] ; then
      shn_msg removing file $f
      rm $f
    fi
  done
  for d in autom4te.cache .auxdir ; do
    if [[ "$d" ]] && [[ -d "$d" ]] ; then
      shn_msg removing dir $d
      rm -Rf $d
    fi
  done
  for l in zocversion.txt vimrc-mastar sh gvim-vimenter.vim gvimrc-mastar gvim-funcs.vim .localrc ; do
    if [[ "$l" ]] && [[ -L "$l" ]] ; then
      shn_msg removing link $l
      rm -Rf $l
    fi
  done
}
function shn_build_clean ()
{
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:clean]=`datemt`
  shn_build_common_make clean && shn_msg make clean ok || return $?
}
function shn_build_distclean ()
{
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:distclean]=`datemt`
  shn_build_common_make distclean && shn_msg make distclean ok || return $?
# shn_build_list
}
function shn_build_install ()
{
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:install]=`datemt`
  shn_build_common_make install && shn_msg installed
}
function shn_build_dist ()
{
  local retcode=0
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  if [[ "${MAS_SHN_DIR[build]}" ]] && [[ -d "${MAS_SHN_DIR[build]}" ]] ; then
    shn_dbgmsg shn_build_dist ${MAS_SHN_DIR[build]}
    shn_build_common_make distcheck && shn_msg make distcheck ok || return $?
#   shn_build_list
    if pushd "${MAS_SHN_DIR[build]}" &>/dev/null ; then
      # TODO for ....
      if [[ "${MAS_SHN_PROJECT_FULLNAME}" ]] && [[ -f "${MAS_SHN_PROJECT_FULLNAME}.tar.gz" ]] && [[ -f "${MAS_SHN_PROJECT_FULLNAME}.tar.bz2" ]] && [[ -d "${MAS_SHN_DIR[savedist]}" ]] ; then
	shn_mv ${MAS_SHN_PROJECT_FULLNAME}.tar.{gz,bz2} "${MAS_SHN_DIR[savedist]}"  || { retcode=$? ; shn_errmsg "mv dist" ; return $retcode ; }
      else
	shn_errmsg "${MAS_SHN_PROJECT_FULLNAME}.tar.gz ${MAS_SHN_PROJECT_FULLNAME}.tar.bz2 ${MAS_SHN_DIR[savedist]}"
      fi
      popd &>/dev/null
    fi
  fi
  shn_dbgmsg "$MAS_SHN_PROJECT_FULLNAME ==> ${MAS_SHN_DIR[savedist]}"
  return 0
}
function shn_build_ebuild_update ()
{
  local retval=0
  local ebuild_prefix='' distname ebname
  local ebuild_dir=${MAS_SHN_DIR[ebuild]}/${ebuild_prefix}${MAS_SHN_PROJECT_NAME}
  shn_dbgmsg "ebuild_dir:$ebuild_dir"
  if ! [[ -d "$ebuild_dir" ]] ; then
    mkdir "$ebuild_dir"
  fi
  if [[ "${MAS_SHN_DIR[savedist]}" ]] && [[ "${MAS_SHN_DIR[savegentoo]}" ]] \
  	&& [[ -d "${MAS_SHN_DIR[savedist]}" ]] && [[ -d "${MAS_SHN_DIR[savegentoo]}" ]] \
	&& [[ -d "$ebuild_dir" ]] \
	&& pushd $ebuild_dir >/dev/null ; then
    ebname_base=$( ls -1tr *.ebuild | tail -1 )
    shn_dbgmsg "ebname_base:$ebname_base"
    ebname="${ebuild_prefix}${MAS_SHN_PROJECT_FULLNAME}.ebuild"
    shn_dbgmsg "ebname:$ebname"

    distname="${MAS_SHN_PROJECT_FULLNAME}.tar.bz2"
    shn_dbgmsg "distname:$distname"
    distfile="${MAS_SHN_DIR[savedist]}/$distname"
    shn_dbgmsg "distfile:$distfile"

    if [[ "$distfile" ]] && [[ -f "$distfile" ]] ; then
      if ! [[ -f "$ebname" ]] ; then
	shn_cp $ebname_base $ebname \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 1 $FUNCNAME ; return $retval ; }
      fi

      if [[ -f $ebname ]] ; then
        shn_chmod a+r $ebname $distfile
	# all versions:
	shn_cp -a  ${MAS_SHN_DIR[savedist]}/${MAS_SHN_PROJECT_NAME}-*.tar.bz2 ${MAS_SHN_DIR[savegentoo]} \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 2 $FUNCNAME ; return $retval ; }
	if [[ "${ebuild_prefix}" ]] ; then 
	  shn_rename "${MAS_SHN_PROJECT_NAME}" "${ebuild_prefix}${MAS_SHN_PROJECT_NAME}" \
	  	${MAS_SHN_DIR[savegentoo]}/${MAS_SHN_PROJECT_NAME}-*.tar.bz2 \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 3 $FUNCNAME ; return $retval ; }
	fi
	shn_cp -a ${MAS_SHN_DIR[savegentoo]}/${ebuild_prefix}$distname  /usr/portage/distfiles/ \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 4 $FUNCNAME ; return $retval ; }
	if [[ -f Manifest ]] ; then
	  shn_rm Manifest \
	  		|| { retval=$? ; popd &>/dev/null ; shn_errmsg 5 $FUNCNAME ; return $retval ; }	  
	fi
	shn_xcommand /usr/bin/ebuild $ebname manifest && shn_msg enuild $ebname manifest - ok || retval=$?
      else
	shn_errmsg $ebname
	retval=1
      fi
    else
      shn_errmsg $distfile
      retval=1
    fi
    popd &>/dev/null
  else
    [[ "${MAS_SHN_DIR[savedist]}" ]]       || shn_errmsg "savedist"
    [[ "${MAS_SHN_DIR[savegentoo]}" ]]     || shn_errmsg "savegentoo"
    [[ -d "${MAS_SHN_DIR[savedist]}" ]]    || shn_errmsg "d savedist"
    [[ -d "${MAS_SHN_DIR[savegentoo]}" ]]  || shn_errmsg "d savegentoo"
    [[ -d "$ebuild_dir" ]]                 || shn_errmsg "d '$ebuild_dir'"
    retval=1
  fi
  return $retval
}
function shn_build_ebuild_check ()
{
  local f name
  local ebuild_prefix='' distname ebname
  local ebuild_dir=${MAS_SHN_DIR[ebuild]}/${ebuild_prefix}${MAS_SHN_PROJECT_NAME}
  local distdir=/usr/portage/distfiles/
  for f in $ebuild_dir/* ; do
    name=$( shn_basename $f )
    if [[ "$name" =~ ^(.*)\.ebuild$ ]] ; then
      short=${BASH_REMATCH[1]}
      distfile1="${short}.tar.gz"
      distfile2="${short}.tar.bz2"
#     if [[ -f "$distdir/$distfile1" ]] ; then
#       shn_msg $short gz ok 
#     else
#       shn_msg $short gz absent : $distdir/$distfile1
#     fi
      if [[ -f "$distdir/$distfile2" ]] ; then
#       shn_msg $short bzip2 ok 
	:
      else
        shn_msg "bz2 absent : $distdir/$distfile2"
        shn_msg "for $f"
      fi
    fi
  done
# shn_msg "$ebuild_dir"
# shn_msg "$distdir"
  return 0
}
