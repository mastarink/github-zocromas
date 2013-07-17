function autoreconf_m ()
{
  if [[ "$build_at" ]] && pushd "$build_at"  >/dev/null ; then
    make -s maintainer-clean &>/dev/null
  fi
  popd  >/dev/null || return 1 
  autoreconf -i &>/dev/null || return 1
  return 0
}
function configure_m ()
{
  local cfgdir
  if [[ "$1" ]] ; then
    cfgdir=$1
    shift
  else
    cfgdir=$configuredir
  fi
# echo "$cfgdir/configure $prj_configure_opts" >&2
  if [[ "$cfgdir" ]] && [[ -d "$cfgdir" ]] && [[ -f $cfgdir/configure ]] && [[ -x $cfgdir/configure ]] \
  		&& [[ "$build_at" ]] ; then
    if pushd "$build_at"  >/dev/null ; then 
      if $cfgdir/configure $prj_configure_opts $@ ; then
	popd >/dev/null || return 1
	return 0
      fi
      popd >/dev/null
    fi
  fi
# setup_vers
  return 1
}
function remove_unpacked_z ()
{
  if [[ "$unpackdir" ]] && [[ -d "$unpackdir" ]] ; then
#   echo "TO REMOVE $unpackdir" >&2
    rm -Rf "$unpackdir"
  fi
}
function unpack_z ()
{
  if [[ "$tmpunpack" ]] && [[ -d "$tmpunpack" ]] && [[ "$distfile" ]] && [[ -f "$distfile" ]] && cd "$tmpunpack" ; then
    if [[ "$distfile" =~ \.tar\.gz ]] ; then
#     echo "gz unpack $distfile (`pwd`)" >&2
      tar -zxf $distfile
    elif [[ "$distfile" =~ \.tar\.bz2 ]] ; then
#     echo "bz2 unpack $distfile (`pwd`)" >&2
      tar -jxf $distfile
    fi
  fi
}
function install_z ()
{
  local version
  version=$1
  shift
  if [[ $mas_name ]] && [[ "$version" ]] ; then
    if type -t setup_vers &>/dev/null ; then setup_vers "$mas_name" "$version" || return 1 ; fi
  fi
# show_setup
  if [[ $mas_name ]] && [[ "$mas_vers" ]] ; then
    if [[ -f "$distfile" ]] ; then
#     echo "distfile ok: $distfile" >&2
      remove_unpacked_z
      if [[ "$unpackdir" ]] && [[ -d "$unpackdir" ]] ; then
        zoc_error "$LINENO" "${BASH_SOURCE[0]}" not removed: $unpackdir
#        echo "$LINENO ERROR not removed: $unpackdir" >&2
        return 1
      fi
      unpack_z
      if [[ "$unpackdir" ]] ; then
#       echo "unpack at $unpackdir" >&2
#       if [[ -d "$unpackdir" ]] ; then
#         echo "unpack dir exists: $unpackdir" >&2
#       else
#         echo "unpack dir absent: $unpackdir" >&2
#       fi
	if [[ -d "$unpackdir" ]] ; then
	  if [[ "$ibuilddir" ]] ; then
#	    echo "to build at $ibuilddir" >&2
	    if [[ -d "$ibuilddir" ]] ; then
#	      echo "build dir exists: $ibuilddir" >&2
	      rm -Rf $ibuilddir
	    fi
	    if mkdir "$ibuilddir" ; then
#             echo "configuredir: $configuredir" >&2
#             echo "From $unpackdir" >&2
#             echo "At `pwd`" >&2
	      build_at="$ibuilddir" || return 1
 	      configure_m $unpackdir || return 1
	      make_m
	      make_target install
	    else
#	      echo "ERROR $LINENO" >&2
              zoc_error "$LINENO" "${BASH_SOURCE[0]}" "mkdir $ibuilddir"
	      return 1
	    fi
	  fi
	fi	
      else
	echo "ERROR $LINENO" >&2
	return 1
      fi
    else
      echo "ERROR $LINENO" >&2
      (
        cd $savedirdist
        ls -ltr ${mas_name}*.tar.{gz,bz2} >&2
      )
      return 1
    fi
  else
    echo "ERROR $LINENO mas_name:$mas_name; mas_vers:$mas_vers;" >&2
    return 1
  fi
  return 0
}
function testdist_m ()
{
  local zips errfile
  echo "PKG_CONFIG_PATH: $PKG_CONFIG_PATH" >&2
  if [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
    errfile="/tmp/distcheck.${mas_name}-${mas_vers}.tmp"
    # make -d dist
    # make -s dist
#       echo "INCLUDE_PATH: $INCLUDE_PATH" >&2
    if ! make_target distcheck ; then
      zoc_error "$LINENO" "${BASH_SOURCE[0]}" "can't make testdist"
#     echo "$LINENO ERROR testdist" >&2
      return 1
    fi
    zips=$( echo $build_at/${mas_fullname}.tar.{bz2,gz} )
    echo "saving $zips to $savedirdist" >&2
    mv $zips $savedirdist || return 1
    if [[ -f $savedirdist/${mas_fullname}.tar.bz2 ]] && [[ -d /mnt/new_misc/vboxshared/zoc/ ]] ; then
      cp $savedirdist/${mas_fullname}.tar.bz2  /mnt/new_misc/vboxshared/zoc/
      ls -l /mnt/new_misc/vboxshared/zoc/${mas_fullname}.tar.bz2
      chmod a+r /mnt/new_misc/vboxshared/zoc/${mas_fullname}.tar.bz2
    fi
    return 0
  fi
  return 1
}
function makd_dinst_script ()
{
  if [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
    if [[ "$projectsdir" ]] && [[ "$instshname" ]] && cd $projectsdir \
    			&& [[ "$mas_name" ]] && [[ "$mas_vers" ]] && [[ "$instshdir" ]] && [[ -d "$instshdir" ]] ; then
      if [[ -f "$instshname" ]] ; then
        rm "$instshname"
      fi
      if ! [[ -f "$instshname" ]] ; then
        cat <<SC > "$instshname"
#!/bin/sh
# $( date )
if [[ -d "$tmpunpack" ]] ; then
  if [[ "$unpackdir" ]] && [[ -d $unpackdir ]] ; then
    rm -Rf $unpackdir
  fi
  if [[ "$ibuilddir" ]] && [[ -d $ibuilddir ]] ; then
    rm -Rf $ibuilddir
  fi
  cd $tmpunpack
  tar -jxf $distfile
  if [[ -d "$unpackdir" ]] ; then
    if mkdir $ibuilddir && cd $ibuilddir ; then
      $unpackdir/configure $prj_configure_opts
      make -s
      make -s install
    fi
  fi
fi
SC
#       echo '#!/bin/sh'			>> $instshname
#       echo "# $( date )"			>> $instshname
#       echo "rm -Rf $unpackdir $ibuilddir"	>> $instshname
#       echo "cd $tmpunpack"			>> $instshname
#       echo "tar -jxf $distfile"		>> $instshname
#       echo "if [[ -d \"$unpackdir\" ]] ; then" >> $instshname
#       echo "  mkdir $ibuilddir"		>> $instshname
#       echo "  cd $ibuilddir"		>> $instshname
#       echo "  ./configure $prj_configure_opts"	>> $instshname
#       echo "  make -s"			>> $instshname
#       echo "  make install"			>> $instshname
#       echo "fi"				>> $instshname
        chmod +x "$instshname"
      fi
    fi
#   echo "mas_name: $mas_name" >&2
#   echo "mas_vers: $mas_vers" >&2
#   echo "instshname: $instshname" >&2
#   echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2
  fi
  return 0
}
function ebuild_m ()
{
  local cmd ebname ebname_base
  cmd=$1
  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "[ebuild: $ebuild_dir]" >&2 ; fi
  if [[ "$ebuild_dir" ]] && [[ -d "$ebuild_dir" ]] ; then
    pushd $ebuild_dir >/dev/null
         ebname="${ebuild_prefix}${mas_name}-${mas_vers}.ebuild"
    case $cmd in
      list)
	ls -l
      ;;
      check)
	if [[ -f $ebname ]] ; then
	  ls -l $ebname
	else
	  echo "no file '$ebname'"
	  return 1
	fi
      ;;
      update)
        if ! [[ -f "$ebname" ]] ; then 
	  ebname_base="${ebuild_prefix}${mas_name}-${mas_base_vers}.ebuild"
	  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "creating $ebname ebuild from $ebname_base (${ebuild_prefix}${mas_name}-${mas_base_vers}.ebuild)" >&2 ; fi
	  ebname_base=$( ls -1tr *.ebuild | tail -1 )
	  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "creating $ebname ebuild from $ebname_base" >&2 ; fi
	  if [[ "$ebname_base" ]] && [[ -f "$ebname_base" ]] ; then
	    cp $ebname_base $ebname
	    echo "created $ebname" >&2
	  else
	    zoc_error "$LINENO" "${BASH_SOURCE[0]}" "can't create ebuild - no template"
	  fi
	else
	  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "$ebname made" >&2 ; fi
	fi
	chmod a+r $ebname
	if [[ "$distfile" ]] && [[ -f "$distfile" ]] && [[ -f $ebname ]] \
			&& [[ "$savedirdist" ]] && [[ "$savedirgentoo" ]] \
			&& [[ -d "$savedirdist" ]] && [[ -d "$savedirgentoo" ]] ; then
