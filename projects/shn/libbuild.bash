function shn_mark_touch ()
{
  local name=$1
  local marksdir=${MSH_SHN_DIRS[marks]}
  if [[ $name ]] && [[ $MSH_SHN_PROJECT_DIR ]] && [[ -d $MSH_SHN_PROJECT_DIR ]] ; then
    if ! [[ -d $marksdir ]] ; then
      mkdir $marksdir
    fi
    if [[ -d $marksdir ]] ; then
      touch $marksdir/${name}
    fi
  fi
}
function shn_std_command ()
{
  local std=$1 retcode=0
  shift
  shn_dbgmsg "SC1 `pwd`" >&2
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg "SC2 `pwd`" >&2
  if [[ "${MSH_SHN_DIRS[$std]}" ]] && [[ -d "${MSH_SHN_DIRS[$std]}" ]] ; then
    if pushd "${MSH_SHN_DIRS[$std]}" &>/dev/null ; then
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
# echo " A `declare -p MSH_SHN_DIRS`" >&2
  if [[ "${MSH_SHN_DIRS[error]}" ]] && [[ -d "${MSH_SHN_DIRS[error]}" ]] ; then
    shn_dbgmsg 2 $FUNCNAME ${MSH_SHN_DIRS[error]}
    if [[ "${MSH_SHN_DIRS[build]}" ]] && [[ -d "${MSH_SHN_DIRS[build]}" ]] ; then
      shn_dbgmsg 3 $FUNCNAME ${MSH_SHN_DIRS[build]}
      if pushd "${MSH_SHN_DIRS[build]}" &>/dev/null ; then
  #     shn_pwd
	shn_dbgmsg 4 $FUNCNAME ${MSH_SHN_DIRS[build]}
	cmd=`which $cmd` || { retcode=$? ; popd &>/dev/null ; return $retcode ; }
	cmd_base=$( shn_basename $cmd ) || { retcode=$? ; popd &>/dev/null ; return $retcode ; }
	errname="${MSH_SHN_DIRS[error]}/shn_build.${cmd_base}.${MSH_SHN_PROJECT_NAME}.result" \
			|| { retcode=$? ; popd &>/dev/null ; return $retcode ; }
	if [[ "$cmd" ]] && [[ "$errname" ]] && [[ "${MSH_SHN_DIRS[error]}" ]] && [[ -d "${MSH_SHN_DIRS[error]}" ]] && [[ -x "$cmd" ]] ; then
	  shn_dbgmsg 5 $FUNCNAME -- "cmd:$cmd"
	  echo "$cmd $@ @ $(pwd)" >&2
	  if $cmd $@  >$errname 2>&1 ; then
	    shn_dbgmsg 6 $FUNCNAME -- "cmd:$cmd"
	    shn_dbgmsg "$cmd_base $@ # [$MSH_SHN_PROJECT_FULLNAME] ok"
	    shn_dbgmsg "cmd:" $cmd $@
#	  elif ! [[ "$shn_ignore_error" ]] ; then
	  elif ! [[ "$shn_no_error_message" ]] ; then
	    shn_errmsg "$cmd_base $@ ------"
	    shn_show_errors $errname
#    shn_msg "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
	    retcode=1
	  fi
	else
	  shn_errmsg 2 "cmd_base:$cmd_base; cmt:$cmd; errname:$errname; $@ # [$MSH_SHN_PROJECT_NAME] `shn_project_version`"
	  ! [[ "$cmd" ]]                      && shn_errmsg 2.1 $cmd
	  ! [[ "$errname" ]]                  && shn_errmsg 2.2 $errname
	  ! [[ "${MSH_SHN_DIRS[error]}" ]]     && shn_errmsg 2.3 ${MSH_SHN_DIRS[error]}
	  ! [[ -d "${MSH_SHN_DIRS[error]}" ]]  && shn_errmsg 2.4 ${MSH_SHN_DIRS[error]}
	  ! [[ -x "$cmd" ]]                   && shn_errmsg 2.5 $cmd
	  retcode=1
	fi
	popd &>/dev/null
      else
	shn_errmsg 3 "Can't cd build dir: ${MSH_SHN_DIRS[build]}"
      fi
    else
      shn_errmsg 4 "No build dir: ${MSH_SHN_DIRS[build]}"
    fi
  else
    shn_errmsg 5 "error dir : ${MSH_SHN_DIRS[error]}"
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
  errname="${MSH_SHN_DIRS[error]}/shn_build.${cmd_base}.${MSH_SHN_PROJECT_NAME}.result"
  if [[ "${MSH_SHN_DIRS[error]}" ]] && [[ -d "${MSH_SHN_DIRS[error]}" ]] ; then
    shn_dbgmsg 2 $FUNCNAME ${MSH_SHN_DIRS[error]}
    if [[ "${MSH_SHN_DIRS[build]}" ]] && [[ -d "${MSH_SHN_DIRS[build]}" ]] ; then
      shn_dbgmsg 3 $FUNCNAME ${MSH_SHN_DIRS[build]}
  #     shn_pwd
      shn_dbgmsg 4 $FUNCNAME ${MSH_SHN_DIRS[build]}
      if [[ "$cmd" ]] && [[ "$errname" ]] && [[ "${MSH_SHN_DIRS[error]}" ]] && [[ -d "${MSH_SHN_DIRS[error]}" ]] && [[ -x "$cmd" ]] ; then
	shn_dbgmsg 5 $FUNCNAME -- "cmd:$cmd"
	if $cmd $@  >$errname 2>&1 ; then
	  shn_dbgmsg 6 $FUNCNAME -- "cmd:$cmd"
	  shn_dbgmsg "$cmd_base $@ # [$MSH_SHN_PROJECT_FULLNAME] ok"
	  shn_dbgmsg "cmd:" $cmd $@
#	  elif ! [[ "$shn_ignore_error" ]] ; then
	elif ! [[ "$shn_no_error_message" ]] ; then
	  shn_errmsg 1 "$cmd_base $@ # [$MSH_SHN_PROJECT_FULLNAME]"
	  shn_show_errors $errname
#    shn_msg "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
	  retcode=1
	fi
      else
	shn_errmsg 2 "$cmd_base $@ # [$MSH_SHN_PROJECT_NAME] `shn_project_version`"
	! [[ "$cmd" ]]                      && shn_errmsg 2.1 $cmd
	! [[ "$errname" ]]                  && shn_errmsg 2.2 $errname
	! [[ "${MSH_SHN_DIRS[error]}" ]]     && shn_errmsg 2.3 ${MSH_SHN_DIRS[error]}
	! [[ -d "${MSH_SHN_DIRS[error]}" ]]  && shn_errmsg 2.4 ${MSH_SHN_DIRS[error]}
	! [[ -x "$cmd" ]]                   && shn_errmsg 2.5 $cmd
	retcode=1
      fi
    else
      shn_errmsg build "${MSH_SHN_DIRS[build]}"
    fi
  else
    shn_errmsg "${MSH_SHN_DIRS[error]}"
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
# shn_build_xcommand make -s $@ && shn_msgns common make $@ ok || return $?
  shn_build_xcommand make -s $@ && shn_msgns . || return $?
}
function shn_build_autoreconf ()
{
  local retcode=0 errname
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  shn_dbgmsg start maintainer-clean -- $MSH_SHN_PROJECT_NAME -- $MSH_SHN_PROJECT_DIR
  local shn_no_error_message=yes
  shn_build_common_make maintainer-clean # no error
  unset shn_no_error_message
  shn_dbgmsg end maintainer-clean
  if [[ "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$MSH_SHN_PROJECT_DIR" ]] ; then
    shn_mark_touch ${FUNCNAME}.0
    if pushd $MSH_SHN_PROJECT_DIR &>/dev/null ; then
      shn_dbgmsg start autoreconf -i $MSH_SHN_PROJECT_DIR
      errname="${MSH_SHN_DIRS[error]}/shn_build_autoreconf.${MSH_SHN_PROJECT_NAME}.result"
      shn_msg "see $errname"
      autoreconf -i $MSH_SHN_PROJECT_DIR &>$errname && shn_msgns autoconf ok || { retcode=$? ; }
      shn_msg "see $errname"
      shn_dbgmsg end autoreconf -i
      popd &>/dev/null
    fi
    shn_mark_touch ${FUNCNAME}
  else
    shn_errmsg  "$MSH_SHN_PROJECT_DIR"
  fi
# shn_msg autoconf "$retcode"
  return $retcode
}
function shn_build_configure ()
{
  local configure_opts="$@"
  local global_opts_file=$MSH_SHN_PROJECTS_DIR/configure_opts.${MSH_SHN_FLAVOUR:-default}
  local project_opts_file=$MSH_SHN_PROJECT_DIR/configure_opts.${MSH_SHN_FLAVOUR:-default}
  local configscript=${MSH_SHN_DIRS[configure]}/configure
  declare -a MSH_SHN_GLOBAL_CONFIGURE_OPTS
  declare -a MSH_SHN_PROJECT_CONFIGURE_OPTS
  declare -a MSH_SHN_ADD_CONFIGURE_OPTS
  declare -a MSH_SHN_CONFIGURE_OPTS
  export PKG_CONFIG_PATH="${MSH_SHN_DIRS[flavour]}/lib/pkgconfig"
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:configure]=`datemt`
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  if [[ "$global_opts_file" ]] && [[ -f "$global_opts_file" ]] ; then
    readarray -t MSH_SHN_GLOBAL_CONFIGURE_OPTS < $global_opts_file
    shn_dbgmsg "global opts from $global_opts_file"
    shn_dbgmsg "global opts : ${MSH_SHN_GLOBAL_CONFIGURE_OPTS[*]}"
  fi
  if [[ "$project_opts_file" ]] && [[ -f "$project_opts_file" ]] ; then
    readarray -t MSH_SHN_PROJECT_CONFIGURE_OPTS < $project_opts_file
    shn_dbgmsg "project opts from $project_opts_file"
    shn_dbgmsg "project opts : ${MSH_SHN_PROJECT_CONFIGURE_OPTS[*]}"
    local o
  fi
