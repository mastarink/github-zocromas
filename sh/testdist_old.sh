#!/bin/sh
if [[ -f "sh/setup.sh" ]] && [[ -f "configure.ac" ]] ; then
  . sh/setup.sh
  function testdist_old ()
  {
    local instshdir instshname
    if [[ "$name" ]] ; then
      # name=$( basename $indir )
  #   name=$( cd $indir && ./configure -V|head -1|awk '{print $1}' )

      if [[ "$name" ]] && [[ "$ver" ]] && [[ "$unpackdir" ]] && [[ "$ibuilddir" ]] ; then
	if cd "$indir" ; then
	# make -d dist
	# make -s dist
  #       echo "INCLUDE_PATH: $INCLUDE_PATH" >&2
	  make -s distcheck || return 1
	  echo "Saving *.tar.{bz2,gz} to $savedirdist" >&2
	  mv *.tar.{bz2,gz} $savedirdist || return 1

	  if cd "$workdir" ; then
	    if [[ -d "$ibuilddir" ]] ; then
	      rm -Rf "$ibuilddir" || return 1
	    fi
	    mkdir -p "$ibuilddir" || return 1

	    if [[ -d "$unpackdir" ]] ; then
	      rm -Rf "$unpackdir" || return 1
	    fi
	    if cd $workdir && [[ -f "$distfile" ]] ; then
	      tar -jxf "$distfile" || return 1
	      if [[ "$ibuilddir" ]] && cd "$ibuilddir" ; then
		pwd
		echo "wbuilddir  : $wbuilddir"
		echo "ibuilddir  : $ibuilddir"
		echo "workdir    : $workdir"
		echo "unpackdir  : $unpackdir"
	      # ls -l ../zocromas-*/configure
	      # sleep 5
		if [[ -f "$unpackdir/configure" ]] ; then
		  configuredir=$unpackdir
		  configure_m || return 1
  #		$unpackdir/configure --enable-tracemem --enable-debug --silent --enable-silent-rules || return 1
		  make -s || return 1
	      # ls -lAtr src
		else
		  echo "No file $unpackdir/configure" >&2
		  return 1
		fi
	      else
		echo "No dir $ibuilddir" >&2
		return 1
	      fi
	    else
	      echo "No file: $distfile OR no dir $workdir" >&2
	      return 1
	    fi
	  else
	    echo "No dir: $workdir" >&2
	    return 1
	  fi
	else
	  echo "No dir: '$indir'" >&2
	  return 1
	fi
      fi
      echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2
      echo "$unpackdir">&2
      echo "TODO as root:">&2
      echo "cd $ibuilddir && make install" >&2
      instshdir="$rootdir/install.sh"
      if [[ "$rootdir" ]] && cd $rootdir && [[ "$name" ]] && [[ "$ver" ]] && [[ -d "$instshdir" ]]; then
	instshname="$instshdir/${name}-${ver}.sh"
	if ! [[ -f "$instshname" ]] ; then
	  echo '#!/bin/sh' >> $instshname
	  echo "# $( date )" >> $instshname
	  echo "cd $ibuilddir && make install" >> $instshname
	  chmod +x "$instshname"
	fi
      fi
      echo "name: $name" >&2
      echo "ver: $ver" >&2
      echo "instshdir: $instshdir" >&2
      echo "instshname: $instshname" >&2
      echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2
    fi
    return 0
  }
  testdist_old $@
fi

