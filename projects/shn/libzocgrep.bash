function grepch ()
{
  if pushd $MSH_SHN_PROJECTS_DIR &>/dev/null ; then
#   shn_msg grep "$*" -- ${MSH_SHN_PROJECTS[@]}
    eval grep $* -r --inc='*.[ch]' -- ${MSH_SHN_PROJECTS[@]}
    popd &>/dev/null
  fi
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
  for f in `grepch -l $*` ; do
    shn_msg "$f : `shn_project_by_file $f`"
  done
}
