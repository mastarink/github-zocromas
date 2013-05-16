! type -t setup_vers || . sh/setupz.sh
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
  if [[ -d "$projectsdir" ]] && ! [[ -d "$tmpdir" ]]; then
    mkdir "$tmpdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpbuild" ]]; then
    mkdir "$tmpbuild" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$auxdir" ]] && ! [[ -d "$auxdir" ]]; then
    mkdir "$auxdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$m4dir" ]] && ! [[ -d "$m4dir" ]]; then
    mkdir "$m4dir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpunpack" ]]; then
    mkdir "$tmpunpack" || echo "$LINENO ERROR make_dirs" >&2
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
    indir="$( realpath "$shdirup" )" || return 1
    topdir="$( realpath "$shdirupr/.." )" || return 1
    admindir="$topdir/admin"
    savedir="$admindir/saved"
    instdir="$admindir/install"
    instshdir="$admindir/install.sh"
    tmpdir="$admindir/tmp/"

    savedirdist="$savedir/dist"
    savedirgentoo="$savedir/gentoo"
    savedirtar="$savedir/tar"
    savedirtarme="${savedirtar}/${stamp}"
    
    tmpbuild="$tmpdir/build/"
    tmpunpack="$tmpdir/unpack/"
    
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

      export PKG_CONFIG_PATH=$instdir/lib/pkgconfig
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

  setup_vers || return 1
  return 0
}
function cdproj ()
{
  export MAS_PROJECTS_LIST MAS_CD_PROJECT_NAME MAS_CD_PROJECT_DIR
  local project="$1" project_name
  shift
  local prefix="${1:-zocromas}"
  shift
# if [[ "$project" ]] ; then 
#   project="${prefix}_${project}" 
# else
#   project="${prefix}" 
# fi
  local projectsdir projectsfile projects_list prj
  if [[ "$project" ]] ; then
    if [[ "${projects_list:=${MAS_PROJECTS_LIST:=`cat ${projectsfile:=${projectsdir:=${MAS_PROJECTS_DIR:-/tmp}}/projects.list}`}}" ]] ; then
      for prj in $projects_list ; do
        unset project_name
	if [[ "${project_name:=$( basename ${MAS_PROJECTS_DIR}/$prj )}" ]] \
			&& [[ "$project_name" =~ ^.*$project ]] ; then
          unset MAS_CD_PROJECT_NAME MAS_CD_PROJECT_DIR
	  MAS_CD_PROJECT_NAME=$project_name
	  cd ${MAS_CD_PROJECT_DIR:=${MAS_PROJECTS_DIR}/${prj}}
#	  echo "CD ${MAS_CD_PROJECT_DIR} [$prj : $project_name]" >&2
	  break
	fi
        unset project_name
      done
#     echo "$MAS_CD_PROJECT_NAME :: $MAS_CD_PROJECT_DIR" >&2
      if ! [[ "$project_name" ]] ; then
	for prj in $projects_list ; do
	  basename "$prj"
	done  | sed -e 's/zocromas_//' | sort >&2
      fi      
    else
      echo "ERROR '$projectsdir' : '$projects_list' : '$projectsdir'" >&2
      unset MAS_PROJECTS_LIST
    fi
  fi
}
alias j='cdproj'