# MSH_SHN_ADD_CONFIGURE_OPTS[${#MSH_SHN_ADD_CONFIGURE_OPTS[@]}]="--prefix=${MSH_SHN_DIRS[flavour]}"
  MSH_SHN_ADD_CONFIGURE_OPTS=( '--prefix='${MSH_SHN_DIRS[flavour]} )
  MSH_SHN_CONFIGURE_OPTS=()
  MSH_SHN_CONFIGURE_OPTS+=(${MSH_SHN_GLOBAL_CONFIGURE_OPTS[*]/\#*/})
  MSH_SHN_CONFIGURE_OPTS+=(${MSH_SHN_PROJECT_CONFIGURE_OPTS[*]/\#*/})
  MSH_SHN_CONFIGURE_OPTS+=(${MSH_SHN_ADD_CONFIGURE_OPTS[*]/\#*/})
  shn_msg "-= configure with: =-"
  for o in ${MSH_SHN_CONFIGURE_OPTS[*]} ; do
    shn_msg " $o"
  done
  configure_opts+=" ${MSH_SHN_GLOBAL_CONFIGURE_OPTS[*]/\#*/} ${MSH_SHN_PROJECT_CONFIGURE_OPTS[*]/\#*/} ${MSH_SHN_ADD_CONFIGURE_OPTS[*]/\#*/}"

  shn_dbgmsg "configure_opts : $configure_opts"
  shn_msg "$configscript  $configure_opts @ [$MSH_SHN_PROJECT_NAME] `shn_project_version`"
  shn_build_xcommand $configscript  $configure_opts && shn_msgns configure ok || return $?
  shn_dbgmsg "C1 `pwd`" >&2
# shn_build_list . config.status config.log config.h
  shn_mark_touch ${FUNCNAME}
  return 0
}
function shn_build_recheck ()
{
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:recheck]=`datemt`
  shn_msg $FUNCNAME 1
  shn_setup_projects || return $?
  shn_msg $FUNCNAME 2
  local shn_dont_setup=yes
  shn_dbgmsg "recheck [$MSH_SHN_PROJECT_NAME] `shn_project_version`"
  shn_msg $FUNCNAME 3
  shn_build_xcommand ./config.status --recheck && shn_msgns recheck ok || return $?
  shn_msg $FUNCNAME 4
  shn_mark_touch ${FUNCNAME}
  return 0
}
function shn_build_make ()
{
  shn_dbgmsg "$FUNCNAME 1"
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:make]=`datemt`
#	  if [[ $MSH_SHN_BUILD_DIR ]] && pushd $MSH_SHN_BUILD_DIR >/dev/null && $cmd $@  >$errname 2>&1 ; then
  shn_build_common_make && shn_msgns make ok || return $?
  shn_dbgmsg "$FUNCNAME 2"
# shn_build_list src || shn_build_list inc
  shn_mark_touch ${FUNCNAME}
  return 0
}
function shn_build_superclean ()
{
  local wash=$1
  shift
  local f d l
# rm -Rf Makefile.in autom4te.cache configure aclocal.m4 vrb.tmp ltmain.sh ac.mased.viminfo src.mased.viminfo config.h Makefile config.h.in config.h.in~ config.log libtool stamp-h1
#
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:superclean]=`datemt`
  shn_msg "for $MSH_SHN_PROJECT_NAME:"
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
	"${MSH_SHN_PROJECT_NAME}.pc"
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
  for l in sh vimrc-mastar.vim gvimrc-mastar.vim gvim-vimenter.vim gvim-funcs.vim .localrc ; do
    if [[ "$l" ]] && [[ -L "$l" ]] ; then
      shn_msg removing link $l
      rm -Rf $l
    fi
  done
  if [[ "$wash" == wash ]] ; then
    for l in zocversion.txt zocvdate.txt m4zoc shn ; do
      if [[ "$l" ]] && [[ -L "$l" ]] ; then
	shn_msg removing link $l
	rm -Rf $l
      fi
    done
  fi
  shn_mark_touch ${FUNCNAME}
  return 0
}
function shn_build_clean ()
{
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:clean]=`datemt`
  shn_build_common_make clean && shn_msgns make clean ok || return $?
  shn_mark_touch ${FUNCNAME}
  return 0
}
function shn_build_distclean ()
{
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:distclean]=`datemt`
  shn_build_common_make distclean && shn_msgns make distclean ok || return $?
# shn_build_list
  shn_mark_touch ${FUNCNAME}
  return 0
}
function shn_build_install ()
{
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:install]=`datemt`
  shn_build_common_make install && shn_msgns installed
  shn_mark_touch ${FUNCNAME}
  return 0
}
function shn_build_uninstall ()
{
  MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:uninstall]=`datemt`
  shn_build_common_make uninstall && shn_msgns uninstalled
  shn_mark_touch ${FUNCNAME}
  return 0
}