#	  echo "Version:$mas_vers" >&2
#	  echo "distfile:$distfile" >&2
########  if [[ -f "/usr/portage/distfiles/$distname" ]] ; then
########    rm /usr/portage/distfiles/$distname || return 1
########  fi
#	  echo "---saved dist---" >&2
#	  ls -l $savedirdist/${mas_name}-*.tar.bz2 >&2
          if [[ "$MAS_SH_VERBOSE" ]] ; then echo "1. $savedirdist/${mas_name}-*.tar.bz2" >&2 ; fi
 	  cp -a  $savedirdist/${mas_name}-*.tar.bz2 $savedirgentoo/ || return 1
          if [[ "$MAS_SH_VERBOSE" ]] ; then echo "2. $savedirgentoo/${mas_name}-*" >&2 ; fi
 	  /usr/bin/rename "$mas_name" "${ebuild_prefix}${mas_name}" $savedirgentoo/${mas_name}-*.tar.bz2 || return 1
          if [[ "$MAS_SH_VERBOSE" ]] ; then echo "3. $savedirgentoo/${ebuild_prefix}${mas_name}-${mas_vers}.tar.bz2" >&2 ; fi
	  /bin/cp -a $savedirgentoo/${ebuild_prefix}${mas_name}-${mas_vers}.tar.bz2 /usr/portage/distfiles/ || return 1
	  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "4." >&2 ; fi
	  if [[ -f Manifest ]] ; then
	    rm Manifest || return 1
	  fi
	  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "5." >&2 ; fi
#	  echo "---portage dist---" >&2
#	  ls -l /usr/portage/distfiles/${mas_name}-*.tar.bz2 >&2
#	  echo "---ebuilds etc.---" >&2
#	  ls -l >&2
	  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "6. $mas_vers : updating Manifest" >&2 ; fi
	  ebuild $ebname manifest &>/dev/null || return 1
	  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "7." >&2 ; fi
	  return 0
#	  ls -l Manifest || return 1
	else
	  echo "no distfile ($distfile) or no '$ebname'" >&2
	  return 1
	fi
      ;;
      *)
        echo "unknown cmd fpr ebuild_m : '$cmd'" >&2
      ;;
    esac
    popd >/dev/null
  else
    if [[ "$mas_name" == 'zocromas' ]] ; then
      echo "skipping '$ebuild_dir'" >&2
      return 0
    else
      echo "ERROR: [$mas_name] no dir '$ebuild_dir'" >&2
    fi
  fi
  return 1
}

