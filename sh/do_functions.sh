. sh/build_functions.sh
. sh/make_functions.sh

function mas_enabled_doprj ()
{
  local act disfile
  act=$1
  shift
  disfile=$rootdir/disabled_do.txt
  if [[ "$act" ]] && [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
    if [[ -f "$disfile" ]] ; then
  #    echo ">>>>>>>> [$act:$mas_name-$mas_vers] $disfile" >&2
      grep "^${act}$" $disfile && return 1
      grep "^${act}:${mas_name}$" $disfile && return 1
      grep "^${act}:${mas_name}-${mas_vers}$" $disfile && return 1
    fi
  fi
  return 0
}

function doprj ()
{
  local prj act
  local list
  local cnt
  cnt=0
  setup_dirs
  setup_vers
  if [[ "$1" == 'full' ]] ; then
    list='autoreconf configure make install'
  else
    list=$@
  fi
  prj=$(basename `pwd`)
  for act in $list ; do
    if [[ "$act" ]] ; then
      case $act in
        m)
	  act=make
	;;
        a)
	  act=autoreconf
	;;
        c)
	  act=configure
	;;
        t)
	  act=testdist
	;;
        i)
	  act=install
	;;
        u)
	  act=uninstall
	;;
        e)
	  act=ebuild
	;;
      esac
      shfile="sh/${act}.sh"

    # echo "$act at $dir" >&2
      if ! mas_enabled_doprj $act ; then
	echo "SKIP ${act}:${mas_name}-${mas_vers}" >&2
      elif [[ "$act" == 'make'       ]] ; then
	make_m || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ "$act" == 'autoreconf' ]] ; then
	autoreconf_m || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ "$act" == 'configure'  ]] ; then
	configure_m || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ "$act" == 'testdist'   ]] ; then
	testdist_m || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ "$act" == 'install'    ]] ; then
	make_m && make_target install || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ "$act" == 'uninstall'  ]] ; then
	make_target uninstall || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ "$act" == 'ebuild'     ]] ; then
	ebuild_m update || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && ! eval $shfile ; then
	echo "$LINENO FAIL action:$act prj:$prj mas_name:$mas_name ($shfile)" >&2
	return 1
      fi
    else
      echo "${nn}.	>>>> skipping act [$act]" >&2
      return 1
    fi
  done
  echo "DONE $cnt actions for ${mas_name}-${mas_vers}" >&2
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
      echo "-- prj:$prj" >&2
      dir=$( realpath $prj )  || return 1
      if [[ "$prj" ]] && [[ -d "$prj" ]] ; then
	cd $dir                 || return 1
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

