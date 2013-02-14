function autoreconf_m ()
{
  if [[ "$indir" ]] && [[ -d "$indir" ]] ; then
    cd $indir
    if ! [[ -d m4 ]] ; then
      mkdir m4
    fi
    make -s maintainer-clean
    autoreconf -i
  fi
}
function configure_m ()
{
  local cfgdir
  if [[ "$1" ]] ; then
    cfgdir=$1
  else
    cfgdir=$configuredir
  fi
  echo "$cfgdir/configure $configure_opts" >&2
  $cfgdir/configure $configure_opts || return 1
  return 0
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
      echo "gz unpack $distfile (`pwd`)" >&2
      tar -zxf $distfile
    elif [[ "$distfile" =~ \.tar\.bz2 ]] ; then
      echo "bz2 unpack $distfile (`pwd`)" >&2
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
      echo "distfile ok: $distfile" >&2
      remove_unpacked_z
      if [[ "$unpackdir" ]] && [[ -d "$unpackdir" ]] ; then
        echo "$LINENO ERROR not removed: $unpackdir" >&2
        return 1
      fi
      unpack_z
      if [[ "$unpackdir" ]] ; then
        echo "unpack at $unpackdir" >&2
        if [[ -d "$unpackdir" ]] ; then
          echo "unpack dir exists: $unpackdir" >&2
	else
          echo "unpack dir absent: $unpackdir" >&2
        fi
	if [[ -d "$unpackdir" ]] ; then
	  if [[ "$ibuilddir" ]] ; then
	    echo "to build at $ibuilddir" >&2
	    if [[ -d "$ibuilddir" ]] ; then
	      echo "build dir exists: $ibuilddir" >&2
	      rm -Rf $ibuilddir
	    fi
	    if mkdir "$ibuilddir" && cd "$ibuilddir" ; then
	      echo "configuredir: $configuredir" >&2
	      echo "From $unpackdir" >&2
	      echo "At `pwd`" >&2
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
function testdist ()
{
  local zips errfile
  if [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
    errfile="/tmp/distcheck.${name}-${ver}.tmp"
    # make -d dist
    # make -s dist
#       echo "INCLUDE_PATH: $INCLUDE_PATH" >&2
    if ! make -s distcheck >$errfile 2>&1 ; then
      cat $errfile
      return 1
    fi
    zips=$( echo ${indir}/${mas_fullname}.tar.{bz2,gz} )
    echo "Saving $zips to $savedirdist" >&2
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
    echo "mas_name: $mas_name" >&2
    echo "mas_vers: $mas_vers" >&2
    echo "instshname: $instshname" >&2
    echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2
  fi
  return 0
}