function shn_build_dist ()
{
  local retcode=0
  shn_setup_projects || return $?
  local shn_dont_setup=yes
  if [[ "${MSH_SHN_DIRS[build]}" ]] && [[ -d "${MSH_SHN_DIRS[build]}" ]] ; then
    shn_dbgmsg shn_build_dist ${MSH_SHN_DIRS[build]}
    if shn_build_common_make distcheck ; then
      shn_msgns make distcheck ok
    else
      shn_errmsg "distcheck"
      return $?
    fi
    shn_msgns '⇒ «dist»'
#   shn_build_list
    if pushd "${MSH_SHN_DIRS[build]}" &>/dev/null ; then
      # TODO for ....
      if [[ "${MSH_SHN_PROJECT_FULLNAME}" ]] && \
      			[[ -f "${MSH_SHN_PROJECT_FULLNAME}.tar.gz"  ]] && \
      			[[ -f "${MSH_SHN_PROJECT_FULLNAME}.tar.bz2" ]] && [[ -d "${MSH_SHN_DIRS[savedist]}" ]] ; then
#	ls -l ${MSH_SHN_PROJECT_FULLNAME}.tar.{gz,bz2} >&2
	shn_mv ${MSH_SHN_PROJECT_FULLNAME}.tar.{gz,bz2} "${MSH_SHN_DIRS[savedist]}"  || \
			{ retcode=$? ; shn_errmsg "mv dist" ; }
	shn_dbgmsg "moved to ${MSH_SHN_DIRS[savedist]}"
      else
#	ls -l ${MSH_SHN_PROJECT_FULLNAME}.tar.{gz,bz2} >&2
	shn_errmsg "${MSH_SHN_PROJECT_FULLNAME}.tar.gz ${MSH_SHN_PROJECT_FULLNAME}.tar.bz2 ${MSH_SHN_DIRS[savedist]}"
	retcode=1
      fi
      popd &>/dev/null
    else
      retcode=1
    fi
  else
    retcode=1
  fi
  shn_dbgmsg "$MSH_SHN_PROJECT_FULLNAME ==> ${MSH_SHN_DIRS[savedist]}"
  shn_mark_touch ${FUNCNAME}
  return $retcode
}
# function shn_build_ebuild_update_here ()
# {
# }
function shn_build_ebuild_update ()
{
  local retval=0
  local ebuild_prefix='' distname ebname
  local ebuild_dir=${MSH_SHN_DIRS[ebuild]}/${ebuild_prefix}${MSH_SHN_PROJECT_NAME}
  shn_dbgmsg "ebuild_dir:$ebuild_dir"
  if ! [[ -d "$ebuild_dir" ]] ; then
    shn_mkdir "$ebuild_dir"
  fi
  if [[ "${MSH_SHN_DIRS[savedist]}" ]] && [[ "${MSH_SHN_DIRS[savegentoo]}" ]] \
  	&& [[ -d "${MSH_SHN_DIRS[savedist]}" ]] && [[ -d "${MSH_SHN_DIRS[savegentoo]}" ]] \
	&& [[ -d "$ebuild_dir" ]] \
	&& pushd $ebuild_dir >/dev/null ; then
    ebname_base=$( ls -1tr *.ebuild 2>/dev/null | tail -1 )
    if ! [[ "$ebname_base" ]] || ! [[ -f $ebname_base ]] ; then
      shn_errmsg $LINENO $FUNCNAME E
      return 1
    fi
    shn_dbgmsg "ebname_base:$ebname_base"
    ebname="${ebuild_prefix}${MSH_SHN_PROJECT_FULLNAME}.ebuild"
    shn_dbgmsg "ebname:$ebname"

    distname="${MSH_SHN_PROJECT_FULLNAME}.tar.bz2"
    shn_dbgmsg "distname:$distname"
    distfile="${MSH_SHN_DIRS[savedist]}/$distname"
    shn_dbgmsg "distfile:$distfile"

    if [[ "$distfile" ]] && [[ -f "$distfile" ]] ; then
      if ! [[ -f "$ebname" ]] ; then
	shn_cp $ebname_base $ebname \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg $LINENO $FUNCNAME E ; return $retval ; }
      fi

      if [[ -f $ebname ]] ; then
        shn_chmod a+r $ebname $distfile
	# all versions:
	shn_cp -a  ${MSH_SHN_DIRS[savedist]}/${MSH_SHN_PROJECT_NAME}-*.tar.bz2 ${MSH_SHN_DIRS[savegentoo]} \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg $LINENO $FUNCNAME E ; return $retval ; }
	if [[ "${ebuild_prefix}" ]] ; then 
	  shn_rename "${MSH_SHN_PROJECT_NAME}" "${ebuild_prefix}${MSH_SHN_PROJECT_NAME}" \
	  	${MSH_SHN_DIRS[savegentoo]}/${MSH_SHN_PROJECT_NAME}-*.tar.bz2 \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg $LINENO $FUNCNAME E ; return $retval ; }
	fi
	shn_cp -a ${MSH_SHN_DIRS[savegentoo]}/${ebuild_prefix}$distname  /usr/portage/distfiles/ \
			|| { retval=$? ; popd &>/dev/null ; shn_errmsg $LINENO $FUNCNAME E ; return $retval ; }
	if [[ -f Manifest ]] ; then
	  shn_rm Manifest \
	  		|| { retval=$? ; popd &>/dev/null ; shn_errmsg $LINENO $FUNCNAME E ; return $retval ; }	  
	fi
