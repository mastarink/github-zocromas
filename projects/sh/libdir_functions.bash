export MAS_PROJECTS_DIR MAS_PROJECTS_TMPDIR
function datem () 
{ 
    /bin/date '+%Y%m%d'
}
function datemt () 
{ 
    /bin/date '+%Y%m%d.%H%M%S'
}
function make_dirs ()
{
# if [[ -d "$projectsdir" ]] && ! [[ -d "$savedir" ]]; then
#   mkdir "$savedir" || echo "$LINENO ERROR make_dirs" >&2
# fi
  if ! [[ -d "$projectsdir" ]] ; then return 1 ; fi
  if ! [[ -d "$MAS_PROJECTS_TMPDIR" ]]; then
    mkdir "$MAS_PROJECTS_TMPDIR" || echo "$LINENO ERROR make_dirs" >&2
  fi
  
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirdist" ]]; then
    mkdir "$savedirdist" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirgentoo" ]]; then
    mkdir "$savedirgentoo" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirtar" ]]; then
    mkdir "$savedirtar" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedirtar" ]] && ! [[ -d "$savedirtarme" ]]; then
    mkdir "$savedirtarme" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$projectsdir" ]] && ! [[ -d "$instdir" ]]; then
    mkdir "$instdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$projectsdir" ]] && ! [[ -d "$flavourdir" ]]; then
    mkdir "$flavourdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$projectsdir" ]] && ! [[ -d "$tworkdir" ]]; then
    mkdir "$tworkdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tworkdir" ]] && ! [[ -d "$tworkbuild" ]]; then
    mkdir "$tworkbuild" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$auxdir" ]] && ! [[ -d "$auxdir" ]]; then
    mkdir "$auxdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$m4dir" ]] && ! [[ -d "$m4dir" ]]; then
    mkdir "$m4dir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tworkdir" ]] && ! [[ -d "$tworkunpack" ]]; then
    mkdir "$tworkunpack" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$debugdir" ]] && ! [[ -d "$debugdir" ]]; then
    mkdir "$debugdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$testdir" ]] && ! [[ -d "$testdir" ]]; then
    mkdir "$testdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$wbuilddir" ]] && ! [[ -d "$wbuilddir" ]]; then
    mkdir "$wbuilddir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$make_errdir" ]] && ! [[ -d "$make_errdir" ]]; then
    mkdir "$make_errdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
}
function setup_global_dirs ()
{
 export MAS_ADMIN_DIR MAS_PROJECT_AUXDIR MAS_PROJECT_BUILDDIR MAS_PROJECT_MAKE_ERRDIR MAS_PROJECT_MAKE_LOGNAME
 MAS_ADMIN_DIR=${MAS_ADMIN_DIR:=`realpath $MAS_PROJECTS_DIR/../admin`}
 MAS_PROJECT_MAKE_LOGNAME=${MAS_PROJECT_MAKE_ERRDIR:=${MAS_PROJECT_BUILDDIR:=${MAS_PROJECT_AUXDIR:=$MAS_PROJECT_DIR/.auxdir}/.build}/.errors}/makes.log
 echo ">> $MAS_PROJECT_MAKE_LOGNAME" >&2
}
function setup_dirs ()
{
  local me=${1:-${BASH_SOURCE[3]}}
  if ! [[ "$stamp" ]] ; then
    if [[ "$MAS_DOALL_STAMP" ]] ; then
      stamp=$MAS_DOALL_STAMP
    else
      stamp=`datem`
    fi
  fi
  if ! [[ "$now_stamp" ]] ; then
    if [[ "$MAS_DOALL_NOW_STAMP" ]] ; then
      now_stamp=$MAS_DOALL_NOW_STAMP
    else
      now_stamp=`datemt`
    fi
  fi  
  wd=`pwd`
  if [[ "$0" == `which bash` ]] ; then
    if ! [[ "$me" ]] ; then
      if [[ "${BASH_SOURCE[3]}" ]] ; then
  #    me="$wd/${BASH_SOURCE[2]}"
	me=${BASH_SOURCE[3]}
      else
	echo "ERROR BASH_SOURCE[3] -- ${BASH_SOURCE[*]}" >&2
      fi
    fi
    MAS_SOURCED=$me
  else
    me="$wd/$0"
    unset MAS_SOURCED
  fi
  [[ "$me" ]] || return 1
  mer="$(realpath $me)" || return 1
  if [[ -f "$me" ]] && [[ -x "$me" ]] ; then
    if [[ "$me" =~ \/([^\/]+)\/([^\/]+)\/([^\/]+)\.sh$ ]] ; then
      shdir=${BASH_REMATCH[1]}/${BASH_REMATCH[2]}
      prjname=${BASH_REMATCH[1]}
    fi
    shdir=$( dirname "$me" )
    shdirr=$( dirname "$mer" )
    shdirup=$( dirname $shdir )
    shdirupr=$( dirname $shdirr )
    projectsdir="$( realpath "$shdirupr" )" || return 1
    mas_flavour=${mas_flavour:-`cat $projectsdir/projects.flavour`}
    indir="$( realpath "$shdirup" )" || return 1
    topdir="$( realpath "$shdirupr/.." )" || return 1
    admindir="$topdir/admin"
    savedir="$admindir/saved"
    instdir="$admindir/install"
    flavourdir="$instdir/${mas_flavour:-default}"
    if [[ "$MAS_SH_VERBOSE" ]] ; then echo "FLAVOUR: [${mas_flavour:-default}] -- $flavourdir" >&2 ; fi
    instshdir="$admindir/install.sh"
    tworkdir="$admindir/tmp"
    MAS_PROJECTS_TMPDIR="/tmp/zoc"

# see `sysctl kernel.core_pattern`
    coredir="/tmp"

    savedirdist="$savedir/dist"
    savedirgentoo="$savedir/gentoo"
    savedirtar="$savedir/tar"
    savedirtarme="${savedirtar}/${stamp}"
    
    tworkbuild="$tworkdir/build/"
    tworkunpack="$tworkdir/unpack/"
    
    runconfigdirname=.zocromas
    runconfigdir="$HOME/$runconfigdirname/"
    runconfigdir="$projectsdir/zocromas/$runconfigdirname/"
    export    MAS_WORK_ROOT_DIR=$projectsdir
    export    MAS_WORK_IN_DIR=$projectsdir


    debugdir="$indir/debug"
    testdir="$indir/test"
    
    auxdir="$indir/.auxdir"
    m4dir="$auxdir/m4"
    inbuilddir="$auxdir/.build"
    wbuilddir="$inbuilddir"
    build_at="$wbuilddir"
    bsrcdir="$build_at/src"
    make_errdir="$inbuilddir/.errors"
    make_logname="makes.log"
    make_logfile="$make_errdir/$make_logname"
    logdir="$admindir/log"

    if [[ -d "$projectsdir" ]] ; then
      prjgroup=$(basename $projectsdir)
      make_dirs || return 1
  #   export LD_LIBRARY_PATH=/usr/local/lib

      export PKG_CONFIG_PATH=$flavourdir/lib/pkgconfig
    fi
    

    binprefix=zocromas_
  else
    echo "FATAL : '$me' bad SCRIPT call ($0)" >&2
  fi
# if [[ $indir =~ \/([^\/]+)$ ]] ; then
#   prjname=${BASH_REMATCH[1]}
# else
#   prjname=$( basename $indir )
# fi

  if type -t setup_vers &>/dev/null ; then setup_vers || return 1 ; fi
  return 0
}
function grepch0 ()
{  
  local project projects_list res
  if [[ "${projects_list:=${MAS_PROJECTS_LIST:=`cat ${projectsfile:=${projectsdir:=${MAS_PROJECTS_DIR:-${MAS_PROJECTS_TMPDIR:-/tmp}}}/projects.list}|tr '\n' ' '`}}" ]] ; then
      pushd ${projectsdir:=${MAS_PROJECTS_DIR:-${MAS_PROJECTS_TMPDIR:-/tmp}}} >/dev/null || return 1
      arg="$@"
#     find $projects_list \( -name .build -prune \) -o -type f -name '*.[ch]' -okdir grep -H --color=yes $@ \{\} \; || return 1
      echo "find..." >&2
      find $projects_list \( -name .build -prune \) -o -type f -name '*.[ch]' -execdir grep -H --color=yes $arg \{\} \+
      popd >/dev/null
#   grep --color=yes -r --inc='*.[ch]' "$@" {commonlibs,bins,zoc*}
#   find {commonlibs,bins,zoc*} -not -path '*/.build/*' -type f -name '*.[ch]' -execdir grep -H --color=yes $@ \{\} \; | sed -ne 's@^\.\/@@p'
#     find {commonlibs,bins,zoc*} -not -path '*/.build/*' -type f -name '*.[ch]' -execdir grep -H --color=yes $@ \{\} \+
  else
    echo "ERROR" >&2
    return 1
  fi
  return 0
}
function grepch ()
{
  local saveresult="${MAS_PROJECTS_DIR}/last_grepch_result.txt"
  echo "# `date`" >$saveresult
  echo "# grepau $@" >>$saveresult
  grepch0 $@ | sed  's/^\(.*\)\.\/\([^:]*\)\s*:\s*\(.*\)$/\1\2\t\3/' | cat -n >>$saveresult
  cat $saveresult
}
function egrepch ()
{
  local num
  local gfile
  local saveresult="${MAS_PROJECTS_DIR}/last_grepch_result.txt"
  num=$(( 0 + $1 ))
# grep  "^\s*$num\>" $saveresult >&2
  gfile=$(sed -ne "s/^[[:space:]]*\($num\b\)[[:space:]]\+\x1b\[33m\x1b\[K\([^[:space:]\n\r\x1b]\+\)\(.*\)$/\2/p" $saveresult)
  echo -n "[$gfile]" >&2
  if [[ "$MAS_PERSONAL_EDITOR" ]] && [[ "$gfile" ]] ; then
   "$MAS_PERSONAL_EDITOR" $gfile
  fi
}
function grepau0 ()
{
  local project projects_list 
  if [[ "${projects_list:=${MAS_PROJECTS_LIST:=`cat ${projectsfile:=${projectsdir:=${MAS_PROJECTS_DIR:-${MAS_PROJECTS_TMPDIR:-/tmp}}}/projects.list}|tr '\n' ' '`}}" ]] ; then
      pushd ${projectsdir:=${MAS_PROJECTS_DIR:-${MAS_PROJECTS_TMPDIR:-/tmp}}} || return 1
      arg="$@"
      echo "arg: '$arg'" >&2
#      find $projects_list \( -name .build -prune \) -o -type f -name '*.a[mc]' || return 1
#      find $projects_list \( -name .build -prune \) -o -type f -name '*.a[mc]' -okdir grep -H --color=yes $@ \{\} \; || return 1
      find $projects_list \( -name .build -prune \) -o -type f -name '*.a[mc]' -exec grep -H --color=yes $arg \{\} \+
      popd >/dev/null
#   grep --color=yes -r --inc='*.[ch]' "$@" {commonlibs,bins,zoc*}
#   find {commonlibs,bins,zoc*} -not -path '*/.build/*' -type f -name '*.[ch]' -execdir grep -H --color=yes $@ \{\} \; | sed -ne 's@^\.\/@@p'
#     find {commonlibs,bins,zoc*} -not -path '*/.build/*' -type f -name '*.[ch]' -execdir grep -H --color=yes $@ \{\} \+
  else
    echo "ERROR" >&2
    return 1
  fi
  return 0
}
function grepau ()
{
  local saveresult="${MAS_PROJECTS_DIR}/last_grepau_result.txt"
  echo "# `date`" >$saveresult
  echo "# grepau $@" >>$saveresult
  grepau0 $@ | sed  's/^\(.*\)\.\/\([^:]*\)\s*:\s*\(.*\)$/\1\2\t\3/' | cat -n  >$saveresult
  cat $saveresult
}

