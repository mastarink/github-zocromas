function autoreconf_m ()
{
  if ! [[ -d m4 ]] ; then
    mkdir m4 || return 1
  fi
  if [[ "$build_at" ]] && pushd "$build_at"  >/dev/null ; then
    make -s maintainer-clean >/dev/null
  fi
  popd  >/dev/null || return 1 
  autoreconf -i
  return 0
}
function configure_m ()
{
  local cfgdir
  if [[ "$1" ]] ; then
    cfgdir=$1
  else
    cfgdir=$configuredir
  fi
# echo "$cfgdir/configure $configure_opts" >&2
  if [[ "$cfgdir" ]] && [[ -d "$cfgdir" ]] && [[ -f $cfgdir/configure ]] && [[ -x $cfgdir/configure ]] \
  		&& [[ "$build_at" ]] && pushd "$build_at"  >/dev/null ; then 
    if $cfgdir/configure $configure_opts ; then
      popd >/dev/null || return 1
      return 0
    fi
  fi
  popd >/dev/null
  setup_vers
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
    setup_vers "$mas_name" "$version"
  fi
# show_setup
  if [[ $mas_name ]] && [[ "$mas_vers" ]] ; then
    if [[ -f "$distfile" ]] ; then
#     echo "distfile ok: $distfile" >&2
      remove_unpacked_z
      if [[ "$unpackdir" ]] && [[ -d "$unpackdir" ]] ; then
        echo "$LINENO ERROR not removed: $unpackdir" >&2
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
	      echo "ERROR $LINENO" >&2
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
    errfile="/tmp/distcheck.${name}-${ver}.tmp"
    # make -d dist
    # make -s dist
#       echo "INCLUDE_PATH: $INCLUDE_PATH" >&2
    if ! make_target distcheck >$errfile 2>&1 ; then
      cat $errfile
      echo "$LINENO ERROR testdist" >&2
      return 1
    fi
    zips=$( echo $build_at/${mas_fullname}.tar.{bz2,gz} )
    echo "saving $zips to $savedirdist" >&2
    mv $zips $savedirdist || return 1
    return 0
  fi
  return 1
}
function makd_dinst_script ()
{
  if [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
    if [[ "$rootdir" ]] && [[ "$instshname" ]] && cd $rootdir \
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
      $unpackdir/configure $configure_opts
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
#       echo "  ./configure $configure_opts"	>> $instshname
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
  echo "[$ebuild_dir]" >&2
  if [[ "$ebuild_dir" ]] && [[ -d "$ebuild_dir" ]] ; then
    pushd $ebuild_dir >/dev/null
    ebname="${mas_name}-${mas_vers}.ebuild"
    ebname_base="${mas_name}-${mas_base_vers}.ebuild"
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
        if ! [[ -f "$ebname" ]] && [[ -f "$ebname_base" ]] ; then
	  cp $ebname_base $ebname
	  echo "creating $ebname" >&2
	else
	  echo "$ebname OK" >&2
	fi
	if [[ "$distfile" ]] && [[ -f "$distfile" ]] && [[ -f $ebname ]] ; then
#	  echo "Version:$mas_vers" >&2
#	  echo "distfile:$distfile" >&2
	  if [[ -f "/usr/portage/distfiles/$distname" ]] ; then
	    rm /usr/portage/distfiles/$distname || return 1
	  fi
#	  echo "---saved dist---" >&2
#	  ls -l $savedirdist/${mas_name}-*.tar.bz2 >&2
	  cp -a $savedirdist/${mas_name}-*.tar.bz2 /usr/portage/distfiles/ || return 1
	  if [[ -f Manifest ]] ; then
	    rm Manifest || return 1
	  fi
#	  echo "---portage dist---" >&2
#	  ls -l /usr/portage/distfiles/${mas_name}-*.tar.bz2 >&2
#	  echo "---ebuilds etc.---" >&2
#	  ls -l >&2
	  echo "$mas_vers : updating Manifest" >&2
	  ebuild $ebname manifest 2>/dev/null || return 1
	  return 0
#	  ls -l Manifest || return 1
	else
	  echo "no distfile ($distfile) or no '$ebname'" >&2
	  return 1
	fi
      ;;
      *)
      ;;
    esac
    popd >/dev/null
  fi
  return 1
}

