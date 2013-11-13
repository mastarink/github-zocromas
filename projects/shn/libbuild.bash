function shn_std_command ()
{
  local std=$1 retcode=0
  shift
  shn_dbgmsg "SC1 `pwd`" >&2
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg "SC2 `pwd`" >&2
  if [[ "${MAS_SHN_DIRS[$std]}" ]] && [[ -d "${MAS_SHN_DIRS[$std]}" ]] ; then
    if pushd "${MAS_SHN_DIRS[$std]}" &>/dev/null ; then
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
  local cmd=$1
  shift
  local cmd_base
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg 1 $FUNCNAME
# echo " A `declare -p MAS_SHN_DIRS`" >&2
  if [[ "${MAS_SHN_DIRS[error]}" ]] && [[ -d "${MAS_SHN_DIRS[error]}" ]] ; then
    shn_dbgmsg 2 $FUNCNAME ${MAS_SHN_DIRS[error]}
    if [[ "${MAS_SHN_DIRS[build]}" ]] && [[ -d "${MAS_SHN_DIRS[build]}" ]] ; then
      shn_dbgmsg 3 $FUNCNAME ${MAS_SHN_DIRS[build]}
      if pushd "${MAS_SHN_DIRS[build]}" &>/dev/null ; then
  #     shn_pwd
	shn_dbgmsg 4 $FUNCNAME ${MAS_SHN_DIRS[build]}
	cmd=`which $cmd` || { retcode=$? ; popd &>/dev/null ; return $retcode ; }
	cmd_base=$( shn_basename $cmd ) || { retcode=$? ; popd &>/dev/null ; return $retcode ; }
	errname="${MAS_SHN_DIRS[error]}/shn_build.${cmd_base}.${MAS_SHN_PROJECT_NAME}.result" \
			|| { retcode=$? ; popd &>/dev/null ; return $retcode ; }
	if [[ "$cmd" ]] && [[ "$errname" ]] && [[ "${MAS_SHN_DIRS[error]}" ]] && [[ -d "${MAS_SHN_DIRS[error]}" ]] && [[ -x "$cmd" ]] ; then
	  shn_dbgmsg 5 $FUNCNAME -- "cmd:$cmd"
	  if $cmd $@  >$errname 2>&1 ; then
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
	  ! [[ "${MAS_SHN_DIRS[error]}" ]]     && shn_errmsg 2.3 ${MAS_SHN_DIRS[error]}
	  ! [[ -d "${MAS_SHN_DIRS[error]}" ]]  && shn_errmsg 2.4 ${MAS_SHN_DIRS[error]}
	  ! [[ -x "$cmd" ]]                   && shn_errmsg 2.5 $cmd
	  retcode=1
	fi
	popd &>/dev/null
      else
	shn_errmsg 3 "${MAS_SHN_DIRS[build]}"
      fi
    else
      shn_errmsg 4 "${MAS_SHN_DIRS[build]}"
    fi
  else
    shn_errmsg 5 "error dir : ${MAS_SHN_DIRS[error]}"
  fi
  return $retcode
}
function shn_xcommand ()
{
  local retcode=0
  local errname 
  local cmd=`which $1` || return $?
  shift
  local cmd_base=$( shn_basename $cmd )
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg 1 $FUNCNAME
  errname="${MAS_SHN_DIRS[error]}/shn_build.${cmd_base}.${MAS_SHN_PROJECT_NAME}.result"
  if [[ "${MAS_SHN_DIRS[error]}" ]] && [[ -d "${MAS_SHN_DIRS[error]}" ]] ; then
    shn_dbgmsg 2 $FUNCNAME ${MAS_SHN_DIRS[error]}
    if [[ "${MAS_SHN_DIRS[build]}" ]] && [[ -d "${MAS_SHN_DIRS[build]}" ]] ; then
      shn_dbgmsg 3 $FUNCNAME ${MAS_SHN_DIRS[build]}
  #     shn_pwd
      shn_dbgmsg 4 $FUNCNAME ${MAS_SHN_DIRS[build]}
      if [[ "$cmd" ]] && [[ "$errname" ]] && [[ "${MAS_SHN_DIRS[error]}" ]] && [[ -d "${MAS_SHN_DIRS[error]}" ]] && [[ -x "$cmd" ]] ; then
	shn_dbgmsg 5 $FUNCNAME -- "cmd:$cmd"
	if $cmd $@  >$errname 2>&1 ; then
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
	! [[ "${MAS_SHN_DIRS[error]}" ]]     && shn_errmsg 2.3 ${MAS_SHN_DIRS[error]}
	! [[ -d "${MAS_SHN_DIRS[error]}" ]]  && shn_errmsg 2.4 ${MAS_SHN_DIRS[error]}
	! [[ -x "$cmd" ]]                   && shn_errmsg 2.5 $cmd
	retcode=1
      fi
    else
      shn_errmsg build "${MAS_SHN_DIRS[build]}"
    fi
  else
    shn_errmsg "${MAS_SHN_DIRS[error]}"
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
  shn_build_xcommand make -s $@ && shn_msgns common make $@ ok || return $?
}
function shn_build_autoreconf ()
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
    touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}.0
    if pushd $MAS_SHN_PROJECT_DIR &>/dev/null ; then
      shn_dbgmsg start autoreconf -i
      autoreconf -i && shn_msgns autoconf ok || { retcode=$? ; }
      popd  &>/dev/null
      shn_dbgmsg end autoreconf -i
    fi
    touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
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
  local configscript=${MAS_SHN_DIRS[configure]}/configure
  declare -a MAS_SHN_GLOBAL_CONFIGURE_OPTS
  declare -a MAS_SHN_PROJECT_CONFIGURE_OPTS
  declare -a MAS_SHN_ADD_CONFIGURE_OPTS
  export PKG_CONFIG_PATH="${MAS_SHN_DIRS[flavour]}/lib/pkgconfig"
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
  MAS_SHN_ADD_CONFIGURE_OPTS[${#MAS_SHN_ADD_CONFIGURE_OPTS[@]}]="--prefix=${MAS_SHN_DIRS[flavour]}"
  configure_opts="${MAS_SHN_GLOBAL_CONFIGURE_OPTS[*]} ${MAS_SHN_PROJECT_CONFIGURE_OPTS[*]} ${MAS_SHN_ADD_CONFIGURE_OPTS[*]}"
  shn_dbgmsg "configure_opts : $configure_opts"
  shn_dbgmsg "configure [$MAS_SHN_PROJECT_NAME] `shn_project_version`"
  shn_build_xcommand $configscript  $configure_opts && shn_msgns configure ok || return $?
  shn_dbgmsg "C1 `pwd`" >&2
# shn_build_list . config.status config.log config.h
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
  return 0
}
function shn_build_recheck ()
{
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:recheck]=`datemt`
  shn_msg $FUNCNAME 1
  shn_setup_projects || return $?
  shn_msg $FUNCNAME 2
  local shn_dont_setup=yes
  shn_dbgmsg "recheck [$MAS_SHN_PROJECT_NAME] `shn_project_version`"
  shn_msg $FUNCNAME 3
  shn_build_xcommand ./config.status --recheck && shn_msgns recheck ok || return $?
  shn_msg $FUNCNAME 4
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
  return 0
}
function shn_build_make ()
{
  shn_dbgmsg "$FUNCNAME 1"
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:make]=`datemt`
  shn_build_common_make && shn_msgns make ok || return $?
  shn_dbgmsg "$FUNCNAME 2"
