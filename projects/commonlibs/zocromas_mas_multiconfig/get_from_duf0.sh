function fromduf ()
{
  local srcdir f fromf fun fromp cfiles hfiles deffiles newpref=muc_
  local -a scfiles shfiles sdeffiles
  unalias cp pushd popd 2>/dev/null
  srcdir=$(realpath ../../bins/zocromas_duf/)
  if [[ -d $srcdir ]] ; then
    if pushd $srcdir/src &>/dev/null ; then
      scfiles=($(echo duf_*option*.c))
      if pushd inc &>/dev/null ; then
        shfiles=($(echo duf_*option*.h))
        sdeffiles=($(echo duf_*option*.def))
        popd &>/dev/null
      fi
      popd &>/dev/null
    fi
    for (( i=0 ; $i < ${#scfiles[@]} ; i++ )) ; do echo "$i: ${scfiles[$i]##duf_}"; done
    ls -l src/*.c | nl
    for (( i=0 ; $i < ${#shfiles[@]} ; i++ )) ; do echo "$i: ${shfiles[$i]##duf_}"; done
    ls -l src/inc/*.h | nl
    echo ${scfiles[@]} > scfiles.tmp
    echo ${shfiles[@]} > shfiles.tmp
    echo ${sdeffiles[@]}> sdeffiles.tmp
#   fromp=mas_
#   rename ${fromp} duf_ -- src/${fromp}*.c src/inc/${fromp}*.h src/inc/${fromp}*.def
    for fromp in masmuc_ mas_ muc_ ; do 
      rename ${fromp} duf_ -- src/${fromp}*.c src/inc/${fromp}*.h src/inc/${fromp}*.def 2>/dev/null
    done
    if pushd src &>/dev/null ; then
      for f in *.c ; do
	if [[ $f =~ ^(duf_)(.*)(\.c)$ ]] ; then
	  fromf=$srcdir/src/duf_${BASH_REMATCH[2]}${BASH_REMATCH[3]}
	  if [[ -f $fromf ]] ; then
#	    echo "$fromf => $f"
	    cp -a $fromf $f
	  fi
	else
	  echo "- $f"
	fi
      done

      if pushd inc &>/dev/null ; then
	for f in *.h ; do
	  if [[ $f =~ ^(duf_)(.*)(\.h)$ ]] ; then
	    fromf=$srcdir/src/inc/duf_${BASH_REMATCH[2]}${BASH_REMATCH[3]}
	    if [[ -f $fromf ]] ; then
#	      echo "$fromf => $f"
	      cp -a $fromf $f
	    fi
	  else
	    echo "- $f"
	  fi
	done
	for f in *.def ; do
	  if [[ $f =~ ^(duf_)(.*)(\.def)$ ]] ; then
	    fromf=$srcdir/src/inc/duf_${BASH_REMATCH[2]}${BASH_REMATCH[3]}
	    if [[ -f $fromf ]] ; then
#	      echo "$fromf => $f"
	      cp -a $fromf $f
	    fi
	  else
	    echo "- $f"
	  fi
	done
	popd &>/dev/null
      fi
      popd &>/dev/null
    fi
    rename duf_ ${newpref} -- src/duf_*.c src/inc/duf_*.h src/inc/duf_*.def
    if pushd src &>/dev/null ; then
      cfiles=$(echo ${newpref}*.c)
      if pushd inc &>/dev/null ; then
	hfiles=$(echo ${newpref}*.h) 
	deffiles=$(echo ${newpref}*.def) 
	popd &>/dev/null
      fi
      popd &>/dev/null
    fi    
    echo "-= (${cfiles}) =-" 
    echo "-= ($hfiles) =-" 
    sed --in-place 's@^\(AC_CONFIG_SRCDIR(\[src/\)\([a-z_]\+\)\(options.c\])\)\s*$@\1'"${newpref}"'\3@' configure.ac
    sed --in-place 's@^\(\s*libzocromas_mas_multiconfig_la_SOURCES\s*=\s*\).*$@\1'"${cfiles}"'@' src/Makefile.am
    sed --in-place 's@^\(#*\s*\(noinst\|libzocromas_mas_multiconfiginclude\)_HEADERS\s*=\s*\).*$@\1'"${hfiles} ${deffiles}"'@' src/inc/Makefile.am
    sed --in-place  's@\(#\s*include\s\+"\)duf_\(.*\.\(def\|h\)"\)@\1'"${newpref}"'\2@' src/*.c src/inc/*.h  src/inc/*.def
    sed --in-place 's@\<\(duf_\)\([a-z_]\+_t\)\>@'"${newpref}"'\2@g' src/*.c src/inc/*.h  src/inc/*.def
    sed --in-place 's@\<F2N(duf_,@F2N('${newpref}',@' src/inc/*.h
    sed --in-place 's@\<F2NW(duf_,@F2NW('${newpref}',@' src/inc/*.h
#?  sed --in-place 's@\<DUF_\(OPTION_\(STAGE\|VTYPE\)_\)@'${newpref^^}'\1@g' src/*.c src/inc/*.h  src/inc/*.def
#?  sed --in-place 's@\<DUF_\(NUMOPT\|MINMAXOPT\|MOPT\)@'${newpref^^}'\1@g' src/*.c src/inc/*.h  src/inc/*.def
#?  sed --in-place 's@\<DUF_\(TRACE\)@MAST_\1@g' src/*.c src/inc/*.h  src/inc/*.def
#?  sed --in-place 's@\<DUF_\(CLEAR_ERROR\)@MASE_\1@g' src/*.c src/inc/*.h  src/inc/*.def
  #  echo E
    ctags  --c-kinds=f  -x src/*.c src/inc/*.h src/inc/*.def | grep -v '^\(main\|SR\)\>' | grep '^duf_' | sed 's@\s\+function\>.*$@@' > funlist1.tmp
    grep -r --inc='*.h' 'DR(\s*OPTIONS,' src/ | sed 's@^.*DR(\s*OPTIONS,\s*\([a-z][a-z_]\+\).*$@duf_\1@' > funlist2.tmp
    sort  funlist1.tmp funlist2.tmp | uniq > funlist.tmp
    if true ; then
      while read fun ; do
       if [[ $fun =~ ^(duf)_([a-z][a-z0-9_]+)$ ]] ; then
	 sfun=${BASH_REMATCH[2]}
    #    echo "$fun : $sfun"
	 sed --in-place  "s@\<\(duf_\)\($sfun\>\)@${newpref}\2@g" src/*.c src/inc/*.h  src/inc/*.def
       else
	 echo "? $fun"
       fi
      done < funlist.tmp
    fi
  fi
# cp -a src/inc/${newpref}printn_defs0.h  src/inc/${newpref}printn_defs.h
}
fromduf
