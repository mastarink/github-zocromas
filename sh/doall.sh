#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh

function doall ()
{  
  local name act prj dir atdir nn
  if [[ -d "$rootdir/tmp" ]] ; then
    if [[ "$rootdir" ]] && [[ -d "$rootdir" ]] && cd $rootdir ; then
      if [[ -f projects.list ]] ; then
	list=`cat projects.list`
#	echo "pwd: {$( pwd )}" >&2
      else
	list=$( echo $rootdir/{zocmds,zocromas,zoctypes,commonlibs/*} )
      fi
      nn=0
      for prj in $list ; do
#	echo "pwd: {$( pwd )}" >&2
#	echo "[$dir]" >&2
	if [[ "$prj" ]] && [[ -d "$prj" ]] ; then
	  dir=$( realpath $prj ) || return 1
	  cd $dir || return 1
##	    echo "[$dir]" >&2
	  name=$( basename $dir ) || return 1
	  for act in $@ ; do
	    shfile="sh/${act}.sh"
	    if [[ "$act" ]] && [[ -L "sh" ]] && [[ -x $shfile ]] ; then
	      echo "${nn}.	$act $prj" >&2
	    # echo "$act at $dir" >&2
###	      if ! eval $shfile > $rootdir/tmp/doall.$name.log ; then
	      if ! eval $shfile ; then
	        echo "FAIL $act $prj" >&2
		return 1
	      fi
	    else
	      echo "${nn}.	>>>> skipping act $act" >&2
	      return 1
	    fi
	  done
	else
	  echo "${nn}.	>>>> skipping dir $dir" >&2
	fi
	cd $rootdir || return 1
      nn=$(( $nn + 1 ))
      done
    else
      return 1
    fi
  else
    return 1
  fi
  return 0
}
doall $@