# shn_build_list src || shn_build_list inc
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
  return 0
}
function shn_build_superclean ()
{
  local wash=$1
  shift
  local f d l
# rm -Rf Makefile.in autom4te.cache configure aclocal.m4 vrb.tmp ltmain.sh ac.mased.viminfo src.mased.viminfo config.h Makefile config.h.in config.h.in~ config.log libtool stamp-h1
#
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:superclean]=`datemt`
  shn_msg "for $MAS_SHN_PROJECT_NAME:"
  for f in \
	aclocal.m4 \
	ac.mased.viminfo \
	autom4te.cache \
	config.h \
	config.h.in \
	config.h.in~ \
	config.log \
	config.status \
	configure \
	inc/Makefile.in \
	inc/Makefile \
	libtool \
	ltmain.sh \
	Makefile \
	Makefile.in \
	sh.mased.viminfo \
	shn.mased.viminfo \
	src/inc/Makefile.in \
	src/inc/Makefile \
	src/Makefile.in \
	src/Makefile \
	src.mased.viminfo \
	stamp-h1 \
	vrb.tmp \
	"${MAS_SHN_PROJECT_NAME}.pc"
  do
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
  for l in vimrc-mastar sh gvim-vimenter.vim gvimrc-mastar gvim-funcs.vim .localrc ; do
    if [[ "$l" ]] && [[ -L "$l" ]] ; then
      shn_msg removing link $l
      rm -Rf $l
    fi
  done
  if [[ "$wash" == wash ]] ; then
    for l in zocversion.txt m4zoc shn ; do
      if [[ "$l" ]] && [[ -L "$l" ]] ; then
	shn_msg removing link $l
	rm -Rf $l
      fi
    done
  fi
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
}
function shn_build_clean ()
{
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:clean]=`datemt`
  shn_build_common_make clean && shn_msgns make clean ok || return $?
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
}
function shn_build_distclean ()
{
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:distclean]=`datemt`
  shn_build_common_make distclean && shn_msgns make distclean ok || return $?
