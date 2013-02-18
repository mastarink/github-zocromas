#!/bin/sh
edit_file ()
{
  local prj infile filename wfile ffile mased_file mased_name fileset findpath mased_all sname fline
  infile=$1
  shift
  if [[ $infile =~ \/([^\/]+)$ ]] ; then
    filename=${BASH_REMATCH[1]}
  else
    filename=$infile
  fi
  if [[ -d mased ]] ; then
#   wfile=$( grep "\(tab sp\|sp\|e\)\>\s\+$infile" mased/*.mased.vim )
    wfile=$( sed -sne "s/^\s*\(tab\s+sp\|sp\|tab\s\+sfind\|sfind\|find\|e\)\>\s\+\($filename\)\s*$/\2/p" mased/*.mased.vim )
    mased_file=$( grep -l $wfile mased/*.mased.vim | head -1 )
    if [[ $mased_file =~ ^mased\/(.+)\.mased\.vim$ ]] ; then
      fileset=${BASH_REMATCH[1]}
      mased_name="${fileset}.mased"
    fi
  fi
  case "$fileset" in
    ac)
      findpath=.
    ;;
    src)
      if [[ "$infile" =~ \.c$ ]] ; then 
        findpath="src/"
      elif [[ "$infile" =~ \.h$ ]] ; then
        findpath="src/inc/"
      else
        findpath='src/'
      fi
    ;;
    sh)
      findpath=sh/
    ;;
    *)
      findpath=.
    ;;
  esac
  if [[ -f "$infile" ]] ; then
    ffile="$infile"
  elif [[ "$wfile" ]] ; then
    ffile=$( find $findpath -type f -name "$wfile" | head -1 )
  fi
  prj=$( basename `pwd` )
  sname="server-$prj-$mased_name"
  echo "prj:$prj" >&2
  echo "infile:$infile" >&2
  echo "filename:$filename" >&2
  echo "wfile:$wfile" >&2
  echo "mased_file:$mased_file" >&2
  echo "fileset:'$fileset'" >&2
  echo "findpath:$findpath" >&2
  echo "sname:$sname" >&2
  echo "ffile:'$ffile'" >&2
  if [[ "$mased_file" ]] && [[ -f "$mased_file" ]] ; then
    if ! ${MAS_GVIM_CMD:-/usr/bin/gvim} --serverlist | ${MAS_GREP_CMD:-/bin/grep} -i "^${sname}$" >/dev/null 2>&1 ; then      
      ${MAS_GVIM_CMD:-/usr/bin/gvim} --servername "$sname" $mased_name
      for  (( wcnt=0 ; $wcnt<10 ; wcnt++ )) ; do
        if ${MAS_GVIM_CMD:-/usr/bin/gvim} --serverlist | ${MAS_GREP_CMD:-/bin/grep} -i "^${sname}$" >/dev/null 2>&1 ; then
	  break
	fi
	sleep 0.1
      done
    fi
    if [[ "$ffile" ]] && [[ -f "$ffile" ]] ; then
      ${MAS_GVIM_CMD:-/usr/bin/gvim} --servername "$sname" --remote "$ffile"
      if [[ "$fline" ]] ; then
        ${MAS_GVIM_CMD:-/usr/bin/gvim} --servername "$sname" --remote-expr "MasGo($fline, $fcol)"
      fi
    fi
  else
    ${MAS_GVIM_CMD:-/usr/bin/gvim} $infile
  fi
}
edit_file $@
