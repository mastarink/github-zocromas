. sh/build_functions.sh
. sh/make_functions.sh

function doprj ()
{
  local prj act
  local list
  
  setup_dirs
  if [[ "$1" == 'full' ]] ; then
    list='autoreconf configure make install'
  else
    list=$@
  fi
  prj=$(basename `pwd`)
  for act in $list ; do
    shfile="sh/${act}.sh"
    if [[ "$act" ]] ; then
      echo "${nn}.	$act $mas_name" >&2
    # echo "$act at $dir" >&2
      if false ; then
	echo "SKIP $shfile @ `pwd`" >&2
      elif [[ "$act" == 'make'       ]] || [[ "$act" == 'm' ]] ; then
	make_m || return 1
      elif [[ "$act" == 'autoreconf' ]] || [[ "$act" == 'a' ]] ; then
	autoreconf_m || return 1
      elif [[ "$act" == 'configure'  ]] || [[ "$act" == 'c' ]] ; then
	configure_m || return 1
      elif [[ "$act" == 'testdist'   ]] || [[ "$act" == 't' ]] ; then
	testdist_m || return 1
      elif [[ "$act" == 'install'    ]] || [[ "$act" == 'i' ]] ; then
	make_m && make_target install || return 1
      elif [[ "$act" == 'uninstall'  ]] || [[ "$act" == 'u' ]] ; then
	make_target uninstall || return 1
      elif [[ "$act" == 'ebuild'     ]] || [[ "$act" == 'e' ]] ; then
	ebuild_m update || return 1
      elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && ! eval $shfile ; then
	echo "$LINENO FAIL action:$act prj:$prj mas_name:$mas_name ($shfile)" >&2
	return 1
      fi
    else
      echo "${nn}.	>>>> skipping act [$act]" >&2
      return 1
    fi
  done
  return 0
}
function doall ()
{  
  local prj nn
  if [[ -d "$tmpdir" ]] && [[ "$rootdir" ]] && [[ -d "$rootdir" ]] && cd $rootdir ; then
    if [[ -f projects.list ]] ; then
      list=`cat projects.list`
#	echo "pwd: {$( pwd )}" >&2
    else
      echo "not exists projects.list" >&2
      return 1
    fi
    nn=0
    for prj in $list ; do
      cd $rootdir
      dir=$( realpath $prj )  || return 1
      if [[ "$prj" ]] && [[ -d "$prj" ]] ; then
	cd $dir                 || return 1
#	setup_vers
	doprj $@ || return 1
      else
	echo "${nn}.	>>>> skipping dir [$dir]" >&2
      fi
      nn=$(( $nn + 1 ))
    done
  else
    return 1
  fi
  return 0
}

