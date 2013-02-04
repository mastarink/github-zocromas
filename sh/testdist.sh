#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh
  # name=$( basename $indir )
  name=$( cd $indir && ./configure -V|head -1|awk '{print $1}' )
  distfile="${name}-${ver}.tar.bz2"

  if [[ "$name" ]] && [[ "$ver" ]] && [[ "$unpackdir" ]] && [[ "$builddir" ]] ; then
    if cd "$indir" ; then
    # make -d dist
    # make -s dist
      make -s distcheck
      echo "Saving *.tar.{bz2,gz} to $savedir" >&2
      mv *.tar.{bz2,gz} $savedir

      if cd "$workdir" ; then
	if [[ -d "$builddir" ]] ; then
	  rm -Rf "$builddir"
	fi
	mkdir -p "$builddir"

	if [[ -d "$unpackdir" ]] ; then
	  rm -Rf "$unpackdir"
	fi
	if cd $workdir && [[ -f "$savedir/$distfile" ]] ; then
	  tar -jxf "$savedir/$distfile"
	  if cd $builddir ; then
    #	pwd
	  # ls -l ../zocromas-*/configure
	  # sleep 5
	    if [[ -f "$unpackdir/configure" ]] ; then
	      $unpackdir/configure --enable-tracemem --enable-debug --silent
	      make -s
	  # ls -lAtr src
	    fi
	  fi
	else
	  echo "No file: $savedir/$distfile OR no dir $unpackdir" >&2
	fi
      else
	echo "No dir: $workdir" >&2
      fi
    else
      echo "No dir: '$indir'" >&2
    fi
  fi
  echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2
  echo "$unpackdir">&2
  echo "TODO as root:">&2
  echo "cd $builddir && make install" >&2
  echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2

