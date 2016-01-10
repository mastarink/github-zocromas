function grepchg ()
{
  local qargs
  for (( i=1; i <= $# ; i++ )) ; do
    qargs+=" '${!i}'"
  done

  if pushd $MSH_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MSH_SHN_PROJECTS[@]}
    eval grep -r --inc='*.[ch]' --inc='*.def' $qargs -- ${MSH_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function grepchpx ()
{
  local ex
  local qargs
  ex=$1
  shift
  for (( i=1; i <= $# ; i++ )) ; do
    qargs+=" ${!i}"
    shn_msg B ${!i}
  done

  if pushd $MSH_SHN_PROJECT_DIR &>/dev/null ; then
#   shn_msg grep "$*"
    shn_msg  -r --inc='*.['${ex:-ch}']' --inc='*.def' $qargs src/ mod/
    eval grep  -r --inc='*.['${ex:-ch}']' --inc='*.def' $qargs src/ mod/
    popd &>/dev/null
  fi
}
function grepchp ()
{
  local qargs
  for (( i=1; i <= $# ; i++ )) ; do
    qargs+=" '${!i}'"
#   shn_msg A ${!i}
  done
  grepchpx ch $qargs
}
function grephp ()
{
  local qargs
  for (( i=1; i <= $# ; i++ )) ; do
    qargs+=" '${!i}'"
#   shn_msg A ${!i}
  done
  grepchpx h $qargs
}

function grepnc ()
{  
  local opts 
  local i args finarg
  for ((i=1 ; i <= $# ; i++)) ; do
    if [[ ${!i} == -- ]] ; then 
      finarg=yes
    elif [[ ${!i} == -* ]] && ! [[ $finarg ]]; then
      if [[ $opts ]] ; then
        opts+=" ${!i}"
      else
        opts=${!i}
      fi
    else
       if [[ $args ]] ; then
        args+=" '${!i}'"
      else
        args="'${!i}'"
      fi
    fi
  done
# eval /bin/grep --color=yes --inc='*.[ch]' -r '^[[:blank:]]*[^[:blank:]/#\*]$@'
# eval echo --inc='*.c' -r $opts '^[[:blank:]]*[^[:blank:]\/\#\*].*'$args src >&2
  eval grep --inc='*.c' -r $opts '^[[:blank:]]*[^[:blank:]\/\#\*].*'"$args" src/ mod/
}
function grepc ()
{
  if pushd $MSH_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MSH_SHN_PROJECTS[@]}
    eval grep $* -r --inc='*.c' -- ${MSH_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function greph ()
{
  if pushd $MSH_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MSH_SHN_PROJECTS[@]}
    eval grep $* -r --inc='*.h' -- ${MSH_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function grepac ()
{
  if pushd $MSH_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MSH_SHN_PROJECTS[@]}
    eval grep $* -r --inc='{configure.ac,Makefile.am}' -- ${MSH_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function grepf ()
{
  if pushd $MSH_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MSH_SHN_PROJECTS[@]}
    eval grep "^$1\(.*\)$" -r --inc='*.c' -- ${MSH_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function wfile ()
{
  local f
  for f in `grepchg -l $*` ; do
    shn_msg "$f : `shn_project_by_file $f`"
  done
}
