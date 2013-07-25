. ${MAS_PROJECTS_DIR:-.}/sh/build_functions.sh
. ${MAS_PROJECTS_DIR:-.}/sh/libmake_functions.bash

function mas_enabled_doprj ()
{
  local act disfile
  act=$1
  shift
  disfile=$projectsdir/projects_disabled.lst
##    echo ">>>>>>>> [$act:$mas_name-$mas_vers] $disfile" >&2
##    echo ">>>>>>>> `pwd`" >&2
  if [[ "$act" ]] && [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
    if [[ -f "$disfile" ]] ; then
      grep "^${act}$" $disfile &>/dev/null && return 1
      grep "^${act}:${mas_name}$" $disfile &>/dev/null && return 1
      grep "^${act}:${mas_name}-${mas_vers}$" $disfile &>/dev/null && return 1
      grep "^\*:${mas_name}$" $disfile &>/dev/null && return 1
      grep "^\*:${mas_name}-${mas_vers}$" $disfile &>/dev/null && return 1
    fi
  fi
  return 0
}

function doprj ()
{
  local prj act shfile shfun wprjname done
  local list
  local cnt
  cnt=0
  setup_dirs || return 1
  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "$prjname $act ..." >&2 ; fi
  wprjname=`printf '%-30s' $prjname`
  setup_vers || return 1
  if [[ "$1" == 'full' ]] ; then
    list='autoreconf configure make install'
  else
    list=$@
  fi
  prj=$(basename `pwd`)
  for act in $list ; do
    done=''
    if [[ "$act" ]] ; then
      case $act in
        n) act=nop          ;;
        m) act=make         ;;
        a) act=autoreconf   ;;
        c) act=configure    ;;
        d) act=distclean    ;;
        t) act=testdist     ;;
        i) act=install      ;;
        u) act=uninstall    ;;
        e) act=ebuild       ;;
        l) act=clean        ;;
        L) act=distclean    ;;
        M) act=maiclean     ;;
        Z) act=virgin-clean ;;
#	*) echo "unknown act [$act]" >&2
      esac
      if [[ "$MAS_SH_VERBOSE" ]] ; then echo "[$act]" >&2 ; fi
      shfile="sh/${act}.sh"
      shfun="dosh_${act}"

    # echo "$act at $dir" >&2
      if ! mas_enabled_doprj $act ; then
#	echo "SKIPped ${act}:${mas_name}-${mas_vers}" >&2
        echo -e "\t$wprjname\t(-$act)\t -" >&2
      else
        case $act in
	  nop)                                                       done='done' ; cnt=$(( $cnt + 1 )) ;;
	  make)          make_m                        || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  autoreconf)    autoreconf_m                  || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  configure)     configure_m                   || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  testdist)      testdist_m                    || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
          distclean)     make_target distclean         || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
          maiclean)      make_target maintainer-clean  || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
          clean)         make_target clean             || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  install)       make_m && make_target install || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  uninstall)     make_target uninstall         || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  ebuild)        ebuild_m update               || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  virgin-clean)  virgin_clean                  || return 1 ; done='done' ; cnt=$(( $cnt + 1 )) ;;
	  *)
	    if [[ "$shfun" ]] && type -t $shfun &>/dev/null ; then
	      $shfun
	      done='done'
	      cnt=$(( $cnt + 1 ))
	    elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && . $shfile ; then
	      done='done'
	      cnt=$(( $cnt + 1 ))
	    elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && eval $shfile ; then
	      done='done'
	      cnt=$(( $cnt + 1 ))
	    else
	      echo "$LINENO FAIL action:$act prj:$prj mas_name:$mas_name ($shfile)" >&2
	      return 1
	    fi	  
        esac
      fi
#     elif [[ "$act" == 'make'       ]] ; then
#       make_m || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$act" == 'autoreconf' ]] ; then
#       autoreconf_m || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$act" == 'configure'  ]] ; then
#       configure_m || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$act" == 'testdist'   ]] ; then
#       testdist_m || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$act" == 'install'    ]] ; then
#       make_m && make_target install || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$act" == 'uninstall'  ]] ; then
#       make_target uninstall || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$act" == 'ebuild'     ]] ; then
#       ebuild_m update || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$act" == 'virgin-clean'     ]] ; then
#       virgin_clean || return 1
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ "$shfun" ]] && type -t $shfun &>/dev/null ; then
#       $shfun
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && . $shfile ; then
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     elif [[ -L "sh" ]] && [[ -d sh ]] && [[ -x $shfile ]] && eval $shfile ; then
#       done=yes
#       cnt=$(( $cnt + 1 ))
#     else
#       echo "$LINENO FAIL action:$act prj:$prj mas_name:$mas_name ($shfile)" >&2
#       return 1
#     fi
    else
      echo "${nn}.	>>>> skipping act [$act]" >&2
      return 1
    fi
    if [[ "$done" ]] ; then echo -e "${MAS_PROJECT_POS:-}.\t${wprjname}\t  ${act}\t${done}\t${mas_name}-${mas_vers}" >&2 ; fi
  done
# echo "DONE $cnt actions for ${mas_name}-${mas_vers}" >&2
  return 0
}
function doall ()
{  
  local prj doit dir
  local cntall
  if [[ -d "$tmpdir" ]] && [[ "$projectsdir" ]] && [[ "$projects_list" ]] && [[ -d "$projectsdir" ]] ; then
    cntall=0
    if ! [[ "$MAS_DO_FROM_PROJECT" ]] ; then
      doit='yes'
    fi
    for prj in $projects_list ; do
      MAS_PROJECT_POS=$cntall
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
          cntall=$(( $cntall + 1 ))
	else
	  echo "${cntall}.	>>>> skipping dir [$prj]" >&2
	fi
      fi
    done
  else
    return 1
  fi
  return 0
}