function wdproj_scan_project_1 ()
{
  local prj=$1
  shift
  local project=$1
  shift
  local match=$1
  shift
# echo "1 : $prj :: $match ? $project" >&2
  [[ "$match" =~ ^zocromas_.*$project ]]
}
function wdproj_scan_project_2 ()
{
  local prj=$1
  shift
  local project=$1
  shift
  local match=$1
  shift
# echo "2 : $prj :: $match ? $project" >&2
  [[ "$match" == "$project" ]]
}
function proj_scan ()
{
  local project=$1
  shift
  local scanner=$1
  shift
  if [[ "${MAS_PROJECTS_DIR}" ]] ; then
    for prj in ${MAS_PROJECTS_LIST:=`cat ${projectsfile:=${projectsdir:=${MAS_PROJECTS_DIR:-${MAS_PROJECTS_TMPDIR:-/tmp}}}/projects.list}|tr '\n' ' '`} ; do
      tmp_wd=${MAS_PROJECTS_DIR}/${prj}
      tmp_name=$( basename $tmp_wd )
      if [[ "$tmp_wd" ]] && [[ "$tmp_name" ]] && "$scanner" "$prj" "$project" "$tmp_name" ; then
	return 0
      fi
      unset tmp_name tmp_wd
    done
  fi
  return 1
}
function wdproj_scan ()
{
  local tmp_wd tmp_name
  local project=$1
  shift
  local scanner=$1
  shift
  local docd=$1
  shift
  if proj_scan $project $scanner ; then
    case "$docd" in
     cd|set)
      export MAS_PROJECT_NAME=$tmp_name
      export MAS_PROJECT_DIR=$tmp_wd
      cd $tmp_wd && return 0
     ;;
     wd)
      echo "$tmp_wd"
      return 0
     ;;
     name)
      echo "$tmp_name"
      return 0
     ;;
     *)
      return 0
     ;;
    esac
  fi
  return 1
}
function wdproj ()
{
  export MAS_PROJECT_NAME MAS_PROJECT_DIR
  unset MAS_ADMIN_DIR MAS_PROJECT_AUXDIR MAS_PROJECT_BUILDDIR MAS_PROJECT_MAKE_ERRDIR MAS_PROJECT_MAKE_LOGNAME MAS_PROJECTS_TMPDIR
  local project="$1" 
  shift
  local docd=${1:-cd}
  shift
  local meth
  local prj
  if [[ "$project" ]] ; then
    if [[ "$project" == '.' ]] && [[ "$MAS_PROJECT_NAME" ]] ; then
      echo "DOT : '$MAS_PROJECT_NAME'" >&2
      project=$MAS_PROJECT_NAME
    fi
    if [[ "${MAS_PROJECTS_LIST:=`cat ${MAS_PROJECTS_DIR:-${MAS_PROJECTS_TMPDIR:-/tmp}}/projects.list|tr '\n' ' '`}" ]] ; then
      for meth in wdproj_scan_project_1 wdproj_scan_project_2 ; do
#        echo "meth: $meth" >&2
        wdproj_scan "$project" $meth ${docd:-cd} && return 0
      done
#     echo "$MAS_PROJECT_NAME :: $MAS_PROJECT_DIR" >&2
      if [[ "$docd" == 'cd' ]] ; then
        for prj in $MAS_PROJECTS_LIST ; do
	  basename "$prj"
        done  | sed -e 's/zocromas_//' | sort | cat -n >&2
      fi
    else
      echo "ERROR '$MAS_PROJECTS_DIR' : '$MAS_PROJECTS_LIST' : '$MAS_PROJECTS_DIR'" >&2
      unset MAS_PROJECTS_LIST
    fi
  elif [[ "${MAS_WORK_DIR}" ]] ; then
    cd ${MAS_WORK_DIR}
    return 0
  fi
  return 1
}
function is_in_project ()
{
  for p in ${MAS_PROJECTS_LIST:=`cat ${projectsfile:=${projectsdir:=${MAS_PROJECTS_DIR:-${MAS_PROJECTS_TMPDIR:-/tmp}}}/projects.list}|tr '\n' ' '`} ; do
    echo PRoject:$p >&2
  done
}
# export wdproj_scan wdproj
unset cdproj cdproj_scan MAS_PROJECTS_LIST
alias j='wdproj'
