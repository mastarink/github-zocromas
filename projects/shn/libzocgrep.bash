function grepch ()
{
  if pushd $MAS_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MAS_SHN_PROJECTS[@]}
    eval grep $* -r --inc='*.[ch]' -- ${MAS_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function grepc ()
{
  if pushd $MAS_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MAS_SHN_PROJECTS[@]}
    eval grep $* -r --inc='*.c' -- ${MAS_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function greph ()
{
  if pushd $MAS_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MAS_SHN_PROJECTS[@]}
    eval grep $* -r --inc='*.h' -- ${MAS_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function grepac ()
{
  if pushd $MAS_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MAS_SHN_PROJECTS[@]}
    eval grep $* -r --inc='{configure.ac,Makefile.am}' -- ${MAS_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function grepf ()
{
  if pushd $MAS_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MAS_SHN_PROJECTS[@]}
    eval grep "^$1\(.*\)$" -r --inc='*.c' -- ${MAS_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
}
function wfile ()
{
  local f
  for f in `grepch -l $*` ; do
    shn_msg "$f : `shn_project_by_file $f`"
  done
}
