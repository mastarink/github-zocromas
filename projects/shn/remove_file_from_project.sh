#!/bin/bash

function prepare_file ()
{
  local file=$1
  local bfile
  if [[ $file =~ ^\<\(duf[a-z_]*\)\.[ch]\>$ ]] ; then
    bfile=${BASH_REMATCH[1]}
  else
    bfile=$file
  fi
  if [[ -f "src/${bfile}.c" ]] && [[ -f "src/inc/${bfile}.h" ]] \
  	&& grep -q "${bfile}.c" src/Makefile.am && grep -q "${bfile}.h" src/inc/Makefile.am \
	&& edfile=$(grep -l "\<\(sfind\|find\)\s\+${bfile}\.[ch]\s*$" editing/mased/*.vim ) && [[ -f $edfile ]]  ; then
    bfiles[$bfile]=$edfile
#   sed --in-place 's@\<'$bfile'\.c\>@@g' src/Makefile.am_test
#   sed --in-place 's@\<'$bfile'\.h\>@@g' src/inc/Makefile.am_test
  fi
}
function remove_all_from_c_am ()
{
  local -A bfiles
  local f efile
  for f in $@ ; do
    prepare_file $f
  done
  for bfile in ${!bfiles[@]} ; do
    efile=${bfiles[$bfile]}
    echo "bfile: ${bfile} : ${efile}" >&2
    if [[ -f "src/$bfile.c" ]] && [[ "src/inc/$bfile.h" ]] ; then
      if  \
         git rm src/$bfile.c src/inc/$bfile.h \
	   && \
         sed --in-place=.sedtmp 's@\<'$bfile'\.c\>@@'  src/Makefile.am \
	   && \
         sed --in-place=.sedtmp 's@\<'$bfile'\.h\>@@'  src/inc/Makefile.am \
	   && \
         sed --in-place=.sedtmp 's@^\(.*\<'$bfile'\.[ch]\>\)@" \1@'  $efile \
	   && \
	 sed --in-place=.sedtmp 's@^\s*\(#\s*include\>\s*"'"$bfile"'\.h\>"\)@\/\* \1 \*\/@' src/*.c src/inc/*.h
      then
        echo "OK $bfile" >&2
      else
        echo "Error 2 $bfile" >&2
      fi
    else
      echo "Error 1 $bfile" >&2
    fi
  done
}
remove_all_from_c_am $@