#	shn_xcommand /usr/bin/ebuild $ebname manifest && shn_msgns ebuild $ebname manifest - ok || retval=$?
	shn_xcommand /usr/bin/ebuild $ebname manifest && shn_msgns «manifest $MSH_SHN_PROJECT_FULLNAME» || retval=$?
      else
	shn_errmsg $LINENO $FUNCNAME $ebname E
	retval=1
      fi
    else
      shn_errmsg $LINENO $FUNCNAME $distfile E
      retval=1
    fi
    popd &>/dev/null
  else
    [[ "${MSH_SHN_DIRS[savedist]}" ]]       || shn_errmsg "savedist"
    [[ "${MSH_SHN_DIRS[savegentoo]}" ]]     || shn_errmsg "savegentoo"
    [[ -d "${MSH_SHN_DIRS[savedist]}" ]]    || shn_errmsg "d savedist"
    [[ -d "${MSH_SHN_DIRS[savegentoo]}" ]]  || shn_errmsg "d savegentoo"
    [[ -d "$ebuild_dir" ]]                 || shn_errmsg "d '$ebuild_dir'"
    retval=1
  fi
  shn_mark_touch ${FUNCNAME}
  return $retval
}
function shn_build_ebuild_check ()
{
  local f name
  local ebuild_prefix='' distname ebname
  local ebuild_dir=${MSH_SHN_DIRS[ebuild]}/${ebuild_prefix}${MSH_SHN_PROJECT_NAME}
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
        shn_errmsg "bz2 absent : $distdir/$distfile2 for $f"
      fi
    fi
  done
# shn_msg "$ebuild_dir"
# shn_msg "$distdir"
  shn_mark_touch ${FUNCNAME}
  return 0
}
