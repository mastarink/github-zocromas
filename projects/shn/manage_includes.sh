#!/bin/sh
# bash
function check_file ()
{
  local fname=$1
  local lin1 lin2 mark
  if ! [[ $fname =~ ^([a-z0-9_]+)\.(h)$ ]] ; then
    echo "$fname: error fname '$fname'" >&2
  else
#   echo "ok fname '$fname'" >&2
    mark=mas_${BASH_REMATCH[1]}_${BASH_REMATCH[2]}
    mark=${mark^^}
  fi

  if read lin1 ; then
    if ! [[ $lin1 =~ \#[[:blank:]]*ifndef[[:blank:]]+$mark ]] ; then
      echo "$fname: error line 1 '$lin1' -- '$mark'" >&2
#   else
#     echo "ok line 1 '$lin1' -- '$fname' -- '$mark'" >&2
    fi
    if read lin2 ; then
      if ! [[ $lin2 =~ \#[[:blank:]]*define[[:blank:]]+$mark ]] ; then
	echo "$fname: error line 2 '$lin2' -- '$mark'" >&2
#     else
#	echo "ok line 2 '$lin2' -- '$fname' -- '$mark'" >&2
      fi
    else
      echo "$fname: error can't read $fname" >&2
    fi    
  else
    echo "$fname: error can't read $fname" >&2
  fi
}
function check_path ()
{
  local path=$1
  local fn
  if [[ -f $path ]] && [[ $path == *.h ]] ; then
    fn=$(basename $path)
#   echo "opened $path" >&2
    check_file $fn < $path
  fi
}
function check_h ()
{
  local fpath
  for fpath in src/inc/*.h  ; do
    check_path $fpath
  done
}
check_h $@
