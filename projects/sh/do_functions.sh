. sh/build_functions.sh
. sh/libmake_functions.bash

function mas_enabled_doprj ()
{
  local act disfile
  act=$1
  shift
  disfile=$projectsdir/disabled_do.txt
##    echo ">>>>>>>> [$act:$mas_name-$mas_vers] $disfile" >&2
##    echo ">>>>>>>> `pwd`" >&2
  if [[ "$act" ]] && [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
    if [[ -f "$disfile" ]] ; then
      grep "^${act}$" $disfile &>/dev/null && return 1
      grep "^${act}:${mas_name}$" $disfile &>/dev/null && return 1
      grep "^${act}:${mas_name}-${mas_vers}$" $disfile &>/dev/null && return 1
    fi
  fi
  return 0
}

function doprj ()
{
  local prj act shfile shfun
  local list
  local cnt
  cnt=0
  setup_dirs || return 1
  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "$prjname $act ..." >&2 ; fi
  setup_vers || return 1
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
        Z)
	  act=virgin-clean
	;;
      esac
      shfile="sh/${act}.sh"
      shfun="dosh_${act}"

    # echo "$act at $dir" >&2
      if ! mas_enabled_doprj $act ; then
#	echo "SKIPped ${act}:${mas_name}-${mas_vers}" >&2
        echo -e "$prjname action $act \t SKIPped" >&2
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
      elif [[ "$act" == 'virgin-clean'     ]] ; then
	virgin_clean || return 1
        cnt=$(( $cnt + 1 ))
      elif [[ "$shfun" ]] && type -t $shfun &>/dev/null ; then
#	echo "shfun:`type $shfun`"
        $shfun
      elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && . $shfile ; then
        :
      elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && eval $shfile ; then
        :
      else
	echo "$LINENO FAIL action:$act prj:$prj mas_name:$mas_name ($shfile)" >&2
	return 1
      fi
    else
      echo "${nn}.	>>>> skipping act [$act]" >&2
      return 1
    fi
    echo -e "$prjname action $act \t done" >&2
  done
# echo "DONE $cnt actions for ${mas_name}-${mas_vers}" >&2
  return 0
}
function doall ()
{  
  local prj nn doit dir
  if [[ -d "$tmpdir" ]] && [[ "$projectsdir" ]] && [[ "$projects_list" ]] && [[ -d "$projectsdir" ]] ; then
    nn=0
    if ! [[ "$MAS_DO_FROM_PROJECT" ]] ; then
      doit='yes'
    fi
    MAS_PROJECT_POS=0
    for prj in $projects_list ; do
      MAS_PROJECT_POS=$(( $MAS_PROJECT_POS + 1 ))
      unset prj_configure_opts
      if [[ "$MAS_DO_FROM_PROJECT" ]] && [[ "$prj" =~ $MAS_DO_FROM_PROJECT ]] ; then
        doit='yes'
      fi
      if [[ "$doit" ]] ; then
	cd "$projectsdir" || return 1
# 	echo "-- prj:$prj" >&2
#	dir=$( realpath $prj )  || return 1
	if [[ "$prj" ]] && [[ -d "$prj" ]] ; then
	  cd "$prj"                 || return 1
	  doprj $@ || return 1
	else
	  echo "${nn}.	>>>> skipping dir [$prj]" >&2
	fi
      fi
      nn=$(( $nn + 1 ))
    done
  else
    return 1
  fi
  return 0
}
