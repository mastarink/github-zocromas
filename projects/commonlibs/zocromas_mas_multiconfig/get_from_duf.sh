function fromduf ()
{
  local srcdir dstdir f fromf fun fromp newpref=muc_ x et fil
  local -a traces etraces
  local -a  cfiles hfiles hfiles_plus deffiles
  local -a scfiles shfiles shfiles_plus sdeffiles
  unalias cp pushd popd 2>/dev/null
  srcdir=$(realpath ../../bins/zocromas_duf/)
  readarray -t scfiles <scfiles.txt
  readarray -t shfiles <shfiles.txt
  readarray -t shfiles_plus <shfiles_plus.txt
  readarray -t sdeffiles <sdeffiles.txt
  if [[ -d $srcdir ]] ; then
    if pushd src &>/dev/null ; then
#     rm *.c
      if [[ ${#scfiles[@]} -gt 0 ]] ; then
	for (( i=0 ; $i < ${#scfiles[@]} ; i++ )) ; do
	  echo "$i: ${scfiles[$i]##duf_}"
	  if [[ -f "$srcdir/src/${scfiles[$i]}" ]] && git mv "$srcdir/src/${scfiles[$i]}" "${newpref}${scfiles[$i]##duf_}" ; then
	      cfiles+=(${newpref}${scfiles[$i]##duf_})
	  else
	    echo "Error copy $srcdir/src/${scfiles[$i]}" >&2
	    exit 
	  fi
	done
      fi
      if pushd inc &>/dev/null ; then
#       rm *.h *.def
	if [[ ${#shfiles[@]} -gt 0 ]] ; then
	  for (( i=0 ; $i < ${#shfiles[@]} ; i++ )) ; do
	    echo "$i: ${shfiles[$i]##duf_}"
	    if [[ -f "$srcdir/src/inc/${shfiles[$i]}" ]] && git mv "$srcdir/src/inc/${shfiles[$i]}" "${newpref}${shfiles[$i]##duf_}" ; then
	      hfiles+=(${newpref}${shfiles[$i]##duf_})
	    else
	      echo "Error copy $srcdir/src/inc/${shfiles[$i]}" >&2
	      exit
	    fi
	  done
	fi
	if [[ ${#sdeffiles[@]} -gt 0 ]] ; then
	  for (( i=0 ; $i < ${#sdeffiles[@]} ; i++ )) ; do
	    echo "$i: ${sdeffiles[$i]##duf_}"
	    if [[ -f "$srcdir/src/inc/${sdeffiles[$i]}" ]] && git mv "$srcdir/src/inc/${sdeffiles[$i]}" "${newpref}${sdeffiles[$i]##duf_}" ; then
	      deffiles+=(${newpref}${sdeffiles[$i]##duf_})
	    else
	      echo "Error copy $srcdir/src/inc/${sdeffiles[$i]}" >&2
	      exit
	    fi
	  done
	fi

	if [[ ${#shfiles_plus[@]} -gt 0 ]] ; then
	  for (( i=0 ; $i < ${#shfiles_plus[@]} ; i++ )) ; do
	    echo "$i: ${shfiles_plus[$i]##duf_}"
	    if [[ -f "$srcdir/src/inc/${shfiles_plus[$i]}" ]] && cp -a "$srcdir/src/inc/${shfiles_plus[$i]}" "${newpref}${shfiles_plus[$i]##duf_}" ; then
	      hfiles_plus+=(${newpref}${shfiles_plus[$i]##duf_})
	    else
	      echo "Error copy $srcdir/src/inc/${shfiles_plus[$i]}" >&2
	      exit
	    fi
	  done
	fi
	popd &>/dev/null
      fi
      popd &>/dev/null
    fi

    
#   echo "-= (${cfiles[@]}) =-" 
#   echo "-= (${hfiles[@]}) =-" 
#   echo "-= (${hfiles_plus[@]}) =-" 
#   echo "-= (${deffiles[@]}) =-" 
    sed --in-place 's@^\(AC_CONFIG_SRCDIR(\[src/\)\([a-z_]\+\)\(options.c\])\)\s*$@\1'"${newpref}"'\3@' configure.ac || exit
cat <<MMM > src/Makefile.am
SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_mas_multiconfig.la
libzocromas_mas_multiconfig_la_SOURCES = ${cfiles[@]}


libzocromas_mas_multiconfig_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@
MMM
cat <<MMM >src/inc/Makefile.am
libzocromas_mas_multiconfigincludedir = \$(includedir)/mastar/multiconfig
libzocromas_mas_multiconfiginclude_HEADERS =  ${hfiles[@]} ${deffiles[@]} ${hfiles_plus[@]}

# noinst_HEADERS = 

MMM
  traces=($(sed -ne 's@^.*\<\(MAST\|DUF\)_TRACE(\s*\([a-z0-9]\+\),.*).*$@\2@p' src/*.c | sort | uniq ))
  etraces+=("ENUM_WRAPP(ERRORS_TRACE, trace, errors			)")
  etraces+=("ENUM_WRAPP(ERRORR_TRACE, trace, errorr			)")
  etraces+=("ENUM_WRAPP(TEMP_TRACE, trace, temp				)")
  etraces+=("ENUM_WRAPP(TEMPORARY_TRACE, trace, temporary		)")
  for ((i=0 ; $i<${#traces[@]} ; i++ )) ; do
    if ! [[ ${traces[$i]} =~ ^(temp|temporary|errors|errorr)$ ]] ; then
      etraces+=("ENUM_WRAPP(ERRORS_${traces[$i]^^}, trace, ${traces[$i]}			)")
    fi
  done
# echo "${etraces[@]} -----"
cat <<MMM > src/inc/${newpref}optimpl_enum_trace.def
#ifdef ENUM_WRAPP
  /* trace */
MMM
for x in "${etraces[@]}" ; do
  echo "  $x" >> src/inc/${newpref}optimpl_enum_trace.def
done
cat <<MMM >> src/inc/${newpref}optimpl_enum_trace.def

  ENUM_WRAPP(MAX, trace, MAX					)

#endif
/*
vi: ft=c
*/
MMM
    ctags  --c-kinds=f  -x src/*.c src/inc/*.h src/inc/*.def | grep -v '^\(main\|SR\)\>' | grep '^duf_' | sed 's@\s\+function\>.*$@@' > funlist1.tmp
    sed -ne 's@^.*DR(\s*[A-Z]\+,\s*\([a-z][a-z_]\+\).*$@duf_\1@p' src/inc/*.h > funlist2.tmp
    sed -ne 's@^.*SR(\s*[A-Z]\+,\s*\([a-z][a-z_]\+\).*$@duf_\1@p'  src/*.c > funlist2a.tmp
    sed -ne 's@^.*SRP(\s*[A-Z]\+,[^,]\+,[^,]\+,\s*\([a-z][a-z_]\+\).*$@duf_\1@p' src/*.c > funlist2b.tmp
    sed -ne 's@^.*\<F2N(\s*duf_\s*,\s*\([[:alnum:]_]\+\)\s*).*$@duf_\1@gp' src/*.c > funlist3.tmp
    
    sed --in-place  's@\(#\s*include\s\+"\)duf_\(.*\.\(def\|h\)"\)@\1'"${newpref}"'\2@' src/*.c src/inc/*.h  src/inc/*.def || exit
    sed --in-place 's@\<\(duf_\)\([a-z_]\+_[ts]\)\>@'"${newpref}"'\2@g'  src/*.c src/inc/*.h  src/inc/*.def || exit
    sed --in-place 's@\<\(F2N\|F2NW\)(\s*duf_\s*,\s*@\1('"${newpref}"',@g'        src/*.c src/inc/*.h || exit
    sed --in-place 's@\<mas_fundecl_t\>@'"${newpref}"'fundecl_t@g'      src/*.c src/inc/*.h || exit
    sed --in-place 's@\<\(QT\|QTR\|F2N\|QSTR\|STT\|STTT\|T2T\|ETT\|ERT\|DOCR\|CR\|CRV\|IF_CR\|IF_CRV\)\>@'"${newpref}"'\1@g'  src/*.c src/inc/*.h || exit
    sed --in-place 's@\<\(ST\|DT\|ET\|DR\|SR\|ER\)\(P\|\)\>@'"${newpref}"'\1\2@g'  src/*.c src/inc/*.h || exit

    sed --in-place 's@\<DUF_\(TMP_INDEX_MAX\|CONFIG_FILE_NAME\|IS_SOURCE\|LAYER\|FLAG_[a-z]\|START\|END\|FPRINTF\|VFPRINTF\|PUTS\|TRACE\|PRINTF\|TEST_R\|DO_TEST_R\|CLEAR_ERROR\|NUMOPT\|MINMAXOPT\|MOPT\|OPTION_NAME\|OPTION[GAW]\|OPTION_\(STAGE\|CHECK_STAGE\|SOURCE\|CLASS\|CALL_TYPE\|VTYPE\)\|OFFSET\|OPTSRC\|OUTPUTFILE\)@'${newpref^^}'\1@g' src/*.c src/inc/*.h  src/inc/*.def
#?  sed --in-place 's@\<DUF_\(CLEAR_ERROR\)@MASE_\1@g' src/*.c src/inc/*.h  src/inc/*.def
    sed --in-place 's@^\s*\(#\s*define\s\+MAST_TRACE_CONFIG\>\)\s*duf_get_trace_config(\s*)\s*$@\1 duf_get_cli_options_trace_config(cli)@' src/*.c src/inc/*.h
    if pushd src/inc &>/dev/null ; then
      for fil in ${hfiles[@]} ${hfiles_plus[@]} ; do
        local nn="${fil/\./_}"
	nn="MAS_${nn^^}"
#	echo "-------- '$fil' -> '${nn}'" >&2
	sed  --in-place '1,1s@\(#\s*ifndef\s\+\)MAS_\([A-Z_]\+\)_H\s*$@\1'${nn}'@' $fil
        sed  --in-place '2,2s@\(#\s*define\s\+\)MAS_\([A-Z_]\+\)_H\s*$@\1'${nn}'@' $fil
      done
      popd  &>/dev/null
    fi
  #  echo E
    sort  funlist1.tmp funlist2.tmp funlist2a.tmp funlist2b.tmp funlist3.tmp | uniq > funlist.tmp
    if true ; then
      while read fun ; do
       if [[ $fun =~ ^(duf)_([a-z][a-z0-9_]+)$ ]] ; then
	 sfun=${BASH_REMATCH[2]}
         echo "$fun : $sfun"
	 sed --in-place  "s@\<\(duf_\)\($sfun\>\)@${newpref}\2@g" src/*.c src/inc/*.h  src/inc/*.def || exit
       else
	 echo "? $fun"
       fi
      done < funlist.tmp
    fi
  fi
# cp -a src/inc/${newpref}printn_defs0.h  src/inc/${newpref}printn_defs.h
}
fromduf