# shn_build_list
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
}
function shn_build_install ()
{
  MAS_SHN_LAST_ACTION[$MAS_SHN_PROJECT_NAME:install]=`datemt`
  shn_build_common_make install && shn_msgns installed
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
}
function shn_build_dist ()
{
  local retcode=0
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  if [[ "${MAS_SHN_DIRS[build]}" ]] && [[ -d "${MAS_SHN_DIRS[build]}" ]] ; then
    shn_dbgmsg shn_build_dist ${MAS_SHN_DIRS[build]}
    shn_build_common_make distcheck && shn_msgns make distcheck ok || return $?
#   shn_build_list
    if pushd "${MAS_SHN_DIRS[build]}" &>/dev/null ; then
      # TODO for ....
      if [[ "${MAS_SHN_PROJECT_FULLNAME}" ]] && [[ -f "${MAS_SHN_PROJECT_FULLNAME}.tar.gz" ]] && \
      			[[ -f "${MAS_SHN_PROJECT_FULLNAME}.tar.bz2" ]] && [[ -d "${MAS_SHN_DIRS[savedist]}" ]] ; then
	shn_mv ${MAS_SHN_PROJECT_FULLNAME}.tar.{gz,bz2} "${MAS_SHN_DIRS[savedist]}"  || \
			{ retcode=$? ; shn_errmsg "mv dist" ; return $retcode ; }
      else
	shn_errmsg "${MAS_SHN_PROJECT_FULLNAME}.tar.gz ${MAS_SHN_PROJECT_FULLNAME}.tar.bz2 ${MAS_SHN_DIRS[savedist]}"
      fi
      popd &>/dev/null
    fi
  fi
  shn_dbgmsg "$MAS_SHN_PROJECT_FULLNAME ==> ${MAS_SHN_DIRS[savedist]}"
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
  return 0
}
function shn_build_ebuild_update ()
{
  local retval=0
  local ebuild_prefix='' distname ebname
  local ebuild_dir=${MAS_SHN_DIRS[ebuild]}/${ebuild_prefix}${MAS_SHN_PROJECT_NAME}
  shn_dbgmsg "ebuild_dir:$ebuild_dir"
  if ! [[ -d "$ebuild_dir" ]] ; then
    shn_mkdir "$ebuild_dir"
  fi
  if [[ "${MAS_SHN_DIRS[savedist]}" ]] && [[ "${MAS_SHN_DIRS[savegentoo]}" ]] \
  	&& [[ -d "${MAS_SHN_DIRS[savedist]}" ]] && [[ -d "${MAS_SHN_DIRS[savegentoo]}" ]] \
	&& [[ -d "$ebuild_dir" ]] \
	&& pushd $ebuild_dir >/dev/null ; then
    ebname_base=$( ls -1tr *.ebuild | tail -1 )
    shn_dbgmsg "ebname_base:$ebname_base"
    ebname="${ebuild_prefix}${MAS_SHN_PROJECT_FULLNAME}.ebuild"
    shn_dbgmsg "ebname:$ebname"

    distname="${MAS_SHN_PROJECT_FULLNAME}.tar.bz2"
    shn_dbgmsg "distname:$distname"
    distfile="${MAS_SHN_DIRS[savedist]}/$distname"
    shn_dbgmsg "distfile:$distfile"

    if [[ "$distfile" ]] && [[ -f "$distfile" ]] ; then
      if ! [[ -f "$ebname" ]] ; then
	shn_cp $ebname_base $ebname \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 1 $FUNCNAME ; return $retval ; }
      fi

      if [[ -f $ebname ]] ; then
        shn_chmod a+r $ebname $distfile
	# all versions:
	shn_cp -a  ${MAS_SHN_DIRS[savedist]}/${MAS_SHN_PROJECT_NAME}-*.tar.bz2 ${MAS_SHN_DIRS[savegentoo]} \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 2 $FUNCNAME ; return $retval ; }
	if [[ "${ebuild_prefix}" ]] ; then 
	  shn_rename "${MAS_SHN_PROJECT_NAME}" "${ebuild_prefix}${MAS_SHN_PROJECT_NAME}" \
	  	${MAS_SHN_DIRS[savegentoo]}/${MAS_SHN_PROJECT_NAME}-*.tar.bz2 \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 3 $FUNCNAME ; return $retval ; }
	fi
	shn_cp -a ${MAS_SHN_DIRS[savegentoo]}/${ebuild_prefix}$distname  /usr/portage/distfiles/ \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg 4 $FUNCNAME ; return $retval ; }
	if [[ -f Manifest ]] ; then
	  shn_rm Manifest \
	  		|| { retval=$? ; popd &>/dev/null ; shn_errmsg 5 $FUNCNAME ; return $retval ; }	  
	fi
	shn_xcommand /usr/bin/ebuild $ebname manifest && shn_msgns enuild $ebname manifest - ok || retval=$?
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
    [[ "${MAS_SHN_DIRS[savedist]}" ]]       || shn_errmsg "savedist"
    [[ "${MAS_SHN_DIRS[savegentoo]}" ]]     || shn_errmsg "savegentoo"
    [[ -d "${MAS_SHN_DIRS[savedist]}" ]]    || shn_errmsg "d savedist"
    [[ -d "${MAS_SHN_DIRS[savegentoo]}" ]]  || shn_errmsg "d savegentoo"
    [[ -d "$ebuild_dir" ]]                 || shn_errmsg "d '$ebuild_dir'"
    retval=1
  fi
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
  return $retval
}
function shn_build_ebuild_check ()
{
  local f name
  local ebuild_prefix='' distname ebname
  local ebuild_dir=${MAS_SHN_DIRS[ebuild]}/${ebuild_prefix}${MAS_SHN_PROJECT_NAME}
  local distdir=/usr/portage/distfiles/
  for f in $ebuild_dir/* ; do
    name=$( shn_basename $f )
    if [[ "$name" =~ ^(.*)\.ebuild$ ]] ; then
      short=${BASH_REMATCH[1]}
      distfile1="${short}.tar.gz"
      distfile2="${short}.tar.bz2"
#     if [[ -f "$distdir/$distfile1" ]] ; then
#       shn_msgns $short gz ok 
#     else
#       shn_msg $short gz absent : $distdir/$distfile1
#     fi
      if [[ -f "$distdir/$distfile2" ]] ; then
#       shn_msgns $short bzip2 ok 
	:
      else
        shn_msg "bz2 absent : $distdir/$distfile2"
        shn_msg "for $f"
      fi
    fi
  done
# shn_msg "$ebuild_dir"
# shn_msg "$distdir"
  touch $MAS_SHN_PROJECT_DIR/.${FUNCNAME}
  return 0
}
